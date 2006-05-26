/***************************************************************************
 *  $Id: ometahcats.hpp,v 1.1 2006/05/26 11:00:42 nojhan Exp $
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

#include <hash_map.h>
#include <string>
#include <iostream>
#include <fstream>

// common stuff
#include <ometah/common/logic.hpp>
#include <ometah/common/itsSet.hpp>
#include <ometah/common/string.hpp>

// metaheuristics
#include <ometah/metaheuristic/estimation/itsEstimationOfDistribution.hpp>
#include <ometah/metaheuristic/estimation/itsHybridEstimationOfDistribution.hpp>
#include <ometah/metaheuristic/random/itsRandom.hpp>
#include <ometah/metaheuristic/sampling/itsGridSampling.hpp>
#include <ometah/metaheuristic/neldermead/itsNelderMead.hpp>

#include <ometah/metaheuristic/antcolony/itsHybridContinuousInteractingAntColony.hpp>

#include <ometah/metaheuristic/genetic/itsSimpleGenetic.hpp>
#include <ometah/metaheuristic/genetic/itsJpGenetic.hpp>

#include <ometah/metaheuristic/annealing/itsSimulatedAnnealing.hpp>

// problems
#include <ometah/problem/CEC05/itsCEC05_SSRPO_Base.hpp>

// communication
#include <ometah/communication/itsCommunicationServer_embedded.hpp>
#include <ometah/communication/itsCommunicationClient_embedded.hpp>

// interfaces
#include <ometah/interface/itsArgument.hpp>


#include "itsCatsProblem.hpp"


#define VERSION "0.1"

using namespace std;
