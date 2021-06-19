#ifndef ADMINUSER_H
#define ADMINUSER_H
#include "abstractuser.h"

class AdminUser:public AbstractUser
{
public:
     AdminUser(QString userName,AbstractUserVerifier *parent);

};

#endif // ADMINUSER_H
