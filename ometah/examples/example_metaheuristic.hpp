/***************************************************************************
 *  $Id: example_metaheuristic.hpp,v 1.4 2006/06/03 18:58:41 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
 *  for difficult optimization.
 */
 
#ifndef EXAMPLEMETAHEURISTIC
#define EXAMPLEMETAHEURISTIC

#include <ometah/metaheuristics.h>

using namespace std;
using namespace ometah;

class ITSEXAMPLEMETAHEURISTIC : public itsMetaheuristic
{
  
protected:
  //! the intensification
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
