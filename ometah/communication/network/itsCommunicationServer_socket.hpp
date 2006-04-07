/***************************************************************************
 *  $Id: itsCommunicationServer_socket.hpp,v 1.1 2006/04/07 16:55:57 nojhan Exp $
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
 
 
 #ifndef ITSCOMMUNICATIONSERVERSOCKET
 #define ITSCOMMUNICATIONSERVERSOCKET

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
//#include "../itsCommunicationClient_socket.hpp"
#include "../../common/itsPoint.hpp"
#include "../../common/string.hpp"
#include "../itsCommunicationServer.hpp"

#include "itsSocketServer.hpp"
#include "Exception_Socket.hpp"
#include "SocketProtocol.hpp"

using namespace std;

class itsCommunicationServer_socket : public itsCommunicationServer
{
private:
    //! the port to which the server is listening
    int port;

public:
    itsCommunicationServer_socket();


    itsPoint call(itsPoint point);
    vector<double> boundsMinima();
    vector<double> boundsMaxima();
    vector<vector<double> > bounds();
    int getDimension();

    //! Set the port
    void setPort(int portNumber);

    //! Parse the string received from a client and handle the commands
    /*!
      The command should be of the form :
        COMMAND arguments
      Available commands :
        CALL x1,x2,x3,...
        BOUNDSMINIMA
        BOUNDSMAXIMA
        BOUNDS
        DIMENSION
    */
    string handleRequest( string data );

    //! Start the server
    void start();
};

class itsCommunicationServerFactory_socket : public itsCommunicationServerFactory
{
public:
    itsCommunicationServer* create();
};

#endif
