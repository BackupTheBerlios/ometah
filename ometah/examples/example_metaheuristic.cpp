/***************************************************************************
 *  $Id: example_metaheuristic.cpp,v 1.2 2006/05/14 07:33:28 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
 *  for difficult optimization.
 */
 


#include "example_metaheuristic.hpp"


using namespace std;
using namespace ometah;

ITSEXAMPLEMETAHEURISTIC::~ITSEXAMPLEMETAHEURISTIC()
{
}

ITSEXAMPLEMETAHEURISTIC::ITSEXAMPLEMETAHEURISTIC()
{
    // Describe your algorithm here, so that one can easily comprehend it
    setName("The complete name of your algorithm"); // example : Super Simulated Annealing 2' turbo FX edition
    setKey("A unique key identifiying it"); // example : SSA_2, for Super Simulated Annealing 2
    setAccronym("the accronym"); // example : SSA
    setDescription("A description of your metaheuristic"); // example : A wonderful metaheuristic based on simulated annealing
    setCitation("The paper where it is described"); // example : a BiBTeX citation entry
    setFamily("The family of your metaheuristic"); // example : Simulated annealing
}


void ITSEXAMPLEMETAHEURISTIC::learning()
{ 
  // put here your learning code.
}
 

void ITSEXAMPLEMETAHEURISTIC::diversification()
{
    // put here your diversification code.
}

void ITSEXAMPLEMETAHEURISTIC::intensification()
{
  // put here your intensification code.
}


// The abstract factory pattern is necessary if you want to easily manipulate several metaheuristics
itsMetaheuristic* ITSEXAMPLEMETAHEURISTICFactory::create()
{
    return new ITSEXAMPLEMETAHEURISTIC;
}
