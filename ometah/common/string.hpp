/***************************************************************************
 *  $Id: string.hpp,v 1.1 2005/06/13 09:05:02 nojhan Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
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

using namespace std;

//! Print a vector
template<class T>
void print( T aVector, char * aSeparator=",", ostream * outer)
{
  *outer << aVector[0];
  
  for( unsigned int i=1; i<aVector.size(); i++ ) {
    *outer << aSeparator << aVector[i];
  }
};

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
};


//! Make a double vector from a string vector
/*
template<template <class T> class V>
V<double> stringToDouble(V<string> VString)
{

  double aDouble;
  V<double> VDouble;

  VDouble.reserve( VString.size() );

  for (unsigned int i=0; i<VString.size(); i++) {
    aDouble = atof( VString[i].c_str() );
    VDouble.push_back(aDouble);
  }
  return VDouble;
}
*/

//! Make a double from a string
/*
double stringToDouble(string aString)
{
  double aDouble;

  aDouble = atof( aString.c_str() );

  return aDouble;
};
*/

//! Make a double vector from a string, with shortcut options
/*! 
  Examples :
    "1.2,1" => <1.2,1>
    "3:1.0" => <1.0,1.0,1.0>
*/

/*
template<template <class T> class V>
V<double> stringToDouble(string aString, string itemSeparator=",", string multiSeparator=":")
{
  V<double> res;
  
  // if find a separator
  if ( aString.find(multiSeparator) < aString.size() ) {
    V<double> tempvec = stringToDouble( stringSplit( aString, multiSeparator ) );
    res.reserve( (int)tempvec[0]);

    for(unsigned int i=0; i<tempvec[0]; i++) {
        res.push_back(tempvec[1]);
    }
  } else {
    res = stringToDouble( stringSplit( aString, itemSeparator ) );
  }
  return res;
}
*/

//! Make a string vector from a string
/*!
  This version modify directly the vector by reference
 */
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

//! Make a string vector from a string
template<template<class T> class V>
V<string> stringSplit (string text, string separators)
{
  int n = text.length();
  int start, stop;
 
  V<string> words;

  start = text.find_first_not_of(separators);
  while ((start >= 0) && (start < n)) {
    stop = text.find_first_of(separators, start);

    if ((stop < 0) || (stop > n)) stop = n;

    words.push_back(text.substr(start, stop - start));
    start = text.find_first_not_of(separators, stop+1);
  }

  return words;
};

#endif
