/***************************************************************************
 *  $Id: itsEstimationOfDistribution.cpp,v 1.12 2006/09/09 20:18:34 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
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


#include "itsEstimationOfDistribution.hpp"


using namespace std;

namespace ometah {

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
    if( transposedSimplifiedSample.size() != (unsigned int)this->getProblem()->getDimension() ) {
        ostringstream err;
        err << "ErrorSize: transposed matrix size (" 
            << transposedSimplifiedSample.size() 
            << ") does not correspond to problem dimension (" 
            << this->getProblem()->getDimension() 
            << ")";
        throw Exception_Size_Match(err.str(), EXCEPTION_INFOS );
    }

    // find the minimum and the maximum for each dimension  
    this->parameterUniformMinima.clear();
    this->parameterUniformMaxima.clear();
    for(unsigned int dim=0; dim < this->getProblem()->getDimension(); dim++) {
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
        simplifiedSample.push_back( getSamplePointAddr(i)->getSolution() );
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
      //this->isInternalStoppingCriterion = true;
      // redraw in a uniform distribution within the hypercube of the sample
      sol = randomUniform( getSampleSolutionsMin(), getSampleSolutionsMax() );
    }

    // test if is in bounds
    if( this->isKeepBounds ) {
        for(unsigned int i=0; i < this->getProblem()->getDimension(); i++) {
            // if < min or > max
            if( sol[i] < this->getProblem()->boundsMinima()[i] ||
                sol[i] > this->getProblem()->boundsMaxima()[i] ) {
                    // redraw in a uniform distribution on bounds
                    sol = randomUniform( this->getProblem()->boundsMinima(), this->getProblem()->boundsMaxima() );
            }
        }
    }

    return sol;
}

void itsEstimationOfDistribution::diversification()
{
    // get the best point
    itsPoint optim = getOptimum();

    // reinit all
    setSampleSize( getSampleSize() );

    // draw each point in an hyper cube
    for( unsigned int i=1; i < getSampleSize(); i++) { // i=1 because we always keep the best point if the sample is sorted
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
        setSamplePoint(i, evaluate(p) );
    }
  
    // keep the best point
    setSamplePoint(0, optim);
}

void itsEstimationOfDistribution::selection()
{

    int selectNumber = (int) floor( (double)getSampleSizeCurrent() * this->selectRatio);
    printDebug("selectNumber",selectNumber);

    setSample( selectOnValues( getSample(), selectNumber ) );

}

void itsEstimationOfDistribution::intensification()
{
  selection();
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

}//ometah
