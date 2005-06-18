/***************************************************************************
 *  $Id: itsProblem.cpp,v 1.5 2005/06/18 08:17:59 nojhan Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
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


itsProblem::~itsProblem()
{
}

//! the default constructor
itsProblem::itsProblem()
{
  setName("");
  setDescription("");
  setCitation("");
  setFormula("f(x)");
  
  setDimension(1);

  setBoundsMaximaAll(1);
  setBoundsMinimaAll(0);

  //vector<itsPoint> optim;
  //optim.setSolution(vector<double> sol);
  //optim.setValues(vector<double> val);
  //setOptima(optim);
}

string itsProblem::getName() 
{
  return this->name;
}

void itsProblem::setName(string name)
{
  this->name = name;
}


string itsProblem::getDescription()
{
  return this->description;
}

void itsProblem::setDescription(string description)
{
  this->description = description;
}


string itsProblem::getCitation()
{
  return this->citation;
}

void itsProblem::setCitation(string citation)
{
  this->citation = citation;
}


unsigned int itsProblem::getDimension()
{
  return this->dimension;
}

void itsProblem::setDimension(unsigned int dimension)
{
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
  msg << "Dimension" << sep << getDimension() << endl;
  

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
  

  // print vectors of the optimums
  msg << "<optimums>" << endl;
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
    // if the optimum does not correspond to dimension
    if( this->optima[0].getSolutionDimension() == 1 ) {
    
        // we duplicate its solutions to match the dimension
        for( unsigned int i=0; i < this->optima.size(); i++ ) {
            vector<double> sol(getDimension(), this->optima[i].getSolution()[0] );
            this->optima[i].setSolution(sol);
        }
    }

    return this->optima;
}

void itsProblem::setOptima(vector<itsPoint> optima)
{
    this->optima = optima;
}

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

vector<double> itsProblem::getBoundsMinima()
{
    return this->boundsMinima;
}

void itsProblem::setBoundsMinima(vector<double> minima)
{
    if(minima.size() != getDimension() ) {
        ostringstream msg;
        msg << "ErrorSize: number of bounds (" << minima.size() 
            << ") does not correspond to dimension  (" << getDimension() << ")";
        throw msg.str().c_str();
    } else {
        this->boundsMinima = minima;
    }
}

vector<double> itsProblem::getBoundsMaxima()
{
    return this->boundsMaxima;
}

void itsProblem::setBoundsMaxima(vector<double> maxima)
{
    if(maxima.size() != getDimension() ) {
        ostringstream msg;
        msg << "ErrorSize: number of bounds (" << maxima.size() 
            << ") does not correspond to dimension  (" << getDimension() << ")";
        throw msg.str().c_str();
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
string itsProblem::getFormula()
{
    return this->formula;
}

void itsProblem::setFormula(string formula)
{
    this->formula = formula;
}

string itsProblem::getKey()
{
    return this->key;
}


void itsProblem::setKey( string key )
{
    this->key = key;
}
