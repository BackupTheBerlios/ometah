/***************************************************************************
 * $Id: itsCEC05_SSRPO_Base.hpp,v 1.4 2006/05/13 10:05:56 nojhan Exp $
 *  Copyright : Free Software Foundation
 * Author: Johann Dréo <nojhan@gmail.com>
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

/*
 * Base functions for the CEC05-SSRPO problem set.
 *
 * Special Session on Real-Parameter Optimization at the IEEE Congress on
 * Evolutionary Computation, Edinburgh, UK, 2-5 Sept. 2005
 */

/*
 * Ackley
 * Rastrigrin
 * Weierstrass
 * Griewangk
 * Sphere
 * Schwefel
 * Rosenbrock
 */
 
#ifndef ITSCEC05SSRPOBASE
#define ITSCEC05SSRPOBASE

#include <vector>
#include <cmath>
#include "../../common/geometry.hpp"
#include "../../common/itsPoint.hpp"
#include "../itsProblem.hpp"

using namespace std;

namespace ometah {

/***** Ackley *****/
 
class itsAckley : public itsProblem
{
public:
    itsAckley();
    itsPoint objectiveFunction(itsPoint point);
};

class itsAckleyFactory : public itsProblemFactory
{
public:
    itsProblem * create();
};



/***** Rastrigin *****/

class itsRastrigin : public itsProblem
{
public:
    itsRastrigin();
    itsPoint objectiveFunction(itsPoint point);
};

class itsRastriginFactory : public itsProblemFactory
{
public:
    itsProblem * create();
};


/***** Weierstrass *****/

class itsWeierstrass : public itsProblem
{
public:
    itsWeierstrass();
    itsPoint objectiveFunction(itsPoint point);
};

class itsWeierstrassFactory : public itsProblemFactory
{
public:
    itsProblem * create();
};


/***** Griewank *****/

class itsGriewank : public itsProblem
{
public:
    itsGriewank();
    itsPoint objectiveFunction(itsPoint point);
};

class itsGriewankFactory : public itsProblemFactory
{
public:
    itsProblem * create();
};


/***** Sphere *****/

class itsSphere : public itsProblem
{
public:
    itsSphere();

    itsPoint objectiveFunction(itsPoint point);
};

class itsSphereFactory : public itsProblemFactory
{
public:
    itsProblem * create();
};


/***** Schwefel *****/

class itsSchwefel : public itsProblem
{
public:
    itsSchwefel();
    itsPoint objectiveFunction(itsPoint point);
};

class itsSchwefelFactory : public itsProblemFactory
{
public:
    itsProblem * create();
};


/***** Rosenbrock *****/

class itsRosenbrock : public itsProblem
{
public:
    itsRosenbrock();
    itsPoint objectiveFunction(itsPoint point);
};

class itsRosenbrockFactory : public itsProblemFactory
{
public:
    itsProblem * create();
};

}//ometah

#endif
