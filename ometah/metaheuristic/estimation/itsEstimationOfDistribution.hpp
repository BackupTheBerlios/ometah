/***************************************************************************
 *  $Id: itsEstimationOfDistribution.hpp,v 1.6 2006/04/11 10:42:36 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
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
 
#ifndef ITSESTIMATIONOFDISTRIBUTION
#define ITSESTIMATIONOFDISTRIBUTION

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
#include "../../common/Exception_oMetah.hpp"

using namespace std;

class itsEstimationOfDistribution : public itsMetaheuristic
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

  //! The minima parameter for the uniform distribution
  vector<double> parameterNormalMean;

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

  //! selection of the best points
  /*!
    uses the selectRatio parameter
  */
  void selection();

  //! the diversification
  void diversification();
      
  //! the learning
  void learning();
      
  //! Learn the parameters of an uniform distribution
  void learningUniform(vector< vector<double> > aSample);

  //! Learn the parameters of a normal distribution
  void learningNormal(vector< vector<double> > aSample);

  //! Draw the sample in the learned normal distribution
  vector<double> diversificationNormal();

public:

  //! Constructor
  /*!
    Here are set default values for attributes
   */
  itsEstimationOfDistribution();

  ~itsEstimationOfDistribution();

  //! Return the distribution
  string getDistribution();
  //! Change the distribution
  void setDistribution(string distribution);

  //! Return the select ratio
  double getSelectRatio();

  //! Change the select ratio
  void setSelectRatio( double ratio );

  //! Keep points within bounds
  void keepBounds();
  
  //! Don't keep point within bounds
  void keepBoundsNot();

};

class itsEstimationOfDistributionFactory : public itsMetaheuristicFactory
{
public:
    itsMetaheuristic* create();
};

#endif
