/***************************************************************************
 *  $Id: ometah.cpp,v 1.4 2005/06/13 15:06:00 jpau Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
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
 
 
#include "ometah.hpp"

using namespace std;


int main(int argc, char ** argv)
{

  // differents sets of objects
  itsSet<itsMetaheuristic*> setMetaheuristic;
  itsSet<itsProblem*> setProblem;
  itsSet<itsCommunicationClient*> setCommunicationClient;
  itsSet<itsCommunicationServer*> setCommunicationServer;


  /*
   *  Metaheuristics part
   */
    
  // the factory for metaheuristics
  itsMetaheuristicFactory* factoryMetaheuristics;

  // add the estimation of distribution algorithm
  factoryMetaheuristics = new itsEstimationOfDistributionFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );

  
  /*
   *  Problems part
   */
    
  // the factory for problems
  itsProblemFactory* factoryProblems;
    
  // add Rosenbrock
  factoryProblems = new itsRosenbrockFactory;
  setProblem.add( factoryProblems->create() );
  // add Ackley
  factoryProblems = new itsAckleyFactory;
  setProblem.add( factoryProblems->create() );
  // add Rastrigin
  factoryProblems = new itsRastriginFactory;
  setProblem.add( factoryProblems->create() );
  // add Weierstrass
  factoryProblems = new itsWeierstrassFactory;
  setProblem.add( factoryProblems->create() );
  // add Griewank
  factoryProblems = new itsGriewankFactory;
  setProblem.add( factoryProblems->create() );
  // add Sphere
  factoryProblems = new itsSphereFactory;
  setProblem.add( factoryProblems->create() );
  // add Schwefel
  factoryProblems = new itsSchwefelFactory;
  setProblem.add( factoryProblems->create() );
    
    
  /*
   *  Communication part
   */
      
  // the factory for communication client
  itsCommunicationClientFactory * factoryClient;
  itsCommunicationServerFactory * factoryServer;
    
  // add the embedded protocol
  factoryClient = new itsCommunicationClientFactory_embedded;
  setCommunicationClient.add( factoryClient->create() );
    
  factoryServer = new itsCommunicationServerFactory_embedded;
  setCommunicationServer.add( factoryServer->create() );

  /*
   * Arguments
   */

  // fill the argv vector
  vector<string> argumentsVector;
  for(int i=0; i<argc; i++){
    argumentsVector.push_back(argv[i]);
  }

  // create a parser
  itsArgumentParser argumentParser(argc, argumentsVector);

  int VERBOSE;
  if (argumentParser.defArg("-v", "--verbose", "verbose level", true, "int", "0"))
    VERBOSE = argumentParser.getIntValue("-v");
  else
    VERBOSE = 0;
    
  
  /* 
   * Make the usage strings
   */
  
  stringstream problemUsage;
  problemUsage << "problem name (" << print(setProblem.getKeyList()) << ")";
  
  stringstream metahUsage;
  metahUsage << "metaheuristic name (" << print(setMetaheuristic.getKeyList()) << ")";
  
  stringstream clientUsage;
  clientUsage << "protocol name for client (" << print(setCommunicationClient.getKeyList()) << ")";
  
  stringstream serverUsage;
  serverUsage << "protocol name for server (" << print(setCommunicationServer.getKeyList()) << ")";


  try {
    // arguments definitions (only here !!)
    argumentParser.defArg("-p", "--problem",
			  (problemUsage.str()).c_str(), 
			  true, "string", "Rosenbrock");
    argumentParser.defArg("-m", "--metah",
			  (metahUsage.str()).c_str(), 
			  true, "string", "CEDA");
    argumentParser.defArg("-C", "--com-client", 
			  (clientUsage.str()).c_str() ,
			  true, "string", "Embedded");
    argumentParser.defArg("-S", "--com-server", 
			  (serverUsage.str()).c_str() ,
			  true, "string", "Embedded");
    argumentParser.defArg("-r", "--ran-seed", 
			  "seed of the pseudo-random generator", true, "int", "0");
    argumentParser.defArg("-D", "--debug", 
			  "debug key" ,true, "string", "");
    argumentParser.defArg("-i", "--iterations", 
			  "maximum number of iterations" ,true, "int", "10");
    argumentParser.defArg("-e", "--evaluations", 
			  "maximum number of evaluations" ,true, "int", "1000");
    argumentParser.defArg("-P", "--precision", 
			  "minimum value to reach" ,true, "double", "0.0");
    argumentParser.defArg("-s", "--sample-size", 
			  "number of points in the sample" ,true, "int", "10");
    argumentParser.defArg("-d", "--dimension", 
			  "dimension of the problem" ,true, "int", "1");
  }
  catch(const char * s) {
    cerr << s;
    return -1;
  }


  // look for end flags (-V, -h, etc...)
  try {
    argumentParser.searchEndFlags();
  }
  catch (const char * s) {
    if ( !strcmp(VERSION, s) ) {
      cerr << s << endl;
      return -1;
    }
    else if (!strcmp(USAGE, s)) {
      argumentParser.usage();
      return -1;
    }
  }


  try {
    if (argumentParser.syntaxCorrect()){      
      if (VERBOSE)
	clog << "syntax ok" << endl;      
    }
  }
  catch (const char * s) {
    cerr << s;
    argumentParser.usage();
    return -1;
  }
  
  
  if (VERBOSE){
    clog << "\ngetValues :" 
	 << "\n problem: " << argumentParser.getStringValue("-p") 
	 << "\n metah: "<< argumentParser.getStringValue("-m")
	 << "\n client: " << argumentParser.getStringValue("-C") 
	 << "\n server " << argumentParser.getStringValue("-S")
	 << "\n seed: " << argumentParser.getIntValue("-r")
	 << "\n debug: "<< argumentParser.getStringValue("-D") 
	 << "\n iterations: " << argumentParser.getIntValue("-i")
	 << "\n evaluations: " << argumentParser.getIntValue("-e")
	 << "\n precision: " << argumentParser.getDoubleValue("-P")
	 << "\n sample size: " << argumentParser.getIntValue("-s")
	 << "\n dimension: " << argumentParser.getIntValue("-d")
	 << endl;
  }


  /* 
   *  Choose the items
   */
    
  try {
    setMetaheuristic.choose(argumentParser.getStringValue("-m"));
    setProblem.choose(argumentParser.getStringValue("-p"));
    setCommunicationClient.choose(argumentParser.getStringValue("-C"));
    setCommunicationServer.choose(argumentParser.getStringValue("-S"));
  }
  catch (const char * s){
    cerr << s;
    return -1;
  }

  if (VERBOSE)
    clog << "items chosen" << endl;


  /*
   *  Links
   *  Warning : be sure to do the "choose" step before
   */
    
  // metaheuristic -> client
  setMetaheuristic.item()->problem = setCommunicationClient.item();
    
  // server -> problem
  setCommunicationServer.item()->problem = setProblem.item();
    
  // Special case for the embedded protocol : we must link client and server
  if( setCommunicationClient.item()->getKey() == argumentParser.getStringValue("-C") && 
      setCommunicationServer.item()->getKey() ==  argumentParser.getStringValue("-S")) {
    setCommunicationClient.item()->problem = setCommunicationServer.item();
  }

  if (VERBOSE)
    clog << "links done" << endl;
  
    
  /*
   *  Parameter setting
   */
    
  // giving parameters
  hash_map<string,string, eqstr> parameters;
  setCommunicationClient.item()->initialization( parameters );

  if (VERBOSE) {
    clog << "initialization communication client done" << endl;
  }
  setMetaheuristic.item()->setLogLevel(VERBOSE);
  
    
  /*
   *  Launch the optimization
   */
    
  // TESTS

  // Debug keys
  setMetaheuristic.item()->addDebugKey(argumentParser.getStringValue("-D"));
  //setMetaheuristic.item()->addDebugKey("selectNumber");

  // Log
  //setMetaheuristic.item()->setLogLevel(0);

  // parameters
  setProblem.item()->setDimension( argumentParser.getIntValue("-d") );
  setMetaheuristic.item()->setSampleSize( argumentParser.getIntValue("-s") );

  // Stopping criteria
  setMetaheuristic.item()->setEvaluationsMaxNumber( argumentParser.getIntValue("-e") );
  setMetaheuristic.item()->setIterationsMaxNumber( argumentParser.getIntValue("-i") );
  setMetaheuristic.item()->setValueMin( argumentParser.getDoubleValue("-p") );

  // Initialize pseudo random generator with time unit
  // (overloaded method exists with an unsigned parameter as seed)
  setMetaheuristic.item()->initRandom();// argumentParser.getIntValue("-r") );

  if (VERBOSE)
    clog << "parameters ok, starting optimization..." << endl;
    
  // Starting the optimization
  
  if (VERBOSE)
    clog << "Launching " << setMetaheuristic.item()->getName() 
	 << " on " << setProblem.item()->getName() 
	 << " using " << setCommunicationClient.item()->getKey() << endl;
  
  
  setMetaheuristic.item()->setOutProcessResult(&cout);
  cout << "<? xml-version=\"1.0\" encoding=\"iso-8859-15\" ?>" << endl;
  cout << "<ometah>" << endl;
  cout << setProblem.item()->getInformations_XML() << endl;
  cout << setMetaheuristic.item()->getInformations_XML() << endl;
  
  try {
    setMetaheuristic.item()->start();
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