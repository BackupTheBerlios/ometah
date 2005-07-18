/***************************************************************************
 *  $Id: itsNelderMead.hpp,v 1.6 2005/07/18 12:32:39 nojhan Exp $
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
 
#ifndef ITSNELDERMEAD
#define ITSNELDERMEAD

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include "../itsMetaheuristic.hpp"
#include "../../common/matrix.hpp"
#include "../../common/random.hpp"
#include "../../common/itsPoint.hpp"
#include "../../common/string.hpp"

using namespace std;

class itsNelderMead : public itsMetaheuristic
{

protected:

  //! sample points sorted according to their values, from best to worst
  vector<itsPoint> sortedSample;

  // transformations coefficients :
  
  //! reflection 
  float reflection;

  //! expansion
  float expansion;

  //! contraction
  float contraction;

  // simplexes after transformation

  //! reflection simplex
  vector<itsPoint> reflectionSimplex;

  //! expansion simplex
  vector<itsPoint> expansionSimplex;

  //! contraction simplex
  vector<itsPoint> contractionSimplex;

protected:

  //! initialization phase
  void initialization();

  //! the intensification phase
  void intensification();
      
  //! the diversification, ie the algo itself
  void diversification();
      
  //! the learning
  void learning();

  //! Init a simplex from a base point and a vector af lengths for the edges
  void initSimplexFromBasePoint(itsPoint basePoint, vector<double> edgesLengths);

  //! return the minimum value of a point of the vector
  double simplexOptimum(vector<itsPoint> points);

  //! sort current sample in sortedSample vector
  void sortSample();

  //! return the coordinates of the transformed point
  itsPoint getTransformedPoint(itsPoint point, float coef);


  //! !!!!!!!!!!!!!!
  void makeReflectionSimplex();

  void makeExpansionSimplex();

  void makeContractionSimplex();
  
public:

  //! Constructor
  /*!
    Here are set default values for attributes
   */
  itsNelderMead();

  ~itsNelderMead();

};

class itsNelderMeadFactory : public itsMetaheuristicFactory
{
public:
    itsMetaheuristic* create();
};

#endif
