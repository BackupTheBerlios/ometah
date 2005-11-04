/***************************************************************************
 * $Id: itsCEC05_SSRPO_Problems.cpp,v 1.2 2005/11/04 17:28:19 nojhan Exp $
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

itsCEC05_SSRPO_Problem::itsCEC05_SSRPO_Problem()
{
    this->ssrpoID = 0;
    this->f_bias = 0.0;
}


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


void itsCEC05_SSRPO_Problem::noise()
{
    vector<double> val = currentPoint.getValues();

    vector<double> means(val.size(), 0.0);
    vector<double> stdev(val.size(), 1.0);

    vector<double> noise = randomNormal(means, stdev) ;

    currentPoint.setValues( addition( val, noise) );
}

void itsCEC05_SSRPO_Problem::addBias()
{
    vector<double> val = currentPoint.getValues();

    for( unsigned int i=0; i<val.size(); i++ ) {
        val[i] += this->f_bias;
    }
    
    currentPoint.setValues( val );
}
