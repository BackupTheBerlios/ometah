 /***************************************************************************
 *  $Id: random.hpp,v 1.1 2005/06/13 09:05:02 nojhan Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*
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
 

#ifndef RANDOM
#define RANDOM

#include <cmath>

// for PI :
#include "geometry.hpp" 


//! Draw a random number between 0 and 1
template<class T>
T randomO1()
{
  return (T)rand()/RAND_MAX;
}

//! Draw a random number in a normal probability density function
template<class T>
T randomNormal( T mean, T std )
{
  T x1, x2, w, y1;
  static T y2;
  static int use_last = 0;

  if (use_last)		        /* use value from previous call */
    {
      y1 = y2;
      use_last = 0;
    }
  else
    {
      do {
	x1 = 2* (T)rand()/RAND_MAX-1; // rand in [0,1]
	x2 = 2* (T)rand()/RAND_MAX-1;
	w = x1 * x1 + x2 * x2;
      } while ( w >= 1.0 );

      w = sqrt( (-2.0 * log( w ) ) / w );
      y1 = x1 * w;
      y2 = x2 * w;
      use_last = 1;
    }
  return (T)(mean + y1 * std);

}


//! Draw a vector with each scalar drawn in a normal distribution
template<class T>
vector< T > randomNormal( vector< T > & means, vector< T > & stds )
{
  vector< T > v;

  unsigned int i;
  for(i=0; i<means.size(); i++) {
    T x;
    x = randomNormal(means[i], stds[i]);
    v.push_back(x);
  }
    
  return v;
}


//! Draw a vector in a true multi-normal distribution
template<class T, class U>
T randomNormalMulti( T means, U varcovar )
{
/*  
  - Let $u$ a vector of $n$ number, following a centered/reducted
  normal distribution; 
  - let $L$ be the matrix resulting from the Cholesky decomposition
  of $V$; 
  - the vector $y=m+Lu$ follow the multi-normal distribution, 
  with a mean $m$ and a variance-covariance matrix $V$.
*/
  T finalPoint;
  
  // Cholesky decomposition of the variance-covariance matrix
  U popVarCholesky; // low triangular matrix
  popVarCholesky = cholesky( varcovar );

  // Vector with terms in a centered/reducted normal distribution
  T u;
  T mean(means.size(),0.0);
  T variance(means.size(),1.0);
  u = randomNormal( mean,variance );

  // temporary vector for multiplication
  U tempU;
  tempU.push_back(u);
  
  // post multiplication by the u vector
  U tempCompVar;
  tempCompVar = multiply( popVarCholesky, transpose(tempU) );

  // transposition
  T compVar = transpose(tempCompVar)[0];

  // addition to the mean
  finalPoint = addition( means, compVar );
  
  return finalPoint;
}


//! Draw a random vector in a hyper cube
template<class T>
T randomUniform( T mins, T maxs )
{
  T res;

  for(unsigned int i=0; i<mins.size(); i++) {
      typename T::value_type r;
      r = (maxs[i]-mins[i])* 
          (typename T::value_type)rand()/RAND_MAX
          + mins[i];
          res.push_back(r);
    }

  return res;
}


//! Draw a random vector in a hyper sphere
template<class T, class U>
U randomSphere(U & center, T radius)
{
  //FIXME
}


//! Draw a random vector in a hyper sphere, using a biased method
/*!
  The biased method lead to a pseudo-normal distribution, bounded by a sphere
 */
template<class T, class U>
U randomHyperSphereBiased(U & center, T  radius)
{
  T r;
  U res;

  r = (radius)*
      (T)rand()/RAND_MAX;

  res.push_back(r);

  unsigned int i;

  for(i=0; i<center.size(); i++)
    {
      r=(2*PI)*(T)rand()/RAND_MAX; // a random angle on 2Pi

      res.push_back(r);
    }

  return res;

}

#endif
