/***************************************************************************
 *  $Id: itsEstimationOfDistribution.hpp,v 1.2 2005/06/13 12:06:04 jpau Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
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
 
#include "itsMetaheuristic.hpp"

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

protected:
  //! the intensification is a the a method
  void intensification();
      
  //! the diversification
  void diversification();
      
  //! the learning
  void learning();
      
  //! Learn the parameters of an uniform distribution
  void learningUniform(vector< vector<double> > aSample);

  //! Learn the parameters of a normal distribution
  void learningNormal(vector< vector<double> > aSample);

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

};

class itsEstimationOfDistributionFactory : public itsMetaheuristicFactory
{
public:
    itsMetaheuristic* create();
};

#endif
