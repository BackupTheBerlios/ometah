/***************************************************************************
 *  $Id: itsJpGenetic.cpp,v 1.1 2005/07/13 12:51:26 jpau Exp $
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


#include "itsJpGenetic.hpp"


using namespace std;

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

    mutationProba = 0.2;
    totalMutationProba = 0.3;
    selectionCoef = 0.8;
}


void itsJpGenetic::learning()
{ 
  
  // if is first call
  if (getEvaluationNumber() == getSampleSize()) {
    sample = sortOnValues(sample, 0);
    cout << "HELO\n";
  }
  // sample already sorted, when diversification was done before
  for (unsigned i = 0; i<(unsigned)(getSampleSize() * selectionCoef); i+= 2) {

    vector<itsPoint> v;
    v = makeChildren(sample[i], sample[i+1]);
    sample.push_back( v[0] );
    sample.push_back( v[1] );
  }
}
 

void itsJpGenetic::diversification()
{
  
  vector<itsPoint> sortedSample = sortOnValues(sample, 0);
  // darwinist selection
  vector<itsPoint> v;
  sample = v;
  for(unsigned i=0; i<getSampleSize(); i++) {
    sample.push_back(sortedSample[i]);
  }            
}

void itsJpGenetic::intensification()
{
  // No intensification.
}



// directly use proba insteand of "else if ()..; else if" ??
vector<itsPoint> itsJpGenetic::makeChildren(itsPoint father, itsPoint mother)
{
  double alpha;

  itsPoint boy;
  itsPoint girl;

  // solutions vectors for father, mother, boy and girl
  vector<double> fsol;
  vector<double> msol;
  vector<double> bsol;
  vector<double> gsol;

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
  v.push_back( mutation(boy) );
  v.push_back( mutation(girl) );
  return v;
}


itsPoint itsJpGenetic::mutation(itsPoint point)
{

  float proba;

  // if current optimal point, make a mutation
  if ( evaluate(point).getValues()[0] == sample[0].getValues()[0] ) {
    proba = 0;
    cout << "MUTE\n";
  }
  else
    proba = drand48();
    
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
    // partial mutation
    // CAUTION : don't go beyond search space limits
    vector<double> npsol;
    vector<double> psol;
    vector<double> mins;
    vector<double> maxs;
    mins = this->problem->boundsMinima();
    maxs = this->problem->boundsMaxima();
    psol = point.getSolution();
    float coef;
    double buf;
    for ( int i=0; i<this->problem->getDimension(); i++) {
      
      // P(up) = P(down) = P(no change) = 0.3
      proba = drand48();
      coef = drand48();

      // calcul new coordinate, and check if its into bounds
      if ( proba < 0.3333 ){
	buf = psol[i] * coef;	
	if ( buf > mins[i] )
	  npsol.push_back( buf );
	else
	  npsol.push_back( psol[i] );
      }
      else if ( proba < 0.6666 ) {
	buf = psol[i] * ( 1 / coef );
	if ( buf < maxs[i] )
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
  

itsMetaheuristic* itsJpGeneticFactory::create()
{
    return new itsJpGenetic;
}
