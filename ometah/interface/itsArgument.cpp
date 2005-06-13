/***************************************************************************
 *  $Id: itsArgument.cpp,v 1.2 2005/06/13 12:06:04 jpau Exp $
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


#include "itsArgument.hpp"

using namespace std;


// ITS ARGUMENT


itsArgument::~itsArgument(){

}


itsArgument::itsArgument(string flagShort, string flagLong, string usage,
			 bool hasValue, string type, string value)
{

  this->flagShort = flagShort;
  this->flagLong = flagLong;
  this->usage = usage;
  this->hasValue = hasValue;
  this->type = type;
  this->value = value;
}


string itsArgument::getKey()
{
  return this->flagShort;
}


string itsArgument::getLongKey()
{
  return this->flagLong;
}


string itsArgument::getValue()
{
  if (this->hasValue)
    return this->value;
  return "";
}

string itsArgument::getUsage()
{
  return this->usage;
}

bool itsArgument::getHasValue()
{
  return this->hasValue;
}


// ITSPARSER 


itsArgumentParser::~itsArgumentParser()
{

}


itsArgumentParser::itsArgumentParser(int argc, vector<string> argv)
{
  this->argv = argv;
  this->argc = argc;
}


void itsArgumentParser::searchEndFlags() 
{
  if (this->defArg("-V", "--version", "check version", false, "", "")){
    throw VERSION;  
  }
  if (this->defArg("-h", "--help", "show help", false, "", "")){
    throw USAGE;
  }
}


// parse argv searching the given flag, then update the vector with a new argument
bool itsArgumentParser::defArg(string flagShort, string flagLong, string usage, 
                      bool hasValue, string type, string valueDefault)
{
  unsigned i = 1;
  bool found = false;
  string value = valueDefault;
  

  while ( (i < this->argv.size()) && (!found)) {

    if ( (flagShort == this->argv.at(i)) || (flagLong == this->argv.at(i))){

      if (hasValue){
	
	// if not end of line AND next word isnt a flag
	if ( (++i) < this->argv.size() && (this->argv.at(i)[0] != '-' ) )	
	  value = this->argv.at(i);	
	else { 
	  stringstream s;
	  s << "Value missing for " << flagShort << endl;
	  throw (s.str()).c_str();
	}      
      }
      else { // no value associated => arg has boolean value
	value = "true";
      }
      found = true;
    }
    i++;
  }
 
  itsArgument newArgument(flagShort, flagLong, usage, hasValue, type, value);
  arguments.push_back(newArgument);

  return found;
}


string itsArgumentParser::getStringValue(string key)
{

  vector<itsArgument>::iterator iter;
  iter = arguments.begin();
  while (iter != arguments.end()){
    if (iter->getKey() == key)
      return iter->getValue();
    iter++;
  }
  stringstream s;
  s << "getStringValue error with " << key << endl;
  throw (s.str()).c_str();
  return "";
}

double itsArgumentParser::getDoubleValue(string key)
{
  
  vector<itsArgument>::iterator iter;
  iter = arguments.begin();
  while (iter != arguments.end()){
    if (iter->getKey() == key){
      return atof((iter->getValue()).c_str());
    }
    iter++;
  }
  stringstream s;
  s << "getDoubleValue error with " << key << endl;
  throw (s.str()).c_str();
  return -1.;
}

int itsArgumentParser::getIntValue(string key){

  int i = -1;
  vector<itsArgument>::iterator iter;
  iter = arguments.begin();
  while (iter != arguments.end()){
    if (iter->getKey() == key){
      istringstream istr(iter->getValue() ); 
      istr >> i;
      return i;
    }
    iter++;
  }
  stringstream s;
  s << "getIntValue error with " << key << endl;
  throw (s.str()).c_str();
  return i;
}

bool itsArgumentParser::getBoolValue(string key)
{

  vector<itsArgument>::iterator iter;
  iter = arguments.begin();
  while ( iter != arguments.end() ) {
    if (iter->getKey() == key)
      return (iter->getValue() != "");
    iter++;
  }
  stringstream s;
  s << "getBoolValue error with " << key << endl;
  throw (s.str()).c_str();
  return false;
}


void itsArgumentParser::usage()
{

  cout << "\nUsage: " << this->argv.at(0) << " [options]" <<  endl; 
  cout << "Options:" << endl;
  vector<itsArgument>::iterator iter;
  iter = this->arguments.begin();
  while (iter != this->arguments.end()){
    printf(" %s, %13s\t%s\n", iter->getKey().c_str(), 
	   iter->getLongKey().c_str(), iter->getUsage().c_str());
    iter++;
  }  
}


// check if all flags ( "-? | --* like ) are known 
bool itsArgumentParser::syntaxCorrect()
{

  stringstream s;
  vector<string>::iterator iter;
  iter = this->argv.begin();

  iter ++; // reach first word after "./ometah"

  // check if all flags are known 
  while (iter < this->argv.end()){    
    if ((*iter)[0] == '-'){ // if is a flag (known or not)
      if (!isAnArgument(*iter)) { // if is not a known flag	
	s << "Unknown option: " << *iter << endl;	
	throw (s.str()).c_str();
	return false;
      }
      else { // is a correct flag (defArg-ed)	
	if (!this->hasValue(*iter)){
	  if (( (iter+1) != this->argv.end()) && ( (*(iter+1))[0] != '-')){
	     s << "Value not needed for " << *iter << endl;
	     throw (s.str()).c_str();
	     return false;
	  }
	}
	else { // hasValue, checked by defArg
	  iter++;
	}
      }
    }
    else { // != -? && --*
      s << "Unknown argument: " << *iter << endl;
      throw (s.str()).c_str();
      return false;
    }
    iter++;
  }
  return true;
}


// returns true if s is known as a long or short flag
bool itsArgumentParser::isAnArgument(string s)
{
  if (s[0] != '-') // if doesn't start with '-', cannot be a flag
    return false;
  vector<itsArgument>::iterator iter;
  iter = this->arguments.begin();
  while (iter != this->arguments.end()){
    if ((s == iter->getKey()) || (s == iter->getLongKey()))
      return true;
    iter++;
  }
  return false;
}


bool itsArgumentParser::hasValue(string flag)
{
  vector<itsArgument>::iterator iter;
  iter = arguments.begin();
  while (iter != arguments.end()){
    if ((flag == iter->getKey()) || (flag == iter->getLongKey())){
      return iter->getHasValue();
    }
    iter++;
  }
  stringstream s;
  s << "Unexpected error in hasValue()" << endl;
  throw (s.str()).c_str();
  return false;
}