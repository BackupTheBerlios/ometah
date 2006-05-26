
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
