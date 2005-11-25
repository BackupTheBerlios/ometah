/***************************************************************************
 * $Id: example_problem.hpp,v 1.1 2005/11/25 18:24:14 nojhan Exp $
 *  Copyright : Free Software Foundation
 * Author: Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

 
#ifndef EXAMPLEPROBLEM
#define EXAMPLEPROBLEM

#include <ometah/common/itsPoint.hpp>
#include <ometah/itsProblem.hpp>
 
class ITSEXAMPLEPROBLEM : public itsProblem
{
public:
    ITSEXAMPLEPROBLEM();
    itsPoint objectiveFunction(itsPoint point);
};

class ITSEXAMPLEPROBLEMFactory : public itsProblemFactory
{
public:
    itsProblem * create();
};

#endif
