// SocketException class


#ifndef EXCEPTION_SOCKET
#define EXCEPTION_SOCKET

#include <string>

using namespace std;

class Exception_Socket
{
private:
  //! Description of the exception
  string msg;
 
public:
  Exception_Socket ( string msg ) {this->msg=msg;};
  ~Exception_Socket () {};

  string description() { return msg; }
};

#endif
