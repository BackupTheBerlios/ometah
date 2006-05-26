/***************************************************************************
 *  $Id: itsSetItem.cpp,v 1.2 2006/05/26 09:57:20 nojhan Exp $
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

#include "itsSetItem.hpp"

using namespace std;

namespace ometah {

void itsSetItem::setKey( string key )
{
    this->key = key;
}


void itsSetItem::setName(string name)
{
    this->name = name;
}


void itsSetItem::setAccronym(string accronym)
{
    this->accronym = accronym;
}

void itsSetItem::setFamily(string family)
{
    this->family = family;
}

void itsSetItem::setDescription(string description)
{
    this->description = description;
}
  

void itsSetItem::setCitation(string citation)
{
    this->citation = citation;
}



string itsSetItem::getInformations()
{
    // a stream is easier to manipulate
    stringstream infos;

    // Separator used between key and value
    string sep = ":\t";

    infos << "\t" << "Key" << sep << getKey() << endl;
    infos << "\t" << "Family" << sep << getFamily() << endl;
    infos << "\t" << "Name" << sep << getName() << endl;
    infos << "\t" << "Accronym" << sep << getAccronym() << endl;
    infos << "\t" << "Description" << sep << getDescription() << endl;
    infos << "\t" << "Reference" << sep << getCitation() << endl;

    // get back the string only
    return infos.str();
}


string itsSetItem::getInformations_XML()
{
    // a stream is easier to manipulate
    stringstream infos;

    infos << "<item>" << endl;

    infos << "<key>" << getKey() << "</key>" << endl;
    infos << "<family>" << getFamily() << "</family>" << endl;
    infos << "<name>" << getName() << "</name>" << endl;
    infos << "<accronym>" << getAccronym() << "</accronym>" << endl;
    infos << "<description>" << getDescription() << "</description>" << endl;
    infos << "<reference>" << getCitation() << "</reference>" << endl;

    infos << "</item>" << endl;

    // get back the string only
    return infos.str();
}

} // ometah
