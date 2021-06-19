#include "guestuser.h"


GuestUser::GuestUser(QString userName, AbstractUserVerifier *parent):
    AbstractUser(userName,parent)
{
    _accessLevel = Guest;
}
