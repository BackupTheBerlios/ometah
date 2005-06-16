/***************************************************************************
 * $Id: itsPoint.hpp,v 1.2 2005/06/16 08:55:11 nojhan Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
 * Author :  Walid TFAILI <tfaili@univ-paris12.fr>
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
 
#ifndef ITSPOINT
#define ITSPOINT
 
#include <vector>
#include <functional>

using namespace std;
 
//! The point abstraction
/*! 
  A point is a value and a solution vector
*/
class itsPoint
{
protected:
  //! the solution vector 
  vector<double> solution;
  //! the value vector
  vector<double> values;

public:
  //! Get the solution vector
  vector<double> getSolution();

  //! Set the solution vector
  void setSolution(vector<double> solution);
  
  //! Get the value vector
  vector<double> getValues();
 
  //! Set the value vector
  void setValues(vector<double> values);

  //! Return the dimension of the solution
  unsigned int getSolutionDimension();

  //! Return the number of values
  unsigned int getValuesNumber();

};

//! Check if a point has a smaller value than another one for a given dimension
bool isValueSmaller(itsPoint p1, itsPoint p2, int dimension=0);

//! Check if a point has a greater value than another one for a given dimension
bool isValueGreater(itsPoint p1, itsPoint p2, int dimension=0);

//! Check if a point has an equal value than another one for a given dimension
bool isValueEqual(itsPoint p1, itsPoint p2, int dimension=0);

//! Sort a vector on values for a given dimension
/*!
    It uses a merge sorting algorithm
*/
vector<itsPoint> sortOnValues(vector<itsPoint> vec, int dimension);


//! Print the values of a point
/*!
    If a dimension is given, it print only values for this dimension.
    Else, it print all values.
    By default :
|11 12 13|
|21 22 23| => 11,12,13  21,22,23  31,32,33
|31 32 33|
*/
string printValues( vector<itsPoint> vec, int dimension=-1, const char* separatorDimension=",", const char* separatorPoint="  " );

//! Print the solution of a point
/*!
    If a dimension is given, it print only solutions for this dimension.
    Else, it print all values.
    By default :
|11 12 13|
|21 22 23| => 11,12,13  21,22,23  31,32,33
|31 32 33|
*/
string printSolutions( vector<itsPoint> vec, int dimension=-1, const char* separatorDimension=",", const char* separatorPoint="  " );

#endif
