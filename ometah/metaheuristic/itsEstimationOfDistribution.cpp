/***************************************************************************
 *  $Id: itsEstimationOfDistribution.cpp,v 1.4 2005/06/24 19:36:51 nojhan Exp $
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
 
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>

#include "itsEstimationOfDistribution.hpp"
#include "../common/matrix.hpp"
#include "../common/random.hpp"
#include "../common/itsPoint.hpp"
#include "../common/string.hpp"

using namespace std;

itsEstimationOfDistribution::~itsEstimationOfDistribution()
{
}

itsEstimationOfDistribution::itsEstimationOfDistribution()
{
    setName("Continuous Estimation of Distribution Algorithm");
    setKey("CEDA");
    setAccronym("CEDA");
    setDescription("An estimation of distribution algorithm");
    setCitation(
"@InBoo{{Bengoetxea02,"
"  Author         = {Bengoetxea, E. and Miqu\\'elez, T. and Larra\\~naga, P."
"                   and Lozano, J. A.},"
"  Editor         = {Larranaga, P. and Lozano, J. A.},"
"  Title          = {Estimation of {D}istribution {A}lgorithms, {A} {N}ew"
"                   {T}ool for {E}volutionary {C}omputation},"
"  Chapter        = {Experimental Results in Function Optimization with"
"                   EDAs in Continuous Domain},"
"  Pages          = {181--194},"
"  Publisher      = {Kluwer Academic Publishers},"
"  Series         = {Genetic Algorithms and Evolutionary Computation},"
"  year           = 2002"
"}"
);
    setFamily("Estimation of Distribution Algorithm");
    

    //setDistribution("Uniform");
    setDistribution("Normal");
    keepBounds();
    
    
    setSelectRatio(0.5);
}


void itsEstimationOfDistribution::learningUniform(vector< vector<double> > aSample)
{
    // transpose it so that the row correspond to a dimension, and not a solution
    vector< vector<double> > transposedSimplifiedSample = transpose(aSample);

    // verify that we have the correct dimension
    if( transposedSimplifiedSample.size() != (unsigned int)this->problem->getDimension() ) {
        ostringstream err;
        err << "ErrorSize: transposed matrix size (" 
            << transposedSimplifiedSample.size() 
            << ") does not correspond to problem dimension (" 
            << this->problem->getDimension() 
            << ")";
        throw err.str();
    }

    // find the minimum and the maximum for each dimension  
    this->parameterUniformMinima.clear();
    this->parameterUniformMaxima.clear();
    for(int dim=0; dim < this->problem->getDimension(); dim++) {
        // store them
        this->parameterUniformMinima.push_back( min( aSample[dim] ) );
        this->parameterUniformMaxima.push_back( max( aSample[dim] ) );
    }
}

void itsEstimationOfDistribution::learningNormal(vector< vector<double> > aSample)
{
    //! mean on each dimension
    this->parameterNormalMean = mean( transpose(aSample) );

    //! variance covariance matrix
    this->parameterNormalVarCovar = varianceCovariance(aSample);
}


void itsEstimationOfDistribution::learning()
{
    vector< vector<double> > simplifiedSample;
    
    // for each point, extract the solution vector
    for(unsigned int i=0; i < getSampleSizeCurrent(); i++) {
        // put this vector in a matrix
        simplifiedSample.push_back( sample[i].getSolution() );
    }


    if ( distribution == "Uniform" ) {
        learningUniform( simplifiedSample );
    
    } else if (distribution == "Normal") {  
        learningNormal( simplifiedSample );
    
    } else {
        learningUniform( simplifiedSample );
    }
}
 
vector<double> itsEstimationOfDistribution::diversificationNormal()
{
    vector<double> sol;

    // random in the distribution
    try {
        sol = randomNormalMulti(this->parameterNormalMean, this->parameterNormalVarCovar);
    } catch (const char * s) {
        this->isInternalStoppingCriterion = true;
        sol = randomUniform( this->problem->boundsMinima(), this->problem->boundsMaxima() );
    }

    // test if is in bounds
    if( this->isKeepBounds ) {
        for(int i=0; i < this->problem->getDimension(); i++) {
            // if < min or > max
            if( sol[i] < this->problem->boundsMinima()[i] ||
                sol[i] > this->problem->boundsMaxima()[i] ) {
                    // redraw in a uniform distribution on bounds
                    sol = randomUniform( this->problem->boundsMinima(), this->problem->boundsMaxima() );
            }
        }
    }

    return sol;
}

void itsEstimationOfDistribution::diversification()
{
    // reinit all
    setSampleSize( getSampleSize() );

    // draw each point in an hyper cube
    for( unsigned int i=0; i < getSampleSize(); i++) {
        // draw solution
        itsPoint p;
    
        if ( distribution == "Uniform" ) {
            p.setSolution( randomUniform(this->parameterUniformMinima, this->parameterUniformMaxima) );
        
        } else if (distribution == "Normal") {  
            p.setSolution( diversificationNormal() );
        
        } else {
            p.setSolution( randomUniform(this->parameterUniformMinima, this->parameterUniformMaxima) );
        }
    
        // get values
        sample[i] = evaluate(p);
    }
}

void itsEstimationOfDistribution::intensification()
{
    // sort the sample
    this->sample = sortOnValues(this->sample,0);

    // get the best points
    int selectNumber = (int) floor( (double)this->sample.size() * this->selectRatio);
    printDebug("selectNumber",selectNumber);

    vector<itsPoint> bests(this->sample.begin(), this->sample.begin() + selectNumber );
    
    if(bests.size()<=0) {
        throw "ErrorSize: no more points left after selection";
    }

    this->sample = bests;
}

string itsEstimationOfDistribution::getDistribution()
{
  return this->distribution;
}

void itsEstimationOfDistribution::setDistribution(string distribution)
{
  if(distribution == "Uniform") {
    this->distribution = distribution;

  } else {
    // The uniform distribution is used by default
    this->distribution = distribution;
  }
}


void itsEstimationOfDistribution::setSelectRatio( double ratio ) 
{
    this->selectRatio = ratio;
}

double itsEstimationOfDistribution::getSelectRatio()
{
    return this->selectRatio;
}

void itsEstimationOfDistribution::keepBounds()
{
    this->isKeepBounds = true;
}
  
void itsEstimationOfDistribution::keepBoundsNot()
{
    this->isKeepBounds = false;
}

itsMetaheuristic* itsEstimationOfDistributionFactory::create()
{
    return new itsEstimationOfDistribution;
}
