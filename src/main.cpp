/* Dicto
 * main.cpp
 *
 * Copyright (C) 2015 Gregory P. Hedger
 * greg@hedgersoftware.com
 * 30329 112th Pl. SE
 * Auburn, WA 98092
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Also add information on how to contact you by electronic and paper mail.
 *
 */

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <fstream>
#include <string>
#include <set>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include "templ_node.h"
#include "ternary_tree.h"
#include "heap_mgr.h"
#include "log.h"


// declutter: allow cout instead of std::cout
using namespace std;

int getWordCount( ifstream *file )
{
    int lineTot = std::count(std::istreambuf_iterator<char>(*file),
             std::istreambuf_iterator<char>(), '\n');

    file->seekg( 0 );
    return lineTot;
}

// readDictionaryFile
// Read dictionary file into our data structure.
// TODO (RFE): Would be nice to have the tree self-balance
// instead of reading the sorted file in two halves...
//
// @In:     -
// @Out:    -
void readDictionaryFile( const char *path, TernaryTree *pTree, TNode *& pRoot )
{
    try
    {
        ifstream file( path );
        string line;
        int lineTot = getWordCount( &file );
        int start = lineTot >> 1;
        int idx = 0;

        VERBOSE_LOG( LOG_INFO, "Reading " << lineTot << " words." << endl );

        // Begin at middle of file
        while( idx < start )
        {
            getline( file, line );
            idx++;
        }

        // Insert second half
        VERBOSE_LOG( LOG_INFO, "Reading phase 1..." << endl );
        while( getline( file, line ) )
        {
            VERBOSE_LOG( LOG_DEBUG, "|" << line.c_str() << "|" << endl );
            pTree->insert( line.c_str(), &pRoot );
        }

        // Insert first half
        VERBOSE_LOG( LOG_INFO, "Reading phase 2..." << endl );
        idx = 0;
        file.clear();
        file.seekg( 0, ios::beg );
        while( idx < start )
        {
            getline( file, line );
            VERBOSE_LOG( LOG_DEBUG, "|" << line.c_str() << "|" << endl );
            pTree->insert( line.c_str(), &pRoot );
            idx++;
        }
    }
    catch(...)
    {
        VERBOSE_LOG( 0, "Error reading file." );
    }
}

// outPreamble
//
// @In:     -
// @Out:    -
void outPreamble()
{
    cout <<  "Dicto" << endl;
    cout <<  "Copyright (C) 2015 Gregory P. Hedger" << endl;
    cout << "This program comes with ABSOLUTELY NO WARRANTY." << endl;
    cout << "This is free software, and you are welcome to redistribute it" << endl;
    cout << "under certain conditions covered by GNU Public License v3.0." << endl;
}


// outPrompt
// output prompt
//
// @In:     -
// @Out:    -
void outPrompt()
{
    cout << endl << ">" ;
}

// outUsage
// Print basic program usage/invocation
//
// @In:     -
// @Out:    -
void outUsage()
{
    outPreamble();
    cout << "Usage:" << endl;
    cout << "dicto [flags]:" << endl;
    cout << "Flags:" << endl;
    cout << "\t-v set verbosity: -v0 none -v1 info -v2 debug" << endl;
}

// LEG is used for printTraversal to tell what leg the current node is on.
enum LEG
{
    LEG_L   = 0,
    LEG_C,
    LEG_R,
};

// We're going to render into a buffer
const int screenWidth = 80;
const int screenHeight = 8;
static char _renderBuf [ screenWidth * screenHeight ];

// printTraversal
// Render and prints a rudimentary treelike representation to console.
// Note this is rather rough, added at the end of this demo; it could be
//  improved.
//
// @In:     pRoot pointer to root node
//          leg left, right, or center
//          htab starting htab
// @Out:    -
void printTraversal( TNode *pNode, LEG leg, int htab, int depth = 0 )
{
    int vtab = depth;

    // Initialize render buf if at top
    if( !depth )
    {
        printf("\n");
        memset( _renderBuf, ' ', sizeof( _renderBuf ) );
        htab = screenWidth >> 1;        // set initial htab to center of rendering buffer
    }

    if( depth > 3 )
        return;


    // Caculate screen layout
    switch( leg )
    {
        case LEG_L:
            htab -= 22 - (depth << 1);
            _renderBuf[ htab - 1 + ( vtab * screenWidth ) ] = '>';
            break;
       case LEG_R:
            htab += 22 - (depth << 1);
            _renderBuf[ htab - 1 + ( vtab * screenWidth ) ] = '<';
            break;
        case LEG_C:
            _renderBuf[ htab - 1 + ( vtab * screenWidth ) ] = '^';
            break;
        default:
            break;
    }

//    printf("htab: %d depth: %d value: %c\n", htab, depth, pNode->getKey());

    sprintf( &_renderBuf[ htab + ( vtab * screenWidth ) ], "%c",  pNode->getKey());
    if( pNode->getLeft() )
    {
        printTraversal( pNode->getLeft(), LEG_L, htab, depth + 1 );
    }
    if( pNode->getCenter() )
    {
        printTraversal( pNode->getCenter(), LEG_C, htab, depth + 1 );
    }
    if( pNode->getRight() )
    {
        printTraversal( pNode->getRight(), LEG_R, htab, depth + 1 );
    }

    // If we're done, print out the rendered tree
    if( !depth )
    {
        int i, j;
        for( j = 0; j < screenHeight; j++ )
        {
            for( i = 0; i < screenWidth; i++ )
            {
                char c = _renderBuf[ i +  ( j * screenWidth )];
                // Replace terminator with space
                c = c ? c : ' ';
                putchar( c );
            }
            printf( "\n" );         // Print LF every screenWidth columns
        }
    }
}

// main
// This is the main entry point and testbed for ternary tree.
//
// @In:     -
// @Out:    0 == success
int main( int argc, const char *argv[] )
{
    const int MAX_IN = 128;
    TNode *pRoot = NULL;
    TernaryTree t;

    // parseargs
    if( 1 < argc ) {
        int i = 1;
        while( i < argc )
        {
            if( '-' == argv[i][0] )
            {
                switch( argv[i][1] )
                {
                    case 'v':
                        {
                            int verbosity;
                            if( isdigit(verbosity = argv[i][2] ) )
                            {
                                LOG_LEVEL ll = (LOG_LEVEL) (verbosity - ( int ) '0');
                                SET_VERBOSITY_LEVEL( ll );
                            }
                        }
                        break;
                    default:
                        outUsage();
                        return 1;
                }
            }
            else
            {
                outUsage();
                return 1;
            }

            i++;
        }
    }


    outPreamble();

    readDictionaryFile( "dict.txt", &t, pRoot );

    // Print out the top portion of the tree
    if( LOG_DEBUG <= GET_LOG_VERBOSITY())
        printTraversal( pRoot, LEG_C, 0, 0 );

    char in[ MAX_IN ];
    while(1)
    {
        outPrompt();
        cin >> in;

        // Extrapolate words from a prefix
        TNode *pPrefixNode = NULL;
        const char *pPrefix = in;
        char stem[ MAX_IN ];
        bool directHit = false;
        cout << in << "...let's see..." << endl;

        map< int, string > extrapolation;
        t.fuzzyFind( pPrefix, pRoot, &extrapolation );

        if( !extrapolation.empty() )
        {
            cout << "SUGGESTIONS:" << endl;
            for ( auto it : extrapolation )
            {
                 cout << "(" << ( ( ( it ).first ) >> 6 ) << ") ";
                 cout << ( ( it ).second.c_str() ) << endl;
            }
        }
        else
            cout << "NO SUGGESTION..." << endl;
    }
    return 0;
}


