/***************************************************************************
 * $Id: itsCEC05_SSRPO_Problems.hpp,v 1.7 2006/06/18 18:29:42 walid_tfaili Exp $
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
 
 #ifndef ITSCEC05SSRPOPROBLEMS
 #define ITSCEC05SSRPOPROBLEMS
  
#include <vector>
#include <string>
#include "../itsProblem.hpp"
 
using namespace std;

namespace ometah {

class itsCEC05_SSRPO_Problem : public itsProblem 
{
public:
    itsCEC05_SSRPO_Problem();

    virtual itsPoint objectiveFunction(itsPoint point) =0;

protected:
    //! Parse the file containing the rotation matrix
    vector<double> loadVectorFromFile(string fileName);
    
    //! Translate a vector two a square matrix
    vector< vector<double> > vectorToSquareMatrix( vector<double> V  );

    //! Parse and load the rotation data file in the rotation matrix
    void loadRotation( string problemName );

    //! Apply the translation
    void translation();

protected:
    //! The number of the problem in the original benchmark
    int ssrpoID;

    //! The current point being computed
    itsPoint currentPoint;

    //! The bias on the optimum value
    double f_bias;

    //! The optimum solution
    /*!
        In CEC'05-SSRPO, it is used to shift the function
    */
    vector<double> o;

    //! The rotation matrix
    /*! 
        Initialized with the data files.
    */
    vector< vector<double> > M;

    
/*
    //! The transformation vector for the optimum
    vector<double> optimumShift;

    //! The transformation matrix for global rotation
    vector< vector<double> > rotationMatrix;

    //! Compute the shifted optimum
    void doShiftOptimum();

    //! Compute the rotation
    void doRotate();

    //! Compute the noise
    void noise();

    //! Add the bias
    void addBias();
*/
};


class itsF1_ShiftedSphere : public itsCEC05_SSRPO_Problem
{
public:
    itsF1_ShiftedSphere();
    itsPoint objectiveFunction(itsPoint point);
};

class itsF1_ShiftedSphere_Factory : public itsProblemFactory
{
public:
    itsProblem* create();
};





class itsF2_ShiftedSchwefel : public itsCEC05_SSRPO_Problem
{
public:
    itsF2_ShiftedSchwefel();
    itsPoint objectiveFunction(itsPoint point);
};

class itsF2_ShiftedSchwefel_Factory : public itsProblemFactory
{
public:
    itsProblem* create();
};




class itsF3_ShiftedRotatedElliptic : public itsCEC05_SSRPO_Problem
{
public:
    itsF3_ShiftedRotatedElliptic();
    itsPoint objectiveFunction(itsPoint point);
};

class itsF3_ShiftedRotatedElliptic_Factory : public itsProblemFactory
{
public:
    itsProblem* create();
};





class itsF4_ShiftedSchwefelWithNoise : public itsCEC05_SSRPO_Problem
{
public:
    itsF4_ShiftedSchwefelWithNoise();
    itsPoint objectiveFunction(itsPoint point);
};

class itsF4_ShiftedSchwefelWithNoise_Factory : public itsProblemFactory
{
public:
    itsProblem* create();
};





class itsF5_SchwefelBounds : public itsCEC05_SSRPO_Problem
{
public:
    itsF5_SchwefelBounds();
    itsPoint objectiveFunction(itsPoint point);
};

class itsF5_SchwefelBounds_Factory : public itsProblemFactory
{
public:
    itsProblem* create();
};





class itsF6_ShiftedRosenbrock : public itsCEC05_SSRPO_Problem
{
public:
    itsF6_ShiftedRosenbrock();
    itsPoint objectiveFunction(itsPoint point);
};

class itsF6_ShiftedRosenbrock_Factory : public itsProblemFactory
{
public:
    itsProblem* create();
};





class itsF7_ShiftedRotatedGriewank : public itsCEC05_SSRPO_Problem
{
public:
    itsF7_ShiftedRotatedGriewank();
    itsPoint objectiveFunction(itsPoint point);
};

class itsF7_ShiftedRotatedGriewank_Factory : public itsProblemFactory
{
public:
    itsProblem* create();
};




class itsF8_RotatedAckleyWithBounds : public itsCEC05_SSRPO_Problem
{
public:
    itsF8_RotatedAckleyWithBounds();
    itsPoint objectiveFunction(itsPoint point);
};

class itsF8_RotatedAckleyWithBounds_Factory : public itsProblemFactory
{
public:
    itsProblem* create();
};




class itsF9_ShiftedRastrigin : public itsCEC05_SSRPO_Problem
{
public:
    itsF9_ShiftedRastrigin();
    itsPoint objectiveFunction(itsPoint point);
};

class itsF9_ShiftedRastrigin_Factory : public itsProblemFactory
{
public:
    itsProblem* create();
};




class itsF10_ShiftedRotatedRastrigin : public itsCEC05_SSRPO_Problem
{
public:
    itsF10_ShiftedRotatedRastrigin();
    itsPoint objectiveFunction(itsPoint point);
};

class itsF10_ShiftedRotatedRastrigin_Factory : public itsProblemFactory
{
public:
    itsProblem* create();
};





class itsF11_ShiftedRotatedWeierstrass : public itsCEC05_SSRPO_Problem
{
public:
    itsF11_ShiftedRotatedWeierstrass();
    itsPoint objectiveFunction(itsPoint point);
};

class itsF11_ShiftedRotatedWeierstrass_Factory : public itsProblemFactory
{
public:
    itsProblem* create();
};






class itsF12_Schwefel : public itsCEC05_SSRPO_Problem
{
public:
    itsF12_Schwefel();
    itsPoint objectiveFunction(itsPoint point);
};

class itsF12_Schwefel_Factory : public itsProblemFactory
{
public:
    itsProblem* create();
};


}//ometah

#endif
