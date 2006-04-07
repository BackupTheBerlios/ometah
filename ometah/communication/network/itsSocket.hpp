/***************************************************************************
 *  $Id: itsSocket.hpp,v 1.1 2006/04/07 16:55:57 nojhan Exp $
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
 

#ifndef ITSSOCKET
#define ITSSOCKET


#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "SocketProtocol.hpp"

using namespace std;
/*
const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECEIVE = 500;
*/
class itsSocket
{
private:
  int mem_sock;
  sockaddr_in mem_address;
 
public:
  itsSocket();
  virtual ~itsSocket();

  //! Server initialization
  //!@{
  bool create();
  bool bind( const int port );
  bool listen() const;
  bool accept( itsSocket& ) const;
  //@}

  //! Client initialization
  bool connect( const string host, const int port );

  //! Data Transimission
  //!@{
  bool send( const string s ) const;
  int receive( string& s ) const;
  //@}

  //! Switch to non blocking mode
  void setNonBlocking( const bool blockit );

  //! Test if the socket is valid
  bool isValid() const { return mem_sock != -1; };

};

#endif
