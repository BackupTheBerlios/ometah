/***************************************************************************
 *  $Id: itsCommunicationClient_python.hpp,v 1.3 2006/05/14 07:33:28 nojhan Exp $
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

 #ifndef ITSCOMMUNICATIONCLIENTPYTHON
 #define ITSCOMMUNICATIONCLIENTPYTHON

#include <vector>
#include <stdexcept>

#include <python2.4/Python.h>

#include "../../common/itsPoint.hpp"
#include "../../common/Exception_oMetah.hpp"
#include "../itsCommunicationClient.hpp"

using namespace std;

namespace ometah {

class itsCommunicationClient_python : public itsCommunicationClient
{
private:
    //! The path of the current binary
    char * base_path;

    //! The name of the python module to load
    string py_module_name;
    //! The python module, loaded
    PyObject * py_module;
    
    //! The name of the function to use
    string py_objective_function_name;
    //! The python function, loaded
    PyObject * py_objective_function;
    //! The python tuple to use as arguments for the objective function
    PyObject * py_objective_function_args;

    //!
    string py_dimension_function_name;
    //!
    PyObject * py_dimension_function;

    //!
    string py_bounds_min_function_name;
    //!
    PyObject * py_bounds_min_function;

    //!
    string py_bounds_max_function_name;
    //!
    PyObject * py_bounds_max_function;
    
    

public:
    itsCommunicationClient_python( char* argv_0);
    ~itsCommunicationClient_python();

    itsPoint call(itsPoint point);
    vector<double> boundsMinima();
    vector<double> boundsMaxima();
    vector<vector<double> > bounds();
    int getDimension();
};

class itsCommunicationClientFactory_python : public itsCommunicationClientFactory
{
public:
    itsCommunicationClient* create();
};


//! Declaration of the main exception
EXCEPTION_DECLARATION( Exception_Python, Exception_oMetah );

//! Exceptions when importing a module, searching a callable object, etc.
EXCEPTION_DECLARATION( Exception_Python_LookUp, Exception_Python );

//! Exceptions when checking a type, the existence of a variable, etc.
EXCEPTION_DECLARATION( Exception_Python_Type, Exception_Python );
    
}//ometah    

#endif
