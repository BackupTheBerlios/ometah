/***************************************************************************
 *  $Id: itsSimulatedAnnealing.cpp,v 1.3 2006/09/09 20:18:34 nojhan Exp $
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
 


#include "itsSimulatedAnnealing.hpp"


using namespace std;

namespace ometah {

itsSimulatedAnnealing::~itsSimulatedAnnealing()
{
}

itsSimulatedAnnealing::itsSimulatedAnnealing()
{
    setName("Simulated Annealing");
    setKey("SA");
    setAccronym("SA");
    setDescription("A simulated annealing where temperature is decreasing step by step.");
    setCitation("Unknown");
    setFamily("Simulated annealing");    

    temperature_max = 1;
    temperature = temperature_max;
}


void itsSimulatedAnnealing::initialization()
{
    itsMetaheuristic::initialization();

    // first previous point
    p_current.setSolution( randomUniform( this->getProblem()->boundsMinima(), this->getProblem()->boundsMaxima() ) );
    p_current = evaluate(p_current);
  
    p_new = p_current;
}


void itsSimulatedAnnealing::learning()
{ 
    // restart from the best point found in the previous iteration
    p_current = getOptimum();
}
 

void itsSimulatedAnnealing::diversification()
{
    // reinit all
    setSampleSize( getSampleSize() );

    // draw each point in an hyper cube
    for( unsigned int i=0; i < getSampleSize(); i++) {
        // draw solution
        p_new.setSolution( randomUniform( this->getProblem()->boundsMinima(), this->getProblem()->boundsMaxima() ) );
        p_new = evaluate(p_new);
        setSamplePoint( i, p_new );
    
        // Metropolis rule
        // if the value is the better than the previous point, accept
        if( isValueSmaller(p_current,p_new) ) {
            p_current = p_new;
        } else {
            // else, draw a random number and see if we accept the new point
            if( randomUniform(0.0,temperature_max) < temperature ) {
                p_current = p_new;
            } // else the current point is kept
        }
    }
}

void itsSimulatedAnnealing::intensification()
{
  // temperature decrease
  temperature = temperature - ( temperature_max / 10 );
}


itsMetaheuristic* itsSimulatedAnnealingFactory::create()
{
    return new itsSimulatedAnnealing;
}

}//ometah
