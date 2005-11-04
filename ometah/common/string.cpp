/***************************************************************************
 *  $Id: string.cpp,v 1.3 2005/11/04 17:28:12 nojhan Exp $
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
 

#include "string.hpp"

vector<string> stringSplit (string text, string separators)
{
  int n = text.length();
  int start, stop;
 
  vector<string> words;

  start = text.find_first_not_of(separators);
  while ((start >= 0) && (start < n)) {
    stop = text.find_first_of(separators, start);

    if ((stop < 0) || (stop > n)) stop = n;

    words.push_back(text.substr(start, stop - start));
    start = text.find_first_not_of(separators, stop+1);
  }

  return words;
}


vector<double> stringToDouble(vector<string> VString)
{

  double aDouble;
  vector<double> VDouble;

  VDouble.reserve( VString.size() );

  for (unsigned int i=0; i<VString.size(); i++) {
    aDouble = atof( VString[i].c_str() );
    VDouble.push_back(aDouble);
  }
  return VDouble;
}


double stringToDouble(string aString)
{
  double aDouble;

  aDouble = atof( aString.c_str() );

  return aDouble;
}


vector<double> stringToDouble_shortcuts(string aString, string itemSeparator, string multiSeparator)
{
  vector<double> res;
  
  // if find a separator
  if ( aString.find(multiSeparator) < aString.size() ) {
    vector<double> tempvec = stringToDouble( stringSplit( aString, multiSeparator ) );
    res.reserve( (int)tempvec[0]);

    for(unsigned int i=0; i<tempvec[0]; i++) {
        res.push_back(tempvec[1]);
    }
  } else {
    res = stringToDouble( stringSplit( aString, itemSeparator ) );
  }
  return res;
}
