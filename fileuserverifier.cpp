#include "fileuserverifier.h"
#include <QDebug>
FileUserVerifier::FileUserVerifier(const QString& startPath)
{
    usersDir.setPath(startPath);
}

FileUserVerifier::~FileUserVerifier()
{

}

AbstractUser *FileUserVerifier::verifyUser(const QString &userName, const QString &password)
{

    if(!usersDir.exists(userName + ".txt")){
        this->setError(Error::UserNotFoundError);
        return nullptr;
    }
    QFile userFileReader("userDir/"+userName+".txt");
    userFileReader.open(QIODevice::ReadOnly);
    if(!userFileReader.isOpen()){
        this->setError(Error::UnknownError);
        return nullptr;
    }
    QDataStream in(&userFileReader);
    FileUserData newUser;
    in >> newUser;
    userFileReader.close();
    if(newUser.username==userName && newUser.password==password){
        this->setError(Error::NoError);
        switch (newUser.accessLevel) {
        case AbstractUser::AccessLevel::Admin: return new AdminUser(userName,this);
        case AbstractUser::AccessLevel::Guest: return new GuestUser(userName,this);
        case AbstractUser::AccessLevel::Unknown:this->setError(Error::UnknownError); return nullptr;
        }

    }
    this->setError(Error::WrongPasswordError);
    return nullptr;
}
