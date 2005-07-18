/***************************************************************************
 *  $Id: itsHybridEstimationOfDistribution.hpp,v 1.1 2005/07/18 13:47:01 nojhan Exp $
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
 
#ifndef ITSHYBRIDESTIMATIONOFDISTRIBUTION
#define ITSHYBRIDESTIMATIONOFDISTRIBUTION

#include "itsEstimationOfDistribution.hpp"

using namespace std;


class itsHybridEstimationOfDistribution : public itsEstimationOfDistribution
{
protected:
  //! The method used for intensification
  /*!
    Either selection, simplex or selection-simplex
  */
  string intensificationMethod;

  //! Number of evaluations given to the Nelder-Mead search
  int simplexEvaluations;

public:
  itsHybridEstimationOfDistribution();

  //! Only this method is redefined
  void intensification();
  
  //! Launch several Nelder-Mead local search
  void simplex();

  //! Change the intensification method
  void setIntensificationMethod( string method );

  //! Return the intensification method
  string getIntensificationMethod();

  //! Change the number of evaluations for NMS
  void setSimplexEvaluations( int number );

  //! Return the number of evaluations for NMS
  int getSimplexEvaluations();
};


class itsHybridEstimationOfDistributionFactory : public itsEstimationOfDistributionFactory
{
public:
    itsMetaheuristic* create();
};

#endif
