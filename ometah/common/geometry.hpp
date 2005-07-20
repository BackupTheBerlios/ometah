/***************************************************************************
 *  $Id: geometry.hpp,v 1.2 2005/07/20 13:52:30 nojhan Exp $
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
 
#ifndef GEOMETRY
#define GEOMETRY

#include <vector>

using namespace std;

#define E  2.7182818284590452353602874713526625
#define PI 3.1415926535897932384626433832795029

//! Translate 2D rectangular coordinates to polar ones
template<class T>
T rec2pol_2D(T &recVec)
{
  typename T::value_type a;
  T polVec;
  polVec.reserve(2);
  
  // special case, both components 0
  if (recVec[0] == 0.0 && recVec[1] == 0.0) {
      polVec.push_back(0.0); // angle
      polVec.push_back(0.0); // radius
      return polVec;
  }

  // angle +- pi/2
  a = atan2(recVec[1],recVec[0]);

  // want it to be 0 to 2 pi
  if (a < 0.0) {
    a = a + PI*2;
  }
  
  polVec.push_back(a); // angle
  polVec.push_back(sqrt(recVec[0]*recVec[0] + recVec[1]*recVec[1])); // radius

  return polVec;
}

//! Translate 2D polar coordinates to rectangular ones
template<class T>
T pol2rec_2D(T & polVec)
{
  T recVec;
  recVec.reserve(2);

  recVec.push_back( polVec[0]*cos(polVec[1]) ); // x
  recVec.push_back( polVec[0]*sin(polVec[1]) ); // y

  return recVec;
}

//! Translate nD polar coordinates to rectangular ones
template<class T>
T pol2rec(T & polVec)
{
  T recVec; recVec.reserve( polVec.size() );
  T ProjVec; ProjVec.reserve( 2 );
  typename T::value_type Ri, Ri_1, ai, xi;

  Ri=polVec[0];

  for(unsigned int i=0; i<polVec.size(); i++) {

      Ri_1=Ri;
      ai=polVec[i];

      T Ra;
      Ra.push_back(Ri_1);
      Ra.push_back(ai);

      ProjVec = Pol2Rec_2D(Ra);

      xi=ProjVec[0];
      Ri=ProjVec[1];

      recVec.push_back(xi);
    }

  return recVec;
}

//! Calculate the eucilidan distance between two vectors with n dimensions
template<class T>
T distanceEuclidian(vector<T> vecA, vector<T> vecB)
{
  vector<T> squares; 
  squares.reserve( vecA.size() );

  for(unsigned int i=0; i<vecA.size(); i++) {
      squares.push_back( (vecA[i]-vecB[i])*(vecA[i]-vecB[i]) );
    }

  return sqrt( sum(squares) );
}

//! Calculate the gravity center of a matrix with n dimensions
template<class T, class U>
T gravityCenter(U & points, T & weights)
{
  typename U::value_type Wn = 0;
  T VG;

  // weight sum
  unsigned int i; 
  for(i=0; i < points.size(); i++) {
      Wn = Wn + weights[i];
  }
  
  // gravity center
  unsigned int j; 
  
  // dimensions 
  for(j=0; j < points[0].size(); j++) {
      typename T::value_type g = 0;
      
      unsigned int i;
      for(i=0; i < points.size(); i++) {
	  g = g + (points[i][j] * weights[i] / Wn);
      }
      VG.push_back(g);
  }
    
  return VG;
}

#endif
