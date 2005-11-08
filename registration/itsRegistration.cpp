/***************************************************************************
 *  $Id: itsRegistration.cpp,v 1.2 2005/11/08 16:45:27 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
 *  for difficult optimization.
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <cmath>
#include <vector>

#include <CImg.h>

#include "itsRegistration.hpp"

#include <cstdlib>
#include <iostream>
#include <ometah/common/string.hpp>

using namespace std;
using namespace cimg_library;

itsRegistration::itsRegistration()
{
	setObjectiveFunction("similarity"); // similarity or mutual
	
    // informations 
    setName("Image registration");
    setKey("Registration");
    setDescription("The aim of the problem is to find the best registration of two image, minimizing a similarity criterion");
    setCitation("Unknown");    
    setFormula("${\\displaystyle \\sum_{i=1}^{n}}\\left(I_{1}\\left(i\\right)-I_{2}\\left(i\\right)\\right)^{2}$");
    
    setDimension(4);
    
    setBoundsMinimaAll(-1);
    setBoundsMaximaAll(1);

    setAccuracy(1);

    // The registration problem has no fixe optimum
    // But we need to fix one for the output
    /*
    vector<itsPoint> optim;
    itsPoint pt;
    vector<double> sol(getDimension(),-1);
    pt.setSolution(sol); // a vector of 0
    vector<double> val(1,-2);
    pt.setValues(val); // a value of 0
    optim.push_back(pt);
    setOptima(optim);
    */
    
    setBoundsCoefficient(0.06);
}


itsPoint itsRegistration::objectiveFunction_similarity(itsPoint point)
{
	
    unsigned int rx = (unsigned int) floor( point.getSolution()[0] );
    unsigned int ry = (unsigned int) floor( point.getSolution()[1] );

    // similarity of the registered images
    double similarity = 0;

    // number of pixels concerned
    //int pixelNumber = ( img1.width - rx ) * ( img1.height - ry );
    
    // loop over X and Y
    cimg_mapXY(img1,x,y) {
        double diff = 0.0;
        
          // if we do not try out of bounds
        if ( !(x-rx<0 || x-rx>img1.width || y-ry<0 || y-ry>img1.height)  ) {
            // square error
            diff =  (img1(x,y) - img2(x-rx,y-ry)) 
                  * (img1(x,y) - img2(x-rx,y-ry)) ;
        }
        
        similarity += diff;
    }

    // the average similarity for a pixel
    //similarity = similarity / (double)pixelNumber;

    vector<double> val(1,similarity);
    point.setValues( val );

	return point;
}

itsPoint itsRegistration::objectiveFunction_mutual(itsPoint point)
{
	return objectiveFunction_similarity(point);
}

itsPoint itsRegistration::objectiveFunction(itsPoint point)
{
    // if we ask for a rotation and a zoom
    if(point.getSolution().size()==4) {
        const float angle = point.getSolution()[2];
        const float zoom = point.getSolution()[3];
    
        img2 = img2_initial.get_rotate( angle, 0.5f*img2_initial.width, 0.5f*img2_initial.height, zoom, 0 );
    }
	
	if (objectiveFunction_choice=="similarity") {
		point = objectiveFunction_similarity(point);
	} else {
		point = objectiveFunction_mutual(point);
	}

    return point;
}


void itsRegistration::resizeImages()
{
    // load images
    img1 = img1.load(this->inputImage_static.c_str() );
    img2 = img2.load( this->inputImage_registered.c_str() );

    if (img1.dimx() == 0 || img1.dimy() == 0 || img2.dimx() == 0 || img2.dimy() ==0 ) {
        ostringstream msg;
        msg << "ErrorImage: images seems to be empty. Sizes: " 
            << this->inputImage_static.c_str() << " (" << img1.dimx() << "," << img1.dimy() << "), "
            << this->inputImage_registered.c_str() << " (" << img2.dimx() << "," << img2.dimy() << ")";
        throw msg.str().c_str();
    }

    // resize if necessary
    int sx = max( img1.dimx(), img2.dimx() );
    int sy = max( img1.dimy(), img2.dimx() );

    if ( img2.dimx() != img1.dimx() || img2.dimx() != img1.dimx() ) {
        img1.resize( sx, sy );
        img2.resize( sx, sy );
    }

    // get the bounds
    vector<double> bmin = getBoundsMinima();
    vector<double> bmax = getBoundsMaxima();

    bmin[0] = -1*sx*boundsCoefficient;
    bmin[1] = -1*sy*boundsCoefficient;

    bmax[0] = sx*boundsCoefficient;
    bmax[1] = sy*boundsCoefficient;

    setBoundsMinima( bmin );
    setBoundsMaxima( bmax );
	
	// store img2
	img2_initial = img2;
}



void itsRegistration::setInputImages(string file_static, string file_registered)
{
    if (file_static=="" || file_registered=="") {
        throw "Error: missing input images";
    }

    this->inputImage_static = file_static;
    this->inputImage_registered = file_registered;

    resizeImages();
}


void itsRegistration::setBoundsCoefficient(float coef)
{
    this->boundsCoefficient = coef;
}


float itsRegistration::getBoundsCoefficient()
{
    return this->boundsCoefficient;
}

void itsRegistration::setObjectiveFunction(string name)
{
	if(name=="similarity" || name=="mutual") {
		objectiveFunction_choice = name;
	} else {
		ostringstream msg;
		msg << "Error: unknown objective function (" << name << ")";
		throw msg.str().c_str();
	}
}

itsProblem * itsRegistrationFactory::create()
{
    return new itsRegistration();
}
