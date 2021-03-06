/***************************************************************************
 *  $Id: ometah.cpp,v 1.42 2006/09/09 20:18:33 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
 *  Author : Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
 *  Author : Walid Tfaili <walidtfaili@yahoo.fr>
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

using namespace ometah;

int main(int argc, char ** argv)
{

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
  
  
  try {
    // arguments definitions
#ifdef WITH_SOCKET
    argumentParser.defArg("t", "port", 
			  "network port to use for communication with the problem server (Socket communication client)" , true, "int", "80277");
    argumentParser.defArg("I", "ip", 
			  "ip address of the problem server (Socket communication client)" , true, "string", "localhost");
#endif
  
#ifdef WITH_PYTHON
    argumentParser.defArg("M", "py-module", 
			  "python module to use as the objective function (Python communication client)" , true, "string", "problem_for_ometah");
#endif
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
			  "dimension of the problem" ,true, "int", "2");
    argumentParser.defArg("o", "output", 
			  "output of the results" ,true, "string", "-");
    argumentParser.defArg("l", "silent", 
			  "only output the number of evaluations and the optimums values" ,false);
  }
  catch(const char * s) {
    cerr << s;
    exit(1);
  }
  catch( Exception_Argument & e ) {
      cerr << "Error: " << e.what() << endl;
      exit(1);
  }


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

#ifdef WITH_EACO
  factoryMetaheuristics = new itsEacoFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );
#endif

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


  // add CEC'05 SSRPO : F1
  factoryProblems = new itsF1_ShiftedSphere_Factory;
  setProblem.add( factoryProblems->create() );
  // add CEC'05 SSRPO : F2
  factoryProblems = new itsF2_ShiftedSchwefel_Factory;
  setProblem.add( factoryProblems->create() );
  // add CEC'05 SSRPO : F3
  factoryProblems = new itsF3_ShiftedRotatedElliptic_Factory;
  setProblem.add( factoryProblems->create() );
  // add CEC'05 SSRPO : F4
  factoryProblems = new itsF4_ShiftedSchwefelWithNoise_Factory;
  setProblem.add( factoryProblems->create() );
  // add CEC'05 SSRPO : F5
  factoryProblems = new itsF5_SchwefelBounds_Factory;
  setProblem.add( factoryProblems->create() );
  // add CEC'05 SSRPO : F6
  factoryProblems = new itsF6_ShiftedRosenbrock_Factory;
  setProblem.add( factoryProblems->create() );
  // add CEC'05 SSRPO : F7
  factoryProblems = new itsF7_ShiftedRotatedGriewank_Factory;
  setProblem.add( factoryProblems->create() );
  // add CEC'05 SSRPO : F8
  factoryProblems = new itsF8_RotatedAckleyWithBounds_Factory;
  setProblem.add( factoryProblems->create() );
  // add CEC'05 SSRPO : F9
  factoryProblems = new itsF9_ShiftedRastrigin_Factory;
  setProblem.add( factoryProblems->create() );
  // add CEC'05 SSRPO : F10
  factoryProblems = new itsF10_ShiftedRotatedRastrigin_Factory;
  setProblem.add( factoryProblems->create() );
  // add CEC'05 SSRPO : F11
  factoryProblems = new itsF11_ShiftedRotatedWeierstrass_Factory;
  setProblem.add( factoryProblems->create() );
  // add CEC'05 SSRPO : F12
  factoryProblems = new itsF12_Schwefel_Factory;
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

#ifdef WITH_SOCKET
  // add the socket protocol
  factoryClient = new itsCommunicationClientFactory_socket;
  setCommunicationClient.add( factoryClient->create() );
    
  factoryServer = new itsCommunicationServerFactory_socket;
  setCommunicationServer.add( factoryServer->create() );
#endif

#ifdef WITH_PYTHON
    bool python_path_error = false;
    try {
        // add the python protocol
        // we must use a dynamic casting to bypass the default constructor prototype
        itsCommunicationClient_python * ccp = new itsCommunicationClient_python( argv[0], argumentParser.getStringValue("py-module") );
        setCommunicationClient.add( dynamic_cast< itsCommunicationClient * >( ccp ) );
    }
    catch( Exception_Python_LookUp & e ) {
        cerr << "Warning: " << e.what() << endl;
        python_path_error = true;
    }
    catch( Exception_oMetah & e ) {
        cerr << "Error: " << e.what() << endl;
        exit(1);
    }
#endif
  
  /* 
   * Make the usage strings
   */
  
  stringstream problemUsage;
  problemUsage << "problem name [" << print(setProblem.getKeyList()) << "]";
  
  stringstream metahUsage;
  metahUsage << "metaheuristic name [" << print(setMetaheuristic.getKeyList()) << "]";
  
  stringstream clientUsage;
  clientUsage << "protocol name for client [" << print(setCommunicationClient.getKeyList()) << "]";
  
  stringstream serverUsage;
  serverUsage << "protocol name for server [" << print(setCommunicationServer.getKeyList()) << "]";

try {
    argumentParser.defArg("p", "problem",
			  (problemUsage.str()).c_str(), 
			  true, "string", "Ackley");
    argumentParser.defArg("m", "metah",
			  (metahUsage.str()).c_str(), 
			  true, "string", "CEDA");
    argumentParser.defArg("C", "com-client", 
			  (clientUsage.str()).c_str() ,
			  true, "string", "Embedded");
    argumentParser.defArg("S", "com-server", 
			  (serverUsage.str()).c_str() ,
			  true, "string", "Embedded");
    argumentParser.defArg("H", "help-on", 
			  "print informations about a given item (\"list\" show the available items)", true, "string", "list");
} catch(const char * s) {
    cerr << s;
    exit(1);
} catch( Exception_Argument & e ) {
      cerr << "Error: " << e.what() << endl;
      exit(1);
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
      cout << "Open Metaheuristics, a free framework for metaheuristics "
           << "(targets hard optimization problems with real parameters)." << endl;
      cout << "LGPL license, (c) Free Software Fundation." << endl;
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
  catch ( Exception_Argument & e) {
    cerr << "Error: " << e.what() << endl;
    argumentParser.usage();
    exit(1);
  }
  
  if( argumentParser.isAsked("help-on") ) {
      const string asked = argumentParser.getStringValue("help-on");
      
      const bool hm = get_help_on( &setMetaheuristic, asked, "metaheuristic" );
      const bool hp = get_help_on( &setProblem, asked, "problem" );
      const bool hc = get_help_on( &setCommunicationClient, asked, "communication client" );
      const bool hs = get_help_on( &setCommunicationServer, asked, "communication server" );
  
      if( hm || hp || hc || hs ) {
          // end without error
          exit(0);
          
      } else {
          // end with an error
          //cerr << "Error: Unknown item: " << argumentParser.getStringValue("help-on") << endl;
          //exit(1);
          string sep = ", ";
          cout << "Available items:" << endl;
          cout << "\tMetaheuristics: " 
               << print( setMetaheuristic.getKeyList(), sep )
               << endl;
          cout << "\tProblems: "
              << print( setProblem.getKeyList(), sep )
              << endl;
          cout << "\tCommunication clients: "
              << print( setCommunicationClient.getKeyList(), sep )
              << endl;
          cout << "\tCommunication servers: "
              << print( setCommunicationServer.getKeyList(), sep )
              << endl;
          exit(0);
      }
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
  setCommunicationServer.choose(argumentParser.getStringValue("com-server"));

#ifdef WITH_PYTHON
    if( python_path_error /*&& argumentParser.getStringValue("com-client") == "Python"*/ ) {
        cerr << "Warning: Python cannot find the necessary module/functions, "
             << "thus Python communication client is not available, Embedded protocol used instead." << endl;
        setCommunicationClient.choose("Embedded");
    } else {
        setCommunicationClient.choose(argumentParser.getStringValue("com-client"));
    }
#else
  setCommunicationClient.choose(argumentParser.getStringValue("com-client"));
#endif



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
  communication_bind( setMetaheuristic.item(), 
                      setCommunicationClient.item(), 
                      setCommunicationServer.item(), 
                      setProblem.item() 
                    );

  if (VERBOSE)
    clog << "links done" << endl;
  
    
  /*
   *  Parameter setting
   */
    
  // giving parameters
  //hash_map<string,string, eqstr> parameters;
  map<string,string> parameters;
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

  // the dimension is set only of we asked for it
  // else, the default dimension of the problem is used
  if( argumentParser.isAsked("dimension") ) {
      if( setProblem.item()->isDimensionFixed() ) { // warning if we ask to change a fixed dimension
          cerr << "Warning: dimension is fixed to " << setProblem.item()->getDimension() << " for this problem,"
               << " change it to " << argumentParser.getIntValue("dimension") << " may cause strange behaviour." << endl;
      }
      if(VERBOSE)
          cerr << "Warning: default problem dimension " << setProblem.item()->getDimension();
  
      setProblem.item()->setDimension( argumentParser.getIntValue("dimension") );
      
      if(VERBOSE)
          cerr << " changed to " << setProblem.item()->getDimension() << "." << endl;
  }

  setMetaheuristic.item()->setSampleSize( argumentParser.getIntValue("sample-size") );

  // Stopping criteria
  setMetaheuristic.item()->setEvaluationsMaxNumber( argumentParser.getIntValue("evaluations") );
  setMetaheuristic.item()->setIterationsMaxNumber( argumentParser.getIntValue("iterations") );

  setMetaheuristic.item()->setValueMin( argumentParser.getDoubleValue("precision") );

  // Initialize pseudo random generator with time unit
  // (overloaded method exists with an unsigned parameter as seed)
  setMetaheuristic.item()->initRandom( argumentParser.getIntValue("random-seed") );
  setMetaheuristic.item()->setInitializationSeed( argumentParser.getIntValue("init-random-seed") );

  // NOTE : if you want to set a parameter specific to an algorithms
  //        you must bypass the indirection of the sets
  //        use something like :
  /*
    // You must do it only for an item
    if( setMetaheuristic.item()->getKey() == "CEDA" ) {
        // use dynamic_cast
        itsEstimationOfDistribution * m = dynamic_cast<itsEstimationOfDistribution*>( setMetaheuristic.item() );
        // call the appropriate method
        m->setSelectRatio(0.1);
    }
  */
  
#ifdef WITH_SOCKET
    if( setCommunicationClient.item()->getKey()=="Socket" ) {
        if( argumentParser.isAsked("port") ) {
            itsCommunicationClient_socket * c = dynamic_cast<itsCommunicationClient_socket*>( setCommunicationClient.item() );
            c->setPort( argumentParser.getIntValue("port") );
        }
        if( argumentParser.isAsked("ip") ) {
            itsCommunicationClient_socket * c = dynamic_cast<itsCommunicationClient_socket*>( setCommunicationClient.item() );
            c->setHostIP( argumentParser.getStringValue("ip") );
        }
    }
#endif

  
  if (VERBOSE)
    clog << "parameters ok, starting..." << endl;
    
  
#ifdef WITH_SOCKET
  if( argumentParser.isAsked("com-server") && argumentParser.getStringValue("com-server") == "Socket" ) {
      //if(VERBOSE)
          clog << "Starting the socket server..." << endl;
      setCommunicationServer.item()->start();
      exit(0);
  }
#endif
  
  
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
      *pout << setMetaheuristic.item()->getEvaluationsNumber() << " ";
    
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
      // normal mode
      setMetaheuristic.item()->start();
    }
  }
  catch( Exception_Size_Index_Dimension & e ) {
    cerr << "Error: " << e.what() << endl;
    cerr << "Try asking for a higher dimension, using \"-d <number>\" when invoking ometah." << endl;
    exit(1);
  }
  catch( Exception_oMetah & e ) {
    cerr << "Error: " << e.what() << endl;
    exit(1);
  }
  catch(const char * str) {
    cerr << str << endl;
    exit(1);
  }
  catch(string str) {
    cerr << str << endl;
    exit(1);
  }
  catch (...) {
    cerr << "Cant handle error, stopping." << endl;
    exit(1);
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
