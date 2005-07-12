/***************************************************************************
 *  $Id: itsSimpleGenetic.hpp,v 1.1 2005/07/12 14:04:02 jpau Exp $
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
 
#ifndef SIMPLE_GEN
#define SIMPLE_GEN
 
 
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

class itsSimpleGen : public itsMetaheuristic
{

protected:

  //! probability of mutation for a child
  float mutProba;

  //! fraction of the sample which is selected for procreation
  float coefCreation;

  //! number of children at each generation (~ nb of parents selected)

protected:

  //! the intensification
  void intensification();
      
  //! the diversification, select the bests among parents and children
  void diversification();
      
  //! the learning, which creates the children
  void learning();

  //! return a vector of two children
  vector<itsPoint> makeChildren(itsPoint father, itsPoint mother);

  //! returns the muted form of a point
  itsPoint mutation(itsPoint point);
  
  
public:

  //! Constructor
  /*!
    Here are set default values for attributes
  */
  itsSimpleGen();

  ~itsSimpleGen();

};

class itsSimpleGenFactory : public itsMetaheuristicFactory
{
public:
    itsMetaheuristic* create();
};

#endif
