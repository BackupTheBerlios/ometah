/***************************************************************************
 *  $Id: itsArgument.cpp,v 1.19 2006/05/15 20:58:00 nojhan Exp $
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


#include "itsArgument.hpp"

#define FLAG_SHORT_MARK "-"
#define FLAG_LONG_MARK "--"

using namespace std;


namespace ometah {

itsArgument::~itsArgument(){

}


itsArgument::itsArgument(string flagShort, string flagLong, string usage,
			 bool hasValue, string type, string value, string defaultValue)
{

  this->flagShort = flagShort;
  this->flagLong = flagLong;
  this->usage = usage;
  this->hasValue = hasValue;
  this->type = type;
  this->value = value;
  this->defaultValue = defaultValue;
}


string itsArgument::getKey()
{
  return this->flagLong;
}

string itsArgument::getShortKey()
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

string itsArgument::getDefault()
{
  return this->defaultValue;
}

// ITSPARSER 


itsArgumentParser::~itsArgumentParser()
{

}


itsArgumentParser::itsArgumentParser(int argc, vector<string> argv)
{
  this->argv = argv;
  this->argc = argc;

  sizeMax_DefaultValue = 0;
  sizeMax_FlagLong = 0;
}


void itsArgumentParser::searchEndFlags() 
{
  if (this->defArg("V", "version", "check version", false, "", "")){
    throw VERSION_KEY;  
  }
  if (this->defArg("h", "help", "show help", false, "", "") ||
      this->defArg("u", "usage", "show usage informations", false, "", "")){
    throw USAGE_KEY;
  }
}


// parse argv searching the given flag, then update the vector with a new argument
bool itsArgumentParser::defArg(string flagShort, string flagLong, string usage, 
                      bool hasValue, string type, string valueDefault)
{
  unsigned i = 1;
  bool found = false;
  string value = valueDefault;
  string defaultVal = valueDefault;
  
  // add the flag marks
  flagShort = FLAG_SHORT_MARK + flagShort;
  flagLong = FLAG_LONG_MARK + flagLong;

  while ( (i < this->argv.size()) && (!found)) {

    if ( (flagShort == this->argv.at(i)) || (flagLong == this->argv.at(i))){

      if (hasValue){
	
	// if not end of line AND next word isnt a flag
	if ( (++i) < this->argv.size() && ((this->argv.at(i)[0] != '-' ) || ( this->argv.at(i)[1] < 60  )))	
	  {
	  value = this->argv.at(i);	
	  found = true;
	  }
	else { 
	  stringstream s;
	  s << "Value missing for " << flagShort << endl;
	  throw Exception_Argument_Value( s.str(), EXCEPTION_INFOS );
	}      
      }
      else { // no value associated => arg has boolean value
        value = "true";
      }
      found = true;
    }
    i++;
  }
 
  itsArgument newArgument(flagShort, flagLong, usage, hasValue, type, value, defaultVal);
  arguments.push_back(newArgument);

  // check for the longest strings
  if ( flagLong.size() > this->sizeMax_FlagLong ) {
      sizeMax_FlagLong = flagLong.size();
  }
  if ( defaultVal.size() > this->sizeMax_DefaultValue ) {
      sizeMax_DefaultValue = defaultVal.size() + 2; // +2 stands for the parenthesis
  }

  return found;
}


string itsArgumentParser::getStringValue(string key)
{
  key = FLAG_LONG_MARK + key;

  vector<itsArgument>::iterator iter;
  iter = arguments.begin();
  while (iter != arguments.end()){
    if (iter->getKey() == key)
      return iter->getValue();
    iter++;
  }
  stringstream s;
  s << "getStringValue error with " << key << endl;
  throw Exception_Argument_Value(s.str(), EXCEPTION_INFOS );
  return "";
}

double itsArgumentParser::getDoubleValue(string key)
{
  key = FLAG_LONG_MARK + key;
  
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
  throw Exception_Argument_Value(s.str(), EXCEPTION_INFOS );
  return -1.;
}

int itsArgumentParser::getIntValue(string key)
{
  key = FLAG_LONG_MARK + key;

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
  throw Exception_Argument_Value(s.str(), EXCEPTION_INFOS );
  return i;
}

bool itsArgumentParser::getBoolValue(string key)
{
  key = FLAG_LONG_MARK + key;

  vector<itsArgument>::iterator iter;
  iter = arguments.begin();
  while ( iter != arguments.end() ) {
    if (iter->getKey() == key)
      return (iter->getValue() != "");
    iter++;
  }
  stringstream s;
  s << "getBoolValue error with " << key << endl;
  throw Exception_Argument_Value(s.str(), EXCEPTION_INFOS );
  return false;
}


void itsArgumentParser::usage()
{

  cout << "Usage: " << this->argv.at(0) << " [options]" <<  endl; 
  cout << "Options:" << endl;

  ostringstream format;
  format << " %s,\t%" << sizeMax_FlagLong << "s %-" << sizeMax_DefaultValue << "s %s";
  printf( format.str().c_str(), "Short","Long", "Default","Description\n" );

  vector<itsArgument>::iterator iter;
  iter = this->arguments.begin();

  while (iter != this->arguments.end()){
      string aDefaultValue = "";
      if (iter->getHasValue()){
          aDefaultValue = "(" + iter->getDefault() + ")";
      }
      
      printf(format.str().c_str(), 
          iter->getShortKey().c_str(), 
          iter->getLongKey().c_str(),
          aDefaultValue.c_str(),
          iter->getUsage().c_str() 
      );
    
      printf("\n");
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
        throw Exception_Argument_Definition(s.str(), EXCEPTION_INFOS );
        return false;
      }
      else { // is a correct flag (defArg-ed)	
	if (!this->hasValue(*iter)){
	  if (( (iter+1) != this->argv.end()) && ( (*(iter+1))[0] != '-')){
	     s << "Value not needed for " << *iter << endl;
      throw Exception_Argument_Value(s.str(), EXCEPTION_INFOS );
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
      throw Exception_Argument_Definition(s.str(), EXCEPTION_INFOS );
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
    if ((s == iter->getShortKey()) || (s == iter->getLongKey()))
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
    if ((flag == iter->getShortKey()) || (flag == iter->getLongKey())){
      return iter->getHasValue();
    }
    iter++;
  }
  stringstream s;
  s << "Unexpected error in hasValue()" << endl;
  throw Exception_Argument_Value(s.str(), EXCEPTION_INFOS );
  return false;
}

bool itsArgumentParser::isAsked(string flag)
{
  flag = FLAG_LONG_MARK + flag;

    // find the defined argument
    for ( unsigned int i=0; i < this->arguments.size(); i++ ) {
        if ( arguments[i].getShortKey() == flag || arguments[i].getLongKey() == flag ) {
            // search it in the command line
            for( unsigned int j=0; j < this->argv.size(); j++ ) {
                if ( this->argv[j] == arguments[i].getShortKey() || this->argv[j] == arguments[i].getLongKey() ) {
                    return true;
                }
            }
            return false;
        }
    }
    throw Exception_Argument_Definition("Argument not defined (" + flag + ")", EXCEPTION_INFOS );
}

}//ometah
