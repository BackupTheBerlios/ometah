/***************************************************************************
 *  $Id: itsCommunicationServer_embedded.hpp,v 1.4 2006/05/13 10:05:54 nojhan Exp $
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
 
 #ifndef ITSCOMMUNICATIONSERVEREMBEDDED
 #define ITSCOMMUNICATIONSERVEREMBEDDED
 
 #include "itsCommunicationServer.hpp"
 
namespace ometah {
 
class itsCommunicationServer_embedded : public itsCommunicationServer
{
public:
    //! Constructor
    itsCommunicationServer_embedded();
    //~itsCommunicationServer_embedded() {};
   
    void start();
};

class itsCommunicationServerFactory_embedded : public itsCommunicationServerFactory
{
public:
    //itsCommunicationServerFactory_embedded() {};    
    //~itsCommunicationServerFactory_embedded() {};
    
    itsCommunicationServer* create();
};

} //ometah

#endif
