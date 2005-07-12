/***************************************************************************
 *  $Id: itsGridSampling.cpp,v 1.8 2005/07/12 11:24:26 jpau Exp $
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


void itsGridSampling::intensification()
{
}

void itsGridSampling::initialization()
{
}

void itsGridSampling::learning()
{
}

itsGridSampling::itsGridSampling()  
{
  pointsPerDim = -1;
  setName("Regular sampling");
  setKey("GS");
  setAccronym("GS");
  setDescription("Regular sampling search.");
  setCitation("Unknown");
  setFamily("Sampling algorithm");  

}

void itsGridSampling::diversification()
{

  while ( ! sample.empty() )
    sample.erase( sample.begin() );

  unsigned int dim = this->problem->getDimension();

  // initialize maxs and mins vectors, giving min and max bounds for each dim
  for (unsigned int i = 0; i < dim; i++ ) {
    maxs.push_back(this->problem->boundsMaxima()[i]);
    mins.push_back(this->problem->boundsMinima()[i]);
  }

  // if pointsperDim non initialized
  if (pointsPerDim == -1) {

    // points ~ nb evaluations
    int pointsNb = this->getSampleSize();
    double ppd = pow( (double)pointsNb, 1/(double)dim );
    if ( ( ppd - (int)floor(ppd) ) < 0.5 )
      pointsPerDim = (int)floor( ppd );
    else
      pointsPerDim = (int)floor( ppd ) + 1;
    cout << "PPDIM " << pointsPerDim << endl;
  }

  // resolutions
  resolutions.reserve( dim );
  
  for( unsigned int i=0; i < dim; i++ ) {

    double k = ( maxs[i] - mins[i] ) / (pointsPerDim);
    resolutions.push_back( k );
  }
  
  vector<double> partialPoint;
  pointConstruction( partialPoint );

  // only one iteration needed
  this->isInternalStoppingCriterion = true;
}


/*
  Recursive evaluation of points over dimensions.
*/
void itsGridSampling::pointConstruction( vector<double> & partialPoint )
{  
  int n = partialPoint.size();  

  if(( n >= this->problem->getDimension() )) { // if vector is built
    
    printDebug("GSpoint",print(partialPoint));
  
    // add the point to our sample buffer
    itsPoint p;
    p.setSolution( partialPoint );
    sample.push_back( evaluate(p) );

    evaluationsNumber ++;
    
  } else { // vector not fully filled
    
    // add a dimension
    partialPoint.push_back( 0 );

    // loop over the next dimension (n)
    for( double i = mins[n]; i <= maxs[n]; i += resolutions[n] ) {
      // change current coordinate value
      partialPoint[n] = i;
     
      // recursive call 
      if ( ! isStoppingCriteria() ) {      
        pointConstruction( partialPoint );
      }
    }
  }
}


itsMetaheuristic* itsSamplingFactory::create()
{
    return new itsGridSampling;
}
