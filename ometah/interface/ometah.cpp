/***************************************************************************
 *  $Id: ometah.cpp,v 1.22 2006/02/22 14:02:04 nojhan Exp $
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

  factoryMetaheuristics = new itsHybridEstimationOfDistributionFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );
  
  factoryMetaheuristics = new itsRandomFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );

  factoryMetaheuristics = new itsSamplingFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );
  
  factoryMetaheuristics = new itsNelderMeadFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );
  
  factoryMetaheuristics = new itsSimpleGeneticFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );
  
  factoryMetaheuristics = new itsJpGeneticFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );
  
  factoryMetaheuristics = new itsHybridContinuousInteractingAntColonyFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );
  
  factoryMetaheuristics = new itsSimulatedAnnealingFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );
  
  
  /*
   *  Problems part
   */
    
  // the factory for problems
  itsProblemFactory* factoryProblems;

  // add Ackley
  factoryProblems = new itsAckleyFactory;
  setProblem.add( factoryProblems->create() );
  // add Griewank
  factoryProblems = new itsGriewankFactory;
  setProblem.add( factoryProblems->create() );   
  // add Rastrigin
  factoryProblems = new itsRastriginFactory;
  setProblem.add( factoryProblems->create() );
  // add Rosenbrock
  factoryProblems = new itsRosenbrockFactory;
  setProblem.add( factoryProblems->create() );
  // add Schwefel
  factoryProblems = new itsSchwefelFactory;
  setProblem.add( factoryProblems->create() );
  // add Sphere
  factoryProblems = new itsSphereFactory;
  setProblem.add( factoryProblems->create() );
  // add Weierstrass
  factoryProblems = new itsWeierstrassFactory;
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
  if (argumentParser.defArg("v", "verbose", "verbose level", true, "int", "0"))
    VERBOSE = argumentParser.getIntValue("verbose");
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
    argumentParser.defArg("p", "problem",
			  (problemUsage.str()).c_str(), 
			  true, "string", "Rosenbrock");
    argumentParser.defArg("m", "metah",
			  (metahUsage.str()).c_str(), 
			  true, "string", "CEDA");
    argumentParser.defArg("C", "com-client", 
			  (clientUsage.str()).c_str() ,
			  true, "string", "Embedded");
    argumentParser.defArg("S", "com-server", 
			  (serverUsage.str()).c_str() ,
			  true, "string", "Embedded");
    argumentParser.defArg("r", "random-seed", 
			  "seed of the pseudo-random generator (0 to use the clock)", true, "int", "0");
    argumentParser.defArg("a", "init-random-seed", 
			  "a specific random seed for the initialization step", true, "int", "0");
    argumentParser.defArg("D", "debug", 
			  "debug key" ,true, "string", "");
    argumentParser.defArg("i", "iterations", 
			  "maximum number of iterations" ,true, "int", "10000");
    argumentParser.defArg("e", "evaluations", 
			  "maximum number of evaluations" ,true, "int", "110");
    argumentParser.defArg("P", "precision", 
			  "minimum value to reach" ,true, "double", "0.0");
    argumentParser.defArg("s", "sample-size", 
			  "number of points in the sample" ,true, "int", "10");
    argumentParser.defArg("d", "dimension", 
			  "dimension of the problem" ,true, "int", "1");
    argumentParser.defArg("o", "output", 
			  "output of the results" ,true, "string", "-");
    argumentParser.defArg("l", "silent", 
			  "only output the number of evaluations and the optimums values" ,false);
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
    if ( !strcmp(VERSION_KEY, s) ) {
      cout << VERSION << endl;
      return -1;
    }
    else if (!strcmp(USAGE_KEY, s)) {
      cout << "Open Metaheuristics, a free framework for designing hard optimization algorithms." << endl;
      cout << "Version " << VERSION << endl;
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
	 << "\n problem: " << argumentParser.getStringValue("problem") 
	 << "\n metah: "<< argumentParser.getStringValue("metah")
	 << "\n client: " << argumentParser.getStringValue("com-client") 
	 << "\n server " << argumentParser.getStringValue("com-server")
	 << "\n seed: " << argumentParser.getIntValue("random-seed")
	 << "\n debug: "<< argumentParser.getStringValue("debug") 
	 << "\n iterations: " << argumentParser.getIntValue("iterations")
	 << "\n evaluations: " << argumentParser.getIntValue("evaluations")
	 << "\n precision: " << argumentParser.getDoubleValue("precision")
	 << "\n sample size: " << argumentParser.getIntValue("sample-size")
	 << "\n dimension: " << argumentParser.getIntValue("dimension")
	 << endl;
  }


  /* 
   *  Choose the items
   */
    
  
  setMetaheuristic.choose(argumentParser.getStringValue("metah"));
  setProblem.choose(argumentParser.getStringValue("problem"));
  setCommunicationClient.choose(argumentParser.getStringValue("com-client"));
  setCommunicationServer.choose(argumentParser.getStringValue("com-server"));


  if (VERBOSE)
    clog << "items chosen" << endl;


  /*
   * Check that given metah and problem exist 
   */
  char ok = 0;
  for (unsigned i = 0; i < setMetaheuristic.getKeyList().size() ; i++) {
    if ( setMetaheuristic.getKeyList()[i]  == argumentParser.getStringValue("metah"))
      ok = 1;
  }
  if (!ok) {
    cerr << "Given metaheuristic does not exist." << endl;
    return -1;
  }
  
  for (unsigned i = 0; i < setProblem.getKeyList().size() ; i++) {
    if ( setProblem.getKeyList()[i]  == argumentParser.getStringValue("problem"))
      ok = 1;
  }
  if (!ok) {
    cerr << "Given problem does not exist." << endl;
    return -1;
  }
   



  /*
   *  Links
   *  Warning : be sure to do the "choose" step before
   */
    
  // metaheuristic -> client
  setMetaheuristic.item()->problem = setCommunicationClient.item();
    
  // server -> problem
  setCommunicationServer.item()->problem = setProblem.item();
    
  // Special case for the embedded protocol : we must link client and server
  if( setCommunicationClient.item()->getKey() == argumentParser.getStringValue("com-client") && 
      setCommunicationServer.item()->getKey() ==  argumentParser.getStringValue("com-server")) {
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
  setMetaheuristic.item()->addDebugKey(argumentParser.getStringValue("debug"));
  //setMetaheuristic.item()->addDebugKey("selectNumber");

  // Log
  //setMetaheuristic.item()->setLogLevel(0);

  // parameters
  setProblem.item()->setDimension( argumentParser.getIntValue("dimension") );
  setMetaheuristic.item()->setSampleSize( argumentParser.getIntValue("sample-size") );

  // Stopping criteria
  setMetaheuristic.item()->setEvaluationsMaxNumber( argumentParser.getIntValue("evaluations") );
  setMetaheuristic.item()->setIterationsMaxNumber( argumentParser.getIntValue("iterations") );
  setMetaheuristic.item()->setValueMin( argumentParser.getDoubleValue("precision") );

  // Initialize pseudo random generator with time unit
  // (overloaded method exists with an unsigned parameter as seed)
  setMetaheuristic.item()->initRandom( argumentParser.getIntValue("random-seed") );
  setMetaheuristic.item()->setInitializationSeed( argumentParser.getIntValue("init-random-seed") );

  if (VERBOSE)
    clog << "parameters ok, starting optimization..." << endl;
    
  // Starting the optimization
  
  if (VERBOSE)
    clog << "Launching " << setMetaheuristic.item()->getName() 
	 << " on " << setProblem.item()->getName() 
	 << " using " << setCommunicationClient.item()->getKey() << endl;
  
  ostream * pout;
  ofstream outfile;
  
  // if we want the output on stdout
  if( argumentParser.getStringValue("output") == "" || 
      argumentParser.getStringValue("output") == "-" ) {
      pout = &cout;
      
  // if we want to output to a file
  } else {
      outfile.open ( argumentParser.getStringValue("output").c_str(), ofstream::out | ofstream::app);
      pout = &outfile;
  }
  // link process and en output
  setMetaheuristic.item()->setOutProcessResult(pout);
  setMetaheuristic.item()->setOutEndResult(pout);
  
  // if we are not asking for silence
  if( ! argumentParser.isAsked("silent") ) {
    *pout << "<? xml-version=\"1.0\" encoding=\"iso-8859-15\" ?>" << endl;
    *pout << "<ometah>" << endl;
    *pout << setProblem.item()->getInformations_XML() << endl;
    *pout << setMetaheuristic.item()->getInformations_XML() << endl;
  }

  try {
    // if we are asking for silence
    if( argumentParser.isAsked("silent") ) {
      setMetaheuristic.item()->initialization(); // initialization step is not call in silent mode
      setMetaheuristic.item()->startSilent(); // silent mode
    
      // print out the evaluation number
      *pout << setMetaheuristic.item()->getEvaluationNumber() << " ";
    
      // print out one optimum only
      itsPoint optim = setMetaheuristic.item()->getOptimum();
      
      *pout << optim.getValues()[0];
      for(unsigned int i=1; i < optim.getValuesNumber(); i++) {
        *pout << "," << optim.getValues()[i];
      }
      
      *pout << " ";
      
      *pout << optim.getSolution()[0];
      for(unsigned int i=1; i < optim.getSolutionDimension(); i++) {
        *pout << "," << optim.getSolution()[i];
      }
      *pout << endl;
    
    } else {
      setMetaheuristic.item()->start();
    }
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

  // if we are not asking for silence
  if( ! argumentParser.isAsked("silent") ) {
    *pout << "</ometah>" << endl;
  }
  
  // close the file if necessary
  if( outfile.is_open() ) {
      outfile.close();
  }

}
