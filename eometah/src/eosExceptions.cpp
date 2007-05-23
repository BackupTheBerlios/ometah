//
// C++ Implementation: eosExceptions
//
// Description: 
//
//
// Author: Johann Dréo <nojhan@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <sstream>

#include "eosExceptions.h"

const string Exception::what()
{
   ostringstream msg;
   msg << description << "."
       << " (<" << name << ">"
       << " error in function <" << function
       << ">, in file <" << file << ">"
       << ", at line " << line << ")"
       ;//<< endl;

   return msg.str();
}

