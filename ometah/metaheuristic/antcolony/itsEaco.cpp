/***************************************************************************
 *  $Id: itsEaco.cpp,v 1.2 2006/06/26 08:56:30 walid_tfaili Exp $
 *  Copyright : Free Software Foundation
 *  Author : Walid Tfaili <walidtfaili@yahoo.fr>
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



#include "itsEaco.hpp"



using namespace std;


namespace ometah {

itsEaco::~itsEaco()
{
}

itsEaco::itsEaco()
{
    setName("Ant colony algorithm");
    setKey("EACO");
    setAccronym("EACO");
    setDescription("Enhanced ACO for continuous functions.");
    setCitation("Unknown");
    setFamily("Ant algorithm");
}




/*
   Returns the normal probability density of a vector with given mean and std
   f(x) = 1/(sqrt(2*PI)*sigma)*e^-((x - mu)^2/(2*sigma^2)) 
*/

double itsEaco::dnorm( double x, double mean, double std)
{
  double y;

  y = 1 / ( (sqrt(2*PI)*std) * exp( -( (x-mean)*(x-mean) / (2*std*std) ) ) );

  return y;
}


/*
   Returns a sum of set of normal probability density of a vector with given mean and std
   f(x) = sum(1/(sqrt(2*PI)*sigma)*e^-((x - mu)^2/(2*sigma^2)) ) 
*/

double itsEaco::sum_dnorm( double x, vector<double> mean, vector<double> std)
{

  double y;

  if ( mean.size() != std.size() )
    {
      // à ajouter throw error ?
    }
  else
    {
      y = 0;
      for ( unsigned int i = 0; i < getSampleSize(); i++ )
	{
	  y += 1 / ( (sqrt(2*PI)*std[i]) * exp( -( (x-mean[i])*(x-mean[i]) / (2*std[i]*std[i]) ) ) );
	}
    }

  return y/mean.size();
}


/********** Seuil de prise en charge des spots ou des messages **********/ 
void itsEaco::SetDistribution( double value )
{
  distribution = value;
}


/*
   Initializes the array of distributions
   D[dist][2][vars]
*/

vector< vector< vector< double > > >  itsEaco::initDistributionMatrix( double vars, double dist, double min, double max)
{
  vector< vector< vector< double > > > temp_D;
  vector< vector < double > > temp_C;
  vector< double > temp_vars;

  for ( unsigned int i = 0; i < dist; i++ ){
	for ( unsigned int j = 0; j < vars; j++ )
		temp_vars.push_back( randomUniform(min, max) );
		
	temp_C.push_back( temp_vars );

	while( !temp_vars.empty() ){
		temp_vars.pop_back();
	};

	for ( unsigned int j = 0; j < vars; j++ )
		temp_vars.push_back( (max-min)/dist );

	temp_C.push_back( temp_vars );
		
	while( !temp_vars.empty() ){
		temp_vars.pop_back();
	};

	temp_D.push_back(temp_C);
	
	while( !temp_C.empty() ){
		temp_C.pop_back();	
	}
  }
	
  return temp_D;
}

vector< vector<double> > itsEaco::generatePoints( vector< vector< vector< double > > > d, int n_of_ants)
{

  int dimension = d[1][1].size();
  int distribution = d.size();

  vector< vector< double > > X;
  vector< vector< double > > temp_t;
  vector< double > temp_n;
  vector<double> temp_v;
  vector< int > index;

  temp_v.reserve( dimension );
  X.reserve( n_of_ants );

  for ( int i = 0; i < dimension; i++ ){
	
	while( !temp_t.empty() ){
		temp_t.pop_back();	
	}
	while( !temp_n.empty() ){
		temp_n.pop_back();	
	}
	while( !index.empty() ){
		index.pop_back();	
	}

	for ( int j = 0; j < n_of_ants; j++ ){
		for ( int k = 0; k < dimension; k++ ){
		    temp_v.push_back( randomNormal( d[i][0][k], d[i][1][k] ) );	
		}
		temp_t.push_back( temp_v );	

		while( !temp_v.empty() ){
		temp_v.pop_back();	
		}
	}
	
	for ( int j = 0; j < n_of_ants; j++ ){
		index.push_back( (int)randomUniform( 0, distribution ) +1 );
	}
	
	for ( int j = 0; j < n_of_ants; j++ ){
		temp_n.push_back( temp_t[j][index[j]] );
	}
	
	X.push_back( temp_n );
    }

  return X;

}



void itsEaco::learning()
{
  // No learning.
}


void itsEaco::diversification()
{
    itsPoint pt, temp_p1, temp_p2;
    double minBound = this->problem->boundsMinima()[0];
    double maxBound = this->problem->boundsMaxima()[0];
    double vars = this->problem->getDimension();
    SetDistribution( 6 );
    double dist = distribution;
    double accuracy = 1e-8; //FIXME this->problem->getAccuracy();
    int n_of_ants = 10;
    int index = 0;
    int oldIndex = 0;
    double mini = 999999999;

    vector< vector< double > > X;
    vector< vector< double > > temp_X;
    vector< vector< double > > temp_C;
    vector< double > temp_vars;
    vector< vector< double > > solutionPartiel;


    while( !solutionPartiel.empty() ){
		solutionPartiel.pop_back();	
	}
    while( !temp_X.empty() ){
		temp_X.pop_back();	
	}
    while( !temp_vars.empty() ){
		temp_vars.pop_back();	
	}
    while( !temp_C.empty() ){
		temp_C.pop_back();	
	}

    // Initialize the distribution array
    distributionMatrix = initDistributionMatrix( vars, dist, minBound, maxBound );

    // Generate n_of_ants points based on the distribution matrix
    X = generatePoints( distributionMatrix, n_of_ants ); 

    // Choose only points that fit bounds
    for( int i=0; i<n_of_ants; i++ ){
	if( isInBounds( X[i], this->problem->boundsMinima(),
			this->problem->boundsMaxima() ) ){
		solutionPartiel.push_back( X[i] );
	}
    }

    // Choose only those that fit in bounds
    X = solutionPartiel;

    // Select the best point min value (temp_p1) index [j]
    if ( X.size() > vars ){
	temp_p1.setSolution( X[0] );
	temp_p2.setSolution( X[0] );	

	for( int j=0; j < n_of_ants; j++){
		temp_p2.setSolution( X[j] );
		if( isValueSmaller( temp_p2, temp_p1, 0 ) ){
			temp_p1 = temp_p2;
			index = j;
		}
	}

	

	temp_C.push_back( temp_p1.solution );

	for ( unsigned int j = 0; j < vars; j++ )
		temp_vars.push_back( ( max( temp_p1.solution, 0, temp_p1.solution.size() ) - min( temp_p1.solution, 0, temp_p1.solution.size() ) )/dist );
	temp_C.push_back( temp_vars );

	
	// Create the new distribution based on the best point
	if( temp_p1.getValues()[0] <  mini ){
		mini = temp_p1.getValues()[0];
		distributionMatrix[ (int) dist ] = temp_C; //FIXME erreur hors limites
	}
	else{
		distributionMatrix[ oldIndex ] = temp_C;
		oldIndex = oldIndex % (int)dist + 1;
	}

    }

    // reinit all
    //setSampleSize( getSampleSize() );

    // draw each point in an hyper cube
    for( unsigned int k=0; k < vars; k++)
      {
        // draw solution
        itsPoint p; 
	p.setSolution( X[k] );
	// get values
        sample[k] = evaluate(p);


      }

    // /////////////////////////////////////////////////////
    
    // ////////////////////////////////////////////////////
}

void itsEaco::intensification()
{
  // No intensification.
}



itsMetaheuristic* itsEacoFactory::create()
{
    return new itsEaco;
}

}//ometah
