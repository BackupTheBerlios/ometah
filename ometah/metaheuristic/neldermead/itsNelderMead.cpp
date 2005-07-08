/***************************************************************************
 *  $Id: itsNelderMead.cpp,v 1.1 2005/07/08 08:48:20 jpau Exp $
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
}



void itsNelderMead::learning()
{

}
 

void itsNelderMead::diversification()
{

}

void itsNelderMead::intensification()
{

}


itsMetaheuristic* itsNelderMeadFactory::create()
{
    return new itsNelderMead;
}
