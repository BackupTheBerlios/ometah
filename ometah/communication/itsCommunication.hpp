/***************************************************************************
 *  $Id: itsCommunication.hpp,v 1.5 2006/05/14 17:05:02 nojhan Exp $
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
 
#ifndef ITSCOMMUNICATION
#define ITSCOMMUNICATION

#include "../common/itsPoint.hpp"
#include "../common/logic.hpp"
#include "../common/itsSetItem.hpp"

//#include <hash_map.h>
#include <map>

namespace ometah {

class itsCommunication : public itsSetItem
{
protected:
  //! A unique key identifying the protocol
  //string key;

public:
  //! Virtual destructor
  /*!
    Necessary because of the abstract factory
  */
  virtual ~itsCommunication() {};
  itsCommunication() {};

  //! Call the problem on a point to get its associated values
  virtual itsPoint call(itsPoint point) =0;

  //!
  //virtual void initialization(hash_map<string,string, eqstr> args) {};
  virtual void initialization(map<string,string> args) {};
  
  //! The minima of the problem bounds
  virtual vector<double> boundsMinima() =0;

  //! The maxima of the problem bounds
  virtual vector<double> boundsMaxima() =0;

  //! The bounds matrix
  /*!  
    This method call getBoundsMinima and getBoundsMaxima to produce
    a matrix with each dimension on a row.
   */
  virtual vector<vector<double> > bounds() =0;

  //! Return the dimension
  virtual int getDimension() =0;
/*
  //! Return the key
  virtual string getKey();
  //! Change the key
  virtual void setKey(string key);
*/
};

}//ometah

#endif
