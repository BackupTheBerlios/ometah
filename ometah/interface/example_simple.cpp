/***************************************************************************
 *  $Id: example_simple.cpp,v 1.1 2005/11/04 16:15:05 nojhan Exp $
 *  Copyright : Free Software Fundation
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
 
 
 
// basic classes
#include "../metaheuristic/itsMetaheuristic.hpp"
#include "../problem/itsProblem.hpp"
#include "../communication/itsCommunicationClient.hpp"
#include "../communication/itsCommunicationServer.hpp"

// metaheuristics
#include "../metaheuristic/estimation/itsEstimationOfDistribution.hpp"


// problems
#include "../problem/CEC05/itsCEC05_SSRPO_Base.hpp"

// communication
#include "../communication/itsCommunicationServer_embedded.hpp"
#include "../communication/itsCommunicationClient_embedded.hpp"

using namespace std;


int main(int argc, char ** argv)
{
  /*****************************************************************************
  * Instanciations
  *****************************************************************************/

  // MÉTAHEURISTIQUE
  itsEstimationOfDistribution metaheuristic;
  
  // PROBLÈME
  itsAckley problem;

  // Communication
  itsCommunicationClient_embedded client;
  itsCommunicationServer_embedded server;
  metaheuristic.problem = & client;
  client.problem = & server;
  server.problem = & problem;

  /*****************************************************************************
  * Options 
  *****************************************************************************/
  problem.setDimension(1);

  metaheuristic.setSampleSize(10);
  metaheuristic.setEvaluationsMaxNumber(100);
  metaheuristic.setIterationsMaxNumber(10);
  metaheuristic.setValueMin(0);
  metaheuristic.initRandom(0);

  /*****************************************************************************
  * Lancement & sortie
  *****************************************************************************/
  
  cout << "<? xml-version=\"1.0\" encoding=\"iso-8859-15\" ?>" << endl;
  cout << "<ometah>" << endl;
  
  cout << problem.getInformations_XML() << endl;
  cout << metaheuristic.getInformations_XML() << endl;

  // lancement
  try {
    metaheuristic.start();
  }
  catch(const char * str) {
    cerr << str << endl;
  }
  catch(string str) {
    cerr << str << endl;
  }
  catch (...) {
    cerr << "Unknown error" << endl;
  }

  cout << "</ometah>" << endl;
}
