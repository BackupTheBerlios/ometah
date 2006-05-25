/***************************************************************************
 *  $Id: itsCommunicationClient_embedded.cpp,v 1.5 2006/05/25 08:51:51 nojhan Exp $
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

#include <iostream>

#include "itsCommunicationServer_embedded.hpp"
#include "itsCommunicationClient_embedded.hpp"

using namespace std;

namespace ometah {

itsPoint itsCommunicationClient_embedded::call(itsPoint point)
{
    return this->problem->call(point);
}


vector<double> itsCommunicationClient_embedded::boundsMinima()
{
    return this->problem->boundsMinima();
}


vector<double> itsCommunicationClient_embedded::boundsMaxima()
{
    return this->problem->boundsMaxima();
}


vector<vector<double> > itsCommunicationClient_embedded::bounds()
{
    return this->problem->bounds();
}


unsigned int itsCommunicationClient_embedded::getDimension()
{
    return this->problem->getDimension();
}


itsCommunicationClient_embedded::itsCommunicationClient_embedded()
{
    setKey("Embedded");
    setName("Embedded on pointers");
    setAccronym("None");
    setFamily("Direct");
    setDescription("This communication client embbed the communication server in the same binary.");
    setCitation("None");

    problem = new itsCommunicationServer_embedded;
}


itsCommunicationClient_embedded::~itsCommunicationClient_embedded()
{
    delete problem;
}




itsCommunicationClient* itsCommunicationClientFactory_embedded::create()
{
    return new itsCommunicationClient_embedded;
}

}//ometah
