/***************************************************************************
 *  $Id: itsCommunicationClient_python.cpp,v 1.4 2006/05/15 11:44:52 nojhan Exp $
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
 
#include "itsCommunicationClient_python.hpp"

namespace ometah {

itsPoint itsCommunicationClient_python::call(itsPoint point)
{
    PyObject *result;

    // put the point solution in the args
    for(unsigned int i=0; i < point.getSolution().size(); i++) {
        PyObject *sol = PyFloat_FromDouble( point.getSolution()[i] );
        PyTuple_SetItem( py_objective_function_args, i, sol );
    }

    // call the function on the point
    result = PyObject_CallObject (py_objective_function, py_objective_function_args);

    if (result) {      
        if (PyFloat_Check(result)) {
            // get the result from python
            double value = PyFloat_AsDouble(result);
        
            // put it in the point
            vector<double> val;
            val.push_back( value );
            point.setValues( val );
        
        } else {	// if checked result
            PyErr_Print();
            throw Exception_Python_Type( "Result is not a float", EXCEPTION_INFOS );
        }
    
        Py_XDECREF(result);
    
    } else { // if no result
        PyErr_Print();
        throw Exception_Python_Type( "No results returned from python problem", EXCEPTION_INFOS );
    }
                
    return point;
}


vector<double> itsCommunicationClient_python::boundsMinima()
{
    // FIXME TEMP
    vector<double> p(this->getDimension(),0);
    return p;
}

vector<double> itsCommunicationClient_python::boundsMaxima()
{
    // FIXME TEMP
    vector<double> p(this->getDimension(),1);
    return p;
}

vector<vector<double> > itsCommunicationClient_python::bounds()
{
    // FIXME TEMP
    vector<vector<double> > b;  
    vector<double> bi = this->boundsMinima();
    vector<double> ba = this->boundsMaxima();

    for(int i=0; i<this->getDimension(); i++) {
        vector<double> p;
        p.push_back(bi[i]);
        p.push_back(ba[i]);
        b.push_back(p);
    }

    return b;
}

int itsCommunicationClient_python::getDimension()
{
    long value;
    PyObject * result, * args;

    // empty tuple = no arguments
    args = PyTuple_New(0);

    result = PyObject_CallObject (py_dimension_function, args);

    Py_XDECREF(args);

    if (result) {      
        if (PyInt_Check(result)) {
            // get the result from python
            value = PyInt_AsLong(result);
        
        } else {	// if checked result
            PyErr_Print();
            throw Exception_Python_Type("Result is not an int", EXCEPTION_INFOS );
        }
    
        Py_XDECREF(result);
    
    } else { // if no result
        PyErr_Print();
        throw Exception_Python_Type("No results returned from python problem", EXCEPTION_INFOS );
    }

    return (int)value;
}



itsCommunicationClient_python::itsCommunicationClient_python ( char * argv_0, string _py_module_name ) 
    : base_path( argv_0), py_module_name(_py_module_name)
{
    setKey("Python");

    //py_module_name = "problem_for_ometah";
    py_objective_function_name = "objectiveFunction";
    py_dimension_function_name = "dimension";
    py_bounds_max_function_name = "boundsMinima";
    py_bounds_min_function_name = "boundsMaxima";


    // initialization of the path
    // necessary for the import module command below
    Py_SetProgramName( base_path );
    Py_Initialize();

    string m_name = py_module_name+".py";
    base_argv[0] = (char*)m_name.c_str();
    base_argv[1] = Py_GetProgramFullPath(	);

    PySys_SetArgv(	2, base_argv);

    //clog << "Python path: " << Py_GetPath() << endl;


    PyObject *module_name, *module_dict;

    // get the module from its name
    module_name = PyString_FromString( this->py_module_name.c_str() );
    
    if( module_name == NULL ) {
      throw Exception_Python_LookUp("No given module", EXCEPTION_INFOS);
    }

    // import the module
    py_module = PyImport_Import( module_name );

    if( py_module ) {
        module_dict = PyModule_GetDict(py_module);
    
        // search for the functions
        py_objective_function  = PyDict_GetItemString( module_dict, this->py_objective_function_name.c_str() );
        py_dimension_function  = PyDict_GetItemString( module_dict, this->py_dimension_function_name.c_str() );
        py_bounds_min_function = PyDict_GetItemString( module_dict, this->py_bounds_min_function_name.c_str() );
        py_bounds_max_function = PyDict_GetItemString( module_dict, this->py_bounds_max_function_name.c_str() );
    
        // throw errors if the functions does not exists
        if( ! py_objective_function ) { 
            throw Exception_Python_LookUp( "Cannot find function <"+py_objective_function_name+">", EXCEPTION_INFOS );}
        if( ! py_dimension_function ) { 
            throw Exception_Python_LookUp( "Cannot find function <"+py_dimension_function_name+">", EXCEPTION_INFOS );}
        if( ! py_bounds_min_function ) { 
            throw Exception_Python_LookUp( "Cannot find function <"+py_bounds_min_function_name+">", EXCEPTION_INFOS );}
        if( ! py_bounds_max_function ) { 
            throw Exception_Python_LookUp( "Cannot find function <"+py_bounds_max_function_name+">", EXCEPTION_INFOS );}
    
        // check for their avaibility and launch them    
        // py_objective_function
        if(    py_objective_function 
            && py_dimension_function 
            && py_bounds_min_function 
            && py_bounds_max_function 
          ) {
            
            if(    PyCallable_Check(py_objective_function) 
                && PyCallable_Check(py_dimension_function) 
                && PyCallable_Check(py_bounds_min_function) 
                && PyCallable_Check(py_bounds_max_function) 
              ) {
                
                // create a tuple of D items
                py_objective_function_args = PyTuple_New( this->getDimension() );
            
            } else { // if function not callable
                PyErr_Print();
                throw Exception_Python_Type("Python object is not callable", EXCEPTION_INFOS );
            }
        
        } else { // if function not available
            PyErr_Print();
            throw Exception_Python_LookUp("Python functions not available", EXCEPTION_INFOS );
        }
        
    } else { // if module not available
        PyErr_Print();
        throw Exception_Python_LookUp("Python module <"+py_module_name+"> not available ", EXCEPTION_INFOS );
    }
    
    Py_XDECREF(module_name);
    Py_XDECREF(module_dict);
}



itsCommunicationClient_python::~itsCommunicationClient_python()
{
    Py_XDECREF(py_objective_function_args); 
    Py_XDECREF(py_module);
    Py_XDECREF(py_objective_function);

    Py_Finalize();
}

/*
itsCommunicationClient* itsCommunicationClientFactory_python::create()
{
    return new itsCommunicationClient_python(".");
}
*/

}//ometah
