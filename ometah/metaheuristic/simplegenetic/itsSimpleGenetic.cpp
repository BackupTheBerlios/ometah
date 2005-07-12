/***************************************************************************
 *  $Id: itsSimpleGenetic.cpp,v 1.3 2005/07/12 14:43:55 jpau Exp $
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


#include "itsSimpleGenetic.hpp"


using namespace std;

itsSimpleGenetic::~itsSimpleGenetic()
{
}

itsSimpleGenetic::itsSimpleGenetic()
{
    setName("Simple genetic algorithm");
    setKey("SG");
    setAccronym("SG");
    setDescription("Create several generations of points, using crossing and mutations.");
    setCitation("Unknown");
    setFamily("Genetic algorithm");    

    mutProba = 0.1;
    coefCreation = 0.8;
}


void itsSimpleGenetic::learning()
{ 
  // sample already sorted, when diversification was done before
  for (unsigned i = 0; i<(unsigned)(getSampleSize() * coefCreation); i+= 2) {

    vector<itsPoint> v;
    v = makeChildren(sample[i], sample[i+1]);
    sample.push_back( v[0] );
    sample.push_back( v[1] );
  }
}
 

void itsSimpleGenetic::diversification()
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

void itsSimpleGenetic::intensification()
{
  // No intensification.
}



vector<itsPoint> itsSimpleGenetic::makeChildren(itsPoint father, itsPoint mother)
{

  float proba;

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
  for (int i=0; i<this->problem->getDimension(); i++) {

    // for boy
    proba = rand() / RAND_MAX; //random01();
    if ( proba <= 0.3333 ) {
      //bsol.push_back( fsol[i] );
      bsol.push_back( ( (double)(fsol[i] + msol[i] ) / (double) 2) );
    }
    else if ( proba >= 0.6666 ) {
      //bsol.push_back( msol[i] );
      bsol.push_back( ( (double)(fsol[i] + msol[i] ) / (double) 2) );
    }
    else {
      bsol.push_back( ( (double)(fsol[i] + msol[i] ) / (double) 2) );
    }

    // and girl
    proba = rand() / RAND_MAX; // (float)random01();   
    if ( proba <= 0.3333 ) {
      //gsol.push_back( fsol[i] );
      gsol.push_back( ( (double)(fsol[i] + msol[i] ) / (double) 2) );
    }
    else if ( proba >= 0.6666 ) {
      //gsol.push_back( msol[i] );
      gsol.push_back( ( (double)(fsol[i] + msol[i] ) / (double) 2) );
    }
    else {
      gsol.push_back( ( (double)(fsol[i] + msol[i] ) / (double) 2) );
    }    
  }

  boy.setSolution( bsol );
  girl.setSolution( bsol );

  vector<itsPoint> v;
  v.push_back( mutation(boy) );
  v.push_back( mutation(girl) );
  return v;
}


// TODO !
itsPoint itsSimpleGenetic::mutation(itsPoint point)
{
  float proba = rand() / RAND_MAX; //(double)random01();
  
  if ( proba < 0.5 ) {
    itsPoint p;  
    return evaluate (point);
  }
  else {
    itsPoint p;
    return evaluate (point);
  }
}
  

itsMetaheuristic* itsSimpleGeneticFactory::create()
{
    return new itsSimpleGenetic;
}
