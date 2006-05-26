/***************************************************************************
 *  $Id: ometahcats.cpp,v 1.1 2006/05/26 11:00:42 nojhan Exp $
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

#include "ometahcats.hpp"

using namespace std;


int main(int argc, char ** argv)
{

  // differents sets of objects
  itsSet<itsMetaheuristic*> setMetaheuristic;

  // specified items
  //itsRosenbrock theProblem;
  itsCatsProblem theProblem;
  itsCommunicationClient_embedded theCommunicationClient;
  itsCommunicationServer_embedded theCommunicationServer;

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
   * Arguments
   */

  // fill the argv vector
  vector<string> argumentsVector;
  for(int i=0; i<argc; i++){
    argumentsVector.push_back(argv[i]);
  }

  // create a parser
  itsArgumentParser argumentParser(argc, argumentsVector);

  /* 
   * Make the usage strings
   */
  
  stringstream metahUsage;
  metahUsage << "metaheuristic name (" << print(setMetaheuristic.getKeyList()) << ")";
  

  try {
    // arguments definitions (only here !!)
    argumentParser.defArg("-m", "--metah",
			  (metahUsage.str()).c_str(), 
			  true, "string", "CEDA");
    argumentParser.defArg("-r", "--random-seed", 
			  "seed of the pseudo-random generator", true, "int", "0");
    argumentParser.defArg("-D", "--debug", 
			  "debug key" ,true, "string", "");
    argumentParser.defArg("-i", "--iterations", 
			  "maximum number of iterations" ,true, "int", "10000");
    argumentParser.defArg("-e", "--evaluations", 
			  "maximum number of evaluations" ,true, "int", "110");
    argumentParser.defArg("-P", "--precision", 
			  "minimum value to reach" ,true, "double", "0.0");
    argumentParser.defArg("-s", "--sample-size", 
			  "number of points in the sample" ,true, "int", "10");
    argumentParser.defArg("-o", "--output", 
			  "output of the results" ,true, "string", "");
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
      cout << "Interface using Open Metaheuristics to solve the CATS reconstruction problem." << endl;
      cout << "Version " << VERSION << endl;
      argumentParser.usage();
      return -1;
    }
  }

  try {
    argumentParser.syntaxCorrect();
  }
  catch (const char * s) {
    cerr << s;
    argumentParser.usage();
    return -1;
  }

  /* 
   *  Choose the items
   */
  setMetaheuristic.choose(argumentParser.getStringValue("--metah"));


  /*
   * Check that given metah exist 
   */
  char ok = 0;
  for (unsigned i = 0; i < setMetaheuristic.getKeyList().size() ; i++) {
    if ( setMetaheuristic.getKeyList()[i]  == argumentParser.getStringValue("--metah"))
      ok = 1;
  }
  if (!ok) {
    cerr << "Given metaheuristic does not exist." << endl;
    return -1;
  }
   

  /*
   *  Links
   *  Warning : be sure to do the "choose" step before
   */
    
  // metaheuristic -> client
  setMetaheuristic.item()->problem = & theCommunicationClient;
    
  // server -> problem
  theCommunicationServer.problem = & theProblem;

  // Special case for the embedded protocol : we must link client and server
  theCommunicationClient.problem = & theCommunicationServer;

    
  /*
   *  Launch the optimization
   */
    
  // TESTS

  // Debug keys
  setMetaheuristic.item()->addDebugKey(argumentParser.getStringValue("--debug"));

  // parameters
  setMetaheuristic.item()->setSampleSize( argumentParser.getIntValue("--sample-size") );

  // Stopping criteria
  setMetaheuristic.item()->setEvaluationsMaxNumber( argumentParser.getIntValue("--evaluations") );
  setMetaheuristic.item()->setIterationsMaxNumber( argumentParser.getIntValue("--iterations") );
  setMetaheuristic.item()->setValueMin( argumentParser.getDoubleValue("--precision") );

  // Initialize pseudo random generator with time unit
  // (overloaded method exists with an unsigned parameter as seed)
  setMetaheuristic.item()->initRandom( argumentParser.getIntValue("--random-seed") );

  // Starting the optimization
  ostream * pout;
  ofstream outfile;
  // if we want the output on stdout
  if( argumentParser.getStringValue("--output") == "" || 
      argumentParser.getStringValue("--output") == "-" ) {
      pout = &cout;
      
  // if we want to output to a file
  } else {
      outfile.open ( argumentParser.getStringValue("--output").c_str(), ofstream::out | ofstream::app);
      pout = &outfile;
  }
  // link process and en output
  setMetaheuristic.item()->setOutProcessResult(pout);
  setMetaheuristic.item()->setOutEndResult(pout);
  
  *pout << "<? xml-version=\"1.0\" encoding=\"iso-8859-15\" ?>" << endl;
  *pout << "<ometah>" << endl;
  *pout << theProblem.getInformations_XML() << endl;
  *pout << setMetaheuristic.item()->getInformations_XML() << endl;
  
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

  *pout << "</ometah>" << endl;
  
  // close the file if necessary
  if( outfile.is_open() ) {
      outfile.close();
  }
}
