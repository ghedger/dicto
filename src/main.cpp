/* Dicto
 * main.cpp
 *
 * Copyright (C) 2014 Gregory P. Hedger
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

// outPreamble
//
// @In:     -
// @Out:    -
void outPreamble()
{
    cout <<  " <program>  Copyright (C) 2015 Gregory P. Hedger" << endl;
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

    outPreamble();

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


