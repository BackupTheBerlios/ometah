/***************************************************************************
 *  $Id: ometah_registration.cpp,v 1.9 2005/07/12 12:48:57 jpau Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
 *  for difficult optimization.
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
 
#include <hash_map.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#include <CImg.h>

// common stuff
#include "../../common/logic.hpp"
#include "../../common/itsSet.hpp"
#include "../../common/string.hpp"

// basic classes
#include "../../metaheuristic/itsMetaheuristic.hpp"
#include "../../problem/itsProblem.hpp"
#include "../../communication/itsCommunicationClient.hpp"
#include "../../communication/itsCommunicationServer.hpp"

// metaheuristics
#include "../../metaheuristic/estimation/itsEstimationOfDistribution.hpp"
#include "../../metaheuristic/random/itsRandom.hpp"
#include "../../metaheuristic/sampling/itsGridSampling.hpp"
#include "../metaheuristic/neldermead/itsNelderMead.hpp"
#include "../metaheuristic/simplegen/itsSimpleGen.hpp"

// problems
#include "../../problem/registration/itsRegistration.hpp"

// communication
#include "../../communication/itsCommunicationServer_embedded.hpp"
#include "../../communication/itsCommunicationClient_embedded.hpp"

// interfaces
#include "../itsArgument.hpp"

using namespace std;


int main(int argc, char ** argv)
{

  // differents sets of objects
  itsSet<itsMetaheuristic*> setMetaheuristic;
  itsRegistration problem;
  itsCommunicationClient_embedded communicationClient;
  itsCommunicationServer_embedded communicationServer;


  /*
   *  Metaheuristics part
   */
    
  // the factory for metaheuristics
  itsMetaheuristicFactory* factoryMetaheuristics;

  // add the estimation of distribution algorithm
  factoryMetaheuristics = new itsEstimationOfDistributionFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );

  factoryMetaheuristics = new itsRandomFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );

  factoryMetaheuristics = new itsSamplingFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );

  factoryMetaheuristics = new itsNelderMeadFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );
  
  factoryMetaheuristics = new itsSimpleGenFactory;
  setMetaheuristic.add( factoryMetaheuristics->create() );

  
  /*
   *  Problems part
   */
    
  problem = itsRegistration();
    
    
  /*
   *  Communication part
   */
   
   
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
    argumentParser.defArg("-v", "--verbose", 
              "verbose level", true, "int", "0");
    argumentParser.defArg("-r", "--random-seed", 
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
    argumentParser.defArg("-o", "--output", 
			  "output of the results" ,true, "string", "");
              
    argumentParser.defArg("-a", "--affine", 
			  "perform a affine registration (x+y+rotation+zoom)" ,false);
    argumentParser.defArg("-t", "--bounds-rotation", 
			  "min and max values for the rotation (min,max)" ,true, "string", "-10,10");
    argumentParser.defArg("-g", "--bounds-zoom", 
			  "min and max values for the zoom (min,max)" ,true, "string", "0.9,1.1");
    argumentParser.defArg("-i1", "--image-static", 
			  "input image" ,true, "string", "");
    argumentParser.defArg("-i2", "--image-registered", 
			  "input image" ,true, "string", "");
    argumentParser.defArg("-w", "--save-registered-image", 
			  "output the registered image in a file" ,true, "string", "");
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
    if ( argumentParser.syntaxCorrect() ) {      
    }
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
   * Check that given metah exists 
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
  setMetaheuristic.item()->problem = & communicationClient;
    
  // client -> server
  communicationClient.problem = & communicationServer;

  // server -> problem
  communicationServer.problem = & problem;


  /*
   *  Parameter setting
   */
    
  // giving parameters
  hash_map<string,string, eqstr> parameters;
  communicationClient.initialization( parameters );

  setMetaheuristic.item()->setLogLevel(argumentParser.getIntValue("--verbose"));
  
  if ( argumentParser.isAsked("--affine") ) {
      problem.setDimension(4);
      
      vector<double> bounds_rotation = stringToDouble_shortcuts( argumentParser.getStringValue("--bounds-rotation") );
      vector<double> bounds_zoom = stringToDouble_shortcuts( argumentParser.getStringValue("--bounds-zoom") );
      
      vector<double> mins,maxs;
      
      mins.push_back(0);
      mins.push_back(0);
      mins.push_back( bounds_rotation[0] );
      mins.push_back( bounds_zoom[0] );
  
      maxs.push_back(1);
      maxs.push_back(1);
      maxs.push_back( bounds_rotation[1] );
      maxs.push_back( bounds_zoom[1] );
  
      problem.setBoundsMinima(mins);
      problem.setBoundsMaxima(maxs);
  
  } else {
      problem.setDimension(2);
  }
    
  /*
   *  Launch the optimization
   */
    
  // TESTS

  // Debug keys
  setMetaheuristic.item()->addDebugKey(argumentParser.getStringValue("--debug"));

  // parameters
try {
  problem.setInputImages(
    argumentParser.getStringValue("--image-static"),
    argumentParser.getStringValue("--image-registered")
  );
} catch (const char * s) {
    cerr << s << endl;
    return -1;
}

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
  *pout << problem.getInformations_XML() << endl;
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


  // output the registered image in a file
  if ( argumentParser.getStringValue("--save-registered-image") != "" ) {
      
      CImg<> img1( argumentParser.getStringValue("--image-static").c_str() );
      CImg<> img2( argumentParser.getStringValue("--image-registered").c_str() );
  
      CImg<> result = img1;
  
      itsPoint optimum = setMetaheuristic.item()->getOptimum();

      const unsigned int rx = (int) floor( optimum.getSolution()[0] );
      const unsigned int ry = (int) floor( optimum.getSolution()[1] );

      float angle, zoom;
      if ( problem.getDimension() == 4 ) {
          angle = (float) optimum.getSolution()[2];
          zoom = (float) optimum.getSolution()[3];
          img2 = img2.get_rotate( angle, 0.5f*img2.width, 0.5f*img2.height, zoom, 0 );
      }

      cimg_mapXY(img1,x,y) {
        float diff = 0.0;
          // if we do not try out of bounds
        if ( !(x+rx<0 || x+rx>img1.width || y+ry<0 || y+ry>img1.height)  ) {
            diff = img1(x,y) + img2(x+rx,y+ry);
        }
        result(x,y) = diff;
      }
  
      result.save( argumentParser.getStringValue("--save-registered-image").c_str() );
  }
}
