/***************************************************************************
 *  $Id: itsCommunicationClient_socket.cpp,v 1.4 2006/05/25 08:51:51 nojhan Exp $
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

#include "itsCommunicationClient_socket.hpp"

namespace ometah {

itsPoint itsCommunicationClient_socket::call(itsPoint point)
{
    ostringstream cmd;
    cmd << OMSP_CALL << OMSP_SPLIT_COMMAND << print( point.getSolution() );
  
    string response = sendRequest( cmd.str() );

    point.setValues( stringToDouble( stringSplit( response, OMSP_SPLIT_LEVEL1 ) ) );

    return point;
}


vector<double> itsCommunicationClient_socket::boundsMinima()
{
    // if we are in memorization mode
    // and if the bounds has already been asked
    if( this->isServerMemory && this->mem_boundsMinima.size() != 0 ) {
        return this->mem_boundsMinima;
    
    // if we are not in memorization mode_t
    // or if the bounds has not been asked
    } else {
        ostringstream cmd;
        cmd << OMSP_BOUNDSMINIMA;
      
        string response = sendRequest( cmd.str() );
    
        this->mem_boundsMinima = stringToDouble( stringSplit( response, OMSP_SPLIT_LEVEL1 ) );
        return this->mem_boundsMinima;
    }
}


vector<double> itsCommunicationClient_socket::boundsMaxima()
{
    // if we are in memorization mode
    // and if the bounds has already been asked
    if( this->isServerMemory && this->mem_boundsMaxima.size() != 0 ) {
        return this->mem_boundsMaxima;
    
    // if we are not in memorization mode_t
    // or if the bounds has not been asked
    } else {
        ostringstream cmd;
        cmd << OMSP_BOUNDSMAXIMA;
      
        string response = sendRequest( cmd.str() );
    
        this->mem_boundsMaxima = stringToDouble( stringSplit( response, OMSP_SPLIT_LEVEL1 ) );
        return this->mem_boundsMaxima;
    }
}


vector<vector<double> > itsCommunicationClient_socket::bounds()
{
    // if we are in memorization mode
    // and if the bounds has already been asked
    if( this->isServerMemory && this->mem_bounds.size() != 0 ) {
        return this->mem_bounds;
    
    // if we are not in memorization mode_t
    // or if the bounds has not been asked
    } else {
        ostringstream cmd;
        cmd << OMSP_BOUNDS;
      
        string response = sendRequest( cmd.str() );
        vector<string> veclines = stringSplit( response, OMSP_SPLIT_LEVEL2 );
        
        vector<vector<double> > result;
        for(unsigned int i=0; i<veclines.size(); i++) {
            result.push_back( stringToDouble( stringSplit( veclines[i], OMSP_SPLIT_LEVEL1 ) ) );
        }
        
        this->mem_bounds = result;
        return this->mem_bounds;
  }
}


unsigned int itsCommunicationClient_socket::getDimension()
{
    // if we are in memorization mode
    // and if the dimension has already been asked
    if( this->isServerMemory && this->mem_dimension != -1 ) {
        return this->mem_dimension;
    
    // if we are not in memorization mode_t
    // or if the dimension has not been asked
    } else {
        ostringstream cmd;
        cmd << OMSP_DIMENSION;
      
        string response = sendRequest( cmd.str() );
    
        this->mem_dimension = atoi( response.c_str() );
        return this->mem_dimension;
    }
}

string itsCommunicationClient_socket::sendRequest( string cmd )
{
    //try {

        itsSocketClient client_socket( hostIP, port );

        clog << "Sending:" << cmd << endl;
    
        client_socket << cmd;
    
        string response;
        client_socket >> response;

        clog << "Receiving:" << response << endl;
    
        if ( stringSplit(response, OMSP_SPLIT_COMMAND)[0] == "ERROR" ) {
            throw Exception_Socket ( response, EXCEPTION_INFOS );
        }
    
        return response;
    
    /*} catch ( Exception_Socket& e ) {
        cerr << "Error: " << e.what() << endl;
    }*/
}


void itsCommunicationClient_socket::setPort( int port )
{
    this->port = port;
}


void itsCommunicationClient_socket::setHostIP( string ip )
{
    // FIXME : check the ip validity
    this->hostIP = ip;
}


itsCommunicationClient_socket::itsCommunicationClient_socket()
{
    // parameters
    this->setKey("Socket");
    this->setHostIP(OMSP_IP_DEFAULT);
    this->setPort(OMSP_PORT_DEFAULT);

    // flags & memory initialization
    this->isServerMemory = true;
    this->mem_dimension = -1;
}


itsCommunicationClient* itsCommunicationClientFactory_socket::create()
{
    return new itsCommunicationClient_socket;
}

}//ometah
