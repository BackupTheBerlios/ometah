/***************************************************************************
 *  $Id: itsSimpleGen.cpp,v 1.1 2005/07/12 12:48:57 jpau Exp $
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

#include "itsSimpleGen.hpp"
#include "../../common/matrix.hpp"
#include "../../common/random.hpp"
#include "../../common/itsPoint.hpp"
#include "../../common/string.hpp"

using namespace std;

itsSimpleGen::~itsSimpleGen()
{
}

itsSimpleGen::itsSimpleGen()
{
    setName("Simple genetic algorithm");
    setKey("SG");
    setAccronym("SG");
    setDescription("Create several generations of points, using crossing and mutations.");
    setCitation("Unknown");
    setFamily("Genetic algorithm");    

    mutProba = 0.1;
}


void itsSimpleGen::learning()
{ 

  for (unsigned i = 0; i<(unsigned)(getSampleSize() / 2); i++) {
    itsPoint p;
    p.setSolution( randomUniform(this->problem->boundsMinima(), this->problem->boundsMaxima()) );
    sample.push_back( evaluate(p) );
  }

  // HERE : CREATE CHILDREN FROM SAMPLE POINTS, AND MAKE MUTATION WITH PROBA mutProba

}
 

void itsSimpleGen::diversification()
{
  
  // if generation was done (every time except first one)
  if ( getSampleSizeCurrent() > getSampleSize() ) {
    vector<itsPoint> sortedSample = sortOnValues(sample, 0);
    // select the getSampleSize bests points among all points
    vector<itsPoint> v;
    sample = v;
    for(unsigned i=0; i<getSampleSize(); i++) {
      sample.push_back(sortedSample[i]);
    }    
  }
}

void itsSimpleGen::intensification()
{
  // No intensification.
}



vector<itsPoint> itsSimpleGen::makeChildren(itsPoint father, itsPoint mother)
{

  vector<itsPoint> v;
  return v;
}


itsPoint itsSimpleGen::mutation(itsPoint point)
{
  itsPoint p;
  return p;
}
  

itsMetaheuristic* itsSimpleGenFactory::create()
{
    return new itsSimpleGen;
}
