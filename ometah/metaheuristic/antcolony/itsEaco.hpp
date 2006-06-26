/***************************************************************************
 *  $Id: itsEaco.hpp,v 1.2 2006/06/26 08:56:30 walid_tfaili Exp $
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
 
#ifndef EACO_ALGO
#define EACO_ALGO

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include "../itsMetaheuristic.hpp"
#include "../../common/matrix.hpp"
#include "../../common/random.hpp"
#include "../../common/itsPoint.hpp"
#include "../../problem/itsProblem.hpp"
#include "../../common/string.hpp"

using namespace std;


namespace ometah {

class itsEaco : public itsMetaheuristic
{

protected:
  //! the intensification is a the a method
  void intensification();

  //! the diversification
  void diversification();

  //! the learning
  void learning();

  /* Variables */
  //! the distribution array D[number of distribution][2][number of variables]
  vector< vector< vector< double > > > distributionMatrix;

  //! the distribution number for each dimension
  double distribution;

  void SetDistribution( double value );

public:

  //! Constructor
  /*!
    Here are set default values for attributes
  */
  itsEaco();

  ~itsEaco();

  double dnorm( double x, double mean, double std);

  double sum_dnorm( double x, vector<double> mean, vector<double> std);

  //! Initializes the distribution matrix
  vector< vector< vector< double > > >  initDistributionMatrix( double vars, double dist, double min, double max);

  //! Generates a set of points (sample) based on a given distribution matrix
  vector< vector< double > >  generatePoints( vector< vector< vector < double > > > D, int n_of_ants);


};

class itsEacoFactory : public itsMetaheuristicFactory
{
public:
    itsMetaheuristic* create();
};

#endif
}//ometah
