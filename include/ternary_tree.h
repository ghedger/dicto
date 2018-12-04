/* Dicto
 * ternarytree_.h
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

#pragma once

#include <map>
#include <string>
#include <queue>
#include <deque>
#include <stack>
#include "templ_node.h"

typedef unsigned char UCHAR;

//#define DEBUG
#define INFO

// TNode
// This is the instantiable class from my TemplNode template
class TNode : public TemplNode <UCHAR, TNode> {
 public:
  TNode() {};
  TNode(UCHAR key) : TemplNode <UCHAR, TNode> (key) { };
  ~TNode() {};
  void SetKey(UCHAR key)
  {
      if(isupper(key) )
      {
          SetUpper();
      }
      key_ = (UCHAR) tolower(key);
  }
};

// TernaryTree
// This class is the tree itself. It manages a ternary search tree of TNodes
// This is a dictionary and allows us quick word lookup
// with a yes/no response.
class TernaryTree {
 public:
  TernaryTree() {};
  ~TernaryTree() {};

  // insertNode
  // insert a node into the tree
  //
  // @In: pWord pointer to null-terminated string
  // ppParent pointer to parent pointer
  // @Out: Node *
  TNode * Insert(const char *pWord, TNode **ppNode = NULL);
  // find
  // find a word
  //
  // @In:     @pWord pointer to null-terminated string
  //          @pParent pointer to current parent node
  //          @ppTerminal pointer to terminal node pointer
  // @Out:    true == match found
  bool Find(const char *pWord, TNode *pParent, TNode ** ppTerminal = NULL);

  // Perform an inexact, "fuzzy" lookup of a word
  //
  // @In:     @pWord pointer to null-terminated string
  //          @pParent pointer to current parent node
  // @Out:    true == match found
  //          @map key/value pair map with tiebroken score and word
  void FuzzyFind(
    const char *pWord,
    TNode *pParent,
    std::map< int, std::string > *pWords);
 // extrapolateAll
  // extrapolate all possibilities from an input strung.
  //
  // @In: pNode pointer to starting node
  // pVect vector of words
  // @Out: -
  // pVect filled with words from starting node
  bool ExtrapolateAll(
    TNode *pNode,
    std::map< int, std::string > *pWords,
    std::deque< UCHAR > *accum,
    const char *pStem,
    const char *pWord);

  // extrapolate
  // extrapolate from a word stem.
  //
  // I selected a deque as my accumulator data structure. A stack
  // requires a second stack or some other means of word reversal,
  // while a deque gives me both FIFO representation and FILO
  // functionality.
  //
  // @In:     pNode pointer to starting node
  //          pVect vector of words
  // @Out:    -
  //          pVect filled with words from starting node
  bool Extrapolate(
    TNode *pRoot,
    TNode *pNode,
    std::map< int, std::string > *pWords,
    std::deque< UCHAR > *accum,
    const char *pStem,
    const char *pWord,
    int depth = 0);

 protected:
  // allocNode
  // insert a node into the tree
  //
  // @In: key key of node to add
  // @Out: Pointer to node
  TNode *AllocNode(char key);

  // Utility preprocessor macro for Levenshtein

  // getLevenshtein
  // This is an optimized Levenshtein string distance calculation.
  // It returns how "different" two strings are, effectively performing
  // a commutative subtraction operation.
  //
  // @In: s1 string #1
  // s2 string #2
  // @Out: Levenshtein difference
  int CalcLevenshtein(const char *s1, const char *s2);
};
