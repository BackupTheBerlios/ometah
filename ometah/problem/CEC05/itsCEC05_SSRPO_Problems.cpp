/***************************************************************************
 * $Id: itsCEC05_SSRPO_Problems.cpp,v 1.6 2006/06/18 18:29:42 walid_tfaili Exp $
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


//! Loads the name and path of the data file
void itsCEC05_SSRPO_Problem::loadRotation( string problemName )
{
	try {
		// loading the orthogonal matrix from corresponding data files 
		ostringstream dataFileName;

		// FIXME find a way to handle path of these files :
		dataFileName << "./problem/CEC05/data/" << problemName << "_M_D" << getDimension() << ".txt";
		vector<double> v_m  = loadVectorFromFile( dataFileName.str() );
		v_m.pop_back(); 	     // FIXME must be change !fichier.eof() an extra white space at the end of files
		M  = vectorToSquareMatrix( v_m );
	}
	catch( Exception_oMetah & e ) {
		cerr << e.what() << endl;
		exit(1);
	}
}



//! Loads a vector from a file 
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
		V.push_back(temp); // white spaces and newlines are simply ignored
		}
		V.pop_back();	  // FIXME needed to remove an extra element caused by the last white space in the data files
	}

	fichier.close();    // ferme le fichier	

return V;
}


//! Returns a two dimension square matrix with n*n dimension
vector< vector<double> > itsCEC05_SSRPO_Problem::vectorToSquareMatrix( vector<double> V )
{
	vector< vector<double> > m;
	vector<double> temp_v;
	int dimension = V.size(); 
	double dim = sqrt(dimension);

	if ( ( dimension % (int) dim) != 0 ) 
	{
		ostringstream msg;

		msg << "Vector dimension (" << dimension << ") has no dividable root (sqrt=" << dim << ")";

		throw Exception_Size_Match( msg.str(), EXCEPTION_INFOS );
		//FIXME throw 
	}
	
	else
	{
		int k = 0;

		for(  int i=0; i<dim; i++){
			for( int j=0; j<dim; j++)
			{
			k = i*(int)dim + j; 
			
			temp_v.push_back(V[k]);
			}
		m.push_back( temp_v );
		
		int d = temp_v.size();
		while ( d != 0 ){
			temp_v.pop_back(); 
			d--;
			};
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
 * F2_ShiftedSchwefel Problem 1.2
 ******************************************************************************/

itsF2_ShiftedSchwefel::itsF2_ShiftedSchwefel() : itsCEC05_SSRPO_Problem()
{
	// informations 
	setName("CEC'05-SSRPO F2 Shifted Schwefel’s Problem 1.2");
	setKey("CEC05_F2");
	setDescription("Unimodal, Shifted, Non-separable, scalable.");
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
	3.5626700000000000e+001, -8.2912300000000002e+001, -1.0642300000000001e+001, -8.3581500000000005e+001, 8.3155199999999994e+001, 4.7048000000000002e+001, -8.9435900000000004e+001, -2.7421900000000001e+001, 7.6144800000000004e+001, -3.9059500000000000e+001, 4.8885700000000000e+001, -3.9828000000000001e+000,     -7.1924300000000002e+001, 6.4194699999999997e+001, -4.7733800000000002e+001, -5.9896000000000003e+000, -2.6282800000000002e+001,-5.9181100000000001e+001,      1.4602800000000000e+001, -8.5477999999999994e+001, -5.0490099999999998e+001, 9.2400000000000004e-001, 3.2397799999999997e+001, 3.0238800000000001e+001, -8.5094899999999996e+001, 6.0119700000000002e+001, -3.6218299999999999e+001, -8.5883000000000003e+000, -5.1970999999999998e+000, 8.1553100000000001e+001,      -2.3431600000000000e+001, -2.5350500000000000e+001, -4.1248500000000000e+001, 8.8018000000000001e+000, -2.4222200000000001e+001, -8.7980699999999999e+001,      7.8047300000000007e+001, -4.8052799999999998e+001, 1.4017700000000000e+001, -3.6640500000000003e+001, 1.2216799999999999e+001, 1.8144900000000000e+001, -6.4564700000000002e+001,  -8.4849299999999999e+001, -7.6608800000000002e+001,   -1.7041999999999999e+000, -3.6076099999999997e+001, 3.7033600000000000e+001, 1.8443100000000001e+001, -6.4358999999999995e+001, -1.6750000000000000e+001,      -7.6900000000000004e+000, -2.1280000000000001e+001, 9.2019999999999996e+001, -1.3560000000000000e+001, -1.8109999999999999e+001, -8.8730000000000004e+001,     -4.5509999999999998e+001, 1.3600000000000001e+000, -9.8900000000000006e+001, 5.9579999999999998e+001, -3.6479999999999997e+001, 7.5890000000000001e+001, 3.2549999999999997e+001, -6.1210000000000001e+001, -1.9600000000000001e+001, 2.0239999999999998e+001, -1.3490000000000000e+001, 5.0149999999999999e+001,      4.4729999999999997e+001, 4.1380000000000003e+001, -1.0000000000000001e-001, -4.4500000000000000e+001, 3.0489999999999998e+001, 2.2180000000000000e+001,      5.6299999999999999e+000, -6.2430000000000000e+001, -9.0859999999999999e+001, 6.2289999999999999e+001, 5.0999999999999996e+000, -8.8969999999999999e+001, -5.9960000000000001e+001, -6.3469999999999999e+001, 7.9079999999999998e+001, -8.7780000000000001e+001, 5.9439999999999998e+001, -3.6579999999999998e+001, 5.9780000000000001e+001, 7.7739999999999995e+001, 2.2460000000000001e+001, 9.6790000000000006e+001, -1.1100000000000001e+000, 4.1109999999999999e+001, 7.0609999999999999e+001, 7.1739999999999995e+001, -9.9890000000000001e+001, 9.2439999999999998e+001, 4.4289999999999999e+001, -7.8469999999999999e+001, -6.8659999999999997e+001
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
		f += z * z;
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
 ******************************************************************************/

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
	setDimension(10);

	// bounds of the search space
	setBoundsMinimaAll(-100);
	setBoundsMaximaAll(100);

	// the bias
	f_bias = -450;

	// the shift vector (which is the optimum)
	double temp_o_t[] = 
	{
	-3.2201300e+001, 6.4977600e+001, -3.8300000e+001, -2.3258200e+001, -5.4008800e+001, 8.6628600e+001, -6.3009000e+000, -4.9364400e+001, 5.3499000e+000, 5.2241800e+001, -1.3364300e+001, 7.3126300e+001,     -8.5691000e+000, -2.0491500e+001, -6.0148700e+001, 1.6088400e+001, -7.8331900e+001, 7.0038700e+001, -6.8521000e+000, -6.4797000e+001, 6.5400500e+001, -2.6023300e+001, -3.3875700e+001, 5.1589300e+001, 2.7642700e+001, -6.9448500e+001, 2.5512300e+001, -5.9078200e+001, -6.6548100e+001, -5.1273300e+001, -8.1776000e+001, -7.1657200e+001, 3.7081000e+001, -6.3424800e+001, -6.4778500e+001, 3.1529900e+001, 1.8538700e+001, 9.8342000e+000, -6.0370000e-001, 1.7346000e+000, 7.0160500e+001, -8.2039100e+001, -4.2736800e+001, -8.3593000e+001, -8.5025500e+001, 4.1177300e+001, 4.1649000e+000, -1.3450500e+001, -3.1000000e-001, -3.8794400e+001, 7.1270200e+001, 6.5532000e+001, 8.7753000e+000, -5.5469100e+001, -2.0625200e+001, 2.2290100e+001, 1.3679800e+001, 6.5674500e+001, 7.5841800e+001, 2.7892600e+001, -1.5061600e+001, -1.7303600e+001, 5.7934600e+001, -8.6632600e+001, 6.5059600e+001, 4.7388400e+001, 2.9166000e+001, 6.5543500e+001, 3.4643000e+000, -3.9814000e+001, 1.8226100e+001, 7.7044600e+001, 6.2188200e+001, -1.1400000e+001, -1.0621800e+001, 7.0127600e+001, -4.0867300e+001, -2.4445100e+001, 5.2139800e+001, -1.0513600e+001, 2.9239900e+001, 2.1705000e+000, 4.4086300e+001, 8.1794300e+001, 8.0046600e+001, 8.8326600e+001, 1.6609800e+001, -5.0257300e+001, -7.1699300e+001, 7.1536800e+001, 6.1427300e+001, -3.6739000e+000, 7.7942800e+001, -2.2329400e+001, 6.4763400e+001, -7.4282300e+001, 1.4189900e+001, 3.7847300e+001, -7.7712900e+001, 2.8995900e+001
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

itsPoint itsF3_ShiftedRotatedElliptic::objectiveFunction(itsPoint point)
{
	unsigned int dim = getDimension();
	vector<double> x = point.getSolution();
	vector<double> z;
	vector<double> zRot;
	double f = f_bias;
	
	// Translation
	for (unsigned int d = 0; d < dim; d++) 
		z.push_back(x[d] - o[d]);

	if ( dim == 2 || dim == 10 || dim == 30 || dim == 50 ) {
		// initialize the matrix M
		loadRotation("elliptic");
		zRot = multiplyVectorByMatrix( z, M );
		for (unsigned int d = 0; d < dim; d++) 
			f += pow( 1000000.0, (double) d / (double) dim ) 
				* pow( zRot[d], 2.0 );
	}
	else{
		ostringstream msg;
		msg << "WARNING: dimension (" << dim << ") is not a default one (2, 10, 30 or 50)." << endl;
		throw Exception_oMetah( msg.str(), EXCEPTION_INFOS );

		for (unsigned int d = 0; d < dim; d++) 
			f += pow( 1000000.0, (double) d / (double) dim ) 
				* pow( z[d], 2.0 );
	
	}

	point.setValue(f);

return point;

}


itsProblem* itsF3_ShiftedRotatedElliptic_Factory::create()
{
	return new itsF3_ShiftedRotatedElliptic();
}

/*******************************************************************************
 * F4: Shifted Schwefel’s Problem 1.2 with Noise in Fitness
 ******************************************************************************/

itsF4_ShiftedSchwefelWithNoise::itsF4_ShiftedSchwefelWithNoise() : itsCEC05_SSRPO_Problem()
{
	// informations 
	setName("CEC'05-SSRPO F4 Shifted Schwefel's Problem 1.2 with Noise in Fitness");
	setKey("CEC05_F4");
	setDescription("Unimodal, Shifted, Non-separable, Scalable, Noise in fitness.");
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
	3.5626700000000000e+001, -8.2912300000000002e+001, -1.0642300000000001e+001, -8.3581500000000005e+001, 8.3155199999999994e+001, 4.7048000000000002e+001, -8.9435900000000004e+001, -2.7421900000000001e+001, 7.6144800000000004e+001, -3.9059500000000000e+001, 4.8885700000000000e+001, -3.9828000000000001e+000,     -7.1924300000000002e+001, 6.4194699999999997e+001, -4.7733800000000002e+001, -5.9896000000000003e+000, -2.6282800000000002e+001,-5.9181100000000001e+001,      1.4602800000000000e+001, -8.5477999999999994e+001, -5.0490099999999998e+001, 9.2400000000000004e-001, 3.2397799999999997e+001, 3.0238800000000001e+001, -8.5094899999999996e+001, 6.0119700000000002e+001, -3.6218299999999999e+001, -8.5883000000000003e+000, -5.1970999999999998e+000, 8.1553100000000001e+001,      -2.3431600000000000e+001, -2.5350500000000000e+001, -4.1248500000000000e+001, 8.8018000000000001e+000, -2.4222200000000001e+001, -8.7980699999999999e+001,      7.8047300000000007e+001, -4.8052799999999998e+001, 1.4017700000000000e+001, -3.6640500000000003e+001, 1.2216799999999999e+001, 1.8144900000000000e+001, -6.4564700000000002e+001,  -8.4849299999999999e+001, -7.6608800000000002e+001,   -1.7041999999999999e+000, -3.6076099999999997e+001, 3.7033600000000000e+001, 1.8443100000000001e+001, -6.4358999999999995e+001, -1.6750000000000000e+001,      -7.6900000000000004e+000, -2.1280000000000001e+001, 9.2019999999999996e+001, -1.3560000000000000e+001, -1.8109999999999999e+001, -8.8730000000000004e+001,     -4.5509999999999998e+001, 1.3600000000000001e+000, -9.8900000000000006e+001, 5.9579999999999998e+001, -3.6479999999999997e+001, 7.5890000000000001e+001, 3.2549999999999997e+001, -6.1210000000000001e+001, -1.9600000000000001e+001, 2.0239999999999998e+001, -1.3490000000000000e+001, 5.0149999999999999e+001,      4.4729999999999997e+001, 4.1380000000000003e+001, -1.0000000000000001e-001, -4.4500000000000000e+001, 3.0489999999999998e+001, 2.2180000000000000e+001,      5.6299999999999999e+000, -6.2430000000000000e+001, -9.0859999999999999e+001, 6.2289999999999999e+001, 5.0999999999999996e+000, -8.8969999999999999e+001, -5.9960000000000001e+001, -6.3469999999999999e+001, 7.9079999999999998e+001, -8.7780000000000001e+001, 5.9439999999999998e+001, -3.6579999999999998e+001, 5.9780000000000001e+001, 7.7739999999999995e+001, 2.2460000000000001e+001, 9.6790000000000006e+001, -1.1100000000000001e+000, 4.1109999999999999e+001, 7.0609999999999999e+001, 7.1739999999999995e+001, -9.9890000000000001e+001, 9.2439999999999998e+001, 4.4289999999999999e+001, -7.8469999999999999e+001, -6.8659999999999997e+001
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


itsPoint itsF4_ShiftedSchwefelWithNoise::objectiveFunction(itsPoint point)
{
	vector<double> x = point.getSolution();
	double f = f_bias;
	double t = 0;

	for (unsigned int i = 0; i < getDimension(); i++) 
	{
		double z = 0;
		for (unsigned int j = 0; j < i; j++) 
		{
			z = z + x[j] - o[j];
		}
		t += z * z ;
	}
	f += t * ( 1 + 0.4 * ( rand()/RAND_MAX ) );

	point.setValue(f);

return point;

}


itsProblem* itsF4_ShiftedSchwefelWithNoise_Factory::create()
{

return new itsF4_ShiftedSchwefelWithNoise();

}


/*******************************************************************************
 * F5: Schwefel’s Problem 2.6 with Global Optimum on Bounds
 ******************************************************************************/

itsF5_SchwefelBounds::itsF5_SchwefelBounds() : itsCEC05_SSRPO_Problem()
{
	// informations 
	setName("CEC'05-SSRPO F5 Schwefel’s Problem 2.6 with Global Optimum on Bounds");
	setKey("CEC05_F5");
	setDescription("Unimodal, Non-separable, Scalable, If the initialization procedure initializes the population at the bounds this problem will be solved easily");
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
	f_bias = -310;

	// the shift vector (which is the optimum)
	double temp_o_t[] = 
	{
	-5.5559000e+000, 7.9470000e+000, -1.5380000e+000, 8.3897000e+000, 7.7182000e+000, -8.3147000e+000, -2.1423000e+000, -2.4392000e+000, -3.3787000e+000, -7.3047000e+000, 3.0580000e+000, 6.7613000e+000,      2.3444000e+000, 5.6514000e+000, 1.0491000e+000, -8.3240000e-001, 1.3039000e+000, -6.5100000e-002, 4.2400000e-002, -6.5176000e+000, -8.6977000e+000, 2.7053000e+000, -1.4842000e+000, -8.8158000e+000, 5.6475000e+000, -4.5999000e+000, 3.6337000e+000, -6.4068000e+000, 4.8867000e+000, 8.2225000e+000, 6.6873000e+000, -5.8862000e+000, 2.5925000e+000, 8.0270000e-001, 7.5525000e+000, 4.2621000e+000, 3.5091000e+000, -2.6055000e+000, -8.4063000e+000, 6.1947000e+000, -6.5024000e+000, -8.1440000e+000, -4.8444000e+000, 3.1572000e+000, 3.9624000e+000, -8.4969000e+000, 6.2642000e+000, 1.1448000e+000, 3.9132000e+000, 3.6140000e+000, -8.4785000e+000, 2.9550000e-001, 3.5597000e+000, -5.5854000e+000, -8.9173000e+000, 4.0627000e+000, 8.3870000e+000, -3.7680000e+000, 5.9001000e+000, 3.8212000e+000,   -6.4771000e+000, 6.8886000e+000, -2.4951000e+000, 2.5007000e+000, 1.3866000e+000, -7.3488000e+000, -1.5349000e+000, 2.9223000e+000,7.1813000e+000, -4.7990000e+000, 8.3061000e+000, -8.7911000e+000, -6.3035000e+000, -6.1222000e+000, 5.7116000e+000, 7.4369000e+000, 6.1738000e+000, -7.1118000e+000, 3.7062000e+000, 6.1274000e+000, 1.3696000e+000, -4.5894000e+000, 4.5844000e+000, -1.2133000e+000, 4.1800000e+000, 3.3370000e-001, 7.3355000e+000, 8.1600000e+000, 1.4422000e+000, 7.9909000e+000, -8.1183000e+000, -6.8285000e+000, -2.9079000e+000, 2.0280000e-001, -6.2375000e+000, 1.2940000e-001, 1.4740000e-001, 2.6981000e+000, 4.2330000e-001, 6.1942000e+000
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

itsPoint itsF5_SchwefelBounds::objectiveFunction(itsPoint point)
{
	vector<double> x = point.getSolution();
	vector<double> z;
	double f = f_bias;
	double D = getDimension();
	double max = 0;
	double zz;
	
	vector< double > A = loadVectorFromFile("./problem/CEC05/data/schwefel_206_data.txt");

	// Optimum on bounds
	for (unsigned int i = 0; i < D / 4; i++) 
	{
		o[i] = -100 ;
	}
	for ( unsigned int i = (unsigned int) ( (3 * D / 4) -1 ); i < D; i++) 
	{
		o[i] = 100 ;
	}
	
	// Translation
	for (unsigned int i = 0; i < D; i++) 
	{
		z.push_back( x[i] - o[i] ) ;
	}

	//Evaluation
	max = 0;
	for (unsigned int i = 0; i < D; i++) 
	{
		zz = 0;
		for (unsigned int j = 0; j < D; j++) 
		{
			int k = 100 * i + j;
			zz = zz + A[k] * z[j];
		}
		if (zz > max) 
		{
			max = zz;
		}
	}
	f = f + max;


    point.setValue(f);
    return point;
}


itsProblem* itsF5_SchwefelBounds_Factory::create()
{
    return new itsF5_SchwefelBounds();
}


/*******************************************************************************
 * F6: Shifted Rosenbrock’s Function
 ******************************************************************************/
itsF6_ShiftedRosenbrock::itsF6_ShiftedRosenbrock() : itsCEC05_SSRPO_Problem()
{
	// informations 
	setName("CEC'05-SSRPO F6 Shifted Rosenbrock's Function");
	setKey("CEC05_F6");
	setDescription("Multi-modal, Shifted, Non-separable, Scalable, Having a very narrow valley from local optimum to global optimum");
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
	f_bias = 390;

	// the shift vector (which is the optimum)
	double temp_o_t[] = 
	{
	8.1023200e+001, -4.8395000e+001, 1.9231600e+001, -2.5231000e+000, 7.0433800e+001, 4.7177400e+001, -7.8358000e+000, -8.6669300e+001,      5.7853200e+001, -9.9533000e+000, 2.0777800e+001, 5.2548600e+001, 7.5926300e+001, 4.2877300e+001, -5.8272000e+001, -1.6972800e+001, 7.8384500e+001, 7.5042700e+001, -1.6151300e+001, 7.0856900e+001,      -7.9579500e+001, -2.6483700e+001, 5.6369900e+001, -8.8224900e+001, -6.4999600e+001, -5.3502200e+001, -5.4230000e+001, 1.8682600e+001,     -4.1006100e+001, -5.4213400e+001, -8.7250600e+001, 4.4421400e+001, -9.8826000e+000, 7.7726600e+001, -6.1210000e+000, -1.4643000e+001, 6.2319800e+001, 4.5274000e+000, -5.3523400e+001, 3.0984700e+001,      6.0861300e+001, -8.6464800e+001, 3.2629800e+001, -2.1693400e+001, 5.9723200e+001, 5.0630000e-001, 3.7704800e+001, -1.2799300e+001,     -3.5168800e+001, -5.5862300e+001, -5.5182300e+001, 3.2800100e+001, -3.5502400e+001, 7.5012000e+000, -6.2842800e+001, 3.5621700e+001, -2.1892800e+001, 6.4802000e+001, 6.3657900e+001, 1.6841300e+001,     -6.2050000e-001, 7.1958400e+001, 5.7893200e+001, 2.6083800e+001, 5.7235300e+001, 2.8840900e+001, -2.8445200e+001, -3.7849300e+001,     -2.8585100e+001, 6.1342000e+000, 4.0880300e+001, -3.4327700e+001, 6.0929200e+001, 1.2253000e+001, -2.3325500e+001, 3.6493100e+001, 8.3828000e+000, -9.9215000e+000, 3.5022100e+001, 2.1835800e+001,     5.3067700e+001, 8.2231800e+001, 4.0662000e+000, 6.8425500e+001, -5.8867800e+001, 8.6354400e+001, -4.1139400e+001, -4.4580700e+001,    6.7633500e+001, 4.2715000e+001, -6.5426600e+001, -8.7883700e+001, 7.0901600e+001, -5.4155100e+001, -3.6229800e+001, 2.9059600e+001, -3.8806400e+001, -5.5396000e+000, -7.8339300e+001, 8.7900200e+001
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

itsPoint itsF6_ShiftedRosenbrock::objectiveFunction(itsPoint point)
{
	vector<double> x = point.getSolution();
	double f = f_bias;
	double D = getDimension();
	double f1, f2, zz, zz1;
	
	
	f1 = 0;
	f2 = 0;
	for (unsigned int i = 0; i < D - 1; i++) 
	{
		zz = x[i] - o[i] + 1;
		zz1 = x[i + 1] - o[i + 1] + 1;
		f1 = f1 + pow(zz * zz - zz1, 2.0);
		f2 = f2 + pow(zz - 1, 2.0);
	}
	f = f + 100 * f1 + f2;


	point.setValue(f);
	return point;
}


itsProblem* itsF6_ShiftedRosenbrock_Factory::create()
{
    return new itsF6_ShiftedRosenbrock();
}

/*******************************************************************************
 * F7: Shifted Rotated Griewank's Function without Bounds
 ******************************************************************************/
itsF7_ShiftedRotatedGriewank::itsF7_ShiftedRotatedGriewank() : itsCEC05_SSRPO_Problem()
{
	// informations 
	setName("CEC'05-SSRPO F7 Shifted Rotated Griewank's Function without Bounds");
	setKey("CEC05_F7");
	setDescription("Multi-modal, Rotated, Shifted, Non-separable, Scalable, No bounds for variables x, Initialize population in [0, 600]^D");
	setCitation("TODO");
	setFormula("TODO");

	// default value for all CEC'05 problems
	setAccuracy(1e-8);


	/***** Parameters *****/

	// may be 10, 30, 50
	setDimension(10); 

	// bounds of the search space
	setBoundsMinimaAll(0);
	setBoundsMaximaAll(600);

	// the bias
	f_bias = -180;

	// the shift vector (which is the optimum)
	double temp_o_t[] = 
	{
	-2.7626840e+002, -1.1911000e+001, -5.7878840e+002, -2.8764860e+002, -8.4385800e+001, -2.2867530e+002, -4.5815160e+002, -2.0221450e+002, -1.0586420e+002, -9.6489800e+001, -3.9574680e+002, -5.7294980e+002,      -2.7036410e+002, -5.6685430e+002, -1.5242040e+002, -5.8838190e+002, -2.8288920e+002, -4.8888650e+002, -3.4698170e+002, -4.5304470e+002, -5.0658570e+002, -4.7599870e+002, -3.6204920e+002, -2.3323670e+002,      -4.9198640e+002, -5.4408980e+002, -7.3445600e+001, -5.2690110e+002, -5.0225610e+002, -5.3723530e+002, -2.6314870e+002, -2.0592600e+002, -4.6582320e+002, -9.7565000e+000, -1.6914000e+001, -4.5293970e+002, -4.3061070e+002, -1.6231370e+002, -6.9020400e+001, -3.0240820e+002, -5.2789320e+002, -4.8942740e+002, -7.4256200e+001, -6.8285000e+000, -5.4636310e+002, -4.1518800e+002, -3.2214990e+002, -1.4538220e+002,     -5.2700190e+002, -4.2217790e+002, -2.5425420e+002, -4.9572890e+002, -2.8610230e+002, -1.9616440e+002, -1.0493830e+002, -3.8844900e+002, -5.7932200e+001, -5.3995800e+001, -1.1438820e+002, -5.0943400e+001, -5.0809610e+002, -7.7027800e+001, -2.4296420e+002, -1.1586500e+001, -5.0694900e+002, -2.2216620e+002, -3.0383250e+002, -3.5940410e+002, -1.5607150e+002, -2.5297110e+002, -3.4570620e+002, -1.3306540e+002,    -3.0187400e+001, -3.0661620e+002, -1.6066730e+002, -2.1051130e+002, -1.8625180e+002, -5.5031270e+002, -5.1903050e+002, -3.2077600e+002, -4.7110670e+002, -1.5917240e+002, -4.4075100e+001, -8.4125100e+001,   -5.3803890e+002, -4.4419930e+002, -5.9093380e+002, -2.1680070e+002, -5.2327310e+002, -3.1720000e+002, -3.9366710e+002, -5.4843390e+002, -4.1604030e+002, -3.8038260e+002, -1.0954200e+002, -1.6776100e+002,    -3.4551270e+002, -5.4486310e+002, -3.5782180e+002, -5.9821170e+002
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

itsPoint itsF7_ShiftedRotatedGriewank::objectiveFunction(itsPoint point)
{
	vector<double> x = point.getSolution();
	vector<double> z, zRot;
	double f = f_bias;
	double D = getDimension();
	double f1, f2;

	// Translation
	for ( unsigned int i = 0; i < D; i++ ) {
		z.push_back( x[i] - o[i] );
	}
	
	// Rotation
	if( D==2 || D==10 || D==30 || D==50 ){
		// initialize the matrix M
		loadRotation("griewank");
		// (x - o)*M
		zRot = multiplyVectorByMatrix( z, M );
	}
	else{
		ostringstream msg;
		msg << "WARNING: dimension (" << D << ") is not a default one (2, 10, 30 or 50)." << endl;
		throw Exception_oMetah( msg.str(), EXCEPTION_INFOS );
		zRot = z;
	}
	
	// Evaluation
	for ( unsigned int i = 0; i < D; i++) {
		f1 = f1 + zRot[i] * zRot[i];
		f2 = f2 * cos((zRot[i]) / sqrt((double)(i + 1)));
	}

	f = f + f1 / 4000 - f2 + 1;



	point.setValue(f);
	return point;
}


itsProblem* itsF7_ShiftedRotatedGriewank_Factory::create()
{
    return new itsF7_ShiftedRotatedGriewank();
}


/*******************************************************************************
 * F8: Shifted Rotated Ackley’s Function with Global Optimum on Bounds
 ******************************************************************************/
itsF8_RotatedAckleyWithBounds::itsF8_RotatedAckleyWithBounds() : itsCEC05_SSRPO_Problem()
{
	// informations 
	setName("CEC'05-SSRPO F8 Shifted Rotated Ackley’s Function with Global Optimum on Bounds");
	setKey("CEC05_F8");
	setDescription("Multi-modal, Rotated, Shifted, Non-separable, Scalable, A's condition number Cond(A) increases with the number of variables as O(D2), Global optimum on the bound, If the initialization procedure initializes the population at the bounds, this problem will be solved easily");
	setCitation("TODO");
	setFormula("TODO");

	// default value for all CEC'05 problems
	setAccuracy(1e-8);


	/***** Parameters *****/

	// may be 10, 30, 50
	setDimension(10); 

	// bounds of the search space
	setBoundsMinimaAll(-32);
	setBoundsMaximaAll(32);

	// the bias
	f_bias = -140;

	// the shift vector (which is the optimum)
	double temp_o_t[] = 
	{
	-1.6823000e+001, 1.4976900e+001, 6.1690000e+000, 9.5566000e+000, 1.9541700e+001, -1.7190000e+001, -1.8824800e+001, 8.5110000e-001, -1.5116200e+001, 1.0793400e+001, 7.4091000e+000, 8.6171000e+000,     -1.6564100e+001, -6.6800000e+000, 1.4543300e+001, 7.0454000e+000, -1.8621500e+001, 1.4556100e+001, -1.1594200e+001, -1.9153100e+001, -4.7372000e+000, 9.2590000e-001, 1.3241200e+001, -5.2947000e+000,      1.8416000e+000, 4.5618000e+000, -1.8890500e+001, 9.8008000e+000, -1.5426500e+001, 1.2722000e+000, -4.5920000e-001, -8.2939000e+000, -5.9257000e+000, 1.5606500e+001, 2.0942000e+000, -1.3782900e+001,     -1.3005100e+001, 1.5142400e+001, -1.4621400e+001, 8.6143000e+000, 1.8052200e+001, 1.7577800e+001, -7.6810000e+000, -4.5826000e+000, 2.0896000e+000, 4.7818000e+000, 9.4115000e+000, 1.0168000e+001,     -2.8787000e+000, -4.0770000e-001, 3.0359000e+000, 1.4492800e+001, 8.2224000e+000, 1.8095000e+000, 3.6144000e+000, -3.4366000e+000, -1.0708600e+001, -3.1311000e+000, -9.4393000e+000, -5.0748000e+000,      1.7545800e+001, 1.7436800e+001, 5.4145000e+000, 7.5412000e+000, -1.5661100e+001, 1.7669800e+001, -1.6519000e+001, 1.1152500e+001,      -1.2178100e+001, 1.7768500e+001, 1.0664900e+001, 8.7514000e+000, -1.9787500e+001, 1.3069000e+000, 9.7793000e+000, -1.1666900e+001, 2.6540000e-001, 7.9918000e+000, -4.9550000e+000, 3.5881000e+000,      -7.7524000e+000, -1.6297900e+001, 9.0324000e+000, -6.2570000e-001, 8.8854000e+000, -2.7198000e+000, 9.4307000e+000, 9.0800000e+000, -6.5800000e-001, -1.1024400e+001, 1.9848400e+001, -1.3460000e-001,      1.7819500e+001, 1.1721400e+001, -1.0295300e+001, -7.7800000e-001, 1.2843500e+001, -8.9002000e+000, 1.7685600e+001, 6.1183000e+000
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

itsPoint itsF8_RotatedAckleyWithBounds::objectiveFunction(itsPoint point)
{
	vector<double> x = point.getSolution();
	vector<double> z, zRot;
	double f = f_bias;
	double D = getDimension();
	double f1, f2;
	double e = getAccuracy(); //FIXME not sure that e is the accuracy

	// On bounds
	for ( unsigned int i = 0; i < D / 2; i++ ){
		o[2 * i] = -32;
	}

	// Translation
	for ( unsigned int i = 0; i < D; i++ ){
		z.push_back( x[i] - o[i] );
	}
	
	// Rotation
	if( D==10 || D==30 || D==50 ){
		// initialize the matrix M
		loadRotation("ackley");
		// (x - o)*M
		zRot = multiplyVectorByMatrix( z, M );
	}
	else{
		ostringstream msg;
		msg << "WARNING: dimension (" << D << ") is not a default one ( 10, 30 or 50)." << endl;
		throw Exception_oMetah( msg.str(), EXCEPTION_INFOS );
		zRot = z;
	}

	// Evaluation
        for ( unsigned int i = 0; i < D; i++){
		f1 = f1 + zRot[i] * zRot[i];
		f2 = f2 + cos(2 * PI * zRot[i]);
	}
	
	f = f - 20 * exp( -0.2 * sqrt(f1 / D)) - exp(f2 / D) + 20 + e;

	point.setValue(f);
	return point;
}


itsProblem* itsF8_RotatedAckleyWithBounds_Factory::create()
{
    return new itsF8_RotatedAckleyWithBounds();
}

/*******************************************************************************
 * F9: Shifted Rastrigin's Function
 ******************************************************************************/

itsF9_ShiftedRastrigin::itsF9_ShiftedRastrigin() : itsCEC05_SSRPO_Problem()
{
	// informations 
	setName("CEC'05-SSRPO F9 Shifted Rastrigin's Function");
	setKey("CEC05_F9");
	setDescription("Multi-modal, Shifted, Separable, Scalable, Local optima's number is huge");
	setCitation("TODO");
	setFormula("TODO");

	// default value for all CEC'05 problems
	setAccuracy(1e-8);


	/***** Parameters *****/

	// may be 10, 30, 50
	setDimension(10); 

	// bounds of the search space
	setBoundsMinimaAll(-5);
	setBoundsMaximaAll(5);

	// the bias
	f_bias = -330;

	// the shift vector (which is the optimum)
	double temp_o_t[] = 
	{
	1.9005000e+000, -1.5644000e+000, -9.7880000e-001, -2.2536000e+000, 2.4990000e+000, -3.2853000e+000, 9.7590000e-001, -3.6661000e+000, 9.8500000e-002, -3.2465000e+000, 3.8060000e+000, -2.6834000e+000,      -1.3701000e+000, 4.1821000e+000, 2.4856000e+000, -4.2237000e+000, 3.3653000e+000, 2.1532000e+000, -3.0929000e+000, 4.3105000e+000, -2.9861000e+000, 3.4936000e+000, -2.7289000e+000, -4.1266000e+000,      -2.5900000e+000, 1.3124000e+000, -1.7990000e+000, -1.1890000e+000, -1.0530000e-001, -3.1074000e+000, -3.9641000e+000, -4.3387000e+000, 3.0705000e+000, 3.3205000e+000, -3.8178000e+000, -1.4980000e+000,      -4.3807000e+000, 2.7110000e+000, -3.7956000e+000, -2.3627000e+000, 4.0086000e+000, -1.3728000e+000, -4.4362000e+000, -2.9183000e+000, -2.2457000e+000, 3.0650000e-001, -8.9240000e-001, -3.2364000e+000,      -1.2521000e+000, 2.7198000e+000, -3.9787000e+000, 3.0678000e+000, -4.2400000e+000, -3.9580000e+000, 3.9479000e+000, 2.2030000e-001, -2.6124000e+000, 2.6498000e+000, -2.3256000e+000, -1.5383000e+000,     3.4760000e+000, 2.4462000e+000, 2.4575000e+000, 3.7409000e+000, -2.4887000e+000, 3.8555000e+000, -1.1426000e+000, 1.3389000e+000, 2.2323000e+000, 2.3137000e+000, -4.3370000e+000, 3.9260000e+000,      3.5905000e+000, -1.2858000e+000, -2.0113000e+000, 2.9087000e+000, 3.9278000e+000, 1.0812000e+000, -7.4610000e-001, 3.4740000e+000, 2.3036000e+000, -3.3781000e+000, -4.4910000e-001, 9.4020000e-001,      -3.0583000e+000, -4.2165000e+000, -2.3604000e+000, 8.3640000e-001, 1.4773000e+000, -2.7292000e+000, -1.5904000e+000, 7.6960000e-001, 5.1640000e-001, 2.6576000e+000, -5.4270000e-001, 1.0358000e+000,      6.9260000e-001, -4.2775000e+000, -1.5911000e+000, -3.5775000e+000
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

itsPoint itsF9_ShiftedRastrigin::objectiveFunction(itsPoint point)
{
	vector<double> x = point.getSolution();
	vector<double> z;
	double f = f_bias;
	double D = getDimension();

	// Translation
	for ( unsigned int i = 0; i < D; i++) {
		z.push_back( x[i] - o[i] );
	}

	for ( unsigned int i = 0; i < D; i++) {
		f = f + z[i] * z[i] - 10 * cos(2 * PI * z[i]);
	}
	
	f = f + 10 * D;

	point.setValue(f);
	return point;
}


itsProblem* itsF9_ShiftedRastrigin_Factory::create()
{
    return new itsF9_ShiftedRastrigin();
}


/*******************************************************************************
 * F10: Shifted Rotated Rastrigin’s Function
 ******************************************************************************/

itsF10_ShiftedRotatedRastrigin::itsF10_ShiftedRotatedRastrigin() : itsCEC05_SSRPO_Problem()
{
	// informations 
	setName("CEC'05-SSRPO F10 Shifted Rotated Rastrigin's Function");
	setKey("CEC05_F10");
	setDescription("Multi-modal, Shifted, Rotated, Separable, Scalable, Local optima's number is huge");
	setCitation("TODO");
	setFormula("TODO");

	// default value for all CEC'05 problems
	setAccuracy(1e-8);


	/***** Parameters *****/

	// may be 10, 30, 50
	setDimension(10); 

	// bounds of the search space
	setBoundsMinimaAll(-5);
	setBoundsMaximaAll(5);

	// the bias
	f_bias = -330;

	// the shift vector (which is the optimum)
	double temp_o_t[] = 
	{
	1.9005000e+000, -1.5644000e+000, -9.7880000e-001, -2.2536000e+000, 2.4990000e+000, -3.2853000e+000, 9.7590000e-001, -3.6661000e+000, 9.8500000e-002, -3.2465000e+000, 3.8060000e+000, -2.6834000e+000,      -1.3701000e+000, 4.1821000e+000, 2.4856000e+000, -4.2237000e+000, 3.3653000e+000, 2.1532000e+000, -3.0929000e+000, 4.3105000e+000, -2.9861000e+000, 3.4936000e+000, -2.7289000e+000, -4.1266000e+000,      -2.5900000e+000, 1.3124000e+000, -1.7990000e+000, -1.1890000e+000, -1.0530000e-001, -3.1074000e+000, -3.9641000e+000, -4.3387000e+000, 3.0705000e+000, 3.3205000e+000, -3.8178000e+000, -1.4980000e+000,      -4.3807000e+000, 2.7110000e+000, -3.7956000e+000, -2.3627000e+000, 4.0086000e+000, -1.3728000e+000, -4.4362000e+000, -2.9183000e+000, -2.2457000e+000, 3.0650000e-001, -8.9240000e-001, -3.2364000e+000,      -1.2521000e+000, 2.7198000e+000, -3.9787000e+000, 3.0678000e+000, -4.2400000e+000, -3.9580000e+000, 3.9479000e+000, 2.2030000e-001, -2.6124000e+000, 2.6498000e+000, -2.3256000e+000, -1.5383000e+000,     3.4760000e+000, 2.4462000e+000, 2.4575000e+000, 3.7409000e+000, -2.4887000e+000, 3.8555000e+000, -1.1426000e+000, 1.3389000e+000, 2.2323000e+000, 2.3137000e+000, -4.3370000e+000, 3.9260000e+000,      3.5905000e+000, -1.2858000e+000, -2.0113000e+000, 2.9087000e+000, 3.9278000e+000, 1.0812000e+000, -7.4610000e-001, 3.4740000e+000, 2.3036000e+000, -3.3781000e+000, -4.4910000e-001, 9.4020000e-001,      -3.0583000e+000, -4.2165000e+000, -2.3604000e+000, 8.3640000e-001, 1.4773000e+000, -2.7292000e+000, -1.5904000e+000, 7.6960000e-001, 5.1640000e-001, 2.6576000e+000, -5.4270000e-001, 1.0358000e+000,      6.9260000e-001, -4.2775000e+000, -1.5911000e+000, -3.5775000e+000
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

itsPoint itsF10_ShiftedRotatedRastrigin::objectiveFunction(itsPoint point)
{
	vector<double> x = point.getSolution();
	vector<double> z, zRot;
	double f = f_bias;
	double D = getDimension();

	// Translation
	for ( unsigned int i = 0; i < D; i++) {
		z.push_back( x[i] - o[i] );
	}

	// Rotation
	if ( D == 10 || D == 30 || D == 50 ) {
		// initialize the matrix M
		loadRotation("rastrigin");
		// (x - o)*M
		zRot = multiplyVectorByMatrix( z, M );
	}
	else {
		ostringstream msg;
		msg << "WARNING: dimension (" << D << ") is not a default one ( 10, 30 or 50)." << endl;
		throw Exception_oMetah( msg.str(), EXCEPTION_INFOS );
		zRot = z;
	}

	for ( unsigned int i = 0; i < D; i++) {
		f = f + zRot[i] * zRot[i] - 10 * cos(2 * PI * zRot[i]);
	}
	
	f = f + 10 * D;

	point.setValue(f);
	return point;
	
}


itsProblem* itsF10_ShiftedRotatedRastrigin_Factory::create()
{
    return new itsF10_ShiftedRotatedRastrigin();
}


/*******************************************************************************
 * F11: Shifted Rotated Weierstrass Function
 ******************************************************************************/


itsF11_ShiftedRotatedWeierstrass::itsF11_ShiftedRotatedWeierstrass() : itsCEC05_SSRPO_Problem()
{
	// informations 
	setName("CEC'05-SSRPO F11 Shifted Rotated Weierstrass Function");
	setKey("CEC05_F11");
	setDescription("Multi-modal, Shifted, Rotated, Non-separable, Scalable, Continuous but differentiable only on a set of points");
	setCitation("TODO");
	setFormula("TODO");

	// default value for all CEC'05 problems
	setAccuracy(1e-8);


	/***** Parameters *****/

	// may be 10, 30, 50
	setDimension(10); 

	// bounds of the search space
	setBoundsMinimaAll(-0.5);
	setBoundsMaximaAll(0.5);

	// the bias
	f_bias = 90;

	// the shift vector (which is the optimum)
	double temp_o_t[] = 
	{
	-1.3670000e-001, 1.1860000e-001, -9.6800000e-002, 2.3700000e-002, -2.9330000e-001, -4.7800000e-002, 3.5180000e-001, 3.5790000e-001, -5.8600000e-002, -3.7500000e-002, 2.5300000e-001, 3.1650000e-001,      3.9740000e-001, 3.8130000e-001, 1.7180000e-001, -2.8540000e-001, 8.4900000e-002, -1.8000000e-001, -9.4200000e-002, -1.6840000e-001, -1.9390000e-001, -1.5400000e-002, 1.9800000e-001, -1.2520000e-001,      2.2110000e-001, -5.3600000e-002, -2.0850000e-001, 3.8830000e-001, -2.3050000e-001, 1.9720000e-001, 1.8020000e-001, -2.7420000e-001, 3.2240000e-001, 2.1330000e-001, 1.1600000e-001, -3.0530000e-001,      3.3700000e-001, 2.2320000e-001, -6.1900000e-002, 7.4300000e-002, -1.3370000e-001, -2.0710000e-001, 2.3780000e-001, -1.3490000e-001, -9.0000000e-004, -2.8580000e-001, 1.8920000e-001, 7.7500000e-002,      -3.2990000e-001, -3.8640000e-001, 2.9000000e-002, 1.7970000e-001, -3.5910000e-001, 2.2990000e-001, 2.2270000e-001, -7.4700000e-002, 2.2580000e-001, -2.9100000e-002, -5.5000000e-002, 3.4400000e-001,     2.1900000e-002, -3.5780000e-001, 3.6950000e-001, -3.6000000e-003, 1.4460000e-001, 1.3300000e-002, -3.8030000e-001, 4.8300000e-002, -4.0600000e-002, 1.3140000e-001, -3.7290000e-001, -1.1270000e-001,      -1.5020000e-001, -1.9110000e-001, -3.0040000e-001, -1.8450000e-001, -1.7380000e-001, 7.7100000e-002, -3.1860000e-001, 2.7630000e-001, -1.1970000e-001, -3.6630000e-001, 1.6490000e-001, 2.0500000e-001,      4.3100000e-002, 1.3100000e-002, -1.7090000e-001, 3.4910000e-001, -1.3570000e-001, 1.0720000e-001, 3.9580000e-001, -2.9180000e-001, 1.0650000e-001, -3.6170000e-001, 6.2100000e-002, -1.8300000e-002,      4.4900000e-002, 3.7480000e-001, -3.5780000e-001, -1.2620000e-001
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

itsPoint itsF11_ShiftedRotatedWeierstrass::objectiveFunction(itsPoint point)
{
	vector<double> x = point.getSolution();
	vector<double> z, zRot;
	double f = f_bias;
	double D = getDimension();
	double a = 0.5;
	double b = 3;
	double ak, bk, f1, f2, zz;
	int kmax = 20;

	// Translation
	for ( unsigned int i = 0; i < D; i++) {
		z.push_back( x[i] - o[i] );
	}

	// Rotation
	if ( D == 10 || D == 30 || D == 50 ) {
		// initialize the matrix M
		loadRotation("weierstrass");
		// (x - o)*M
		zRot = multiplyVectorByMatrix( z, M );
	}
	else {
		ostringstream msg;
		msg << "WARNING: dimension (" << D << ") is not a default one ( 10, 30 or 50)." << endl;
		throw Exception_oMetah( msg.str(), EXCEPTION_INFOS );
		zRot = z;
	}

	// Evaluation
	for ( unsigned int i = 0; i < D; i++) {
		zz = zRot[i] + 0.5;
		f1 = 0;
		for ( int k = 0; k <= kmax; k++ ) {
			ak = pow( a, (double) k );
			bk = 2 * PI * pow( b, (double) k );
			f1 = f1 + ak * cos( bk * zz );
		}
		f = f + f1;
	}
	
	f2 = 0;
	
	for ( int k = 0; k <= kmax; k++) {
		ak = pow(a, (double)k);
		bk = 2 * PI * pow(b, (double)k);
		f2 = f2 + ak * cos(bk * 0.5);
	}
	f = f - D * f2;

	point.setValue(f);
	return point;
	
}


itsProblem* itsF11_ShiftedRotatedWeierstrass_Factory::create()
{
    return new itsF11_ShiftedRotatedWeierstrass();
}

/*******************************************************************************
 * F12: Schwefel’s Problem 2.13
 ******************************************************************************/

itsF12_Schwefel::itsF12_Schwefel() : itsCEC05_SSRPO_Problem()
{
	// informations 
	setName("CEC'05-SSRPO F12 Schwefel's Problem 2.13");
	setKey("CEC05_F12");
	setDescription("Multi-modal, Shifted, Non-separable, Scalable.");
	setCitation("TODO");
	setFormula("TODO");
	
	// associated added files 
	// ./problem/CEC05/data/schwefel_213_data_alpha.txt     The optimum
	// ./problem/CEC05/data/schwefel_213_data_a.txt         Aij
	// ./problem/CEC05/data/schwefel_213_data_b.txt		Bij
	
	// default value for all CEC'05 problems
	setAccuracy(1e-8);

	/***** Parameters *****/

	// may be 10, 30, 50
	setDimension(10); 

	// bounds of the search space
	setBoundsMinimaAll(-PI);
	setBoundsMaximaAll(PI);

	// the bias
	f_bias = -460;

	// the shift vector (which is the optimum)
	o = loadVectorFromFile("./problem/CEC05/data/schwefel_213_data_alpha.txt");
	
	// save the optimum (a vector of _points_)
	vector<itsPoint> optim;
	itsPoint pt;
	pt.setSolution( o ); // optimum solution = translation
	pt.setValue( f_bias ); // optimum value = bias

	// only one optimum
	optim.push_back(pt);
	setOptima(optim);
}

itsPoint itsF12_Schwefel::objectiveFunction(itsPoint point)
{
	vector<double> x = point.getSolution();
	double f = f_bias;
	double D = getDimension();

	vector<double> temp_a, temp_b;
	vector< vector<double> > a, b;
	
	// Loadine a , b from corresponding files
	temp_a = loadVectorFromFile("./problem/CEC05/data/schwefel_213_data_a.txt");
	temp_b = loadVectorFromFile("./problem/CEC05/data/schwefel_213_data_a.txt");

	// Translate from Vector to matrix
	a = vectorToSquareMatrix( temp_a );
	b = vectorToSquareMatrix( temp_b );
	
	for (unsigned int i = 0; i < D; i++) 
	{
		for (unsigned int j = 0; j < i; j++) 
		{
			f +=    a[i][j] * sin( o[j] ) +
				b[i][j] * cos( o[j] ) -
				a[i][j] * sin( x[j] ) -
				b[i][j] * sin( x[j] );
		}
	}

	point.setValue(f);

return point;

}


itsProblem* itsF12_Schwefel_Factory::create()
{

return new itsF12_Schwefel();

}

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
