/***************************************************************************
 *  $Id: itsSocketClient.cpp,v 1.2 2006/04/10 20:56:52 nojhan Exp $
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
 
#include "itsSocketClient.hpp"

using namespace std;

itsSocketClient::itsSocketClient ( string host, int port )
{
    if ( ! itsSocket::create() ) {
        throw Exception_Socket_Creation ( "Could not create client socket", EXCEPTION_INFOS );
    }

    if ( ! itsSocket::connect ( host, port ) ) {
        throw Exception_Socket_Binding ( "Could not bind to port", EXCEPTION_INFOS );
    }
}


const itsSocketClient& itsSocketClient::operator << ( const string& s ) const
{
    if ( ! itsSocket::send ( s ) ) {
        throw Exception_Socket_IO ( "Could not write to socket", EXCEPTION_INFOS );
    }

    return *this;
}


const itsSocketClient& itsSocketClient::operator >> ( string& s ) const
{
    if ( ! itsSocket::receive ( s ) ) {
        throw Exception_Socket_IO ( "Could not read from socket", EXCEPTION_INFOS );
    }
    
    return *this;
}
