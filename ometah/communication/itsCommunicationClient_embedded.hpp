/***************************************************************************
 *  $Id: itsCommunicationClient_embedded.hpp,v 1.1 2005/06/13 09:05:03 nojhan Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
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
 
 #ifndef ITSCOMMUNICATIONCLIENTEMBEDDED
 #define ITSCOMMUNICATIONCLIENTEMBEDDED

#include <vector>
#include "../common/itsPoint.hpp"
#include "itsCommunicationClient.hpp"

using namespace std;

class itsCommunicationClient_embedded : public itsCommunicationClient
{
public:
    itsCommunicationClient_embedded();


    itsPoint call(itsPoint point);
    vector<double> boundsMinima();
    vector<double> boundsMaxima();
    vector<vector<double> > bounds();
    int getDimension();

};

class itsCommunicationClientFactory_embedded : public itsCommunicationClientFactory
{
public:
    itsCommunicationClient* create();
};

#endif
