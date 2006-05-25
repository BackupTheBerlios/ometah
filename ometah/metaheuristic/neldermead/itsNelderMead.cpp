/***************************************************************************
 *  $Id: itsNelderMead.cpp,v 1.15 2006/05/25 08:51:52 nojhan Exp $
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
 

#include "itsNelderMead.hpp"

using namespace std;

namespace ometah {

itsNelderMead::~itsNelderMead()
{
}

itsNelderMead::itsNelderMead()
{
    setName("Nelder-Mead search");
    setKey("NMS");
    setAccronym("NMS");
    setDescription("Nelder-Mead search algorithm");
    setCitation("@ARTICLE{Nelder65simplex,"
  "author = {Nelder, J. A. and Mead, R.},"
  "title = {A simplex method for function minimization},"
  "journal = {Computer Journal},"
  "year = {1965},"
  "volume = {7},"
  "pages = {308--313},");
    setFamily("Local search");

    reflection = 1.0;
    expansion = 2.0;
    contraction = 0.5;
}

void itsNelderMead::initSimplexFromBasePoint(itsPoint basePoint, vector<double> edgesLengths)
{
  // reduce the sample size to the dimension + 1
  setSampleSize( this->problem->getDimension() + 1 );

  // empty simplex
  vector<itsPoint> plex;
  for( unsigned int i=0; i < this->problem->getDimension() + 1; i++ ) {
    itsPoint p;
    vector<double> v(this->problem->getDimension(), 0.0);
    p.setSolution( v );
    plex.push_back( p );
  }

  // on number of vertex
  for( unsigned int i=0; i < this->problem->getDimension(); i++ ) {
    // plex[dim][i] = basePoint[i]
    vector<double> v = plex[this->problem->getDimension()].getSolution();
    v[i] = basePoint.getSolution()[i];
    plex[this->problem->getDimension()].setSolution( v );
  
    for( unsigned int j=0; j < this->problem->getDimension(); j++) {
      // plex[i][j] = basePoint[j]
      vector<double> w = plex[i].getSolution();
      w[j] = basePoint.getSolution()[j];
      
      if ( i == j ) {
      // plex[i][j] = basePoint[j] + edges[i]
        w[j] += edgesLengths[i];
      }
      plex[i].setSolution( w );
    }
  }

  // evaluations
  for( unsigned int i=0; i<getSampleSizeCurrent(); i++ ) {
    itsPoint p = evaluate( plex[i] );
    sample[i] = p;
  }
}

void itsNelderMead::initialization()
{
  itsPoint p;

  // p in the center of the search space
  /*p.setSolution( 
    addition( 
      getBoundsMinima(), 
      multiply(
        addition( 
          getBoundsMinima(), 
          getBoundsMaxima() ), 
        0.5 
      ) 
    ) 
  );*/

  // p a random point
  p.setSolution( randomUniform( this->problem->boundsMinima(), this->problem->boundsMaxima() ) );

  // edges length = search space lengths / n^e root of dimensions
  /*vector<double> edges = 
    multiply(
      substraction(
        getBoundsMaxima(),
        getBoundsMinima()
      ),
      1 / pow( getSampleSize(), 1 / getDimension() )
    );*/
    
  // random edges lengths
  vector<double> edges = randomUniform( this->problem->boundsMinima(), this->problem->boundsMaxima() );
  
  initSimplexFromBasePoint( p, edges );
}


void itsNelderMead::learning()
{
  // keep empty for this algo
}
 

void itsNelderMead::diversification()
{
  if (sortedSample.size() == 0) {
    itsPoint p;
    for (unsigned i = 0; i < getSampleSize(); i++) {
      sortedSample.push_back(p);    
      contractionSimplex.push_back(p);
      expansionSimplex.push_back(p);
      reflectionSimplex.push_back(p);
    }
  }

  sortSample();
  makeReflectionSimplex();

  double minR = simplexOptimum( reflectionSimplex );

  if ( sortedSample[0].getValues()[0] <= minR) {
    makeContractionSimplex();
    sample = contractionSimplex;
  }
  else {
    makeExpansionSimplex();
    double minE = simplexOptimum( reflectionSimplex);
    if (( sortedSample[0].getValues()[0] > minR) && ( minR > minE)) {
      sample = expansionSimplex;
    }
    else {
      sample = reflectionSimplex;
    }      
  }
}

void itsNelderMead::intensification()
{
  // keep empty for this algo
}

double itsNelderMead::simplexOptimum( vector<itsPoint> points)
{
  double v = points[0].getValues()[0];
  for(unsigned i = 0; i< points.size(); i++) {
    if ( points[i].getValues()[0] < v )
      v = points[i].getValues()[0];
  }
  return v;
}


void itsNelderMead::sortSample()
{
  sortedSample = sortOnValues(sample, 0);
}


itsPoint itsNelderMead::getTransformedPoint(itsPoint point, float coef)
{
  vector<double> newSol;
  double best, newCo;
  
  for(unsigned i = 0; i < point.getSolution().size() ; i++) {
    
    best = sortedSample[0].getSolution()[i];
    newCo = best - coef * ( point.getSolution()[i] - best );
    newSol.push_back( newCo );
  }

  // force the simplex to stay in bounds
  newSol = forceBounds( newSol, this->problem->boundsMinima(), this->problem->boundsMaxima() );

  itsPoint p;
  p.setSolution(newSol);
  return evaluate (p);
}

void itsNelderMead::makeReflectionSimplex()
{

  for(unsigned i=0; i < sortedSample.size() ; i++) {
    
    reflectionSimplex[i] = ( getTransformedPoint(sortedSample[i], reflection) );
  }
}

void itsNelderMead::makeExpansionSimplex()
{
  
  for(unsigned i=0; i < sortedSample.size() ; i++) {
    
    expansionSimplex[i] = ( getTransformedPoint(sortedSample[i], expansion) );
  }
}

void itsNelderMead::makeContractionSimplex()
{

  for(unsigned i=0; i < sortedSample.size() ; i++) {
    
    contractionSimplex[i] = ( getTransformedPoint(sortedSample[i], contraction) );
  }
}

itsMetaheuristic* itsNelderMeadFactory::create()
{
    return new itsNelderMead;
}

}//ometah
