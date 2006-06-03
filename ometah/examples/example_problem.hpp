/***************************************************************************
 * $Id: example_problem.hpp,v 1.3 2006/06/03 18:58:41 nojhan Exp $
 *  Copyright : Free Software Foundation
 * Author: Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

 
#ifndef EXAMPLEPROBLEM
#define EXAMPLEPROBLEM

#include <ometah/commons.h>
#include <ometah/problems.h>
 
using namespace ometah;
 
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
