/***************************************************************************
 *  $Id: itsGridSampling.hpp,v 1.9 2005/11/04 17:28:19 nojhan Exp $
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
 
#ifndef GRID_SAMPLING
#define GRID_SAMPLING

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

class itsGridSampling : public itsMetaheuristic 
{

protected:

  
  //! number of points in each dimension
  int pointsPerDim;
  
  //! resolutions
  vector<double> resolutions;

  //! max and min coord for each dimension.
  vector<double> maxs;
  vector<double> mins;

  //! the initialization must be empty
  void initialization();
      
  //! the intensification is not used
  void intensification();
      
  //! the diversification phase is the only one used
  void diversification();
      
  //! the learning is not used
  void learning();
    
  //! Recursive function over dimensions.
  void pointConstruction( vector<double> partialPoint );
  
  //! Start algorithm.
  void start_algo();
  

public:

  //! Constructor.
  itsGridSampling();

  //! Destructor.
  ~itsGridSampling();
};


class itsSamplingFactory : public itsMetaheuristicFactory
{
public:
    itsMetaheuristic* create();
};

#endif
