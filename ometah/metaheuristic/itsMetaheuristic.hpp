/***************************************************************************
 *  $Id: itsMetaheuristic.hpp,v 1.23 2006/05/13 10:05:54 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Walid Tfaili <tfaili@univ-paris12.fr>
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
 *  for difficult optimization.
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Publi License as published by
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
 
#ifndef ITSMETAHEURISTIC
#define ITSMETAHEURISTIC

#if !WIN32
#include <sys/types.h>
#include <unistd.h>
#endif


#include <ostream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <set.h>
#include <sys/timeb.h>

#include <map>

#include "../common/itsPoint.hpp"
#include "../common/Exception_oMetah.hpp"
#include "../communication/itsCommunicationClient.hpp"

#include "../common/itsSetItem.hpp"
#include "../common/random.hpp"
#include "../common/matrix.hpp"
#include "../common/string.hpp"
#include "../common/itsPoint.hpp"
#include "../problem/itsProblem.hpp"
#include "../communication/itsCommunicationClient.hpp"


using namespace std;

namespace ometah {

/*! 
  is based on the "Abstract Factory" Pattern
*/
 
class itsMetaheuristic : public itsSetItem
{
protected:
  //! The intensification phase
  virtual void intensification() {};
      
  //! The diversification phase
  virtual void diversification() {};
      
  //! The learning phase
  virtual void learning() {};

  //! Called at the end of the start() method
  /*! 
    Use this method if you want to add a terminal
    process after the optimization (for example a 
    terminal local search).
  */
  virtual void end();
  
public:
  //! The initialization phase
  virtual void initialization();

protected:
  //! Test if a stopping criteria have been reached
  bool isStoppingCriteria();
  

  //! A specific parse
  /*! 
    A particular protocole to pass data from 
    the Metaheuristics to the Problems 
  */
  virtual void parseSpecific() {};
    
  //! The name of the algorithm
  /*! 
    This must be a unique name in the source tree
  */
  //string name;

  //! The unique key identifying the algorithm among other
  //string key;

  //! The acronym of the algorithm
  //string accronym;

  //! The family of the metaheuristic
  /*!
    One should put here :
      - Evolutionnary Algorithm
      - Simulated Annealing
      - Ant Colony Algorithm
      - Tabu Search
      - Estimation of Distribution Algorithm
      - Immunitary Algorithm
      - Particule Swarm Optimization
      - ...
      - Unknown
   */
  //string family;

  //! A brief description 
  //string description;

  //! The paper reference (if available)
  /*!
    The reference should be formated in BiBTeX.
  */
  //string citation;

  
  //! The points sample
  /*!
    This is the main data structure of the metaheuristic.
    This is the collection of point manipulated by the algorithm.
   */
  vector<itsPoint> sample;

  //! Number of points in the sample
  unsigned int sampleSize;

  //! Current iterations number
  unsigned int iterationsCurrent;

  //! Maximum iterations number
  /*!
    This is used if you choose the associated stopping criteria
   */
  unsigned int iterationsMaxNumber;  

  //! The true evaluations number
  unsigned int evaluationsNumber;

  //! Maximum evaluations number
  unsigned int evaluationsMaxNumber;

  //! The value threshold for stopping criterion
  /*!
      If the optimum value is better than this threshold,
      the stopping criterion is reached.
  */
  double valueThreshold;
  
  //! Flag for an internal stopping criterion
  bool isInternalStoppingCriterion;

  //! The random seed used
  unsigned int seed;
  
  //! A specific random seed to use at initialization step
  unsigned int initializationSeed;

  //! Stream for the main result of the optimization
  /*!  
    This is used for the output of the global optimum point
    (position and values).
  */
  ostream* outEndResult;

  //! Stream for the in progress results of the optimization
  /*!  
    This is used for the output of the points during the
    optimization process.
  */
  ostream* outProcessResult;

  //! Stream for the errors
  /*!
    Use this stream to output unrecoverable errors.
   */
  ostream* outErrors;

  //! Stream for log
  ostream* outLog;

  //! Stream for debug
  ostream* outDebug;

  //! The debug keys we want to use
  /*!
      This is a simple associative container, it contains only keys.
  */
  set<string> debugKeys;
  
  //! The log levels
  /*!
      This is an associative container of the form :
      level name => level number
  */
  //hash_map< const char*, int, hash<const char*>, eqstr > logKeys;
    map<string, int> logKeys;
  
  //! The current log levels
  int logLevel;
  
  //! Print a parameter in xml formated
  template<class T>
  string printParameter_XML(string name, T value) {
    stringstream infos;
    infos << "<parameter>" 
        << "<name>" << name << "</name>" 
        << "<value>" << value << "</value>" 
    << "</parameter>" << endl;
    return infos.str();
}


public:
  //! Virtual destructor
  /*!
    Necessary because of the abstract factory
  */
  virtual ~itsMetaheuristic();

  //! Constructor
  itsMetaheuristic()  : /*name("Unknown"), key(""), accronym("Unknown"), family("Unknown"), description("Unknown"), citation("Unknown"),*/
                        sampleSize(1), iterationsCurrent(0), iterationsMaxNumber(2), 
                        evaluationsNumber(0), evaluationsMaxNumber(0), valueThreshold(0), isInternalStoppingCriterion(false), 
                        seed(0), initializationSeed(0), 
                        outEndResult(&cout), outProcessResult(&cout), outErrors(&cerr), outLog(&clog), outDebug(&clog),
                        logLevel(0)
    {
        logKeys["optimum_value"] = 0;
        logKeys["optimum"] = 1;
        logKeys["steps"] = 1;
        logKeys["communications"] = 2;
        logKeys["instanciations"] = 2;
        logKeys["iterations"] = 2;
        logKeys["sample_steps"] = 3;
    
        initRandom();

        clog << "itsMetaheuristic::itsMetaheuristic  " /*- getLogLevel:" << getLogLevel() */ << " - logLevel:" << logLevel << endl;
        clog << "itsMetaheuristic::itsMetaheuristic  " << " - logKeys.size:" << logKeys.size() << endl;
    }

  //! The pointer to the problem
  itsCommunicationClient* problem;

  //! Start executing a metaheuristic
  void start();

  //! Start a metaheuristic in silent mode
  /*! 
    WARNING : contrary to start(), startSilent() does not
    call initialization() as a first step !
  */
  void startSilent();
      
  //! Take a point and evaluate its values
  itsPoint evaluate(itsPoint p);
  
  //! Evaluate all the points in the sample itself
  void evaluate();
  
  //! Find the optimum
  itsPoint getOptimum();
  
  //! Parse data from an interface
  void parameterParse();
    
  //! Parse data common to all metaheuristics
  void parameterParseCommon();

/*
  //! Simple attribute accesses
  //!@{

  //! Return the name
  string getName() const {return name;}
  //! Set the name
  void setName(string name);

  //! Return the key
  string getKey() const {return key;}
  //! Change the key
  void setKey( string key );
  
  //! Return the accronym
  string getAccronym() const {return accronym;}
  //! Change the Accronym
  void setAccronym(string accronym);

  //! Return the description
  string getDescription() const {return description;}
  //! Change the description
  void setDescription(string description);
  
  //! Return the reference
  string getCitation() const {return citation;}
  //! Change the reference
  void setCitation(string citation);

  //! Return the family
  string getFamily() const {return family;}
  //! Change the reference
  void setFamily(string family);
*/
  //! Return a formated output of all the informations
  /*!
    Return the Key, Family, Name, Accronym, Description and Reference 
    (in this order) of the algorithm, if given.
    The format is the following :
        Key:\tValue\n
  */
  //string getInformations();

  //! Return a formated output of all the informations
  /*!
    Return the Key, Family, Name, Accronym, Description and Reference 
    (in this order) of the algorithm, if given. In xml format.
  */
  //string getInformations_XML();
  
  //! Return a formated output with the values of parameters
  string getParameters_XML();

  //! Return the sample size
  unsigned int getSampleSize() const {return sampleSize;}
  //! Return the true current sample size_t
  unsigned int getSampleSizeCurrent() const {return sample.size();}
  //! Change the sample size
  /*!
    WARNING : changing the size may cause data loss
  */
  void setSampleSize(unsigned int size);

  //! Return only the coordinates part of the sample
  vector<vector<double> > getSampleSolutions();

  //! Return the minimums coordinates of the population
  vector<double> getSampleSolutionsMin();

  //! Return the maximums coordinates of the population
  vector<double> getSampleSolutionsMax();
  
  
  //! Return the maximum iterations number
  unsigned int getIterationsMaxNumber() const {return iterationsMaxNumber;}
  //! Change the  maximum iterations number
  void setIterationsMaxNumber(unsigned int nbr);
  
  //! Return the current number of evaluationsNumber
  unsigned int getEvaluationNumber() const {return evaluationsNumber;}
  
  //! Return the maximum evaluations number allowed
  unsigned int getEvaluationsMaxNumber() const {return evaluationsMaxNumber;}
  
  //! Change the max evaluation number
  void setEvaluationsMaxNumber(unsigned int nb);
  
  //! Return the value threshold
  double getValueMin() const {return valueThreshold;}
  
  //! Change the value threshold
  void setValueMin(double val);
  
  //! Return the log level  
  int getLogLevel() const {return logLevel;};
  //! Change the log level
  void setLogLevel(int level);

  //! Return the log keys
  map<string, int> getLogKeys() const {clog << "itsMetaheuristic::getLogKeys - logKeys.size:" << logKeys.size() << endl;return logKeys;}

  //! Change the output for end results
  void setOutEndResult(ostream* out);
  
  //! Change the output for in process results
  void setOutProcessResult(ostream* out);
  
  //! Change the output for errors
  void setOutErrors(ostream* out);
  
  //! Change the output for log
  void setOutLog(ostream* out);
  
  //! Change the output for debug
  void setOutDebug(ostream* out);
  
  //@}
  
  
  //! Output a debug variable
  /*!
      Print the debug variable if its name is in debugKeys.
  */
  template<class T>
  void printDebug(string id, T var, bool header=true) {
    // if the string id is in keys, then print var on debug output
  if( this->debugKeys.count( id.c_str() ) > 0 ) {
      if(header) {
          *outDebug << id << " " << var << endl;
      } else {
          *outDebug << var << endl;
      }
    }
  }

  //! Output a log message
  /*! 
      Print the message if its level is greater than logKeys[level]
  */
  void printLog(string level, string message);

  
  //! Add a debug key
  void addDebugKey(string key);

  //! Formatted output for the end result
  void outputOptimum();

  //! Formatted output for in progress result
  void outputSample();

  //! Format a point in xml
  string printXMLPoint(itsPoint p);

  //! Initialize the random generator on a given seed
  void initRandom(unsigned int seed);

  //! Initialize the random generator on time
  void initRandom();
  
  //! Return the random seed used
  unsigned int getSeed() const {return seed;}
  
  //! Change the initialization seed
  void setInitializationSeed(unsigned int seed);
  
  //! Return the initialization seed
  unsigned int getInitializationSeed() const {return initializationSeed;}
};



  
//! Abstract factory base class for metaheuristic instances
/*! 
  This is the base class for all factory classes
*/

class itsMetaheuristicFactory
{
public:
  //! base method for instanciation in the abstract factory 
  virtual itsMetaheuristic* create() =0;

  virtual ~itsMetaheuristicFactory() {};
};

}//ometah

#endif
