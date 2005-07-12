/***************************************************************************
 *  $Id: itsNelderMead.cpp,v 1.6 2005/07/12 13:11:25 jpau Exp $
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
 

#include "itsNelderMead.hpp"

using namespace std;

itsNelderMead::~itsNelderMead()
{
}

itsNelderMead::itsNelderMead()
{
    setName("Nelder-Mead search algorithm.");
    setKey("NM");
    setAccronym("NM");
    setDescription("Nelder-Mead search algorithm.");
    setCitation("");
    setFamily("Geometric algorithm.");

    reflection = 1.0;
    expansion = 2.0;
    contraction = 0.5;

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
