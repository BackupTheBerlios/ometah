/***************************************************************************
 *  $Id: itsCommunicationClient_socket.hpp,v 1.3 2006/05/25 08:51:51 nojhan Exp $
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

 #ifndef ITSCOMMUNICATIONCLIENTSOCKET
 #define ITSCOMMUNICATIONCLIENTSOCKET

#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include "../../common/string.hpp"
#include "../../common/itsPoint.hpp"
#include "../itsCommunicationClient.hpp"
#include "itsSocketClient.hpp"
#include "Exception_Socket.hpp"
#include "SocketProtocol.hpp"

using namespace std;

namespace ometah {

class itsCommunicationClient_socket : public itsCommunicationClient
{
private:
    //! The socket port used by the server
    int port;

    //! The server IP adress
    string hostIP;

    //! Flag for memorization of previous server answers
    /*!
        True : the client will not ask twice for informations to the server 
               (this permit to save network activity)
        False : the client will ask as many times as asked by the metaheuristic
                (usefull for a dynamix problem)
        Default is set to True
    */
    bool isServerMemory;
                
    //! Memory for the dimension
    int mem_dimension;
                
    //! Memory for the bounds
    vector<vector<double> > mem_bounds;
    
    //! Memory for the minima bounds 
    vector<double> mem_boundsMinima;
    
    //! Memory for the maxima bounds
    vector<double> mem_boundsMaxima;
    

    //! Send a command to the server
    string sendRequest( string cmd );

public:
    itsCommunicationClient_socket();


    itsPoint call(itsPoint point);
    vector<double> boundsMinima();
    vector<double> boundsMaxima();
    vector<vector<double> > bounds();
    unsigned int getDimension();

    void setPort( int port );
    void setHostIP( string ip );
};

class itsCommunicationClientFactory_socket : public itsCommunicationClientFactory
{
public:
    itsCommunicationClient* create();
};

}//ometah

#endif
