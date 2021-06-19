#include "abstractuser.h"
#include "abstractuserverifier.h"

AbstractUser::AbstractUser(QString userName, AbstractUserVerifier *parent)
    : QObject(parent)
    ,_userName(userName)
    ,_parentVerifier(parent)
{}

AbstractUser::~AbstractUser()
{
    _parentVerifier->unregisterUser(this);
}

QString AbstractUser::prettyName() const
{
    QString ptName;
    if(_firstName.length()){
        ptName = _firstName;
        if(_lastName.length()){
            ptName += " " + _lastName;
        }
    }else if(_lastName.length()){
        ptName = _lastName;
    }else{
        ptName = _userName;
    }
    return ptName;
}

QString AbstractUser::firstName() const
{
    return this->_firstName;
}

QString AbstractUser::lastName() const
{
    return this->_lastName;
}

int AbstractUser::userId()
{
    return _userId;
}
void AbstractUser::setPath(const QDir &sharedDir)
{
    this->_path = sharedDir;
}

QDir AbstractUser::path() const
{
    return this->_path;
}


void AbstractUser::setAccessLevel(enum AccessLevel accessLevel)
{
    this->_accessLevel = accessLevel;
}

void AbstractUser::setSharedDir(const QDir &sharedDir)
{
    this->_sharedDir = sharedDir;
}

void AbstractUser::setfName(const QString &fName)
{
    this->_firstName = fName;
    emit firstnameChanged(fName);
}

void AbstractUser::setlName(const QString &lName)
{
    this->_lastName = lName;
    emit lastnameChanged(lName);
}

void AbstractUser::setUserId(const QString &userid)
{
    _userId = userid.toInt();
}

QString AbstractUser::userName() const
{
    return this->_userName;
}

AbstractUser::AccessLevel AbstractUser::accessLevel() const
{
    return this->_accessLevel;
}

QDir AbstractUser::sharedDir() const
{
    return this->_sharedDir;
}

