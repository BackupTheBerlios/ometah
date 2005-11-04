/***************************************************************************
 *  $Id: itsCommunicationClient.hpp,v 1.4 2005/11/04 21:03:00 nojhan Exp $
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
 
#ifndef ITSCOMMUNICATIONCLIENT
#define ITSCOMMUNICATIONCLIENT

#include <vector>
#include <string>
#include <hash_map.h>
#include "../communication/itsCommunication.hpp"
#include "../communication/itsCommunicationServer.hpp"
#include "../common/logic.hpp"

class itsCommunicationClient : public itsCommunication
{
public:
    //! The problem directly embedded, if necessary
    itsCommunicationServer* problem;

    virtual ~itsCommunicationClient() {};

public:
    
    virtual void initialization(hash_map<string,string, eqstr> args) {};

    //! Call the problem on a point to get its associated values
    virtual itsPoint call(itsPoint point) =0;

    //! The minima of the problem boundsitsCommunication
    virtual vector<double> boundsMinima() =0;

    //! The maxima of the problem bounds
    virtual vector<double> boundsMaxima() =0;

    //! The bounds matrix
    /*!  
        This method call getBoundsMinima and getBoundsMaxima to produce
        a matrix with each dimension on a row.
    */
    virtual vector<vector<double> > bounds() =0;

    //! Return the dimension
    virtual int getDimension() =0;

};

class itsCommunicationClientFactory
{
public:
    virtual ~itsCommunicationClientFactory() {};
    //itsCommunicationClientFactory() {};
    
    virtual itsCommunicationClient * create() =0;
};

#endif
