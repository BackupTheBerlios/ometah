/***************************************************************************
 *  $Id: itsGridSampling.hpp,v 1.5 2005/07/08 13:24:27 jpau Exp $
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
 
#ifndef GRID_SAMPLING
#define GRID_SAMPLING

#include "../itsMetaheuristic.hpp"

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

  //! the intensification is a the a method
  void intensification();
      
  //! the diversification
  void diversification();
      
  //! the learning
  void learning();
    
  //! Recursive function over dimensions.
  void recEval( vector<double> partialPoint );
  
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

