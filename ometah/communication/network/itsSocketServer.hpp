/***************************************************************************
 *  $Id: itsSocketServer.hpp,v 1.2 2006/05/13 10:05:54 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
 *  for difficult optimization.
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Publi License as published by
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
 
#ifndef ITSSOCKETSERVER
#define ITSSOCKETSERVER

#include <string>

#include "itsSocket.hpp"
#include "Exception_Socket.hpp"

using namespace std;

class itsSocketServer : public itsSocket
{
public:

    itsSocketServer ( int port );
    itsSocketServer (){};
    virtual ~itsSocketServer();

    //! Stream operator for sending to the socket
    const itsSocketServer& operator << ( const string& ) const;
    
    //! Stream operator for receiving from the socket
    const itsSocketServer& operator >> ( string& ) const;

    //! Accept form the socket
    void accept ( itsSocketServer& );
};

#endif
