/***************************************************************************
 *  $Id: itsProblem.cpp,v 1.15 2006/05/16 13:51:38 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; either version 2.1 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General
Public License along with this program; if not, write to the Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA
*/
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../common/string.hpp"
#include "itsProblem.hpp"

using namespace std;

namespace ometah {

/*
itsProblem::~itsProblem()
{
}
*/

/*
//! the default constructor
itsProblem::itsProblem()
{
  setName("");
  setDescription("");
  setCitation("");
  setFormula("f(x)");
  
  dimensionFixed = false;
  setDimension(1);

  setBoundsMaximaAll(1);
  setBoundsMinimaAll(0);

  setAccuracy(0.1);

  // A fake optimum
  vector<itsPoint> optim;
  itsPoint pt;
  vector<double> sol(getDimension(),0);
  pt.setSolution(sol); // a vector of 0
  vector<double> val(1,0);
  pt.setValues(val); // a value of 0
  optim.push_back(pt);
  setOptima(optim);
}
*/

//~ /*
//~ string itsProblem::getName() 
//~ {
  //~ return this->name;
//~ }
//~ */

//~ void itsProblem::setName(string name)
//~ {
  //~ this->name = name;
//~ }

//~ /*
//~ string itsProblem::getDescription()
//~ {
  //~ return this->description;
//~ }
//~ */

//~ void itsProblem::setDescription(string description)
//~ {
  //~ this->description = description;
//~ }

//~ /*
//~ string itsProblem::getCitation()
//~ {
  //~ return this->citation;
//~ }
//~ */

//~ void itsProblem::setCitation(string citation)
//~ {
  //~ this->citation = citation;
//~ }

//~ /*
//~ unsigned int itsProblem::getDimension()
//~ {
  //~ return this->dimension;
//~ }
//~ */

void itsProblem::setDimension(unsigned int dimension)
{
  // if we can change the dimension
  if ( isDimensionFixed() == false ) {
    this->dimension = dimension;
  
  // else, if we cannot change the dimension
  } else {
    cerr << "Warning: dimension cannot be changed, " 
         << this->dimension << " will be used." << endl;
  }

  // reinitialization of bounds
  vector<double> n(dimension,0);
  setBoundsMinima( n );
  vector<double> x(dimension,1);
  setBoundsMaxima( x );
}

/*
bool itsProblem::isDimensionFixed()
{
    return dimensionFixed;
}
*/
  

void itsProblem::setDimensionFixed(unsigned int dimension)
{
    dimensionFixed = true;

    this->dimension = dimension;

    // reinitialization of bounds
    vector<double> n(dimension,0);
    setBoundsMinima( n );
    vector<double> x(dimension,1);
    setBoundsMaxima( x );
}


string itsProblem::getInformations()
{
  // separator used to format output
  string sep = ":\t";
  
  // put all in the stream
  stringstream msg;
  msg << "Key" << sep << getKey() << endl;
  msg << "Name" << sep << getName() << endl;
  msg << "Description" << sep << getDescription() << endl;
  msg << "Formula" << sep << getFormula() << endl;
  msg << "Dimension" << sep << getDimension() << (isDimensionFixed()?" (fixed)":" (not fixed)") << endl;
  

  if ( getOptima().size() <= 0 ) {
      msg << "Optimum Unknown" << endl;
  } else {
      // print vectors of the optimums
      for(unsigned int i=0;i<getOptima().size();i++) {
          if(getOptima().size()>1) {
              msg << "Optimum " << i << sep;
          } else {
              msg << "Optimum " << sep;
          }
          msg << "<";
          print(getOptima()[i].getValues()," ",&msg);
          msg << "> @ <";
          print(getOptima()[i].getSolution()," ",&msg);
          msg << ">";
          msg << endl;
      }
  }

  // print vectors of bounds
  msg << "Bounds" << sep << "[";
  for(unsigned int i=0;i<getBounds().size();i++) {
      msg << "[";
      print(getBounds()[i]," ",&msg);
      msg << "]";
  }
  msg << "]" << endl;

  msg << "Reference" << sep << getCitation() << endl;

  // return only the string
  return msg.str();  
}


string itsProblem::getInformations_XML()
{
  // put all in the stream
  stringstream msg;
  
  msg << "<problem>" << endl;

  msg << "<key>" << getKey() << "</key>" << endl;
  msg << "<name>" << getName() << "</name>" << endl;
  msg << "<description>" << getDescription() << "</description>" << endl;
  msg << "<formula>" << getFormula() << "</formula>" << endl;
  msg << "<dimension>" << getDimension() << "</dimension>" << endl;
  msg << "<accuracy>" << getAccuracy() << "</accuracy>" << endl;
  

  msg << "<optimums>" << endl;
  if ( getOptima().size() <= 0 ) {
      msg << "Unknown";
  } else {
      // print vectors of the optimums
      for(unsigned int i=0;i<getOptima().size();i++) { 
          if(getOptima().size()>1) {
              msg << "<point id=\"" << i << "\">" << "<values>";
          } else {
              msg << "<point>" << "<values>";
          }
          
          print(getOptima()[i].getValues()," ",&msg);
          msg << "</values>" << "<solution>";
          print(getOptima()[i].getSolution()," ",&msg);
          msg << "</solution>" << "</point>" << endl;
      
      }
  }
  msg << "</optimums>" << endl;

  // print vectors of bounds
  msg << "<bounds>" << endl;

  msg << "<minimums>";
    msg << "<point>" << "<solution>";
    msg << print( getBoundsMinima(), " ");
    msg << "</solution>" << "</point>";
  msg << "</minimums>" << endl;

  msg << "<maximums>";
    msg << "<point>" << "<solution>";
    msg << print( getBoundsMaxima(), " ");
    msg << "</solution>" << "</point>";
  msg << "</maximums>" << endl;

  msg << "</bounds>" << endl;

  // references
  msg << "<reference>" << getCitation() << "</reference>" << endl;


  msg << "</problem>" << endl;

  // return only the string
  return msg.str();  
}



vector<itsPoint> itsProblem::getOptima()
{
    if ( this->optima.size() > 0 ) {
        // if the optimum does not correspond to dimension
        if( this->optima[0].getSolutionDimension() == 1 ) {
        
            // we duplicate its solutions to match the dimension
            for( unsigned int i=0; i < this->optima.size(); i++ ) {
                vector<double> sol(getDimension(), this->optima[i].getSolution()[0] );
                this->optima[i].setSolution(sol);
            }
        }
    }

    return this->optima;
}

/*
void itsProblem::setOptima(vector<itsPoint> theOptima)
{
    optima = theOptima;
}
*/

vector<vector<double> > itsProblem::getBounds()
{
    vector<vector<double> > bounds;
    bounds.reserve(getDimension());

    for(unsigned int i=0; i < getDimension(); i++) {
        double mini = getBoundsMinima()[i];
        double maxi = getBoundsMaxima()[i];
        vector<double> v; 
        v.reserve(2);
        v.push_back(mini);
        v.push_back(maxi);
        bounds.push_back(v);
    }

    return bounds;
}

void itsProblem::setBounds(vector<vector<double> > bounds)
{
    vector<double> mins, maxs;
    mins.reserve(getDimension());
    maxs.reserve(getDimension());

    for(unsigned int i=0; i < getDimension(); i++) {
        mins.push_back(bounds[i][0]);
        maxs.push_back(bounds[i][1]);
    }
    
    setBoundsMinima(mins);
    setBoundsMaxima(maxs);
}

/*
vector<double> itsProblem::getBoundsMinima()
{
    return this->boundsMinima;
}
*/

void itsProblem::setBoundsMinima(vector<double> minima)
{
    if(minima.size() != getDimension() ) {
        ostringstream msg;
        msg << "ErrorSize: number of bounds (" << minima.size() 
            << ") does not correspond to dimension  (" << getDimension() << ")";
        throw Exception_Size_Match(msg.str(), EXCEPTION_INFOS );
    } else {
        this->boundsMinima = minima;
    }
}

/*
vector<double> itsProblem::getBoundsMaxima()
{
    return this->boundsMaxima;
}
*/

void itsProblem::setBoundsMaxima(vector<double> maxima)
{
    if(maxima.size() != getDimension() ) {
        ostringstream msg;
        msg << "ErrorSize: number of bounds (" << maxima.size() 
            << ") does not correspond to dimension  (" << getDimension() << ")";
        throw Exception_Size_Match(msg.str(), EXCEPTION_INFOS );
    } else {
        this->boundsMaxima = maxima;
    }
}
  
void itsProblem::setBoundsMinimaAll(double min)
{
    // empty vector
    vector<double> v;
    v.reserve(getDimension());

    // fill with min
    for(unsigned int i=0; i < getDimension(); i++ ) {
        v.push_back(min);
    }

    // update
    setBoundsMinima(v);
}

void itsProblem::setBoundsMaximaAll(double max)
{
    vector<double> v;
    v.reserve(getDimension());

    for(unsigned int i=0; i < getDimension(); i++ ) {
        v.push_back(max);
    }
    setBoundsMaxima(v);
}

/*
string itsProblem::getFormula()
{
    return this->formula;
}
*/

void itsProblem::setFormula(string formula)
{
    this->formula = formula;
}

/*
string itsProblem::getKey()
{
    return this->key;
}
*/
/*
void itsProblem::setKey( string key )
{
    this->key = key;
}
*/
/*
float itsProblem::getAccuracy()
{
  return this->accuracy;
}
*/

void itsProblem::setAccuracy(float acc)
{
  this->accuracy = acc;
}

} //ometah
