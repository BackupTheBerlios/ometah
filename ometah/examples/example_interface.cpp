/***************************************************************************
 *  $Id: example_interface.cpp,v 1.1 2005/11/25 18:24:14 nojhan Exp $
 *  Copyright : Free Software Fundation
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
 *  for difficult optimization.
 */
 
 
 
// basic classes
#include <ometah/metaheuristic/itsMetaheuristic.hpp>
#include <ometah/problem/itsProblem.hpp>
#include <ometah/communication/itsCommunicationClient.hpp>
#include <ometah/communication/itsCommunicationServer.hpp>

// metaheuristics
#include <ometah/metaheuristic/estimation/itsEstimationOfDistribution.hpp>


// problems
#include <ometah/problem/CEC05/itsCEC05_SSRPO_Base.hpp>

// communication
#include <ometah/communication/itsCommunicationServer_embedded.hpp>
#include <ometah/communication/itsCommunicationClient_embedded.hpp>

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
