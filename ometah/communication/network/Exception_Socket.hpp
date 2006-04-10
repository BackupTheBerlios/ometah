/***************************************************************************
 *  $Id: Exception_Socket.hpp,v 1.2 2006/04/10 20:56:52 nojhan Exp $
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

#ifndef EXCEPTION_SOCKET
#define EXCEPTION_SOCKET

#include "../../common/Exception_oMetah.hpp"

//! Declaration of the main exception
EXCEPTION_DECLARATION( Exception_Socket, Exception_oMetah );

//! Could not create a socket
EXCEPTION_DECLARATION( Exception_Socket_Creation, Exception_Socket );

//! Could not bind to a port
EXCEPTION_DECLARATION( Exception_Socket_Binding, Exception_Socket );

//! Could not read/write to a socket
EXCEPTION_DECLARATION( Exception_Socket_IO, Exception_Socket );

#endif
