/***************************************************************************
 *  $Id: itsProblem.hpp,v 1.2 2005/06/13 12:06:04 jpau Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
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

#include "../common/itsPoint.hpp"

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
  
  
public:
  //! Constructor
  itsProblem();

  //! Virtual destructor
  /*!
    Necessary because of the abstract factory
  */
  virtual ~itsProblem();

  //! Simple attribute accesses
  //!@{

  //! Return the name
  string getName();
  //! Set the name
  void setName(string name);

  //! Return the description
  string getDescription();
  //! Change the description
  void setDescription(string description);
  
  //! Return the reference
  string getCitation();
  //! Change the reference
  void setCitation(string citation);

  //! Return the dimension
  unsigned int getDimension();
  //! Change the dimension
  void setDimension(unsigned int dimension);

  //! Return the formula
  string getFormula();
  //! Change the formula
  void setFormula(string formula);

  //! Return the informations about the global optima
  vector<itsPoint> getOptima();
  //! Change the informations about the global optima
  void setOptima(vector<itsPoint> optima);
  
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
  vector<double> getBoundsMinima();
  //! Change the search space bounds minima
  void setBoundsMinima(vector<double> minima);
  
  //! Search space bounds maxima
  vector<double> getBoundsMaxima();
  //! Search space bounds maxima
  void setBoundsMaxima(vector<double> maxima);
  
  //! Change all the bounds minima to the same value
  void setBoundsMinimaAll(double min);
  
  //! Change all the bounds maxima to the same value
  void setBoundsMaximaAll(double max);
  
  //! Return the key
  string getKey();
  //! Change the key
  void setKey( string key );
    
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
};

#endif
