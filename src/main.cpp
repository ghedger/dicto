// spell.cpp
//
// This is a sample implementation of a spell checker with Levenshtein-sorted
// possibility selection.
//
//
//
// Copyright (C) 2014 Gregory P. Hedger

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
#include "templ_node.h"
#include "ternary_tree.h"
#include "heap_mgr.h"

// declutter: allow cout instead of std::cout
using namespace std;

// readDictionaryFile
// Read dictionary file into our data structure.
//
// @In:     -
// @Out:    -
void readDictionaryFile( const char *path, TernaryTree *pTree, TNode *& pRoot )
{
    try
    {
        ifstream file( path );
        string line;
        while( getline( file, line ) )
        {
            pTree->insert( line.c_str(), &pRoot );
        }
    }
    catch(...)
    {
        cout << "Error reading file.";
    }
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

// main
// This is the main entry point and testbed for ternary tree.
//
// @In:     -
// @Out:    0 == success
int main()
{
    const int MAX_IN = 128;
    TNode *pRoot = NULL;
    TernaryTree t;

    readDictionaryFile( "dict.txt", &t, pRoot );

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


