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
  TernaryTree() { tie_hwm_ = 0; };
  ~TernaryTree() {};
  TNode * Insert(const char *pWord, TNode **ppNode = NULL);
  bool Find(const char *pWord, TNode *pParent, TNode ** ppTerminal = NULL);
  void FuzzyFind(
    const char *pWord,
    TNode *pParent,
    std::map< int, std::string > *pWords);
  bool ExtrapolateAll(
    TNode *pNode,
    std::map< int, std::string > *pWords,
    std::deque< UCHAR > *accum,
    const char *pStem,
    const char *pWord
    );
  bool Extrapolate(
    TNode *pRoot,
    TNode *pNode,
    std::map< int, std::string > *pWords,
    std::deque< UCHAR > *accum,
    const char *pStem,
    const char *pWord,
    const int max_diff = 0,
    int depth = 0);

 int GetMaxTies() { return tie_hwm_; }
 void ClearMaxTies() { tie_hwm_ = 0; }
 protected:
  TNode *AllocNode(char key);
  int CalcLevenshtein(const char *s1, const char *s2);

  // member variables
  int tie_hwm_;
};
