/***************************************************************************
 *  $Id: itsJpGenetic.cpp,v 1.9 2006/05/13 10:05:55 nojhan Exp $
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


#include "itsJpGenetic.hpp"


using namespace std;

namespace ometah {

itsJpGenetic::~itsJpGenetic()
{
}

itsJpGenetic::itsJpGenetic()
{
    setName("JP genetic algorithm");
    setKey("JGEN");
    setAccronym("JGEN");
    setDescription("Experimental genetic algorithm");
    setCitation("Unknown");
    setFamily("Genetic algorithm");    

    mutationProba = 0.7;
    totalMutationProba = 0.4;
    selectionCoef = 0.5;
    reduction = 0.5;
}


void itsJpGenetic::learning()
{ 
  // reproduction

  // bests with bests, etc.
  // if not is first call
  if ( getEvaluationNumber() != getSampleSize() ) {

  unsigned size = getSampleSizeCurrent();
  // sample already sorted, when diversification was done before
  for (unsigned i = 0; i < size; i+= 2) {

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
 

void itsJpGenetic::diversification()
{
  // only make mutation on newly created points
  for(unsigned i= (unsigned)( getSampleSize() * selectionCoef); i<getSampleSize(); i++) {
    
    sample[i] = mutation( sample[i] );
  }       
}

void itsJpGenetic::intensification()
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



vector<itsPoint> itsJpGenetic::makeChildren(itsPoint father, itsPoint mother)
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


itsPoint itsJpGenetic::mutation(itsPoint point)
{

  float proba;
  vector<double> npsol, psol;
  double coef, buf;
  
  // if current optimal point, make a partial mutation
  if ( evaluate(point).getValues()[0] > sample[0].getValues()[0] ) {
    proba = mutationProba * rand(); // force mutation
  }
  else
    proba = rand();
    
  if ( proba > mutationProba ) {
    // no mutation
    return evaluate (point);
  }
  else {
    if ( proba < mutationProba * totalMutationProba ) {
      // full mutation = new randomized point
      point.setSolution( randomUniform(this->problem->boundsMinima(), this->problem->boundsMaxima()) );
      return evaluate (point);
    }
    else {  // partial mutation

      psol = point.getSolution();

      for ( int i=0; i<this->problem->getDimension(); i++) {
      
	// P(up) = P(down) = P(no change) = 0.3
	proba = rand();
	coef = rand();

	// calcul new coordinate, and check if its into bounds
	if ( proba < 0.3333 ){
	  buf = psol[i] * coef * reduction;	
	  if ( buf > this->problem->boundsMinima()[i] )
	    npsol.push_back( buf );
	  else
	    npsol.push_back( psol[i] );
	}
	else if ( proba < 0.6666 ) {
	  buf = psol[i] * ( 1 - coef ) * reduction;
	  if ( buf < this->problem->boundsMaxima()[i] )
	    npsol.push_back( buf );
	  else
	    npsol.push_back( psol[i] );
	}     
	else {
	  npsol.push_back( psol[i] );
	}
      }
      point.setSolution( npsol );
      return evaluate (point);
    }
  }
}
  

itsMetaheuristic* itsJpGeneticFactory::create()
{
    return new itsJpGenetic;
}

}//ometah
