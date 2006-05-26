#ifndef ITSSETITEM
#define ITSSETITEM

#include <string>
#include <sstream>
#include <map>

#include <iostream>

using namespace std;

namespace ometah {

class itsSetItem {
public:
  itsSetItem() :
        name("Unknown"), key("Unknown"), accronym("Unknown"), family("Unknown"), description("Unknown"), citation("Unknown")
  {
  }

  //! Simple attribute accesses
  //!@{

  //! Return the name
  string getName() const {return name;}
  //! Set the name
  void setName(string name);

  //! Return the key
  string getKey() const {return key;}
  //! Change the key
  void setKey( string key );
  
  //! Return the accronym
  string getAccronym() const {return accronym;}
  //! Change the Accronym
  void setAccronym(string accronym);

  //! Return the description
  string getDescription() const {return description;}
  //! Change the description
  void setDescription(string description);
  
  //! Return the reference
  string getCitation() const {return citation;}
  //! Change the reference
  void setCitation(string citation);

  //! Return the family
  string getFamily() const {return family;}
  //! Change the reference
  void setFamily(string family);
  
  //! Return a formated output of all the informations
  /*!
    Return the Key, Family, Name, Accronym, Description and Reference 
    (in this order) of the item, if given.
    The format is the following :
        Key:\tValue\n
  */
  string getInformations();

  //! Return a formated output of all the informations
  /*!
    Return the Key, Family, Name, Accronym, Description and Reference 
    (in this order) of the item, if given. In xml format.
  */
  string getInformations_XML();
  
  //@}

protected:
  //! The name of the item
  /*! 
    This must be a unique name in the source tree
  */
  string name;

  //! The unique key identifying the item among other
  string key;

  //! The acronym of the item
  string accronym;

  //! The family of the item
  string family;

  //! A brief description 
  string description;

  //! The paper reference (if available)
  /*!
    The reference should be formated in BiBTeX.
  */
  string citation;

};

}//ometah

#endif
