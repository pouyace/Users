#include "adminuser.h"

AdminUser::AdminUser(QString userName,AbstractUserVerifier*parent)
    :AbstractUser(userName,parent)
{
    _accessLevel = Admin;
}

