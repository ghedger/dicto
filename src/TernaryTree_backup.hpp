#pragma once

#include <map>
#include <string>
#include <queue>
#include <deque>
#include <stack>
#include "TemplNode.hpp"

using namespace std;

typedef unsigned char UCHAR;

// This is the instantiable class from my TemplNode template
class TNode : public TemplNode <UCHAR, TNode>
{
    public:
        TNode() {};
        TNode(UCHAR key) : TemplNode <UCHAR, TNode> (key) { };
        ~TNode() {};
        void setKey( UCHAR key )
        {
            if( isupper( key ) )
            {
                setUpper();
            }
            _key = ( UCHAR ) tolower( key );
        }
};

// This class is the tree itself. It manages a ternary search tree of TNodes
// This is a dictionary and allows us quick word lookup
// with a yes/no response.
class TernaryTree
{
    public:
        TernaryTree() {};
        ~TernaryTree() {};

        // insertNode
        // insert a node into the tree
        //
        // @In: pWord pointer to null-terminated string
        // ppParent pointer to parent pointer
        // @Out: -
        void insert( const char *pWord, TNode **ppParent = NULL )
        {
            if( !( *ppParent ) )
                *ppParent = allocNode( *pWord );
            if( tolower( *pWord ) < ( ( *ppParent )->getKey() ) )
                insert( pWord, &( ( *ppParent )->_l ) );
            else if ( tolower( *pWord ) > ( *ppParent )->getKey() )
                insert( pWord, &( ( *ppParent )->_r ) );
            else
            {
                if( pWord[ 1 ] )
                    insert( pWord + 1, &( ( *ppParent )->_c ) );
                else
                    ( *ppParent )->setTerminator();
            }
        };

        // find
        // find a word
        //
        // @In: pWord pointer to null-terminated string
        // pParent pointer to current parent node
        // ppTerminal pointer to terminal node pointer
        // @Out: true == match found
        bool find( const char *pWord, TNode *pParent, TNode ** ppTerminal = NULL )
        {
            bool ret = false;
            if( pParent )
            {
                if( ( *pWord ) < pParent->getKey() )
                    ret = find( pWord, pParent->getLeft(), ppTerminal );
                else if( ( *pWord ) > pParent->getKey() )
                    ret = find( pWord, pParent->getRight(), ppTerminal );
                else
                {
                    if( '\0' == pWord[ 1 ] )
                    {
                        ret = pParent->getTerminator();
                        if( ppTerminal ) // Mark pointer to node
                            *ppTerminal = pParent;
                    }
                    else
                        ret = find( pWord + 1, pParent->getCenter(), ppTerminal );
                }
            }
            return ret;
        }

        void fuzzyFind(
            const char *pWord,
            TNode *pParent,
            map<int, string> *pWords )
        {
            string word = pWord;
            TNode *pNode = NULL;
            while( word.length() > 1 )
            {
                if( find( pWord, pParent, &pNode ) )
                {
                    break;
                }
                if( 1 < ( word.length() >> 1 )  )
                    word = word.substr( 0, word.length() >> 1 );
                else
                    break;
            }

            // now extrapolate and score possibilities from stem
            if( pNode )
            {
                deque<char> accum;
                extrapolateAll( pNode, pWords, &accum, word.c_str(), pWord );
            }
        }


        // extrapolateAll
        // extrapolate all possibilities from an input strung.
        //
        // @In: pNode pointer to starting node
        // pVect vector of words
        // @Out: -
        // pVect filled with words from starting node
        bool extrapolateAll(
            TNode *pNode,
            map<int, string> *pWords,
            deque<char> *accum,
            const char *pStem,
            const char *pWord )
        {
            if( pNode )
            {
                extrapolate( pNode, pWords, accum, pStem, pWord );
                return true;
            }
            else
                return false;
        }


        // extrapolate
        // extrapolate from a word stem.
        //
        // I selected a deque as my accumulator data structure. A stack
        // requires a second stack or some other means of word reversal,
        // while a deque gives me both FIFO representation and FILO
        // functionality.
        //
        // @In: pNode pointer to starting node
        // pVect vector of words
        // @Out: -
        // pVect filled with words from starting node
        bool extrapolate(
            TNode *pNode,
            map<int, string> *pWords,
            deque<char> *accum,
            const char *pStem,
            const char *pWord )
        {
            TNode *pChild = NULL;
            bool ret = false;

            // Push this node's key onto our data structure
            accum->push_back( pNode->getKey() );

            // Recurse
            if( ( pChild = pNode->getLeft() ) )
            {
                if( extrapolate( pChild, pWords, accum, pStem, pWord ) )
                {
                    ret |= true;
                }
            }

            if( ( pChild = pNode->getCenter() ) )
            {
                if(  extrapolate( pChild, pWords, accum, pStem, pWord ) )
                {
                    ret |= true;
                    if( !( accum->empty() &&
                                !pChild->getLeft() && !pChild->getCenter() && !pChild->getRight() ) )
                    {
                        accum->pop_back();

                    }
                }
            }

            if( ( pChild = pNode->getRight() ) )
            {
                if(  extrapolate( pChild, pWords, accum, pStem, pWord ) )
                {
                    ret |= true;
                }
            }

            if( pNode->getTerminator() )
            {
                // Got end of word.
                // Note that word may continue. For example, 'p' in "amp"
                // is a terminator, yet "ample" is also a word which uses amp
                // as its prefix stem.
                auto commit = ( *accum );
                string word, compound;

                // Reverse stack. Yes, this could be more efficient with
                // a dequeue.
                while( !commit.empty() )
                {
                    word.push_back( commit.front() );
                    commit.pop_front();
                }

                compound = pStem;
                compound = compound.substr( 0, compound.length() - 1 );
                compound += word;
                int score = getLevenshtein( pWord, compound.c_str() );
                ( *pWords )[ score ] = compound;

                if( !pNode->getCenter() )
                {
                    accum->pop_back();
                }
                ret = true;
            }

           return ret;
        }

    protected:
        // allocNode
        // insert a node into the tree
        //
        // @In: key key of node to add
        // @Out: Pointer to node
        TNode *allocNode( char key )
        {
            TNode *pNode = new TNode(key);
            assert( pNode );
            pNode->setKey( key );
            return pNode;
        }

// Utility preprocessor macro for Levenshtein
#if !defined(MIN3)
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
#else
#error MIN3 Already defined!
#endif

        // getLevenshtein
        // This is an optimized Levenshtein string distance calculation.
        // It returns how "different" two strings are, effectively performing
        // a commutative subtraction operation.
        //
        // @In: s1 string #1
        // s2 string #2
        // @Out: Levenshtein difference
        int getLevenshtein( const char *s1, const char *s2 )
        {
            int score = -1;
            if( s1 && s2 )
            {
                unsigned int len1, len2, x, y, lastdiag, olddiag;
                len1 = strlen( s1 );
                len2 = strlen( s2 );
                unsigned int column[ len1 + 1 ];
                for( y = 1; y <= len1; y++ )
                    column[ y ] = y;
                for( x = 1; x <= len2; x++ )
                {
                    column[ 0 ] = x;
                    for( y = 1, lastdiag = x - 1; y <= len1; y++ )
                    {
                        olddiag = column[ y ];
                        column[y] = MIN3( column[ y ] + 1,
                            column[ y - 1 ] + 1,
                            lastdiag +
                            (
                                s1[ y - 1 ] == s2[ x - 1 ] ? 0 : 1
                            )
                        );
                        lastdiag = olddiag;
                    }
                }
                score = column[ len1 ];
            }
            return score;
        }
};
