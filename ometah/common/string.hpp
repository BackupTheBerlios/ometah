/***************************************************************************
 *  $Id: string.hpp,v 1.7 2006/05/13 10:05:53 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#ifndef STRING
#define STRING

#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace ometah {

//! Print a vector
template<class T>
void print( T aVector, char * aSeparator=",", ostream * outer)
{
  *outer << aVector[0];
  
  for( unsigned int i=1; i<aVector.size(); i++ ) {
    *outer << aSeparator << aVector[i];
  }
}

//! Print a vector
template<class T>
string print( T aVector, char * aSeparator="," )
{
  ostringstream out;
  out << aVector[0];
  
  for( unsigned int i=1; i<aVector.size(); i++ ) {
    out << aSeparator << aVector[i];
  }

  return out.str();
}

//! Print a matrix
template<class T>
string print( vector< vector< T > > aMatrix, char * sepCol = ",", char * sepLine = "\n")
{
  ostringstream out;
  out << print(aMatrix[0]);
  
  for( unsigned int i=1; i<aMatrix.size(); i++ ) {
    out << sepLine << print(aMatrix[i],sepCol);
  }

  return out.str();
}


//! Make a string vector from a string
/*!
  This version modify directly the vector by reference
 */
/*
template<template<class T> class V>
void stringSplit (string text, string separators, V<string> & words)
{
  int n = text.length();
  int start, stop;
 
  start = text.find_first_not_of(separators);
  while ((start >= 0) && (start < n)) {
    stop = text.find_first_of(separators, start);

    if ((stop < 0) || (stop > n)) stop = n;

    words.push_back(text.substr(start, stop - start));
    start = text.find_first_not_of(separators, stop+1);
  }
};
*/

//! Make a string vector from a string
vector<string> stringSplit (string text, string separators);


//! Make a double vector from a string vector
vector<double> stringToDouble(vector<string> VString);


//! Make a double from a string
double stringToDouble(string aString);


//! Make a double vector from a string, with shortcut options
/*! 
  Examples :
    "1.2,1" => <1.2,1>
    "3:1.0" => <1.0,1.0,1.0>
*/
vector<double> stringToDouble_shortcuts(string aString, string itemSeparator=",", string multiSeparator=":");

} // namespace ometah

#endif
