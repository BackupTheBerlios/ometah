/***************************************************************************
 *  $Id: ometah.hpp,v 1.2 2005/06/13 12:06:04 jpau Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
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
 
#include <hash_map.h>
#include <string>

// common stuff
#include "../common/logic.hpp"
#include "../common/itsSet.hpp"
#include "../common/string.hpp"

// basic classes
#include "../metaheuristic/itsMetaheuristic.hpp"
#include "../problem/itsProblem.hpp"
#include "../communication/itsCommunicationClient.hpp"
#include "../communication/itsCommunicationServer.hpp"

// metaheuristics
#include "../metaheuristic/itsEstimationOfDistribution.hpp"

// problems
#include "../problem/CEC05/itsCEC05_SSRPO_Base.hpp"

// communication
#include "../communication/itsCommunicationServer_embedded.hpp"
#include "../communication/itsCommunicationClient_embedded.hpp"

// interfaces
#include "itsArgument.hpp"


using namespace std;
