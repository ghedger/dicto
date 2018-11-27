/* Dicto
 * ternary_tree.cpp
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
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <deque>
#include "templ_node.h"
#include "ternary_tree.h"
#include "log.h"

typedef unsigned char UCHAR;

//#define DEBUG
#define INFO

// This class is the tree itself. It manages a ternary search tree of TNodes
// This is a dictionary and allows us quick word lookup
// with a yes/no response.

// insertNode
// insert a node into the tree
//
// @In: pWord pointer to null-terminated string
// ppParent pointer to parent pointer
// @Out: Node *
TNode * TernaryTree::insert(const char *pWord, TNode **ppNode)
{
  TNode * pChild = NULL;
  VERBOSE_LOG(LOG_DEBUG, "insert >>>>>" << std::endl);
  if (!(*ppNode)) {
    *ppNode = allocNode(*pWord);
    VERBOSE_LOG(LOG_DEBUG, "ALLOC" << std::endl);
  }
  if (tolower(*pWord) < ((*ppNode)->getKey())) {
    VERBOSE_LOG(LOG_DEBUG,  "L: " << pWord);
    insert(pWord, &((*ppNode)->l_));
    (*ppNode)->getLeft()->setParent((*ppNode)->getParent());
  }
  else if (tolower(*pWord) > (*ppNode)->getKey()) {
    VERBOSE_LOG(LOG_DEBUG,  "R: " << pWord << std::endl);
    // Add a peer on the right
    insert(pWord, &((*ppNode)->r_));
    (*ppNode)->getRight()->setParent((*ppNode)->getParent());
  } else {
    // Is this the last letter (is there a char in the second position?)
    if (pWord[ 1 ])
    {
      VERBOSE_LOG(LOG_DEBUG,  "C: " << pWord << std::endl);
      pChild = insert(pWord + 1, &((*ppNode)->c_));
      pChild->setParent(*ppNode);
    }
    else
    {
      // Yep, last one...
      VERBOSE_LOG(LOG_DEBUG,  "T: " << pWord << std::endl);
      (*ppNode)->setTerminator();
    }
  }


  VERBOSE_LOG(LOG_DEBUG,  "insert <<<<<" << std::endl);
  /*
     VERBOSE_LOG(LOG_DEBUG,  "" << std::endl);
     VERBOSE_LOG(LOG_DEBUG,  " K: " << (*ppNode)->getKey());
     VERBOSE_LOG(LOG_DEBUG,  " L: " << (*ppNode)->getLeft());
     VERBOSE_LOG(LOG_DEBUG,  " C: " << (*ppNode)->getCenter());
     VERBOSE_LOG(LOG_DEBUG,  " R: " << (*ppNode)->getRight());
     VERBOSE_LOG(LOG_DEBUG,  " P: " << (*ppNode)->getParent());
     VERBOSE_LOG(LOG_DEBUG,  std::endl);
     */

  return *ppNode;
};

// find
// find a word
//
// @In:     @pWord pointer to null-terminated string
//          @pParent pointer to current parent node
//          @ppTerminal pointer to terminal node pointer
// @Out:    true == match found
bool TernaryTree::find(const char *pWord, TNode *pParent, TNode ** ppTerminal)
{
  bool ret = false;
  if (pParent)
  {
    if ((*pWord) < pParent->getKey())
      ret = find(pWord, pParent->getLeft(), ppTerminal);
    else if ((*pWord) > pParent->getKey())
      ret = find(pWord, pParent->getRight(), ppTerminal);
    else
    {
      if ('\0' == pWord[ 1 ])
      {
        ret = pParent->getTerminator();
        if (ppTerminal) // Mark pointer to node
          *ppTerminal = pParent;
      }
      else
        ret = find(pWord + 1, pParent->getCenter(), ppTerminal);
    }
  }
  return ret;
}

// Perform an inexact, "fuzzy" lookup of a word
//
// @In:     @pWord pointer to null-terminated string
//          @pParent pointer to current parent node
// @Out:    true == match found
//          @map key/value pair map with tiebroken score and word
void TernaryTree::fuzzyFind(
    const char *pWord,
    TNode *pParent,
    std::map< int, std::string > *pWords)
{
  std::string word = pWord;
  TNode *pNode = NULL;
  while (word.length() > 1)
  {
    VERBOSE_LOG(LOG_INFO,  "SEARCHING " << word.c_str() << "(" << pWord << ")" << std::endl);
    if (find(word.c_str(), pParent, &pNode)) {
      (*pWords)[0] = word.c_str();
      break;
    }
    if (pNode)
      break;

    VERBOSE_LOG(LOG_INFO,  "NO \"" << word.c_str() << "\" found; ");

    word = word.substr(0, word.length() - 1);
    VERBOSE_LOG(LOG_INFO,  "TRYING: " << word.c_str() << "(" << pWord << ")" << std::endl);
  }

  // now extrapolate and score possibilities from stem
  if (pNode)
  {
    std::deque< UCHAR > accum;
    //                accum.resize(1 << 16);
    VERBOSE_LOG(LOG_INFO,  "TRYING " << word.c_str() << "(" << pWord << ")" << std::endl);
    extrapolateAll(pNode, pWords, &accum, word.c_str(), pWord);
  }
}

// extrapolateAll
// extrapolate all possibilities from an input strung.
//
// @In:   pNode pointer to starting node
//        pWords vector of words
// @Out:  at least one match found
//        pWords filled with words from starting node
bool TernaryTree::extrapolateAll(
    TNode *pNode,
    std::map< int, std::string > *pWords,
    std::deque< UCHAR > *accum,
    const char *pStem,
    const char *pWord)
{
  if (pNode) {
    extrapolate(pNode, pNode->getCenter(), pWords, accum, pStem, pWord);
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
// @In:     pNode pointer to starting node
//          pWords map of words, keyed by score
// @Out:    true == match found
//          pVect filled with words from starting node
bool TernaryTree::extrapolate(
    TNode *pRoot,
    TNode *pNode,
    std::map< int, std::string > *pWords,
    std::deque< UCHAR > *accum,
    const char *pStem,
    const char *pWord,
    int depth)
{
  if (!pNode)
    return false;

  TNode *pChild = NULL;
  bool ret = false;

  // Is this the end of a full word, ergo "o" in "piano"?
  if (pNode->getTerminator()) {
    VERBOSE_LOG(LOG_DEBUG,  "TERMINATOR: " << pNode << std::endl);
    std::string word, compound;
    TNode *pCur = pNode;
    while (pCur != pRoot && pCur) {
      // Push this node's key onto our candidate accumulator
      accum->push_front(pCur->getKey());
      pCur = pCur->getParent();
    }
    // Reverse stack.
    // NOTE: this could be more efficient with a deque
    auto commit = *accum;
    while (!commit.empty()) {
      VERBOSE_LOG(LOG_DEBUG,  commit.front());
      word.push_back(commit.front());
      commit.pop_front();
    }
    VERBOSE_LOG(LOG_DEBUG,  std::endl);

    compound = pStem;
    compound = compound.substr(0, compound.length());
    compound += "|";
    compound += word;
    VERBOSE_LOG(LOG_DEBUG,  "ADDING " << compound.c_str() << std::endl);

    // print the score out here for now.
    int score = getLevenshtein(pWord, compound.c_str());
    // Note: limit of 64 ties.
    int tieBreaker = 0;
    while ((pWords)->count(tieBreaker + (score << 6)) != 0)
      tieBreaker++;
    (*pWords)[ tieBreaker + (score << 6)] = compound;
    VERBOSE_LOG(LOG_DEBUG,  "SCORING " << pWord << " =|= " << compound.c_str() << " SCORE: " << score << std::endl);
    accum->clear();
  }

  // Recurse
  if ((pChild = pNode->getLeft())) {
    if (extrapolate(pRoot, pChild, pWords, accum, pStem, pWord, depth + 1)) {
      ret |= true;
      if (!accum->empty() &&
          !pChild->getLeft() && !pChild->getCenter() && !pChild->getRight())
      {
      }
    }
  }

  if ((pChild = pNode->getCenter())) {
    if (extrapolate(pRoot, pChild, pWords, accum, pStem, pWord, depth + 1)) {
      ret |= true;
      if ((!accum->empty() &&
            !pChild->getLeft() && !pChild->getCenter() && !pChild->getRight())) {
      }
    }
  }

  if ((pChild = pNode->getRight())) {
    if (extrapolate(pRoot, pChild, pWords, accum, pStem, pWord, depth + 1)) {
      ret |= true;
      if ((!accum->empty() &&
            !pChild->getLeft() && !pChild->getCenter() && !pChild->getRight())) {
      }
    }
  }

  return ret;
}


// allocNode
// insert a node into the tree
//
// @In: key key of node to add
// @Out: Pointer to node
TNode *TernaryTree::allocNode(char key)
{
  TNode *pNode = new TNode(key);
  assert(pNode);
  pNode->setKey(key);
  return pNode;
}

// Utility preprocessor macro for Levenshtein
#if !defined(MIN3)
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
#else
#error MIN3 Already defined!
#endif

// getLevenshtein
//
// This is an optimized Levenshtein string distance calculation.
// It returns how "different" two strings are, effectively performing
// a commutative subtraction operation.
//
// @In: s1 string #1
// s2 string #2
// @Out: Levenshtein difference
int TernaryTree::getLevenshtein(const char *s1, const char *s2)
{
  int score = -1;
  if (s1 && s2) {
    unsigned int len1, len2, x, y, lastdiag, olddiag;
    len1 = strlen(s1);
    len2 = strlen(s2);
    unsigned int column[ len1 + 1 ];
    for (y = 1; y <= len1; y++)
      column[ y ] = y;
    for (x = 1; x <= len2; x++) {
      column[ 0 ] = x;
      for (y = 1, lastdiag = x - 1; y <= len1; y++) {
        olddiag = column[ y ];
        column[y] = MIN3(column[ y ] + 1,
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
