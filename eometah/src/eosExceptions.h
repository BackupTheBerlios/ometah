//
// C++ Interface: eosExceptions
//
// Description: 
//
//
// Author: Johann Dréo <nojhan@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _eosExceptions_h
#define _eosExceptions_h

#include <string>

using namespace std;

class Exception
{
protected:
	string name;
	string description;
	string file;
	string function;
	int line;

public:
	Exception  ( 
		const string & _description, 
		const string & _file, 
		const string & _function, 
		const int _line
	) : 
		description(_description), 
		file(_file), 
		function(_function), 
		line(_line) 
	{};

		//! Return a string describing the exception
		/*! The string has the form : 
					"DESCRIPTION. (<NAME> error in function <FUNCTION>, in file <FILE>, at line LINE)"
		*/
		const string what();
};

// Macro for a short exception declaration, with inheritance management
/* Use it this way : 
EXCEPTION_DECLARATION(Exception,Exception_Size);
   EXCEPTION_DECLARATION(Exception_Size,Exception_Size_Index);
*/
#define EXCEPTION_DECLARATION(Super,Current) class Current : public Super {public: Current ( const string & desc, const string & func="?", const string & f="?", const int l=-1 ) : Super (desc,func,f,l) {name = #Current;} }

// Macro for a short exception use
/* Use it this way :
throw Exception_Size_Index("Empty vector", EXCEPTION_INFOS ); 
*/
#define EXCEPTION_INFOS __FILE__,__FUNCTION__,__LINE__


EXCEPTION_DECLARATION(Exception,Exception_Logic);
EXCEPTION_DECLARATION(Exception_Logic,Exception_Length);

EXCEPTION_DECLARATION(Exception,Exception_Runtime);
	EXCEPTION_DECLARATION(Exception_Runtime,Exception_Range);
		EXCEPTION_DECLARATION(Exception_Range,Exception_Empty);

#endif
