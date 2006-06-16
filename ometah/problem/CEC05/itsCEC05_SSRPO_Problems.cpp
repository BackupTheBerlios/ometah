/***************************************************************************
 * $Id: itsCEC05_SSRPO_Problems.cpp,v 1.5 2006/06/16 17:34:43 walid_tfaili Exp $
 *  Copyright : Free Software Foundation
 * Author: Johann Dréo <nojhan@gmail.com>
 * Author: Walid Tfaili <walidtfaili@yahoo.fr>
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
#include <fstream>
#include <sstream>

#include "itsCEC05_SSRPO_Problems.hpp"

//using std::ifstream;
using namespace std;

namespace ometah {

itsCEC05_SSRPO_Problem::itsCEC05_SSRPO_Problem() : ssrpoID(0), f_bias(0.0)
{
}


void itsCEC05_SSRPO_Problem::loadRotation( string problemName )
{
	try {
		// loading the orthogonal matrix from corresponding data files 
		ostringstream dataFileName;

		// FIXME find a way to handle path of these files :
		dataFileName << "./problem/CEC05/data/" << problemName << "_M_D" << getDimension() << ".txt";
		vector<double> v_m  = loadVectorFromFile( dataFileName.str() );
		vector< vector<double> > M  = vectorToSquareMatrix( v_m );
	} 
	catch( Exception_oMetah & e ) {
		cerr << e.what() << endl;
		exit(1);
	}
}


// Loads a vector from a file 
vector<double> itsCEC05_SSRPO_Problem::loadVectorFromFile( string filename )
{
	ifstream fichier;
	double temp;
	vector<double> V;

	//// ouverture d´un fichier en:
	//// Ecriture  Lecture   EffacerSiExiste   EcrireAPartirDeLaFinA	
	//// ios::in   ios::out  ios::trunc        ios::app

	fichier.open(filename.c_str(), ios::out);
	
	if(!fichier)  // teste l´erreur a l'ouverture...
    	{
		ostringstream msg;
		msg << "Cannot open file: " << filename;
		throw Exception_oMetah( msg.str(), EXCEPTION_INFOS );
	}
  	else
    	{
     		while ( !fichier.eof() ) // keep reading until end-of-file
		{
	  	fichier >> temp;   // sets EOF flag if no value found
	  	V.push_back(temp); // the white spaces and the newlines are simply ignored
		}
	V.pop_back(); 	     // FIXME must be change !fichier.eof()
    	}

  fichier.close();    // ferme le fichier	
  return V;
}

// Returns a two dimension square matrix with n*n dimension
vector< vector<double> > itsCEC05_SSRPO_Problem::vectorToSquareMatrix( vector<double> V )
{
	vector< vector<double> > m;
	int dimension = V.size(); 
	int dim = sqrt(dimension);

	if ( (dimension % dim) != 0 ) 
	{
		ostringstream msg;

		msg << "Vector dimension (" << dimension << ") has no dividable root (sqrt=" << dim << ")";

		throw Exception_Size_Match( msg.str(), EXCEPTION_INFOS );
		//FIXME throw 
	}
	
	else
	{
    		for( int i=0; i<dimension; i++)
			for( int j=0; j<dimension; j++)
	  		{
			int k = i*dim + j; 
	   		m[i][j] = V[k];
	  		}
	}
    
	return m ;
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
    double temp_o_t[] = {
       -3.9311900e+001,  5.8899900e+001, -4.6322400e+001, -7.4651500e+001, -1.6799700e+001, -8.0544100e+001, -1.0593500e+001,  2.4969400e+001,  8.9838400e+001,  9.1119000e+000, -1.0744300e+001, -2.7855800e+001, -1.2580600e+001,  7.5930000e+000,  7.4812700e+001,  6.8495900e+001, -5.3429300e+001,  7.8854400e+001, -6.8595700e+001,  6.3743200e+001 , 3.1347000e+001, -3.7501600e+001,  3.3892900e+001, -8.8804500e+001, -7.8771900e+001, -6.6494400e+001,  4.4197200e+001,  1.8383600e+001,  2.6521200e+001,  8.4472300e+001,  3.9176900e+001, -6.1486300e+001, -2.5603800e+001, -8.1182900e+001,  5.8695800e+001, -3.0838600e+001, -7.2672500e+001,  8.9925700e+001, -1.5193400e+001, -4.3337000e+000 , 5.3430000e+000,  1.0560300e+001, -7.7726800e+001,  5.2085900e+001 , 4.0394400e+001,  8.8332800e+001, -5.5830600e+001,  1.3181000e+000 , 3.6025000e+001, -6.9927100e+001, -8.6279000e+000, -5.6894400e+001 , 8.5129600e+001,  1.7673600e+001,  6.1529000e+000, -1.7695700e+001, -5.8953700e+001,  3.0356400e+001,  1.5920700e+001, -1.8008200e+001,  8.0641100e+001, -4.2391200e+001,  7.6277600e+001, -5.0165200e+001, -7.3573600e+001,  2.8336900e+001, -5.7990500e+001, -2.2732700e+001,  5.2026900e+001,  3.9259900e+001,  1.0867900e+001,  7.7820700e+001 , 6.6039500e+001, -5.0066700e+001,  5.5706300e+001,  7.3714100e+001 , 3.8529600e+001, -5.6786500e+001, -8.9647700e+001,  3.7957600e+001 , 2.9472000e+001, -3.5464100e+001, -3.1786800e+001,  7.7323500e+001 , 5.4790600e+001, -4.8279400e+001,  7.4271400e+001,  7.2610300e+001 , 6.2964000e+001, -1.4144600e+001,  2.0492300e+001,  4.6589700e+001, -8.3602100e+001, -4.6480900e+001,  8.3737300e+001, -7.9661100e+001,  2.4347900e+001, -1.7230300e+001,  7.2340400e+001, -3.6402200e+001
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


/*******************************************************************************
 * F2_ShiftedSchwefel
 ******************************************************************************/

itsF2_ShiftedSchwefel::itsF2_ShiftedSchwefel() : itsCEC05_SSRPO_Problem()
{
    // informations 
    setName("CEC'05-SSRPO F2 Shifted Schwefel’s Problem 1.2");
    setKey("CEC05_F2");
    setDescription("Unimodal, shifted, Non-separable, scalable.");
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
      3.5626700000000000e+001, -8.2912300000000002e+001,
      -1.0642300000000001e+001, -8.3581500000000005e+001,
      8.3155199999999994e+001, 4.7048000000000002e+001,
      -8.9435900000000004e+001, -2.7421900000000001e+001,
      7.6144800000000004e+001, -3.9059500000000000e+001,
      4.8885700000000000e+001, -3.9828000000000001e+000,
      -7.1924300000000002e+001, 6.4194699999999997e+001,
      -4.7733800000000002e+001, -5.9896000000000003e+000,
      -2.6282800000000002e+001, -5.9181100000000001e+001,
      1.4602800000000000e+001, -8.5477999999999994e+001,
      -5.0490099999999998e+001, 9.2400000000000004e-001,
      3.2397799999999997e+001, 3.0238800000000001e+001,
      -8.5094899999999996e+001, 6.0119700000000002e+001,
      -3.6218299999999999e+001, -8.5883000000000003e+000,
      -5.1970999999999998e+000, 8.1553100000000001e+001,
      -2.3431600000000000e+001, -2.5350500000000000e+001,
      -4.1248500000000000e+001, 8.8018000000000001e+000,
      -2.4222200000000001e+001, -8.7980699999999999e+001,
      7.8047300000000007e+001, -4.8052799999999998e+001,
      1.4017700000000000e+001
      , -3.6640500000000003e+001, 1.2216799999999999e+001,
      1.8144900000000000e+001, -6.4564700000000002e+001,
      -8.4849299999999999e+001, -7.6608800000000002e+001,
      -1.7041999999999999e+000, -3.6076099999999997e+001,
      3.7033600000000000e+001, 1.8443100000000001e+001,
      -6.4358999999999995e+001, -1.6750000000000000e+001,
      -7.6900000000000004e+000, -2.1280000000000001e+001,
      9.2019999999999996e+001, -1.3560000000000000e+001,
      -1.8109999999999999e+001, -8.8730000000000004e+001,
      -4.5509999999999998e+001, 1.3600000000000001e+000,
      -9.8900000000000006e+001, 5.9579999999999998e+001,
      -3.6479999999999997e+001, 7.5890000000000001e+001,
      3.2549999999999997e+001, -6.1210000000000001e+001,
      -1.9600000000000001e+001, 2.0239999999999998e+001,
      -1.3490000000000000e+001, 5.0149999999999999e+001,
      4.4729999999999997e+001, 4.1380000000000003e+001,
      -1.0000000000000001e-001, -4.4500000000000000e+001,
      3.0489999999999998e+001, 2.2180000000000000e+001,
      5.6299999999999999e+000, -6.2430000000000000e+001,
      -9.0859999999999999e+001, 6.2289999999999999e+001,
      5.0999999999999996e+000, -8.8969999999999999e+001,
      -5.9960000000000001e+001, -6.3469999999999999e+001,
      7.9079999999999998e+001, -8.7780000000000001e+001,
      5.9439999999999998e+001, -3.6579999999999998e+001,
      5.9780000000000001e+001, 7.7739999999999995e+001,
      2.2460000000000001e+001
      , 9.6790000000000006e+001, -1.1100000000000001e+000,
      4.1109999999999999e+001, 7.0609999999999999e+001,
      7.1739999999999995e+001, -9.9890000000000001e+001,
      9.2439999999999998e+001, 4.4289999999999999e+001,
      -7.8469999999999999e+001, -6.8659999999999997e+001
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

itsPoint itsF2_ShiftedSchwefel::objectiveFunction(itsPoint point)
{
    vector<double> x = point.getSolution();
    double f = f_bias;

	for (unsigned int i = 0; i < getDimension(); i++) 
	   {
	      double z = 0;
	      for (unsigned int j = 0; j < i; j++) 
		{
		  z = z + x[j] - o[j];
		}
	      f = f + z * z;
	   }

    point.setValue(f);
    return point;
}


itsProblem* itsF2_ShiftedSchwefel_Factory::create()
{
    return new itsF2_ShiftedSchwefel();
}


/*******************************************************************************
 * F3: Shifted Rotated High Conditioned Elliptic Function
 ******************************************************************************//*
itsF3_ShiftedRotatedElliptic::itsF3_ShiftedRotatedElliptic() : itsCEC05_SSRPO_Problem()
{
    // informations 
    setName("CEC'05-SSRPO F3 Shifted Rotated High Conditioned Elliptic Function");
    setKey("CEC05_F3");
    setDescription("Unimodal, shifted, rotated, Non-separable, scalable.");
    setCitation("TODO");    
    setFormula("TODO");
    
    // default value for all CEC'05 problems
    setAccuracy(1e-8);
    

    /***** Parameters *****/

    // may be 10, 30, 50
/*    setDimension(10); 
    
    // bounds of the search space
    setBoundsMinimaAll(-100);
    setBoundsMaximaAll(100);

    // the bias
    f_bias = -450;

    // the shift vector (which is the optimum)
    double temp_o_t[] = 
      {
	 -3.2201300e+001, 6.4977600e+001, -3.8300000e+001,
      -2.3258200e+001, -5.4008800e+001, 8.6628600e+001,
      -6.3009000e+000, -4.9364400e+001, 5.3499000e+000,
      5.2241800e+001, -1.3364300e+001, 7.3126300e+001,
      -8.5691000e+000, -2.0491500e+001, -6.0148700e+001,
      1.6088400e+001, -7.8331900e+001, 7.0038700e+001,
      -6.8521000e+000, -6.4797000e+001, 6.5400500e+001,
      -2.6023300e+001, -3.3875700e+001, 5.1589300e+001,
      2.7642700e+001, -6.9448500e+001, 2.5512300e+001,
      -5.9078200e+001, -6.6548100e+001, -5.1273300e+001,
      -8.1776000e+001, -7.1657200e+001, 3.7081000e+001,
      -6.3424800e+001, -6.4778500e+001, 3.1529900e+001,
      1.8538700e+001, 9.8342000e+000, -6.0370000e-001,
      1.7346000e+000, 7.0160500e+001, -8.2039100e+001,
      -4.2736800e+001, -8.3593000e+001, -8.5025500e+001,
      4.1177300e+001, 4.1649000e+000, -1.3450500e+001,
      -3.1000000e-001, -3.8794400e+001, 7.1270200e+001,
      6.5532000e+001, 8.7753000e+000, -5.5469100e+001,
      -2.0625200e+001, 2.2290100e+001, 1.3679800e+001,
      6.5674500e+001, 7.5841800e+001, 2.7892600e+001,
      -1.5061600e+001, -1.7303600e+001
      , 5.7934600e+001, -8.6632600e+001, 6.5059600e+001,
      4.7388400e+001, 2.9166000e+001, 6.5543500e+001,
      3.4643000e+000, -3.9814000e+001, 1.8226100e+001,
      7.7044600e+001, 6.2188200e+001, -1.1400000e+001,
      -1.0621800e+001, 7.0127600e+001, -4.0867300e+001,
      -2.4445100e+001, 5.2139800e+001, -1.0513600e+001,
      2.9239900e+001, 2.1705000e+000, 4.4086300e+001,
      8.1794300e+001, 8.0046600e+001, 8.8326600e+001,
      1.6609800e+001, -5.0257300e+001, -7.1699300e+001,
      7.1536800e+001, 6.1427300e+001, -3.6739000e+000,
      7.7942800e+001, -2.2329400e+001, 6.4763400e+001,
      -7.4282300e+001, 1.4189900e+001, 3.7847300e+001,
      -7.7712900e+001, 2.8995900e+001
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

    loadRotation("elliptic");
}

itsPoint itsF3_ShiftedRotatedElliptic::objectiveFunction(itsPoint point)
{
	unsigned int dim = getDimension();
	
	if ( dim != 2 && dim != 10 && dim != 30 && dim != 50 ) {
		// FIXME : does not seems to output something !
		cerr << "WARNING: dimension (" << dim 
			<< ") is not a default one (2, 10, 30 or 50)." << endl;
	}

    vector<double> x = point.getSolution();
    vector<double> z;
    double f = f_bias;

    // Translation
    for (unsigned int d = 0; d < dim; d++) 
    {
	if ( dim <= 50 ) {
		z[d] = x[d] - o[d];
	} else {
		z[d] = x[d];
	}
    }

	vector< double > zRot = rotation(M,z);

  for (unsigned int d = 0; d < dim; d++) 
    {
	if ( dim <= 50 ) {
		f += pow( 1000000.0, (double) d / (double) dim ) 
			* pow( zRot[d], 2.0 );
	} else {
		f += pow( 1000000.0, (double) d / (double) dim ) 
			* pow( z[d], 2.0 );
	}
    }

    point.setValue(f);
    return point;
}


itsProblem* itsF3_ShiftedRotatedElliptic_Factory::create()
{
    return new itsF3_ShiftedRotatedElliptic();
}*/

/*******************************************************************************
 * F4: Shifted Schwefel’s Problem 1.2 with Noise in Fitness
 ******************************************************************************/

/*******************************************************************************
 * F5: Schwefel’s Problem 2.6 with Global Optimum on Bounds
 ******************************************************************************/

/*******************************************************************************
 * F6: Shifted Rosenbrock’s Function
 ******************************************************************************/

/*******************************************************************************
 * F7: Shifted Rotated Griewank’s Function without Bounds
 ******************************************************************************/

/*******************************************************************************
 * F8: Shifted Rotated Ackley’s Function with Global Optimum on Bounds
 ******************************************************************************/

/*******************************************************************************
 * F9: Shifted Rastrigin’s Function
 ******************************************************************************/

/*******************************************************************************
 * F10: Shifted Rotated Rastrigin’s Function
 ******************************************************************************/

/*******************************************************************************
 * F11: Shifted Rotated Weierstrass Function
 ******************************************************************************/

/*******************************************************************************
 * F12: Schwefel’s Problem 2.13
 ******************************************************************************/

/*******************************************************************************
 * F13: Expanded Extended Griewank’s plus Rosenbrock’s Function (F8F2)
 ******************************************************************************/

/*******************************************************************************
 * F14: Shifted Rotated Expanded Scaffer’s F6
 ******************************************************************************/

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
