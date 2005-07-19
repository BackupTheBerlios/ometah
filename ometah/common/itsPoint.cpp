/***************************************************************************
 * $Id: itsPoint.cpp,v 1.3 2005/07/19 09:15:52 nojhan Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
 * Author : Johann Dréo <nojhan@gmail.com>
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

#include <iostream>
#include <sstream>

#include "itsPoint.hpp"

using namespace std;

//! Get the solution vector
vector<double> itsPoint::getSolution()
{
  return this->solution;
}

//! Set the solution vector
void itsPoint::setSolution(vector<double> solution)
{
  this->solution = solution;
}
  
//! Get the value vector
vector<double> itsPoint::getValues()
{
  return this->values;
}
 
//! Set the value vector
void itsPoint::setValues(vector<double> values)
{
  this->values = values;
}


unsigned int itsPoint::getSolutionDimension()
{
    return this->solution.size();
}


unsigned int itsPoint::getValuesNumber()
{
    return this->values.size();
}

bool isValueSmaller(itsPoint p1, itsPoint p2, int dimension)
{
    if ( p1.getValues().size() <= 0 || p2.getValues().size() <= 0 ) {
      ostringstream msg;
      msg << "ErrorSize: point has no value" 
          << "(p1: " << p1.getValues().size() 
          << " values, p2: " << p2.getValues().size()
          << " values)";
      throw msg.str().c_str();
    }
    return (p1.getValues()[dimension] < p2.getValues()[dimension]);
}


bool isValueGreater(itsPoint p1, itsPoint p2, int dimension)
{
    return (p1.getValues()[dimension] > p2.getValues()[dimension]);
}


bool isValueEqual(itsPoint p1, itsPoint p2, int dimension)
{
    return (p1.getValues()[dimension] == p2.getValues()[dimension]);
}


vector<itsPoint> sortOnValues(vector<itsPoint> vec, int dimension)
{
    if( vec.size() <= 1) {
        return vec;
    
    } else {
        // cut
        unsigned int cent = vec.size()/2;
    
        vector<itsPoint> v1(vec.begin(),vec.begin()+cent);
        vector<itsPoint> v2(vec.begin()+cent,  vec.end());
        // sort
        vector<itsPoint> vec1 = sortOnValues(v1, dimension);
        vector<itsPoint> vec2 = sortOnValues(v2, dimension);
        
        // merge
        vector<itsPoint> res;
        unsigned int k=0;
    
        for(unsigned int i=0; i<vec1.size(); i++) {
            while( k<vec2.size() && isValueSmaller(vec2[k],vec1[i],dimension) ) {
                res.push_back(vec2[k]);
                k++;
            }
            res.push_back(vec1[i]);
        }
        while(k<vec2.size()) {
            res.push_back(vec2[k]);
            k++;
        }
        return res;
    }
}

string printValues( vector<itsPoint> vec, int dimension, const char* separatorDimension, const char* separatorPoint )
{
    if( vec.size() <= 0 ) {
        throw "ErrorSize: cannot print an empty vector";
    
    } else {
        if ( vec[0].getValues().size()<=0 || dimension > (signed int)vec[0].getValues().size() ) {
            throw "ErrorSize: cannot print a vector of empty points";
        }
    }

    ostringstream res;

    if(dimension>-1) { // if we want a specific dimension
        
        res << vec[0].getValues()[dimension];
        for(unsigned int i=1; i<vec.size(); i++) { // points
            res << separatorDimension << vec[i].getValues()[dimension];
        }
    
    } else { // if we want all dimensions
    
        res << vec[0].getValues()[0];
        for(unsigned int i=1; i<vec.size(); i++) { // points
            res << separatorPoint << vec[i].getValues()[0];
            for(unsigned int j=1; j<vec[i].getValues().size(); j++) { // dimensions
                res << separatorDimension << vec[i].getValues()[j];
            }
        }
    }
    return res.str();
}


string printSolutions( vector<itsPoint> vec, int dimension, const char* separatorDimension, const char* separatorPoint )
{
    if( vec.size() <= 0 ) {
        throw "ErrorSize: cannot print an empty vector";
    
    } else {
        if ( vec[0].getSolution().size()<=0 || dimension > (signed int)vec[0].getSolution().size() ) {
            throw "ErrorSize: cannot print a vector of empty points";
        }
    }

    ostringstream res;

    if(dimension>-1) { // if we want a specific dimension
        
        res << vec[0].getSolution()[dimension];
        for(unsigned int i=1; i<vec.size(); i++) { // points
            res << separatorDimension << vec[i].getSolution()[dimension];
        }
    
    } else { // if we want all dimensions
    
        // first point
        res << vec[0].getSolution()[0];
        for(unsigned int j=1; j<vec[0].getSolution().size(); j++) { // dimensions
            res << separatorDimension << vec[0].getSolution()[j];
        }
        
        // other points
        for(unsigned int i=1; i<vec.size(); i++) { // points
            res << separatorPoint << vec[i].getSolution()[0];
            for(unsigned int j=1; j<vec[i].getSolution().size(); j++) { // dimensions
                res << separatorDimension << vec[i].getSolution()[j];
            }
        }
    }
    return res.str();
}
