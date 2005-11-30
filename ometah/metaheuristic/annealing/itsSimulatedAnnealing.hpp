/***************************************************************************
 *  $Id: itsSimulatedAnnealing.hpp,v 1.1 2005/11/24 22:28:48 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmil.com>
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
 
#ifndef ITSSIMULATEDANNEALING
#define ITSSIMULATEDANNEALING

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

class itsSimulatedAnnealing : public itsMetaheuristic
{
protected:
  double temperature;
  double temperature_max;


  itsPoint p_current;
  itsPoint p_new;
  
protected:
  //! the intensification is a the a method
  void intensification();
      
  //! the diversification
  void diversification();
      
  //! the learning
  void learning();

  //! initialization
  void initialization();

public:

  //! Constructor
  /*!
    Here are set default values for attributes
  */
  itsSimulatedAnnealing();

  ~itsSimulatedAnnealing();

};

class itsSimulatedAnnealingFactory : public itsMetaheuristicFactory
{
public:
    itsMetaheuristic* create();
};

#endif