/***************************************************************************
 *  $Id: itsSimpleGenetic.cpp,v 1.3 2005/11/04 17:28:13 nojhan Exp $
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
  if ( getEvaluationNumber() != getSampleSize() ) {

    unsigned size = getSampleSizeCurrent();
    // sample already sorted, when diversification was done before
    for (unsigned i = 0; i<size; i+= 2) {

      vector<itsPoint> v;
      if ( i+1 < size)
	v = makeChildren(sample[i], sample[i+1]);
      else
	v = makeChildren(sample[i], sample[0]);
      sample.push_back( v[0] );
      if ( i+1 < size)
	sample.push_back( v[1] );
    }
  }
}
 

void itsSimpleGenetic::diversification()
{
  // mutation
  
  // only make mutation on newly created points
  for(unsigned i= (unsigned)(getSampleSize() * selectionCoef); i<getSampleSize(); i++) {
    
    sample[i] = mutation( sample[i] );
  }
}

void itsSimpleGenetic::intensification()
{
  // selection

  vector<itsPoint> sortedSample = sortOnValues(sample, 0);
  // darwinist selection
  vector<itsPoint> v;
  sample = v;
  for(unsigned i=0; i< (int) getSampleSize() * selectionCoef; i++) {
    sample.push_back(sortedSample[i]);
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
  for (int i=0; i<this->problem->getDimension(); i++) {

    alpha = drand48();
    bsol.push_back( fsol[i] * alpha + msol[i] * (1 - alpha) );
    alpha = drand48();
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
  if ( evaluate(point).getValues()[0] == sample[0].getValues()[0] ) {
    proba = 0;
  }
  else
    proba = drand48();
    
  if ( proba > mutationProba ) {
    // no mutation
    return evaluate (point);
  }
  else {
    // full mutation = new randomized point
    point.setSolution( randomUniform(this->problem->boundsMinima(), this->problem->boundsMaxima()) );
    return evaluate (point);
  }
}
  

itsMetaheuristic* itsSimpleGeneticFactory::create()
{
    return new itsSimpleGenetic;
}
