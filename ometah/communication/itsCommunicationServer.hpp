/***************************************************************************
 *  $Id: itsCommunicationServer.hpp,v 1.2 2005/11/04 17:28:12 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
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
 
#ifndef ITSCOMMUNICATIONSERVER
#define ITSCOMMUNICATIONSERVER

#include <vector>
#include "itsCommunication.hpp"
#include "../common/itsPoint.hpp"
#include "../problem/itsProblem.hpp"

using namespace std;


class itsCommunicationServer : public itsCommunication
{
public:
    itsCommunicationServer() {};
    virtual ~itsCommunicationServer() {};

    //! The problem 
    itsProblem* problem;

    //! Call the problem on a point to get its associated values
    itsPoint call(itsPoint point);

    //! The minima of the problem bounds
    vector<double> boundsMinima();

    //! The maxima of the problem bounds
    vector<double> boundsMaxima();

    //! The bounds matrix
    /*!  
        This method call getBoundsMinima and getBoundsMaxima to produce
        a matrix with each dimension on a row.
    */
    vector<vector<double> > bounds();

    //! Return the dimension
    int getDimension();
    
};

class itsCommunicationServerFactory
{
public:
    //virtual ~itsCommunicationServerFactory() {};
    //itsCommunicationServerFactory() {};

    virtual itsCommunicationServer * create() =0;
};

#endif
