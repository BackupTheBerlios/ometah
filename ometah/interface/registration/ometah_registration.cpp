/***************************************************************************
 *  $Id: ometah_registration.cpp,v 1.19 2005/07/25 09:36:50 nojhan Exp $
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
 
#include <pthread.h> 
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
#include "../../metaheuristic/estimation/itsHybridEstimationOfDistribution.hpp"
#include "../../metaheuristic/sampling/itsGridSampling.hpp"
#include "../../metaheuristic/neldermead/itsNelderMead.hpp"
#include "../../metaheuristic/genetic/itsSimpleGenetic.hpp"
#include "../../metaheuristic/genetic/itsJpGenetic.hpp"
#include "../../metaheuristic/antcolony/itsHybridContinuousInteractingAntColony.hpp"

// problems
#include "../../problem/registration/itsRegistration.hpp"

// communication
#include "../../communication/itsCommunicationServer_embedded.hpp"
#include "../../communication/itsCommunicationClient_embedded.hpp"

// interfaces
#include "../itsArgument.hpp"

using namespace std;
using namespace cimg_library;



// global variables are evil
float global_Smooth = 0;
float global_Precision = 0.9;
bool global_PerformFlow = false;
itsRegistration problemRegistration;


/*******************************************************************************
* optical flow functions
*******************************************************************************/

CImg<> opticalFlowMono_Reg(const CImg<>& I1, const CImg<>& I2, const CImg<>& u0,
                           float smooth, float precision) 
{

  CImg<> u = u0.get_resize(I1.width,I1.height,1,2,3),dI(u);
  CImg_3x3(I,float);
					   
  float dt = 2;
  float Ep = 1e20f;

  // premières dérivées de I2
  cimg_map3x3(I2,x,y,0,0,I) {
    dI(x,y,0) = 0.5f*(Inc-Ipc);
    dI(x,y,1) = 0.5f*(Icn-Icp);
  }

  // boucle principale EDP
  for (unsigned int iter=0; iter<100000; iter++) {
    //std::fprintf(stderr,"\r- Iteration %d - E = %g",iter,Ep); fflush(stderr);
    cerr << "\rOptical Flow : Iteration: " << iter << " / Ep:" << Ep;
    cerr.flush();
    const float Eold = Ep;
    Ep = 0;
	  
    cimg_3mapXY(u,x,y) {
         
      const float X = x + u(x,y,0);
      const float Y = y + u(x,y,1);
      
      const float deltaI = (float)(I2.linear_pix2d(X,Y) - I1(x,y));
      
      float tmpf = 0;
      
      cimg_mapV(u,k) {
        const float ux  = 0.5f*(u(_nx,y,k)-u(_px,y,k));
        const float uy  = 0.5f*(u(x,_ny,k)-u(x,_py,k));
        
        u(x,y,k) = (float)( u(x,y,k) +
                              dt*(
                                  -deltaI*dI.linear_pix2d(X,Y,k) +
                                  smooth* ( u(_nx,y,k) + u(_px,y,k) + u(x,_ny,k) + u(x,_py,k) )
                                  )
                              )/(1+4*smooth*dt);
        
        tmpf += ux*ux + uy*uy;
      }
      
      Ep += deltaI*deltaI + smooth * tmpf;
    }
	
	
    if (fabs(Eold-Ep)<precision) break;
    if (Eold<Ep) dt*=0.5;
  }
  return u;
}



/*******************************************************************************
* end local search for classes
*******************************************************************************/


CImg<unsigned char> makeRegisteredImage( CImg<unsigned char> imgOrigin, int rx, int ry, double rotation=0, double zoom=1 )
{
  CImg<unsigned char> imgResult = imgOrigin;

  imgOrigin = imgOrigin.get_rotate( rotation, 0.5f*imgOrigin.width, 0.5f*imgOrigin.height, zoom, 0 );

  cimg_mapXY(imgResult,x,y) {
      // if we do not try out of bounds
    if ( !(x+rx<0 || x+rx>imgOrigin.width || y+ry<0 || y+ry>imgOrigin.height)  ) {
      imgResult(x,y) = imgOrigin(x+rx,y+ry);
    } else {
      imgResult(x,y) = 0;
    }
  }

  return imgResult;
}


// Multiline macros are evil too
/*inline
void end_Reg()*/

#define END_REG() \
  if ( global_PerformFlow ) { \
    vector<double> sol = this->getOptimum().getSolution(); \
    int rx = (int) floor(sol[0]); \
    int ry = (int) floor(sol[1]); \
    double rotation, zoom; \
    if ( sol.size() == 4 ) { \
      rotation = sol[2]; \
      zoom = sol[3]; \
    } else { \
      rotation = 0; \
      zoom = 1; \
    } \
    CImg<> img3 = makeRegisteredImage(problemRegistration.img2, rx, ry, rotation, zoom); \
    problemRegistration.img2 = opticalFlowMono_Reg(  \
                                               problemRegistration.img1,  \
                                               img3, img3, \
                                               global_Smooth, global_Precision ); \
    itsPoint p = evaluate( getOptimum() ); \
    setSampleSize( 1 ); \
    sample[0] = p; \
  } else  \
  (void)0


// CEDA
class itsEstimationOfDistribution_Reg : public itsEstimationOfDistribution
{
public: 
    void end() { END_REG(); }
};

class itsEstimationOfDistributionFactory_Reg : public itsEstimationOfDistributionFactory
{
public:
    itsMetaheuristic* create() {return new itsEstimationOfDistribution_Reg;}
};


// CHEDA
class itsHybridEstimationOfDistribution_Reg: public itsHybridEstimationOfDistribution
{
public: 
    void end() { END_REG(); }
};

class itsHybridEstimationOfDistributionFactory_Reg : public itsHybridEstimationOfDistributionFactory
{
public:
    itsMetaheuristic* create() {return new itsHybridEstimationOfDistribution_Reg;}
};


// GS
class itsGridSampling_Reg: public itsGridSampling
{
public: 
    void end() { END_REG(); }
};

class itsSamplingFactory_Reg : public itsSamplingFactory
{
public:
    itsMetaheuristic* create() {return new itsGridSampling_Reg;}
};


// NMS
class itsNelderMead_Reg : public itsNelderMead
{
public: 
    void end() { END_REG(); }
};

class itsNelderMeadFactory_Reg : public itsNelderMeadFactory
{
public:
    itsMetaheuristic* create() {return new itsNelderMead_Reg;}
};


//SGEN
class itsSimpleGenetic_Reg : public itsSimpleGenetic
{
public: 
    void end() { END_REG(); }
};

class itsSimpleGeneticFactory_Reg : public itsSimpleGeneticFactory
{
public:
    itsMetaheuristic* create() {return new itsSimpleGenetic_Reg;}
};


// JGEN
class itsJpGenetic_Reg : public itsJpGenetic
{
public: 
    void end() { END_REG(); }
};

class itsJpGeneticFactory_Reg : public itsJpGeneticFactory
{
public:
    itsMetaheuristic* create() {return new itsJpGenetic_Reg;}
};


//HCIAC
class itsHybridContinuousInteractingAntColony_Reg : public itsHybridContinuousInteractingAntColony
{
public: 
    void end() { END_REG(); }
};

class itsHybridContinuousInteractingAntColonyFactory_Reg : public itsHybridContinuousInteractingAntColonyFactory
{
public:
    itsMetaheuristic* create() {return new itsHybridContinuousInteractingAntColony_Reg;}
};



/*******************************************************************************
* interface
*******************************************************************************/


int main(int argc, char ** argv)
{

  // differents sets of objects
  itsSet<itsMetaheuristic*> setMetaheuristic;
  itsCommunicationClient_embedded communicationClient;
  itsCommunicationServer_embedded communicationServer;


  /*
   *  Metaheuristics part
   */
    
  // the factory for metaheuristics
  itsMetaheuristicFactory* factoryMetaheuristics;

  // add the estimation of distribution algorithm
  factoryMetaheuristics = new itsEstimationOfDistributionFactory_Reg;
  setMetaheuristic.add( factoryMetaheuristics->create() );

  factoryMetaheuristics = new itsHybridEstimationOfDistributionFactory_Reg;
  setMetaheuristic.add( factoryMetaheuristics->create() );

  factoryMetaheuristics = new itsSamplingFactory_Reg;
  setMetaheuristic.add( factoryMetaheuristics->create() );

  factoryMetaheuristics = new itsNelderMeadFactory_Reg;
  setMetaheuristic.add( factoryMetaheuristics->create() );
  
  factoryMetaheuristics = new itsSimpleGeneticFactory_Reg;
  setMetaheuristic.add( factoryMetaheuristics->create() );

  factoryMetaheuristics = new itsJpGeneticFactory_Reg;
  setMetaheuristic.add( factoryMetaheuristics->create() );

  factoryMetaheuristics = new itsHybridContinuousInteractingAntColonyFactory_Reg;
  setMetaheuristic.add( factoryMetaheuristics->create() );

  
  
  /*
   *  Problems part
   */
    
  //problem = itsRegistration();
    
    
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
              
              
    argumentParser.defArg("-c", "--bounds-coefficient", 
			  "x and y bounds ratio from the image size" ,true, "double", "0.06");
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
        
    argumentParser.defArg("-f", "--optical-flow", 
			  "perform a terminal mono-scale optical flow, with the given precision" ,true, "double", "0.9");
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
  communicationServer.problem = & problemRegistration;


  /*
   *  Parameter setting
   */
    
  // giving parameters
  hash_map<string,string, eqstr> parameters;
  communicationClient.initialization( parameters );

  setMetaheuristic.item()->setLogLevel(argumentParser.getIntValue("--verbose"));
  
  problemRegistration.setBoundsCoefficient( (float)argumentParser.getDoubleValue("--bounds-coefficient") );
  
  if ( argumentParser.isAsked("--affine") ) {
      problemRegistration.setDimension(4);
      
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
  
      problemRegistration.setBoundsMinima(mins);
      problemRegistration.setBoundsMaxima(maxs);
  
  } else {
      problemRegistration.setDimension(2);
  }
    
  /*
   *  Launch the optimization
   */
    
  // TESTS

  // Debug keys
  setMetaheuristic.item()->addDebugKey(argumentParser.getStringValue("--debug"));

  // parameters
try {
  problemRegistration.setInputImages(
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
  
  
  if ( argumentParser.isAsked("--optical-flow") ) {
    global_PerformFlow = true;
    global_Precision = argumentParser.getDoubleValue("--optical-flow");
    // FIXME ########################################################################################"
  }
  

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
  *pout << problemRegistration.getInformations_XML() << endl;
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
      if ( problemRegistration.getDimension() == 4 ) {
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
