/***************************************************************************
 * $Id: example_problem.cpp,v 1.3 2006/05/14 07:33:28 nojhan Exp $
 * Copyright : Free Software Foundation
 * Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/


#include "example_problem.hpp"

using namespace std;
 
using namespace ometah;

ITSEXAMPLEPROBLEM::ITSEXAMPLEPROBLEM()
{
    // Describe your problem here, so that one can easily comprehend it
    setName("The complete name of your problem"); // example : Optimization of human brain
    setKey("A unique key identifiying it"); // example : HB
    setDescription("A description of your problem"); // example : Making the human brain producing more ideas
    setCitation("The paper where it is described"); // a BiBTeX entry
    setFormula("A LaTeX formula, if suitable"); // example : $f(x)=x^2$
    
    setDimension(1); // the number of parameter
    
    // use these methods if all your parameters have the same bounds :
    setBoundsMinimaAll(-1);
    setBoundsMaximaAll(1);

    // else you can precise each bounds one by one
    vector<double> bounds;
    bound.push_back(-1);
    setBoundsMinima( bounds ); // example for the minima
    

    // the following code is facultative

    // if suitable : the minimum accuracy to reach
    setAccuracy(0.01); 
    
    // here you can, if suitable, give the known optimum
    vector<itsPoint> optim;
    itsPoint pt;

    vector<double> sol(getDimension(),0);
    pt.setSolution(sol); // a vector of 0

    vector<double> val(1,0);
    pt.setValues(val); // a value of 0

    optim.push_back(pt);
    setOptima(optim);
}


itsPoint ITSEXAMPLEPROBLEM::objectiveFunction(itsPoint point)
{
    // put here the code of your problem
    // for example, x² :
    point.setValue( point.getSolution()[0] * point.getSolution-)[0] );
    return point;
}


itsProblem * ITSEXAMPLEPROBLEMFactory::create()
{
    return new ITSEXAMPLEPROBLEM();
}
