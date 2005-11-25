/***************************************************************************
 *  $Id: example_metaheuristic.hpp,v 1.1 2005/11/25 18:24:14 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
 *  for difficult optimization.
 */
 
#ifndef EXAMPLEMETAHEURISTIC
#define EXAMPLEMETAHEURISTIC

#include <ometah/metaheuristic/itsMetaheuristic.hpp>

using namespace std;

class ITSEXAMPLEMETAHEURISTIC : public itsMetaheuristic
{
  
protected:
  //! the intensification is a the a method
  void intensification();
      
  //! the diversification
  void diversification();
      
  //! the learning
  void learning();

public:

  //! Constructor
  /*!
    Here are set default values for attributes
  */
  ITSEXAMPLEMETAHEURISTIC();

  ~ITSEXAMPLEMETAHEURISTIC();

};

class ITSEXAMPLEMETAHEURISTICFactory : public itsMetaheuristicFactory
{
public:
    itsMetaheuristic* create();
};

#endif
