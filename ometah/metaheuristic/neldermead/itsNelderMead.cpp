/***************************************************************************
 *  $Id: itsNelderMead.cpp,v 1.2 2005/07/08 11:14:16 jpau Exp $
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
 
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>

#include "itsNelderMead.hpp"
#include "../../common/matrix.hpp"
#include "../../common/random.hpp"
#include "../../common/itsPoint.hpp"
#include "../../common/string.hpp"

using namespace std;

itsNelderMead::~itsNelderMead()
{
}

itsNelderMead::itsNelderMead()
{
    setName("Nelder-Mead search algorithm.");
    setKey("NM");
    setAccronym("NM");
    setDescription("Nelder-Mead search algorithm.");
    setCitation("");
    setFamily("Geometric algorithm.");

    reflection = 1.0;
    expansion = 2.0;
    contraction = 0.5;
}



void itsNelderMead::learning()
{
  // keep empty for this algo
}
 

void itsNelderMead::diversification()
{

}

void itsNelderMead::intensification()
{
  // keep empty for this algo
}


double itsNelderMead::bestSolution(int dimension)
{
  return 0;
}

void sortSample()
{

}

double itsNelderMead::transfo(float coef, double x) 
{
  return 2;
}


itsPoint getTransformedPoint(itsPoint point, float coef)
{
  vector<double> newSol;
  /*
  for(unsigned i = 0; i < point.getSolution().size() ; i++) {
    newSol.push_back( transfo(coef, point.getSolution()[i] ) );
  }
  */
  itsPoint p;
  return p;
}

void itsNelderMead::makeReflectionSimplex()
{

}

void itsNelderMead::makeExpansionSimplex()
{

}

void itsNelderMead::makeContractionSimplex()
{

}

itsMetaheuristic* itsNelderMeadFactory::create()
{
    return new itsNelderMead;
}
