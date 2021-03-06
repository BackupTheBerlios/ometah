/***************************************************************************
 * $Id: itsCEC05_SSRPO_Base.cpp,v 1.8 2006/05/16 13:51:38 nojhan Exp $
 * Copyright : Free Software Foundation
 * Author : Johann Dréo <nojhan@gmail.com>
 * Author : Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
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
 


#include "itsCEC05_SSRPO_Base.hpp"

using namespace std;

namespace ometah {

/*******************************************************************************
 * Ackley
 ******************************************************************************/

itsAckley::itsAckley() : itsProblem()
{
    // informations 
    setName("Ackley");
    setKey("Ackley");
    setDescription("Unknown");
    setCitation("Unknown");    setFormula("$-20e^{\\left(-0.2\\sqrt{\\frac{1}{D}{\\displaystyle \\sum_{i=1}^{D}x_{i}^{2}}}\\right)}-e^{\\left(\\frac{1}{D}{\\displaystyle \\sum_{i=1}^{D}\\cos\\left(2\\pi x_{i}\\right)}\\right)}+20+e$");
    
    setDimension(1); // one dimension is the default, but one may change it after instanciation
    
    setBoundsMinimaAll(-32);
    setBoundsMaximaAll(32);

    setAccuracy(0.01);
    
    vector<itsPoint> optim;
    itsPoint pt;

    vector<double> sol(getDimension(),0);
    pt.setSolution(sol); // a vector of 0

    vector<double> val(1,0);
    pt.setValues(val); // a value of 0

    optim.push_back(pt);
    setOptima(optim);
}

itsPoint itsAckley::objectiveFunction(itsPoint point)
{
    double sum1, sum2, res;
    sum1 = 0.0;
    sum2 = 0.0;
    unsigned int dim = getDimension();
    vector<double> x=point.getSolution();

    for ( unsigned int i=0; i < dim; i++ ) {
        sum1 += x[i] * x[i];
        sum2 += cos( 2.0 * PI * x[i] );
    }
    sum1 = -0.2 * sqrt( sum1 / dim );
    sum2 = sum2 / dim;
    res = 20.0 + E - 20.0 * exp( sum1 ) - exp( sum2 );

    vector<double> val(1,res);
    point.setValues(val);
    
    return point;
}

itsProblem * itsAckleyFactory::create()
{
    return new itsAckley();
}


/*******************************************************************************
 * Rastrigin
 ******************************************************************************/


itsRastrigin::itsRastrigin() : itsProblem()
{
    // informations 
    setName("Rastrigin");
    setKey("Rastrigin");
    setDescription("Unknown");
    setCitation("Unknown");
    setFormula("${\\displaystyle \\sum_{i=1}^{D}\\left(x_{i}^{2}-10\\cos\\left(2\\pi x_{i}\\right)+10\\right)}$");
    
    setDimension(1); // one dimension is the default, but one may change it after instanciation
    
    setBoundsMinimaAll(-5);
    setBoundsMaximaAll(5);

    setAccuracy(0.01);    

    vector<itsPoint> optim;
    itsPoint pt;

    vector<double> sol(getDimension(),0);
    pt.setSolution(sol); // a vector of 0

    vector<double> val(1,0);
    pt.setValues(val); // a value of 0

    optim.push_back(pt);
    setOptima(optim);
}

itsPoint itsRastrigin::objectiveFunction(itsPoint point)
{
    double res = 0.0;
    unsigned int dim = getDimension();
    vector<double> x=point.getSolution();

    for (unsigned int i=0; i<dim; i++) {
        res += (x[i]*x[i] - 10.0*cos(2.0*PI*x[i]) + 10.0);
    }

    vector<double> val(1,res);
    point.setValues(val);
    
    return point;
}

itsProblem * itsRastriginFactory::create()
{
    return new itsRastrigin();
}



/*******************************************************************************
 * Weierstrass
 ******************************************************************************/


itsWeierstrass::itsWeierstrass() : itsProblem()
{
    // informations 
    setName("Weierstrass");
    setKey("Weierstrass");
    setDescription("Unknown");
    setCitation("Unknown");
    setFormula("${\\displaystyle \\sum_{i=1}^{D}\\left({\\displaystyle \\sum_{k=0}^{k_{max}}\\left(a^{k}\\cos\\left(2\\pi b^{k}\\left(x_{i}+0.5\\right)\\right)\\right)}\\right)-D\\sum_{k=0}^{k_{max}}\\left(a^{k}\\cos\\left(2\\pi b^{k}\\cdot0.5\\right)\\right)},a=0.5, b=3, k_{max}=20$");
    
    setDimension(1); // one dimension is the default, but one may change it after instanciation
    
    setBoundsMinimaAll(-0.5);
    setBoundsMaximaAll(0.5);

    setAccuracy(0.01);

    vector<itsPoint> optim;
    itsPoint pt;

    vector<double> sol(getDimension(),0);
    pt.setSolution(sol); // a vector of 0

    vector<double> val(1,0);
    pt.setValues(val); // a value of 0

    optim.push_back(pt);
    setOptima(optim);
}

itsPoint itsWeierstrass::objectiveFunction(itsPoint point)
{
    unsigned int dim = getDimension();
    vector<double> x=point.getSolution();

    double res, sum, a, b;
    unsigned int k_max;
    a = 0.5;
    b = 3.0;
    k_max = 20;
    res = 0.0;

    for (unsigned int i=0; i<dim; i++) {
        sum = 0.0;
        for (unsigned int j=0; j<=k_max; j++) {
            sum += pow( a, (double)j ) * cos( 2.0 * PI * pow( b, (double)j ) * ( x[i] + 0.5 ) );
        }
        res += sum;
    }

    vector<double> val(1,res);
    point.setValues(val);
    
    return point;
}

itsProblem * itsWeierstrassFactory::create()
{
    return new itsWeierstrass();
}



/*******************************************************************************
 * Griewank
 ******************************************************************************/


itsGriewank::itsGriewank() : itsProblem()
{
    // informations 
    setName("Griewank");
    setKey("Griewank");
    setDescription("Unknown");
    setCitation("Unknown");
    setFormula("$\\sum_{i=1}^{D}\\left(\\frac{x_{i}^{2}}{4000}\\right)-{\\displaystyle \\prod_{i=1}^{D}\\left(\\cos\\left(\\frac{x_{i}}{\\sqrt{i}}\\right)+1\\right)}$");
    
    setDimension(1); // one dimension is the default, but one may change it after instanciation
    
    setBoundsMinimaAll(0);
    setBoundsMaximaAll(600);

    setAccuracy(0.01);

    vector<itsPoint> optim;
    itsPoint pt;

    vector<double> sol(getDimension(),0);
    pt.setSolution(sol); // a vector of 0

    vector<double> val(1,0);
    pt.setValues(val); // a value of 0

    optim.push_back(pt);
    setOptima(optim);
}

itsPoint itsGriewank::objectiveFunction(itsPoint point)
{
    unsigned int dim = getDimension();
    vector<double> x=point.getSolution();

    double s, p;
    double res;
    s = 0.0;
    p = 1.0;
    for (unsigned int i=0; i<dim; i++) {
        s += x[i] * x[i];
        p *= cos( x[i] / sqrt( 1.0 + i ) );
    }
    res = 1.0 + s / 4000.0 - p;

    vector<double> val(1,res);
    point.setValues(val);
    
    return point;
}

itsProblem * itsGriewankFactory::create()
{
    return new itsGriewank();
}


/*******************************************************************************
 * Sphere
 ******************************************************************************/

#include <iostream>

itsSphere::itsSphere() : itsProblem()
{
    // informations 
    setName("Sphere");
    setKey("Sphere");
    setDescription("Unknown");
    setCitation("Unknown");
    setFormula("${\\displaystyle \\sum_{i=1}^{D}\\left(x_{i}^{2}\\right)}$");
    
    setDimension(1); // one dimension is the default, but one may change it after instanciation
    
    setBoundsMinimaAll(-100);
    setBoundsMaximaAll(100);

    setAccuracy(0.000001);

    // FIXME set the optimum seems to generate a bug when itsPoint's destructors are called
    
    vector<itsPoint> optim;
    itsPoint pt;

    vector<double> sol(getDimension(),0);
    pt.setSolution(sol); // a vector of 0

    vector<double> val(1,0);
    pt.setValues(val); // a value of 0

    optim.push_back(pt);
    setOptima(optim);
    
    /*
    vector<itsPoint> optim;
clog << "&optim:" << &optim << endl;

    vector<double> sol( getDimension(), 0 );
    vector<double> val( 1, 0 );
    itsPoint pt( sol, val );
clog << "&pt:" << &pt << endl;
    optim.push_back(pt);
clog << "&optim[0]:" << &optim[0] << endl;
    setOptima( optim );
    //delete optim;
    */
}


itsPoint itsSphere::objectiveFunction(itsPoint point)
{
    unsigned int dim = getDimension();
    vector<double> x=point.getSolution();

    double res = 0.0;
    for (unsigned int i=0; i<dim; i++) {
        res += x[i]*x[i];
    }

    vector<double> val(1,res);
    point.setValues(val);
    
    return point;
}

itsProblem * itsSphereFactory::create()
{
    return new itsSphere();
}


/*******************************************************************************
 * Schwefel
 ******************************************************************************/


itsSchwefel::itsSchwefel() : itsProblem()
{
    // informations 
    setName("Schwefel");
    setKey("Schwefel");
    setDescription("Unknown");
    setCitation("Unknown");
    setFormula("${\\displaystyle \\sum_{i=1}^{D}\\left(\\sum_{j=1}^{i}\\left(x_{j}^{2}\\right)\\right)}$");
    
    setDimension(1); // one dimension is the default, but one may change it after instanciation
    
    setBoundsMinimaAll(-100);
    setBoundsMaximaAll(100);

    setAccuracy(0.000001);
    
    vector<itsPoint> optim;
    itsPoint pt;

    vector<double> sol(getDimension(),0);
    pt.setSolution(sol); // a vector of 0

    vector<double> val(1,0);
    pt.setValues(val); // a value of 0

    optim.push_back(pt);
    setOptima(optim);
}

itsPoint itsSchwefel::objectiveFunction(itsPoint point)
{
    unsigned int dim = getDimension();
    vector<double> x=point.getSolution();

    double sum1, sum2;
    sum1 = 0.0;

    for (unsigned int i=0; i<dim; i++) {
        sum2 = 0.0;
    
        for (unsigned int j=0; j<=i; j++) {
            sum2 += x[j];
        }
        sum1 += sum2*sum2;
    }

    vector<double> val(1,sum1);
    point.setValues(val);
    
    return point;
}

itsProblem * itsSchwefelFactory::create()
{
    return new itsSchwefel();
}



/*******************************************************************************
 * Rosenbrock
 ******************************************************************************/

itsRosenbrock::itsRosenbrock() : 
    itsProblem()
{
    // informations 
    setName("Rosenbrock");
    setKey("Rosenbrock");
    setDescription("Rosenbrock is a classical test problem");
    setCitation("Unknown");
    setFormula("$R_{n}(\\overrightarrow{x})=\\sum_{i=1}^{n-1}\\left(100\\cdot\\left(x_{i}^{2}-x_{i+1}\\right)^{2}+\\left(x_{i}-1\\right)^{2}\\right)$");

    setDimension(2);

    setBoundsMinimaAll(-100);
    setBoundsMaximaAll(100);

    setAccuracy(0.01);

    vector<itsPoint> optim;
    itsPoint pt;
    vector<double> sol(getDimension(),1);
    pt.setSolution(sol); // a vector of 1
    vector<double> val(1,0);
    pt.setValues(val); // a value of 0
    optim.push_back(pt);
    setOptima(optim);
}

itsPoint itsRosenbrock::objectiveFunction(itsPoint point)
{
    double rosen=0.0;
    int dim=getDimension();

    if(dim<2) {
        throw Exception_Size_Index_Dimension("Dimension of the Rosenbrock problem should be strictly greater than 1", EXCEPTION_INFOS);
    }

    for (int i=0;i<dim-1;i++) {
        double X=point.getSolution()[i];
        double Y=point.getSolution()[i+1];
        rosen+=(100*(X*X-Y)*(X*X-Y)+(X-1)*(X-1));
    }
    
    vector<double> val(1,rosen);
    point.setValues(val);
    
    return point;
}

itsProblem * itsRosenbrockFactory::create()
{
    return new itsRosenbrock();
}

}//ometah
