/***************************************************************************
 *  $Id: Exception_oMetah.cpp,v 1.3 2006/05/13 10:05:53 nojhan Exp $
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
 
 
 #include "Exception_oMetah.hpp"

namespace ometah {

string Exception_oMetah::what()
{
    ostringstream msg;
    msg << description << " (<" << name << "> error in " << file << " : " << function << " : " << line << ")";

    /*msg << description << " (<" << name << "> error in function <" << function 
        << ">, in file <" << file << ">, at line " << line << ")." << endl;*/

    return msg.str();
}

} // namespace ometah
