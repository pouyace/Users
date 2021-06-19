#ifndef SHAREDUSER_H
#define SHAREDUSER_H
#include "abstractuser.h"
class SharedUser : public AbstractUser
{
    Q_OBJECT
public:
    explicit SharedUser(QString, AbstractUserVerifier *parent);

signals:

};

#endif // SHAREDUSER_H
