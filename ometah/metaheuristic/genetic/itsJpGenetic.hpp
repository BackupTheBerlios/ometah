/***************************************************************************
 *  $Id: itsJpGenetic.hpp,v 1.4 2006/05/13 10:05:55 nojhan Exp $
 *  Copyright : Free Software Foundation
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
 
#ifndef JP_GEN
#define JP_GEN
 
 
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdlib.h>
#include "../itsMetaheuristic.hpp"
#include "../../common/matrix.hpp"
#include "../../common/random.hpp"
#include "../../common/itsPoint.hpp"
#include "../../common/string.hpp"

using namespace std;

namespace ometah {

class itsJpGenetic : public itsMetaheuristic
{

protected:

  //! probability of mutation for a child
  float mutationProba;

  // mutationProba !

  //! when mutation needed, probability of this mutation to be total
  float totalMutationProba;

  //! fraction of the sample which is selected for procreation
  // ~ number of children born
  float selectionCoef;

  //! reduction of multiplicative coef in mutation
  float reduction;

protected:

  //! the intensification
  void intensification();
      
  //! the diversification, select the bests among parents and children
  void diversification();
      
  //! the learning, which creates the children
  void learning();

  //! return a vector of two children, make genetic crossings
  vector<itsPoint> makeChildren(itsPoint father, itsPoint mother);

  //! returns the muted form of a point, when its coordinates found from parents'
  itsPoint mutation(itsPoint point);
  
  
public:

  //! Constructor
  /*!
    Here are set default values for attributes
  */
  itsJpGenetic();

  ~itsJpGenetic();

};

class itsJpGeneticFactory : public itsMetaheuristicFactory
{
public:
    itsMetaheuristic* create();
};

}//ometah

#endif
