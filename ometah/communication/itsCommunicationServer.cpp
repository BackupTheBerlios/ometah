/***************************************************************************
 *  $Id: itsCommunicationServer.cpp,v 1.3 2006/05/13 10:05:54 nojhan Exp $
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
 

#include "itsCommunicationServer.hpp"

namespace ometah {

itsPoint itsCommunicationServer::call(itsPoint point)
{
    return this->problem->objectiveFunction(point);
}

vector<double> itsCommunicationServer::boundsMinima()
{
    return this->problem->getBoundsMinima();
}

vector<double> itsCommunicationServer::boundsMaxima()
{
    return this->problem->getBoundsMaxima();
}

vector<vector<double> > itsCommunicationServer::bounds()
{
    return this->problem->getBounds();
}

int itsCommunicationServer::getDimension()
{
    return this->problem->getDimension();
}

}//ometah
