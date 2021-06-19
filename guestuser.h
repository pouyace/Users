#ifndef GUESTUSER_H
#define GUESTUSER_H
#include "abstractuser.h"
#include "abstractuserverifier.h"

class GuestUser : public AbstractUser
{
public:
    GuestUser(QString userName, AbstractUserVerifier* parent);
};

#endif // GUESTUSER_H
