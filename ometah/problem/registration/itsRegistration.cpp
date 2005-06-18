/***************************************************************************
 *  $Id: itsRegistration.cpp,v 1.2 2005/06/18 06:29:18 nojhan Exp $
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
    img1.load( this->inputImage_static.c_str() );
    img2.load( this->inputImage_registered.c_str() );

    // resize if necessary
    int sx = cimg::max( img1.width, img2.width );
    int sy = cimg::max( img1.height, img2.height );

    if ( img2.width != img1.width || img2.height != img1.height ) {
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
