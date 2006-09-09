/***************************************************************************
 *  $Id: itsSimpleGenetic.cpp,v 1.7 2006/09/09 20:18:34 nojhan Exp $
 *  Copyright : Free Software Foundation
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


#include "itsSimpleGenetic.hpp"


using namespace std;

namespace ometah {

itsSimpleGenetic::~itsSimpleGenetic()
{
}

itsSimpleGenetic::itsSimpleGenetic()
{
    setName("Simple genetic algorithm");
    setKey("SGEN");
    setAccronym("SGEN");
    setDescription("Create several generations of points, using crossing and mutations");
    setCitation("Unknown");
    setFamily("Genetic algorithm");    

    mutationProba = 0.5;
    selectionCoef = 0.5;
}


void itsSimpleGenetic::learning()
{ 
  // reproduction 
 
  // if not is first call
  if ( getEvaluationsNumber() != getSampleSize() ) {

    unsigned size = getSampleSizeCurrent();
    // sample already sorted, when diversification was done before
    for (unsigned i = 0; i<size; i+= 2) {

      vector<itsPoint> v;
      if ( i+1 < size)
	v = makeChildren(getSamplePoint(i), getSamplePoint(i+1) );
      else
	v = makeChildren(getSamplePoint(i), getSamplePoint(0) );
      getSampleAddr()->push_back( v[0] );
      if ( i+1 < size)
	getSampleAddr()->push_back( v[1] );
    }
  }
}
 

void itsSimpleGenetic::diversification()
{
  // mutation
  
  // only make mutation on newly created points
  for(unsigned i= (unsigned)(getSampleSize() * selectionCoef); i<getSampleSize(); i++) {
    
    setSamplePoint( i, mutation( getSamplePoint(i) ) );
  }
}

void itsSimpleGenetic::intensification()
{
  // selection

  vector<itsPoint> sortedSample = sortOnValues( getSample(), 0);
  // darwinist selection
  vector<itsPoint> v;
  setSample( v );
  for(unsigned i=0; i< (int) getSampleSize() * selectionCoef; i++) {
    getSampleAddr()->push_back(sortedSample[i]);
  }
}



// directly use proba insteand of "else if ()..; else if" ??
vector<itsPoint> itsSimpleGenetic::makeChildren(itsPoint father, itsPoint mother)
{
  double alpha;
  itsPoint boy, girl;

  // solutions vectors for father, mother, boy and girl
  vector<double> fsol, msol, bsol, gsol;

  fsol = father.getSolution();
  msol = mother.getSolution();

  // for each dimension, set the coordinate
  // ~ find a point with uniform proba in hypercube of parents coords.
  for ( unsigned int i=0; i<this->getProblem()->getDimension(); i++) {

    alpha = rand();
    bsol.push_back( fsol[i] * alpha + msol[i] * (1 - alpha) );
    alpha = rand();
    gsol.push_back( fsol[i] * alpha + msol[i] * (1 - alpha) );
  }
    
  boy.setSolution( bsol );
  girl.setSolution( gsol );
  vector<itsPoint> v;
  v.push_back( evaluate(boy) );
  v.push_back( evaluate(girl) );
  return v;
}


itsPoint itsSimpleGenetic::mutation(itsPoint point)
{

  float proba;

  // if current optimal point, make a mutation
  if ( evaluate(point).getValues()[0] == getSamplePointAddr(0)->getValues()[0] ) {
    proba = 0;
  }
  else
    proba = rand();
    
  if ( proba > mutationProba ) {
    // no mutation
    return evaluate (point);
  }
  else {
    // full mutation = new randomized point
    point.setSolution( randomUniform(this->getProblem()->boundsMinima(), this->getProblem()->boundsMaxima()) );
    return evaluate (point);
  }
}
  

itsMetaheuristic* itsSimpleGeneticFactory::create()
{
    return new itsSimpleGenetic;
}

}//ometah
