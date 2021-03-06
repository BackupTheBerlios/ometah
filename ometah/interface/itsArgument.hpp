/***************************************************************************
 *  $Id: itsArgument.hpp,v 1.14 2006/05/15 20:58:00 nojhan Exp $
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

#ifndef ITSARGUMENT
#define ITSARGUMENT

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>

#include "../common/logic.hpp"
#include "../common/Exception_oMetah.hpp"

#define VERSION_KEY "version_key"
#define USAGE_KEY "usage_key" // string value doesn't matter

using namespace std;

namespace ometah {

/*!
  An argument is a command line specification, like an option, 
  including the flag and its value, it needed.
*/
class itsArgument
{
protected:
  //! short cmd line flag (ie -v for version)
  string flagShort;
  
  //! Short cmd line flag (ie --version for version)
  string flagLong;

  //! True if a value must be associated to the argument
  bool hasValue;
  
  //! Usage instructions
  string usage;

  //! Type of the associated value, boolean if has no value
  string type;

  //! Value of the argument, empty if argument has no value
  string value;

  //! Default value
  string defaultValue;


public:

  //! Destructor
  ~itsArgument();

  //! Constructor
  itsArgument(string flagShort, string flagLong, string usage, 
               bool hasValue=false, string type="", string value="", string defaultValue="");  

  //! Returns default key (short)
  string getKey();

  //! Returns short key ( ie "-v" )
  string getShortKey();

  //! Returns long key ( ie "--version" )
  string getLongKey();

  //! Returns the value
  string getValue();
  
  //! Returns usage instructions
  string getUsage();

  bool getHasValue();

  string getDefault();

};


/*!
  An argument parser works on the command line arguments and analyze them, 
  look for syntax errors, get values from flags, etc..
 */
class itsArgumentParser
{
protected:

  //! Main's argc, number of command line words
  int argc;

  //! Main's argv, vector containing the command line words
  vector<string> argv;

  //! A vector of itsArguments objects, corresponding to the arguments read on argv
  vector<itsArgument> arguments;

  //! From a supposed flag, short or long, returns true if is a known argument flag
  bool isAnArgument(string flag);

  //! Longest long-flag 
  unsigned int sizeMax_FlagLong;
  
  //! Longest default value
  unsigned int sizeMax_DefaultValue;

public:
  
  //! Destructor
  ~itsArgumentParser();

  //! Constructor
  itsArgumentParser(int argc, vector<string> argv);

  //! Looks for end flags, that is flags like -V for version checking, which terminate the execution
  void searchEndFlags();

  //! Define an option of the program, affecting default value if not specified in command line
  /*!
   *  Returns true if the given argument was found in command line.
   */
  bool defArg(string flagShort, string flagLong, string usage, 
               bool hasValue=false, string type="", string valueDefault="");
  
  //! Accessor for the string value of the given flag (short)
  string getStringValue(string flag);

  //! Accessor for the double value of the given flag (short)
  double getDoubleValue(string flag);

  //! Accessor for the int value of the given flag (short)
  int getIntValue(string flag);

  //! Accessor for the boolean value of the given flag (short)
  bool getBoolValue(string flag);

  //! Prints usage instructions, from defArgs definitions
  void usage();

  //! Checks for syntax correctness, throws exception when an error occurs
  bool syntaxCorrect();

  //! Returns true if the matching argument has a value
  bool hasValue(string flag);
  
  //! Return true if the matching argument has been asked
  bool isAsked(string flag);
};

//! Base exception for problems of arguments
EXCEPTION_DECLARATION(Exception_Argument, Exception_oMetah);

    //! Use this exception if we have no value for an argument which needs one
    EXCEPTION_DECLARATION(Exception_Argument_Value, Exception_Argument);

    //! Use this exception if there is no argument definition
    EXCEPTION_DECLARATION(Exception_Argument_Definition, Exception_Argument);

}//ometah

#endif
