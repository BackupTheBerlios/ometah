/***************************************************************************
 *  $Id: itsCommunicationServer_embedded.cpp,v 1.5 2006/05/25 08:51:51 nojhan Exp $
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

#include "itsCommunicationServer_embedded.hpp"

namespace ometah {

itsCommunicationServer_embedded::itsCommunicationServer_embedded()
{
    setKey("Embedded");
    setName("Embedded on pointers");
    setAccronym("None");
    setFamily("Direct");
    setDescription("Only used as a compatibility layer, this server do nothing.");
    setCitation("None");
}


void itsCommunicationServer_embedded::start()
{
}


itsCommunicationServer* itsCommunicationServerFactory_embedded::create()
{
    return new itsCommunicationServer_embedded;
}

}//ometah
