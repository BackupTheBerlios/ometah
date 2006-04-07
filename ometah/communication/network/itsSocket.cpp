/***************************************************************************
 *  $Id: itsSocket.cpp,v 1.1 2006/04/07 16:55:57 nojhan Exp $
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
 
#include "itsSocket.hpp"

using namespace std;

itsSocket::itsSocket() : mem_sock ( -1 )
{
    // get the address
    memset( &mem_address, 0, sizeof( mem_address ) );
}

itsSocket::~itsSocket()
{
  // close the socket if necessary
  if( isValid() ) {
      close( mem_sock );
  }
}

bool itsSocket::create()
{
    // create the socket
    mem_sock = socket( AF_INET, SOCK_STREAM, 0 );

    // if no socket created
    if( ! isValid() ) {
        return false;
    }


    // if created, try to change its options
    int opt_on = 1;
    if( setsockopt( mem_sock, 
                    SOL_SOCKET, 
                    SO_REUSEADDR, 
                    (const char*) &opt_on, 
                    sizeof( opt_on ) 
                  ) == -1 
      ) {
        return false;
    }

    return true;
}



bool itsSocket::bind ( const int port )
{
    if( ! isValid() ) {
        return false;
    }

    mem_address.sin_family = AF_INET;
    mem_address.sin_addr.s_addr = INADDR_ANY;
    mem_address.sin_port = htons( port );

    // binding
    int isBinded = ::bind( mem_sock, 
                           (struct sockaddr *) &mem_address,
                           sizeof ( mem_address )
                         );

    if( isBinded == -1 ) {
        return false;
    }

    return true;
}


bool itsSocket::listen() const
{
    if( ! isValid() ) {
        return false;
    }

    // listening
    int isListened = ::listen( mem_sock, CONNECTIONS_MAX );


    if( isListened == -1 ) {
        return false;
    }

    return true;
}


bool itsSocket::accept ( itsSocket& new_socket ) const
{
    int address_length = sizeof( mem_address );

    // accept
    new_socket.mem_sock = ::accept( mem_sock, 
                                    (sockaddr *) &mem_address, 
                                    (socklen_t *) &address_length
                                  );

    // check the memory
    if( new_socket.mem_sock <= 0 ) {
        return false;
    } else {
        return true;
    }
}


bool itsSocket::connect ( const string host, const int port )
{
    if( ! isValid() ) {
      return false;
    }

    // port & family
    mem_address.sin_port = htons( port );
    mem_address.sin_family = AF_INET;

    int status = inet_pton( AF_INET, host.c_str(), &mem_address.sin_addr );

    if( errno == EAFNOSUPPORT ) {
      return false;
    }

    // connection
    status = ::connect ( mem_sock, (sockaddr *) &mem_address, sizeof (mem_address) );

    if ( status == 0 ) {
      return true;
    } else {
      return false;
    }
}


bool itsSocket::send ( const string s ) const
{
    // send
    int status = ::send( mem_sock, s.c_str(), s.size(), MSG_NOSIGNAL );

    if ( status == -1 )  {
        return false;
    } else {
        return true;
    }
}


int itsSocket::receive ( string& s ) const
{
    // erasing the string in arguments
    s = "";

    // buffer for reception
    char buffer[ RECEIVE_MAX + 1 ];
    memset( buffer, 0, RECEIVE_MAX + 1 );

    // receiving
    int status = ::recv( mem_sock, buffer, RECEIVE_MAX, 0 );

    if( status == -1 ) {
        clog << "Could not receive, error number: " << errno << "  in itsSocket::receive" << endl;
        return 0;
    
    } else if( status == 0 ) {
        return 0;
      
    } else {
        s = buffer;
        return status;
    }
}



void itsSocket::setNonBlocking ( const bool blockit)
{
    int options;

    options = fcntl( mem_sock, F_GETFL );

    if( options < 0 ) {
        return;
    }

    if( blockit ) {
        options = ( options | O_NONBLOCK );
    } else {
        options = ( options & ~O_NONBLOCK );
    }

    fcntl( mem_sock, F_SETFL, options );
}
