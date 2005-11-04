/***************************************************************************
 * $Id: itsCEC05_SSRPO_Problems.hpp,v 1.2 2005/11/04 17:28:19 nojhan Exp $
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
 
 #ifndef ITSCEC05SSRPOPROBLEMS
 #define ITSCEC05SSRPOPROBLEMS
  
#include <vector>
#include <string>
#include "../itsProblem.hpp"
 
 using namespace std;
 
class itsCEC05_SSRPO_Problem : public itsProblem 
{
protected:
    //! The number of the problem in the original benchmark
    int ssrpoID;

    //! The current point being computed
    itsPoint currentPoint;

    //! The transformation vector for the optimum
    vector<double> optimumShift;

    //! The transformation matrix for global rotation
    vector< vector<double> > rotationMatrix;

    //! The bias on the optimum value
    double f_bias;


protected:    
    //! Compute the shifted optimum
    void doShiftOptimum();

    //! Compute the rotation
    void doRotate();

    //! Compute the noise
    void noise();

    //! Add the bias
    void addBias();


public:
    itsCEC05_SSRPO_Problem();

    virtual itsPoint objectiveFunction(itsPoint point) =0;

};

#endif
