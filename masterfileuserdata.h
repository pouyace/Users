#ifndef MASTERFILEUSERDATA_H
#define MASTERFILEUSERDATA_H
#include <QString>
#include <QDataStream>
#include "abstractuser.h"

struct MasterFileUserData{
QString username;
QByteArray password;
QByteArray machinId;
QString pcName;
QString firstName;
QString lastName;
AbstractUser::AccessLevel accessLevel;

friend QDataStream & operator <<(QDataStream &out,const MasterFileUserData &myObj){
    out << myObj.username;
    out << myObj.password;
    out << myObj.machinId;
    out << myObj.pcName;
    out << myObj.firstName;
    out << myObj.lastName;
    out << myObj.accessLevel;
    return out;
}
friend QDataStream & operator >>(QDataStream &in, MasterFileUserData &myObj){
    in >> myObj.username;
    in >> myObj.password;
    in >> myObj.machinId;
    in >> myObj.pcName;
    in >> myObj.firstName;
    in >> myObj.lastName;
    in >> myObj.accessLevel;
    return in;
}
};
Q_DECLARE_METATYPE(MasterFileUserData)

#endif // MASTERFILEUSERDATA_H
