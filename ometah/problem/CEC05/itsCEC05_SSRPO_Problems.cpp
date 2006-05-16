/***************************************************************************
 * $Id: itsCEC05_SSRPO_Problems.cpp,v 1.4 2006/05/16 13:51:38 nojhan Exp $
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
 
#include"../../common/matrix.hpp"
#include"../../common/random.hpp"

#include "itsCEC05_SSRPO_Problems.hpp"

namespace ometah {

itsCEC05_SSRPO_Problem::itsCEC05_SSRPO_Problem() : ssrpoID(0), f_bias(0.0)
{
}



/*******************************************************************************
 * F1_ShiftedSphere
 ******************************************************************************/

itsF1_ShiftedSphere::itsF1_ShiftedSphere() : itsCEC05_SSRPO_Problem()
{
    // informations 
    setName("CEC'05-SSRPO F1 Shifted Sphere Function");
    setKey("CEC05_F1");
    setDescription("Unimodal, shifted, separable, scalable.");
    setCitation("TODO");    
    setFormula("TODO");
    
    // default value for all CEC'05 problems
    setAccuracy(1e-8);
    

    /***** Parameters *****/

    // may be 10, 30, 50
    setDimension(10); 
    
    // bounds of the search space
    setBoundsMinimaAll(-100);
    setBoundsMaximaAll(100);

    // the bias
    f_bias = -450;

    // the shift vector (which is the optimum)
    double temp_o_t[] = 
    {
      -3.9311900e+001, 5.8899900e+001,
      -4.6322400e+001, -7.4651500e+001, -1.6799700e+001,
      -8.0544100e+001 -
      1.0593500e+001, 2.4969400e+001, 8.9838400e+001,
      9.1119000e+000, -
      1.0744300e+001, -2.7855800e+001, -
      1.2580600e+001, 7.5930000e+000, 7.4812700e+001,
      6.8495900e+001, -
      5.3429300e+001, 7.8854400e+001, -
      6.8595700e+001, 6.3743200e+001, 3.1347000e+001, -
      3.7501600e+001, 3.3892900e+001, -8.8804500e+001, -
      7.8771900e+001, -
      6.6494400e+001, 4.4197200e+001, 1.8383600e+001,
      2.6521200e+001,
      8.4472300e+001, 3.9176900e+001, -6.1486300e+001, -
      2.5603800e+001, -8.1182900e+001, 5.8695800e+001, -
      3.0838600e+001, -7.2672500e+001, 8.9925700e+001, -
      1.5193400e+001, -
      4.3337000e+000, 5.3430000e+000, 1.0560300e+001, -
      7.7726800e+001, 5.2085900e+001, 4.0394400e+001,
      8.8332800e+001, -
      5.5830600e+001, 1.3181000e+000, 3.6025000e+001, -
      6.9927100e+001, -8.6279000e+000, -
      5.6894400e+001, 8.5129600e+001, 1.7673600e+001,
      6.1529000e+000, -
      1.7695700e+001, -
      5.8953700e+001, 3.0356400e+001, 1.5920700e+001, -
      1.8008200e+001, 8.0641100e+001, -
      4.2391200e+001, 7.6277600e+001,
      -5.0165200e+001, -7.3573600e+001, 2.8336900e+001, -
      5.7990500e+001, -
      2.2732700e+001, 5.2026900e+001, 3.9259900e+001,
      1.0867900e+001,
      7.7820700e+001, 6.6039500e+001, -
      5.0066700e+001, 5.5706300e+001, 7.3714100e+001,
      3.8529600e+001, -
      5.6786500e+001, -
      8.9647700e+001, 3.7957600e+001, 2.9472000e+001, -
      3.5464100e+001, -
      3.1786800e+001, 7.7323500e+001, 5.4790600e+001, -
      4.8279400e+001, 7.4271400e+001, 7.2610300e+001,
      6.2964000e+001, -
      1.4144600e+001, 2.0492300e+001, 4.6589700e+001, -
      8.3602100e+001, -4.6480900e+001, 8.3737300e+001, -
      7.9661100e+001, 2.4347900e+001, -
      1.7230300e+001, 7.2340400e+001, -3.6402200e+001
    };
    
    // traduction from a table to a vector of _doubles_
    // the shift vector has 100 values
    vector<double> temp_o_v(temp_o_t, temp_o_t+100);
    o = temp_o_v;

    // save the optimum (a vector of _points_)
    vector<itsPoint> optim;
    itsPoint pt;
    pt.setSolution( o ); // optimum solution = translation
    pt.setValue( f_bias ); // optimum value = bias

    // only one optimum
    optim.push_back(pt);
    setOptima(optim);
}

itsPoint itsF1_ShiftedSphere::objectiveFunction(itsPoint point)
{
    vector<double> x = point.getSolution();
    double f = f_bias;

    for (unsigned int i = 0; i < getDimension(); i++) {
      f = f + pow(x[i] - o[i], 2.0);
    }

    point.setValue(f);
    return point;
}


itsProblem* itsF1_ShiftedSphere_Factory::create()
{
    return new itsF1_ShiftedSphere();
}






/*
void itsCEC05_SSRPO_Problem::doShiftOptimum()
{
    vector<double> sol = currentPoint.getSolution();

    for(unsigned int i=0; i<currentPoint.getSolution().size(); i++) {
        sol[i] += optimumShift[i];
    }
    currentPoint.setSolution( sol );
}


void itsCEC05_SSRPO_Problem::doRotate()
{
    vector<double> sol = currentPoint.getSolution();

    vector<vector< double> > solMat;
    solMat.push_back(sol);

    vector<vector<double> > res;

    res = multiply( solMat, this->rotationMatrix );

    currentPoint.setSolution(res[0]);
}

void itsCEC05_SSRPO_Problem::addBias()
{
    vector<double> val = currentPoint.getValues();

    for( unsigned int i=0; i<val.size(); i++ ) {
        val[i] += this->f_bias;
    }
    
    currentPoint.setValues( val );
}


void itsCEC05_SSRPO_Problem::noise()
{
    vector<double> val = currentPoint.getValues();

    vector<double> means(val.size(), 0.0);
    vector<double> stdev(val.size(), 1.0);

    vector<double> noise = randomNormal(means, stdev) ;

    currentPoint.setValues( addition( val, noise) );
}
*/

}//ometah
