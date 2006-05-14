/***************************************************************************
 * $Id: example_problem.hpp,v 1.2 2006/05/14 07:33:28 nojhan Exp $
 *  Copyright : Free Software Foundation
 * Author: Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

 
#ifndef EXAMPLEPROBLEM
#define EXAMPLEPROBLEM

#include <ometah/common/itsPoint.hpp>
#include <ometah/itsProblem.hpp>
 
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
