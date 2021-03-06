/***************************************************************************
 *  $Id: itsRegistration.hpp,v 1.2 2005/11/08 16:45:27 nojhan Exp $
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
 
#ifndef ITSREGISTRATION
#define ITSREGISTRATION

#include <CImg.h>

#include <ometah/common/itsPoint.hpp>
#include <ometah/problem/itsProblem.hpp>
 
#include<string>
 
using namespace cimg_library;


class itsRegistration : public itsProblem
{
protected:
    //! The file name of the first input image
    string inputImage_static;

    //! The file name of the second input image
    string inputImage_registered;

    //! A coefficient to limit the maximum bounds of x/y registration
    float boundsCoefficient;

    //! Resize images so that their sizes match
    void resizeImages();

	//! Objective function used
	string objectiveFunction_choice;

    //! Temporary image for img2 to be rotated
    CImg<unsigned char> img2_initial;

protected:
	//! Compute the similarity
    itsPoint objectiveFunction_similarity(itsPoint point);

	//! Compute the mutual information
    itsPoint objectiveFunction_mutual(itsPoint point);


public:

    //! The first image
    CImg<unsigned char> img1;

    //! The second image
    CImg<unsigned char> img2;

  
	//! Constructor
    itsRegistration();

	//! The objective function
    itsPoint objectiveFunction(itsPoint point);

    //! Choose the two images
    void setInputImages(string file_static, string file_registered);

    //! Change the bounds coefficient
    void setBoundsCoefficient(float coef);

    //! Return the bounds coefficient
    float getBoundsCoefficient();
	
	//! Choose the objective function to use
	void setObjectiveFunction(string name);
};

class itsRegistrationFactory : public itsProblemFactory
{
public:
    itsProblem * create();
};

#endif
