/***************************************************************************
 * $Id: itsSet.hpp,v 1.2 2005/11/04 17:28:12 nojhan Exp $
 *  Copyright : Free Software Foundation
 * Author: Walid TFAILI <tfaili@univ-paris12.fr>
 * Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
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
 
#ifndef ITSSET
#define ITSSET

#include <string>
#include <hash_map.h>
#include <ostream>

#include "logic.hpp"

using namespace std;
 
//! A common class for all sets of objects
/*!
  It uses templates to permit any kind of instanciation
*/ 
template <class T>
class itsSet
{
protected:

  //! The collection of instance
  /*! 
    The collection hash map contains T classes.
    It associate a string to an object.
    The string is the name of the object (from instance.getName())
  */
  hash_map< const char*, T, hash<const char*>, eqstr > collection;

  //! The current choice 
  string currentItem;

  
public:
  //! Add an object
  /*!  
    Add a T in the collection hash map, which is of the form
    [<string> => <object>]
    It use the "name" method to get the key associated to the object.
  */
  void add(T anItem)
  {
    // key is based on a const char*, so we take the c_str
    collection[anItem->getKey().c_str()] = anItem;
  };
  
  //! Remove an object
  /*! 
    Remove a T member from the collection hash map, specifying its key
  */
  void remove(string name)
  {
    // find the element iterator
    // erase the element
    collection.erase(
		     collection.find(name.c_str())
		     );
  };

  //! Change the current choice
  void choose(string name)
  {
    this->currentItem = name;
  };

  //! Return a pointer on the current object
  T item()
  {
    return collection[currentItem.c_str()];
  };

  //! Return a list of the available keys  
  vector<string> getKeyList() 
  {
    vector<string> list;
    typename hash_map< const char*, T, hash<const char*>, eqstr > ::iterator current = collection.begin();
    typename hash_map< const char*, T, hash<const char*>, eqstr > ::iterator end = collection.end();
    while ( current != end ) {
        string s = (*current).first;
        list.push_back(s);
        current++;
    }
    return list;
  }

};

#endif
