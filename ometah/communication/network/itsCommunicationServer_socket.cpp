/***************************************************************************
 *  $Id: itsCommunicationServer_socket.cpp,v 1.2 2006/05/13 10:05:54 nojhan Exp $
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

#include "itsCommunicationServer_socket.hpp"

namespace ometah {

itsPoint itsCommunicationServer_socket::call(itsPoint point)
{
    return this->problem->objectiveFunction(point);
}

vector<double> itsCommunicationServer_socket::boundsMinima()
{
    return this->problem->getBoundsMinima();
}

vector<double> itsCommunicationServer_socket::boundsMaxima()
{
    return this->problem->getBoundsMaxima();
}

vector<vector<double> > itsCommunicationServer_socket::bounds()
{
    return this->problem->getBounds();
}

int itsCommunicationServer_socket::getDimension()
{
    return this->problem->getDimension();
}



itsCommunicationServer_socket::itsCommunicationServer_socket()
{
    this->setKey("Socket");
    this->setPort(OMSP_PORT_DEFAULT);
}


void itsCommunicationServer_socket::setPort( int portNumber )
{
    this->port = portNumber;
}


string itsCommunicationServer_socket::handleRequest(string data)
{
    // split string on spaces
    vector<string> items = stringSplit( data, OMSP_SPLIT_COMMAND );

    // the command is the first item
    string command = items[0];

    //clog << "  Parsed command: <" << command << ">" << endl;

    if( command == OMSP_CALL ) {
        itsPoint point;
        point.setSolution( stringToDouble( stringSplit( items[1], OMSP_SPLIT_LEVEL1 ) ) );
    
        if( point.getSolution().size() != this->problem->getDimension() ) {
          return "ERROR dimension does not fit";
        }
    
        point = call( point );
      
        vector<itsPoint> ptvec;
        ptvec.push_back( point );
        return printValues( ptvec );
    
    } else if ( command == OMSP_BOUNDSMINIMA) {
        return print( boundsMinima() );
    
    } else if ( command == OMSP_BOUNDSMAXIMA) {
        return print( boundsMaxima() );
    
    } else if ( command == OMSP_BOUNDS) {
        return print( bounds(), OMSP_SPLIT_LEVEL1, OMSP_SPLIT_LEVEL2 );
    
    } else if ( command == OMSP_DIMENSION) {
        ostringstream response;
        response << getDimension();
        return response.str();
    
    } else if ( command == OMSP_PING) {
        string response = OMSP_PONG;
        return response;
    
    } else {
        string response = "ERROR unknown command";
        return response;
    }
}


void itsCommunicationServer_socket::start()
{
    itsSocketServer server( this->port );

    while(1) {
        
        clog << "Waiting for a connexion..." << endl;
    
        // new socket
        itsSocketServer new_socket;
        server.accept( new_socket );
        
        // get a string from the socket
        string data;
        new_socket >> data;
    
        // strip the terminating \n (usefull if you use telnet)
        //data.erase( data.length()-2,2 );

        clog << " Receiving: <" << data << ">" << endl;
    
        // parse the received string and do asked actions
        string response = handleRequest( data );
    
        // send back the response to the client
        new_socket << response;
    
        clog << " Sending: <" << response << ">" << endl;

    }
}


itsCommunicationServer* itsCommunicationServerFactory_socket::create()
{
    return new itsCommunicationServer_socket;
}

} // namespace ometah
