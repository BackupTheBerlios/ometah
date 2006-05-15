/***************************************************************************
 *  $Id: communications.h,v 1.1 2006/05/15 21:23:34 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
 *  for difficult optimization.
 *  
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
 
#ifndef OMETAH_COMMUNICATIONS
#define OMETAH_COMMUNICATIONS
 
#include "communication/itsCommunicationClient.hpp"
#include "communication/itsCommunicationServer.hpp"

#include "communication/itsCommunicationServer_embedded.hpp"
#include "communication/itsCommunicationClient_embedded.hpp"

#ifdef WITH_SOCKET
#include "communication/network/itsCommunicationClient_socket.hpp"
#include "communication/network/itsCommunicationServer_socket.hpp"
#endif

#ifdef WITH_PYTHON
#include "communication/languages/itsCommunicationClient_python.hpp"
#endif

#endif
