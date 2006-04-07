/***************************************************************************
 *  $Id: SocketProtocol.hpp,v 1.1 2006/04/07 16:55:57 nojhan Exp $
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
 
#ifndef SOCKETPROTOCOL
#define SOCKETPROTOCOL

// Command strings
#define OMSP_CALL "CALL"
#define OMSP_BOUNDSMINIMA "BOUNDSMINIMA"
#define OMSP_BOUNDSMAXIMA "BOUNDSMAXIMA"
#define OMSP_BOUNDS "BOUNDS"
#define OMSP_DIMENSION "DIMENSION"
#define OMSP_PING "PING"
#define OMSP_PONG "PONG"

// Parsing options
#define OMSP_SPLIT_COMMAND " "
#define OMSP_SPLIT_LEVEL1 ","
#define OMSP_SPLIT_LEVEL2 ";"
 
// Default parameters
#define OMSP_IP_DEFAULT "localhost"
#define OMSP_PORT_DEFAULT 80277
 
// Socket options
#define CONNECTIONS_MAX 5
#define RECEIVE_MAX 1000

#endif
