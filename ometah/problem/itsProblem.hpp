/***************************************************************************
 *  $Id: itsProblem.hpp,v 1.10 2006/05/11 14:25:20 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; either version 2.1 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General
Public License along with this program; if not, write to the Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA
*/

#ifndef ITSPROBLEM
#define ITSPROBLEM

#include <string>
#include <iostream>

#include "../common/itsPoint.hpp"
#include "../common/Exception_oMetah.hpp"

using namespace std;

//! The base classe for problems instances
/*! 
  All classes of problem inherit from itsProblem, and declares the
  virtual functions.
 */
class itsProblem 
{
protected:
  //! The name of the problem
  /*! 
    This must be a unique name in the source tree
  */
  string name;

  //! A unique key identifying the problem
  string key;

  //! A brief description 
  string description;

  //! The paper reference (if available)
  /*!
    The reference should be formated in BiBTeX.
   */
  string citation;

  //! The formula (if available)
  /*!  
    If the problem is a test one, and have an algebric formula, put
    its LaTeX version here.
   */
  string formula;

  //! Number of variables
  unsigned int dimension;
  
  //! If the dimension is fixed or not
  bool dimensionFixed;

  //! The global optima
  /*!
    This vector store all the known global optima
  */
  vector<itsPoint> optima;
  
  //! Search space bounds minima
  /*!
    This vector is of the form :
      [ min_1, min_2, ... , min_n ]
  */
  vector<double> boundsMinima;
  
  //! Search space bounds maxima
  /*!
    This vector is of the form :
      [ max_1, max_2, ... , max_n ]
  */
  vector<double> boundsMaxima;
  
  //! Accuracy need for a success
  /*!
    It's the maximal error value with the optimum to get a success when calculating success rate
   */
  float accuracy;
  
  
protected:
  //! Change the status of the dimension
  void setDimensionFixed(unsigned int dimension);
  
  
public:
  //! Constructor
  itsProblem() : name("Unknown"), key(""), description("Unknown"), citation("Unknown"), formula("f(x)"),
                  dimension(1), dimensionFixed(false), accuracy(0.1) 
    {
        setBoundsMaximaAll(1);
        setBoundsMinimaAll(0);
    }

  //! Virtual destructor
  /*!
    Necessary because of the abstract factory
  */
  virtual ~itsProblem() {};

  //! Simple attribute accesses
  //!@{

  //! Return the name
  string getName() const {return name;}
  //! Set the name
  void setName(string name);

  //! Return the description
  string getDescription() const {return description;}
  //! Change the description
  void setDescription(string description);
  
  //! Return the reference
  string getCitation() const {return citation;}
  //! Change the reference
  void setCitation(string citation);

  //! Return the dimension
  unsigned int getDimension() const {return dimension;}
  //! Change the dimension
  void setDimension(unsigned int dimension);
  
  //! Return the status of the dimension
  /*!
      True = dimension cannot be changed
      False = dimension can be changed
  */
  bool isDimensionFixed() const {return dimensionFixed;}

  //! Return the formula
  string getFormula() const {return formula;}
  //! Change the formula
  void setFormula(string formula);

  //! Return the informations about the global optima
  vector<itsPoint> getOptima();
  //! Change the informations about the global optima
  void setOptima(const vector<itsPoint> & theOptima) 
  {clog << "&theOptima:" << &theOptima << endl;
      optima=theOptima;
  clog << "&optima:" << &optima << endl;
  }
  
  //! Return a formated output of all the informations
  /*!
    Return the Key, Name, Description, Dimension, Formula, Optimum, Bounds and Reference 
    (in this order) of the algorithm, if given.
    The format is the following :
        Key:\tValue\n
  */
  string getInformations();
  
  
  //! Return a formated output of all the informations
  /*!
    Return the Key, Name, Description, Dimension, Formula, Optimum, Bounds and Reference 
    (in this order) of the algorithm, if given. In a xml format.
  */
  string getInformations_XML();
  
  //! Return the search space bounds
  /*!
    Return a matrix of the form :
      [
       [ min_1, max_1 ],
       [ min_2, max_2 ],
       [ ... ]
       [ min_n, max_n ],
      ]
  */
  vector<vector<double> > getBounds();
  //! Change the whole search space bounds
  void setBounds(vector<vector<double> > bounds);
  
  //! Return the search space bounds minima
  vector<double> getBoundsMinima() const {return boundsMinima;}
  //! Change the search space bounds minima
  void setBoundsMinima(vector<double> minima);
  
  //! Search space bounds maxima
  vector<double> getBoundsMaxima() const {return boundsMaxima;}
  //! Search space bounds maxima
  void setBoundsMaxima(vector<double> maxima);
  
  //! Change all the bounds minima to the same value
  void setBoundsMinimaAll(double min);
  
  //! Change all the bounds maxima to the same value
  void setBoundsMaximaAll(double max);
  
  //! Return the key
  string getKey() const {return key;}
  //! Change the key
  void setKey( string key );

  //! Return the accuracy
  float getAccuracy() const {return accuracy;}
  //! Change the accuracy 
  void setAccuracy( float acc );
    
  //@}



  //! The objective function
  /*! 
    This is the main part of the problem. This function computes the
    value of the point and return it.
   */
  virtual itsPoint objectiveFunction(itsPoint point) =0;// const = 0;
};


//! Abstract factory base class for problem instances
/*! 
  This is the base class for all factory classes
 */
class itsProblemFactory
{
public:
  //! Base method for instanciation in the abstract factory
  virtual itsProblem* create() =0;// const = 0;

  virtual ~itsProblemFactory() {};
};


EXCEPTION_DECLARATION(Exception_Size_Index_Dimension, Exception_Size_Index);

#endif
