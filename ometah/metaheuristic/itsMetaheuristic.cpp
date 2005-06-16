/**************************************************************************** 
 *  $Id: itsMetaheuristic.cpp,v 1.4 2005/06/16 10:17:04 nojhan Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
 *  Author : Walid Tfaili <tfaili@univ-paris12.fr>
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
 

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <sys/timeb.h>
#include "itsMetaheuristic.hpp"
#include "../common/random.hpp"
#include "../common/string.hpp"
#include "../common/itsPoint.hpp"
#include "../problem/itsProblem.hpp"
#include "../communication/itsCommunicationClient.hpp"
 
using namespace std;
 
//! global optimization methods 
/*! 
  All these methods are to be partially or completly 
  instanciated by the user who creates a special 
  metaheuristics
*/
 /*
void itsMetaheuristic::intensification()
{ }
void itsMetaheuristic::diversification()
{ }
void itsMetaheuristic::learning()
{ }
void itsMetaheuristic::parseSpecific()
{ }


itsMetaheuristic::~itsMetaheuristic()
{
}
*/

itsMetaheuristic::itsMetaheuristic()
{
  setName("Unknown");
  setAccronym("Unknown");
  setFamily("Unknown");
  setDescription("Unknown");
  setCitation("Unknown");

  setSampleSize(1);

  setIterationsMaxNumber(2);

  this->outEndResult = &cout;
  this->outProcessResult = &cout;
  this->outErrors = &cerr;
  this->outLog = &clog;
  this->outDebug = &cerr;

  this->iterationsCurrent=0;

  this->logLevel = 0;

  this->logKeys["optimum_value"] = 0;
  this->logKeys["optimum"] = 1;
  this->logKeys["steps"] = 1;
  this->logKeys["communications"] = 2;
  this->logKeys["instanciations"] = 2;
  this->logKeys["iterations"] = 2;
  this->logKeys["sample_steps"] = 3;
 
}

void itsMetaheuristic::initialization()
{
  // ask for the bounds
  vector<double> min = this->problem->boundsMinima();
  vector<double> max = this->problem->boundsMaxima();
  
  // create the initial sample
  for(unsigned int i=0; i<getSampleSize(); i++) {
    itsPoint aRandomPoint;
    // draw a vector in the bounds
    aRandomPoint.setSolution( randomUniform(min,max) );
    // ask for the associated values
    aRandomPoint = evaluate(aRandomPoint);
    // add it to the sample
    //sample.push_back(aRandomPoint);
    sample[i] = aRandomPoint;
  }
}

void itsMetaheuristic::start()
{
  printLog("steps","initialization");

  *outProcessResult << "<optimization>" << endl;

  initialization();

  // no iterations has been computed
  iterationsCurrent=0;

  // log
  ostringstream msg1;
  msg1 << "iteration " << iterationsCurrent;
  printLog("iterations", msg1.str() );
  printLog("sample_steps", "  sample_values "+printValues( this->sample ) );

  *outProcessResult << "<iteration id=\"" << iterationsCurrent << "\">" << endl;
  *outProcessResult << "<step class=\"" << "start" << "\">" << endl;
  outputSample();  
  *outProcessResult << "</step>" << endl;

    *outProcessResult << "<evaluations>";
        *outProcessResult << getEvaluationNumber();
    *outProcessResult << "</evaluations>" << endl;

  *outProcessResult << "</iteration>" << endl;



  // while no stopping criterion reached
  while( !isStoppingCriteria() ) {
  
    // log
    ostringstream msg;
    msg << "iteration " << iterationsCurrent+1;
    printLog("iterations", msg.str() );
  
    *outProcessResult << "<iteration id=\"" << iterationsCurrent << "\">" << endl;
  
  
    /* main phases */
    printLog("steps"," learning");
    learning(); // Learning phase
  
    *outProcessResult << "<step class=\"" << "learning" << "\">" << endl;
        outputSample();  
    *outProcessResult << "</step>" << endl;

    printLog("steps"," diversification");
    diversification(); // Diversification phase
  
    // log sample
    printLog("sample_steps", "  sample_values "+printValues( this->sample ) );
  
    *outProcessResult << "<step class=\"" << "diversification" << "\">" << endl;
        outputSample();  
    *outProcessResult << "</step>" << endl;

  
    printLog("steps"," intensification");
    intensification(); // Intensification phase
    
    // print in one line with a prefix
    printLog("sample_steps", "  sample_values "+printValues( this->sample ) );
    printLog("sample_steps", "  sample_solutions "+printSolutions( this->sample ) );
    // print in paragraphs without prefix
    printDebug("sample_values", printValues( this->sample, -1, " ", "\n"  )+"\n\n", false );
    printDebug("sample_solutions", printSolutions( this->sample, -1, " ", "\n"  )+"\n\n", false );


    *outProcessResult << "<step class=\"" << "intensification" << "\">" << endl;
        outputSample();  
    *outProcessResult << "</step>" << endl;


    *outProcessResult << "<evaluations>";
        *outProcessResult << getEvaluationNumber();
    *outProcessResult << "</evaluations>" << endl;

    *outProcessResult << "</iteration>" << endl;
    
    // one more iteration
    iterationsCurrent++;
  
  }
  
  printLog("steps","end");
  printLog("optimum", "optimum "+print( getOptimum().getValues() ) ); // FIXME : print the solution too

  // print the result
  outputOptimum();

  *outProcessResult << "</optimization>" << endl;
}

void itsMetaheuristic::parameterParse()
{ }
void itsMetaheuristic::parameterParseCommon()
{ }

bool itsMetaheuristic::isStoppingCriteria()
{
  // if the maximum iterations number has been reached
  if(iterationsCurrent>=iterationsMaxNumber) {
    printLog("steps","Maximum iterations reached");
    return true;

  // if the maximum evaluations number has been reached
  } else if ( evaluationsNumber >= evaluationsMaxNumber ) {
    printLog("steps","Maximum evaluations reached");
    return true;
  
  // if the minimum value has been reached
  } else if (getOptimum().getValues()[0] <= getValueMin() ) {
    printLog("steps","Minimum value reached");
    return true;
  
  } else {
    // no stopping criterion reached
    return false;
  }
}


string itsMetaheuristic::getKey()
{
    return this->key;
}


void itsMetaheuristic::setKey( string key )
{
    this->key = key;
}



string itsMetaheuristic::getName()
{
    return this->name;
}

void itsMetaheuristic::setName(string name)
{
    this->name = name;
}


string itsMetaheuristic::getAccronym()
{
    return this->accronym;
}

void itsMetaheuristic::setAccronym(string accronym)
{
    this->accronym = accronym;
}


string itsMetaheuristic::getDescription()
{
    return this->description;
}

void itsMetaheuristic::setDescription(string description)
{
    this->description = description;
}
  

string itsMetaheuristic::getCitation()
{
    return this->citation;
}

void itsMetaheuristic::setCitation(string citation)
{
    this->citation = citation;
}


string itsMetaheuristic::getFamily()
{
    return this->family;
}

void itsMetaheuristic::setFamily(string family)
{
    this->family = family;
}

string itsMetaheuristic::getInformations()
{
    // a stream is easier to manipulate
    stringstream infos;

    // Separator used between key and value
    string sep = ":\t";

    infos << "Key" << sep << getKey() << endl;
    infos << "Family" << sep << getFamily() << endl;
    infos << "Name" << sep << getName() << endl;
    infos << "Accronym" << sep << getAccronym() << endl;
    infos << "Description" << sep << getDescription() << endl;
    infos << "Reference" << sep << getCitation() << endl;

    // get back the string only
    return infos.str();
}


string itsMetaheuristic::getInformations_XML()
{
    // a stream is easier to manipulate
    stringstream infos;

    infos << "<metaheuristic>" << endl;

    infos << "<key>" << getKey() << "</key>" << endl;
    infos << "<family>" << getFamily() << "</family>" << endl;
    infos << "<name>" << getName() << "</name>" << endl;
    infos << "<accronym>" << getAccronym() << "</accronym>" << endl;
    infos << "<description>" << getDescription() << "</description>" << endl;
    infos << "<reference>" << getCitation() << "</reference>" << endl;

    infos << "<parameters>" << endl;
        infos << getParameters_XML();
    infos << "</parameters>" << endl;

    infos << "</metaheuristic>" << endl;

    // get back the string only
    return infos.str();
}


string itsMetaheuristic::getParameters_XML()
{
    stringstream infos;

    infos << printParameter_XML( "sampleSize",getSampleSize() );
    infos << printParameter_XML( "iterationsMaxNumber",getIterationsMaxNumber() );
    infos << printParameter_XML( "evaluationsMaxNumber", getEvaluationsMaxNumber() );
    //infos << printParameter_XML( "", );

    return infos.str();
}
unsigned int itsMetaheuristic::getSampleSize()
{
    return this->sampleSize;
}

unsigned int itsMetaheuristic::getSampleSizeCurrent()
{
    return this->sample.size();
}

void itsMetaheuristic::setSampleSize(unsigned int size)
{
    this->sampleSize = size;
    // WARNING : changing the size may cause data loss or null points
    this->sample.resize(size);
}


unsigned int itsMetaheuristic::getIterationsMaxNumber()
{
    return this->iterationsMaxNumber;
}

void itsMetaheuristic::setIterationsMaxNumber(unsigned int nbr)
{
    this->iterationsMaxNumber = nbr;
}


void itsMetaheuristic::printLog(string level, string message)
{
    if( logLevel >= logKeys[level.c_str()] ) {
        *outLog << message << endl;
    }
}

void itsMetaheuristic::addDebugKey(string key) 
{
    this->debugKeys.insert( key );
}

int itsMetaheuristic::getLogLevel()
{
    return this->logLevel;
}
  

void itsMetaheuristic::setLogLevel(int level)
{
    this->logLevel = level;
}

itsPoint itsMetaheuristic::evaluate(itsPoint p) 
{
    itsPoint res = this->problem->call(p);
    printDebug("evaluations",res.getValues().size());

    // increment the evaluation counter
    this->evaluationsNumber++;

    return res;
}

itsPoint itsMetaheuristic::getOptimum()
{
    // FIXME : check if it is faster to use the sorting function
    itsPoint optimum = sample[0];
    for(unsigned int i=1; i<getSampleSizeCurrent(); i++) {
        if( isValueSmaller(sample[i],optimum) ) {
            optimum = sample[i];
        }
    }
    return optimum;
}

string itsMetaheuristic::printXMLPoint(itsPoint p) 
{
    ostringstream out;

    out << "<point>";
        out << "<values>";
        out << print( p.getValues(), " " );
        out << "</values>";

        out << "<solution>";
        out << print( p.getSolution(), " " );
        out << "</solution>";
    out << "</point>";

    return out.str();
}

void itsMetaheuristic::outputOptimum()
{
    *outEndResult << "<optimum>" << endl;
    *outEndResult << printXMLPoint( this->getOptimum() ) << endl;
    *outEndResult << "</optimum>" << endl;
}


void itsMetaheuristic::outputSample()
{
    *outProcessResult << "<sample>" << endl;
        for (unsigned int i=0; i<getSampleSizeCurrent(); i++) {
            *outProcessResult << printXMLPoint(this->sample[i]) << endl;
        }
    *outProcessResult << "</sample>" << endl;
}


void itsMetaheuristic::setOutEndResult(ostream* out)
{
    this->outEndResult = out;
}
  
void itsMetaheuristic::setOutProcessResult(ostream* out)
{
    this->outProcessResult = out;
}
  
void itsMetaheuristic::setOutErrors(ostream* out)
{
    this->outErrors = out;
}
  
void itsMetaheuristic::setOutLog(ostream* out)
{
    this->outLog = out;
}
  
void itsMetaheuristic::setOutDebug(ostream* out)
{
    this->outDebug = out;
}


unsigned int itsMetaheuristic::getEvaluationNumber()
{
    return this->evaluationsNumber;
}

void itsMetaheuristic::initRandom(unsigned seed){

  if (!seed)
    initRandom();
  srand(seed);
}

void itsMetaheuristic::initRandom(){

  struct timeb * tim;
  tim=(struct timeb*)malloc(sizeof(tim));
  ftime(tim);
  srand(tim->millitm);
  
}


unsigned int itsMetaheuristic::getEvaluationsMaxNumber()
{
    return this->evaluationsMaxNumber;
}


void itsMetaheuristic::setEvaluationsMaxNumber(unsigned int nb)
{
    this->evaluationsMaxNumber = nb;
}


double itsMetaheuristic::getValueMin()
{
    return this->valueThreshold;
}
  

void itsMetaheuristic::setValueMin(double val)
{
    this->valueThreshold = val;
}
