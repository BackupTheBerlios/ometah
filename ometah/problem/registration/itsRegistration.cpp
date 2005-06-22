/***************************************************************************
 *  $Id: itsRegistration.cpp,v 1.4 2005/06/22 14:08:18 nojhan Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
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
#include "../../common/string.hpp"

using namespace std;
using namespace cimg_library;

itsRegistration::itsRegistration()
{
    // informations 
    setName("Image registration");
    setKey("Registration");
    setDescription("The aim of the problem is to find the best registration of two image, minimizing a similarity criterion");
    setCitation("Unknown");    
    setFormula("${\\displaystyle \\sum_{i=1}^{n}}\\left(I_{1}\\left(i\\right)-I_{2}\\left(i\\right)\\right)^{2}$");
    
    setDimension(2);
    
    setBoundsMinimaAll(-1);
    setBoundsMaximaAll(1);

    setAccuracy(1);

    // The registration problem has no fixe optimum
    // But we need to fix one for the output
    vector<itsPoint> optim;
    itsPoint pt;
    vector<double> sol(getDimension(),0);
    pt.setSolution(sol); // a vector of 0
    vector<double> val(1,0);
    pt.setValues(val); // a value of 0
    optim.push_back(pt);
    setOptima(optim);
}

itsPoint itsRegistration::objectiveFunction(itsPoint point)
{
    unsigned int rx = (unsigned int) floor( point.getSolution()[0] );
    unsigned int ry = (unsigned int) floor( point.getSolution()[1] );

    // similarity of the registered images
    double similarity = 0;
    
    // loop over X and Y
    cimg_mapXY(img1,x,y) {
        double diff = 0.0;
        
          // if we do not try out of bounds
        if ( !(x+rx<0 || x+rx>img1.width || y+ry<0 || y+ry>img1.height)  ) {
            // square error
            diff =  (img1(x,y) - img2(x+rx,y+ry)) 
                  * (img1(x,y) - img2(x+rx,y+ry)) ;
        }
        
        similarity += diff;
    }

    vector<double> val(1,similarity);
    point.setValues( val );

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
    vector<double> bmin, bmax;

    bmin.push_back( -1*sx );
    bmin.push_back( -1*sy );

    bmax.push_back( sx );
    bmax.push_back( sy );

    setBoundsMinima( bmin );
    setBoundsMaxima( bmax );
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



itsProblem * itsRegistrationFactory::create()
{
    return new itsRegistration();
}
