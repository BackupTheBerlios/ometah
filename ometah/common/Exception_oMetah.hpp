/***************************************************************************
 *  $Id: Exception_oMetah.hpp,v 1.2 2006/04/11 10:13:57 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*
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


#ifndef EXCEPTION_OMETAH
#define EXCEPTION_OMETAH

#include <string>
#include <sstream>

using namespace std;

//! This macro should be used when calling an exception
/*!   for example :
     throw( Exception_oMetah( "Cannot do something", EXCEPTION_INFOS );
*/
#define EXCEPTION_INFOS __FUNCTION__,__FILE__,__LINE__


//! This macro should be used to declare an exception
/*! The first argument is the name of the exception class
    The second argument is the name of the base class
    for example :
      EXCEPTION_DECLARATION(Exception_Socket,Exception_oMetah);
      creates an new exception class, derived from the base exception
*/
#define EXCEPTION_DECLARATION(Current,Super) class Current : public Super {public: Current ( const string & desc, const string & func="?", const string & f="?", const int l=-1 ) : Super (desc,func,f,l) {name = #Current;} }


//! This is the base class for all exceptions in oMetah
class Exception_oMetah
{
protected:
    //! Name of the current exception class
    string name;

    //! Description of the exception
    string description;

    //! Function where the exception has been raised
    string function;

    //! File where the exception has been raised
    string file;

    //! Line where the exception has been raised
    int line;

public:
    //! Constructor of the exception
    /*!
        This constructor should not be used with hand-made arguments.
        Use the EXCEPTION_INFOS macro to raise the exception, for example :
            throw( Exception_oMetah( "Cannot do something", EXCEPTION_INFOS );
    */
    Exception_oMetah  ( const string & desc, const string & func, const string & f, const int l ) : description(desc), function(func), file(f), line(l) {};

    //! The method to use for printing the complete description of the exception
    string what();
};

//! Use this exception when you have a mathematical problem
EXCEPTION_DECLARATION(Exception_Math,Exception_oMetah);

//! Use this exception when you have a size problem
EXCEPTION_DECLARATION(Exception_Size,Exception_oMetah);

    //! Use this exception when you cant access to an element in a container
    EXCEPTION_DECLARATION(Exception_Size_Index,Exception_Size);

    //! Use this exception when size of containers does not match
    EXCEPTION_DECLARATION(Exception_Size_Match,Exception_Size);

#endif
