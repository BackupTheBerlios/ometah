/***************************************************************************
 *  $Id: itsEaco.cpp,v 1.1 2006/05/30 14:13:23 walid_tfaili Exp $
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

//////////////////////////////////////////////////////////////
// to be added to common/
////////////////////////////////////////////////////////////// 
double pi= 3.141592653589793238;
double e = 2.7182818284590451;



/* 
   Returns the normal probability density of a vector with given mean and std
   f(x) = 1/(sqrt(2*pi)*sigma)*e^-((x - mu)^2/(2*sigma^2)) 
*/

double itsEaco::dnorm( double x, double mean, double std)
{
  double y;
  
  y = 1 / ( (sqrt(2*pi)*std) * exp( -( (x-mean)*(x-mean) / (2*std*std) ) ) );

  return y;
} 


/* 
   Returns a sum of set of normal probability density of a vector with given mean and std
   f(x) = sum(1/(sqrt(2*pi)*sigma)*e^-((x - mu)^2/(2*sigma^2)) ) 
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
	  y += 1 / ( (sqrt(2*pi)*std[i]) * exp( -( (x-mean[i])*(x-mean[i]) / (2*std[i]*std[i]) ) ) );
	}
    }
  
  return y/mean.size();
} 


/* 
   Initializes the array of distributions
   D[dist][2][vars]
*/

vector< vector< vector<double> > >  itsEaco::init_D( double vars, double dist, double min, double max)
{
  vector< vector< vector<double> > > D;
  
  for ( unsigned int i = 0; i < dist; i++ )
    for ( unsigned int j = 0; j < vars; j++ )
      {
	D[i][1][j] = randomUniform(min, max);
	D[i][2][j] = (max-min)/dist;
      }
  return D;
}

vector< vector< vector<double> > > itsEaco::gen_X( vector< vector< vector<double> > > d, double n_of_ants)
{
  
  vector< vector<double> > X;
  int dist = d.size();
  int vars = d[1].size();
  
  for ( unsigned int i = 0; i < vars; i++ )
    {
      
    }
  
  
  return d;
  
}

//////////////////////////////////////////////////////////////
// to be added to common/
////////////////////////////////////////////////////////////// 

void itsEaco::learning()
{ 
  // No learning.
}
 

void itsEaco::diversification()
{
    // reinit all
    setSampleSize( getSampleSize() );

    // draw each point in an hyper cube
    for( unsigned int i=0; i < getSampleSize(); i++) 
      {
        // draw solution
        itsPoint p; 
	p.setSolution( randomUniform( this->problem->boundsMinima(), this->problem->boundsMaxima() ) );
	// get values
        sample[i] = evaluate(p);
	
	// init_D(3, 5, -10, 10);
	// dnorm(1, 1, 1);
	// sum_dnorm(1, p.getSolution(), p.getSolution() );
      }
    
    // /////////////////////////////////////////////////////
    for( unsigned int j=0; j < getSampleSize(); j++)
      {
      }
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
