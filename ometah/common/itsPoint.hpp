/***************************************************************************
 * $Id: itsPoint.hpp,v 1.9 2006/05/11 21:59:46 nojhan Exp $
 *  Copyright : Free Software Foundation
 * Author :  Walid TFAILI <tfaili@univ-paris12.fr>
 * Author : Johann Dré <nojhan@gmail.com>
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

#define SELECT_QUICKSORT_RATIO_LIMIT 0.9

#include <list>
#include <vector>
//#include <functional>
#include <algorithm>

#include "Exception_oMetah.hpp"

#include<iostream>

using namespace std;
 
//! The point abstraction
/*! 
  A point is a value and a solution vector
*/
class itsPoint
{
//protected:
public:
  //! the solution vector 
  vector<double> solution;
  //! the value vector
  vector<double> values;

public:
  //! The constructor without parameters
  /*!
    One must use setSolution and setValues to initialise the point 
    after its instanciation.
  */
  itsPoint() : solution(vector<double>(1,0)), values(vector<double>(1,0)) {}
  
  ~itsPoint() 
  {
  /*
        clog << "~itsPoint:" << this << endl;
        clog << "~itsPoint.&solution:" << &solution << endl;
        clog << "~itsPoint.&values:" << &values << endl;
  */
  }
  
  //! Constructor with explicit initialization of the solution
  itsPoint( vector<double> theSolution ) : solution(theSolution) {}
  
  //! Constructor with explicit initialization of solution & value
  itsPoint( vector<double> theSolution, vector<double> theValues ) : solution(theSolution), values(theValues) {}

  //! Get the solution vector
  vector<double> getSolution() const {return solution;}

  //! Set the solution vector
  void setSolution(const vector<double> & solution);
  
  //! Get the value vector
  vector<double> getValues() const {return values;}
 
  //! Set the value vector
  void setValues(const vector<double> & values);

  //! Put only one value in the value vector 
  void setValue(const double value);

  //! Return the dimension of the solution
  unsigned int getSolutionDimension() const {return solution.size();}

  //! Return the number of values
  unsigned int getValuesNumber() const {return values.size();}

  //! Lesser than operator for two points, compare on value (used for sorting)
  bool operator<(const itsPoint & p1) const {return isValueSmaller(*this,p1);}

  //! Greater than operator for two points, compare on value (used for sorting)
  bool operator>(const itsPoint & p1) const {return isValueGreater(*this,p1);}

  //! Equality operator for two points, compare on value
  bool operator==(const itsPoint & p1) const {return isValueEqual(*this,p1);}


//! Check if a point has a smaller value than another one for a given dimension
friend bool isValueSmaller(const itsPoint & p1, const itsPoint & p2, int dimension=0);

//! Check if a point has a greater value than another one for a given dimension
friend bool isValueGreater(const itsPoint & p1, const itsPoint & p2, int dimension=0);

//! Check if a point has an equal value than another one for a given dimension
friend bool isValueEqual(const itsPoint & p1, const itsPoint & p2, int dimension=0);


//! Sort a vector on values for a given dimension
/*!
    It uses a quicksort algorithm
*/
friend vector<itsPoint> sortOnValues(vector<itsPoint> & vec, int dimension);


//! Select the bests values in a sample
/*!
  You have the choice between two algorithms  (quicksort or comparison)
  at the compilation stage
*/
friend vector<itsPoint> selectOnValues(vector<itsPoint> & vec, unsigned int selectNumber);


//! Print the values of a point
/*!
    If a dimension is given, it print only values for this dimension.
    Else, it print all values.
    By default :
|11 12 13|
|21 22 23| => 11,12,13  21,22,23  31,32,33
|31 32 33|
*/
friend string printValues( vector<itsPoint> vec, int dimension=-1, const char* separatorDimension=",", const char* separatorPoint="  " );

//! Print the solution of a point
/*!
    If a dimension is given, it print only solutions for this dimension.
    Else, it print all values.
    By default :
|11 12 13|
|21 22 23| => 11,12,13  21,22,23  31,32,33
|31 32 33|
*/
friend string printSolutions( vector<itsPoint> vec, int dimension=-1, const char* separatorDimension=",", const char* separatorPoint="  " );

};

#endif
