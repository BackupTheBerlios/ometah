/***************************************************************************
 *  $Id: itsGridSampling.cpp,v 1.2 2005/07/08 07:44:11 jpau Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
 *  Author : Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
 ****************************************************************************/

/*  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
 *  for difficult optimization.
 *  
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
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>

#include "itsGridSampling.hpp"
#include "../../common/matrix.hpp"
#include "../../common/random.hpp"
#include "../../common/itsPoint.hpp"
#include "../../common/string.hpp"

using namespace std;


itsGridSampling::~itsGridSampling()
{
}

itsGridSampling::itsGridSampling()  
{
  pointsPerDim = -1;
  setName("Regular sampling search.");
  setKey("GS");
  setAccronym("GS");
  setDescription("Regular sampling search.");
  setCitation("Unknown");
  setFamily("Sampling algorithm");    
}

void itsGridSampling::learning(){

}


void itsGridSampling::setPointsPerDim(int res)
{
  pointsPerDim = res;
}


void itsGridSampling::diversification(){

  if ( solutions.size() == 0 ) {

    unsigned i;
    for( i=0; i < getSampleSize(); i++) {
      // draw solution
      itsPoint p;
      p.setSolution( randomUniform(this->problem->boundsMinima(), this->problem->boundsMaxima()) );
      // get values
      sample[i] = evaluate(p);
    }
    // initialize solutions
    for ( i=0; i < getSampleSize(); i++){
      itsPoint p;
      solutions.push_back(p);
    }
  }
  else
    {
      itsPoint p;
      for ( unsigned i=0; i < getSampleSize(); i++){
	solutions[i] = p;
      }
      solIndex = 0;
  
      // min and max coordinates buffers
      double minCo, maxCo;

      unsigned dim = this->problem->getDimension();

      // initialize maxs and mins vectors
      for (unsigned i = 0; i < dim; i++) {
	minCo = this->problem->boundsMaxima()[i];
	maxCo = this->problem->boundsMinima()[i];
	// loop over sample
	for (unsigned j = 0; j < getSampleSize(); j++) {

	  if (sample[j].getSolution()[i] < minCo)
	    minCo = sample[j].getSolution()[i];
      
	  if (sample[j].getSolution()[i] > maxCo)
	    maxCo = sample[j].getSolution()[i];
	}
	maxs.push_back( maxCo );
	mins.push_back( minCo );
      }

      // if pointsperDim non initialized
      if(pointsPerDim == -1) {

	int pointsNb = getSampleSize();
    
	pointsPerDim = (int)floor( pow( (double)pointsNb, 1/(double)dim ) );
      }

      // resolutions
      resolutions.reserve( dim );

      double k;
  
      for( unsigned i=0; i < dim; i++ ) {

	k = ( maxs[i] - mins[i] ) / (pointsPerDim);
	resolutions.push_back( k );
      }
  
      vector<double> partialPoint;
  
      recEval( partialPoint );
  
      for ( unsigned i=0; i < solutions.size(); i++) {
	sample[i] = solutions[i];
      }
    }
}


void itsGridSampling::intensification(){
}


/*
  Recursive evaluation of points over dimensions.
*/
void itsGridSampling::recEval( vector<double> partialPoint )
{  
  int n = partialPoint.size();  

  if(( n >= this->problem->getDimension() )) { // if vector is built
    
    // add the point to our sample buffer
    itsPoint p;
    p.setSolution( partialPoint );
    solutions[solIndex] = ( evaluate(p) );
    
    solIndex ++;
    
  } 
  else { // vector not fully filled
    
    // add a dimension
    partialPoint.push_back( 0 );

    // loop over the next dimension (n)
    for( double i = mins[n]; i <= maxs[n]; i += resolutions[n] ) {
      // change current coordinate value
      partialPoint[n] = i;
     
      // to avoid over flow
      if (solIndex < getSampleSize()) {
	 // recursive call
	recEval( partialPoint );
      }
    }
  }
}


itsMetaheuristic* itsSamplingFactory::create()
{
    return new itsGridSampling;
}
