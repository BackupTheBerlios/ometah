/***************************************************************************
 *  $Id: metaheuristics.h,v 1.2 2006/06/15 12:27:29 walid_tfaili Exp $
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
 
#ifndef OMETAH_METAHEURISTICS
#define OMETAH_METAHEURISTICS
 
#include "metaheuristic/itsMetaheuristic.hpp"

#include "metaheuristic/estimation/itsEstimationOfDistribution.hpp"
#include "metaheuristic/estimation/itsHybridEstimationOfDistribution.hpp"

#include "metaheuristic/random/itsRandom.hpp"

#include "metaheuristic/sampling/itsGridSampling.hpp"

#include "metaheuristic/neldermead/itsNelderMead.hpp"

#include "metaheuristic/antcolony/itsHybridContinuousInteractingAntColony.hpp"

#include "metaheuristic/genetic/itsSimpleGenetic.hpp"
#include "metaheuristic/genetic/itsJpGenetic.hpp"

#include "metaheuristic/annealing/itsSimulatedAnnealing.hpp"

#endif


#ifdef WITH_EACO
#include "metaheuristic/antcolony/itsEaco.hpp"
#endif