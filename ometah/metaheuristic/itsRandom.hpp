/***************************************************************************
 *  $Id: itsRandom.hpp,v 1.3 2005/07/06 11:39:43 jpau Exp $
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
 
#ifndef RANDOMETAH
#define RANDOMETAH
 
#include "itsMetaheuristic.hpp"

using namespace std;

class itsRandom : public itsMetaheuristic
{
protected:
  //! The probability density function in used
  /*!
    Either :
      - Uniform
      - Normal
  */
  string distribution;

  //! The minima parameter for the uniform distribution
  vector<double> parameterUniformMinima;

  //! The maxima parameter for the uniform distribution
  vector<double> parameterUniformMaxima;

  //! The maxima parameter for the uniform distribution
  vector<vector<double> > parameterNormalVarCovar;

  //! The select ratio
  /*!
    A number in [0,1] telling what proportion of points
    should be selected in the intensification phase
  */
  double selectRatio;

  //! Bounds respect
  /*! 
      If false, allow points to go out of bounds, 
      if true points are kept within bounds
  */
  bool isKeepBounds;
  
protected:
  //! the intensification is a the a method
  void intensification();
      
  //! the diversification
  void diversification();
      
  //! the learning
  void learning();

public:

  //! Constructor
  /*!
    Here are set default values for attributes
   */
  itsRandom();

  ~itsRandom();

};

class itsRandomFactory : public itsMetaheuristicFactory
{
public:
    itsMetaheuristic* create();
};

#endif
