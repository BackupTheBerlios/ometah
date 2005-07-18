/***************************************************************************
 *  $Id: itsHybridEstimationOfDistribution.cpp,v 1.1 2005/07/18 13:47:01 nojhan Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
 *  Author : Johann Dréo <nojhan@gmail.com>
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
 
#include "itsHybridEstimationOfDistribution.hpp"
#include "../neldermead/itsNelderMead.hpp"

itsHybridEstimationOfDistribution::itsHybridEstimationOfDistribution()
{
  setName("Continuous Hybrid Estimation of Distribution Algorithm");
  setKey("CHEDA");
  setAccronym("CHEDA");
  setDescription("An estimation of distribution algorithm hybrided with a nelder-mead local search");
  setCitation("");
  setFamily("Estimation of Distribution Algorithm");

  setIntensificationMethod("selection-simplex");
}


void itsHybridEstimationOfDistribution::simplex()
{
  setSimplexEvaluations( (int)floor( pow( (this->problem->getDimension() + 1 ), 2.0 ) ) );
  
  for( unsigned int i=0; i < getSampleSizeCurrent(); i++ ) {
    itsNelderMead nms;
  
    // problem optimized
    nms.problem = this->problem;
  
    // no ending stopping criteria
    nms.setValueMin(0.0);
    nms.setIterationsMaxNumber( this->getIterationsMaxNumber() );
  
    // used stopping criterion
    nms.setEvaluationsMaxNumber( this->simplexEvaluations );
  
    // edges from sample hypercube
    vector<double> edges = 
      multiply(
        substraction(
          this->problem->boundsMaxima(),
          this->problem->boundsMinima()
        ),
        1 / ( pow( (double)getSampleSizeCurrent(), (double)1.0/this->problem->getDimension() ) - 1 )
      );
      
    // init on current point
    nms.initSimplexFromBasePoint( sample[i], edges );
  
    // silent launch
    nms.startSilent();
    
    // change the point to the new local optimum
    sample[i] = nms.getOptimum();
  }
}

void itsHybridEstimationOfDistribution::intensification()
{
  if ( getIntensificationMethod() == "simplex" ) {
    simplex();
  
  } else if ( getIntensificationMethod() == "selection" ) {
    selection();
  
  } else { // "selection-simplex"
    selection();
    simplex();
  }
}


void itsHybridEstimationOfDistribution::setIntensificationMethod( string method )
{
  this->intensificationMethod = method;
}


string itsHybridEstimationOfDistribution::getIntensificationMethod()
{
  return this->intensificationMethod;
}


void itsHybridEstimationOfDistribution::setSimplexEvaluations( int number )
{
  this->simplexEvaluations = number;
}


int itsHybridEstimationOfDistribution::getSimplexEvaluations()
{
  return this->simplexEvaluations;
}


itsMetaheuristic* itsHybridEstimationOfDistributionFactory::create()
{
    return new itsHybridEstimationOfDistribution;
}
