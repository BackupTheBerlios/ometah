/***************************************************************************
 *  $Id: ometah.hpp,v 1.24 2006/05/15 21:23:34 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
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
 
#ifndef OMETAH_HPP
#define OMETAH_HPP

//#include <hash_map.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

/*
// common stuff
#include "../common/logic.hpp"
#include "../common/itsSet.hpp"
#include "../common/string.hpp"
#include "../common/Exception_oMetah.hpp"

// basic classes
#include "../metaheuristic/itsMetaheuristic.hpp"
#include "../problem/itsProblem.hpp"
#include "../communication/itsCommunicationClient.hpp"
#include "../communication/itsCommunicationServer.hpp"

// metaheuristics
#include "../metaheuristic/estimation/itsEstimationOfDistribution.hpp"
#include "../metaheuristic/estimation/itsHybridEstimationOfDistribution.hpp"
#include "../metaheuristic/random/itsRandom.hpp"
#include "../metaheuristic/sampling/itsGridSampling.hpp"
#include "../metaheuristic/neldermead/itsNelderMead.hpp"

#include "../metaheuristic/antcolony/itsHybridContinuousInteractingAntColony.hpp"

#include "../metaheuristic/genetic/itsSimpleGenetic.hpp"
#include "../metaheuristic/genetic/itsJpGenetic.hpp"

#include "../metaheuristic/annealing/itsSimulatedAnnealing.hpp"

// problems
#include "../problem/CEC05/itsCEC05_SSRPO_Base.hpp"

// communication
#include "../communication/itsCommunicationServer_embedded.hpp"
#include "../communication/itsCommunicationClient_embedded.hpp"

#ifdef WITH_SOCKET
#include "../communication/network/itsCommunicationClient_socket.hpp"
#include "../communication/network/itsCommunicationServer_socket.hpp"
#endif

#ifdef WITH_PYTHON
#include "../communication/languages/itsCommunicationClient_python.hpp"
#endif


// interfaces
#include "itsArgument.hpp"
*/

#include "../commons.h"
#include "../metaheuristics.h"
#include "../problems.h"
#include "../communications.h"

//#define VERSION "0.2.4" // deprecated, use a compiler flag instead (see the SCOnstruct file)

using namespace std;

namespace ometah {

void communication_bind( itsMetaheuristic* m, itsCommunicationClient* cc, itsCommunicationServer* cs, itsProblem* p)
{
    // metaheuristic -> client
    m->problem = cc;
    
    // server -> problem
    cs->problem = p;
    
    // Special case for the embedded protocol : we must link client and server
    if( cc->getKey() == "Embedded" && 
        cs->getKey() == "Embedded") {
          cc->problem = cs;
    }
}

template <class T>
bool get_help_on( T * theSet, const string asked )
{
      // list of the names of available problems
      vector<string> slist = theSet->getKeyList();
  
      // search the asked item in the problems
      vector<string>::iterator p = find( slist.begin(), slist.end(), asked );
  
      // if the item is find
      if( p != slist.end() ) {
          theSet->choose(*p);
          cout << "Informations:" << endl;
          cout << theSet->item()->getInformations();
          return true;
      } else {
          return false;
      }
}

}//ometah

#endif
