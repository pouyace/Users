
#ifndef FILEUSERDATA_H
#define FILEUSERDATA_H
#include <QtGlobal>
#include "abstractuser.h"
#include <QString>
#include <QDataStream>

struct FileUserData{
QString username;
QString password;
AbstractUser::AccessLevel accessLevel;

friend QDataStream & operator <<(QDataStream &out,const FileUserData &myObj){
    out<<myObj.username;
    out<<myObj.password;
    out<<myObj.accessLevel;
    return out;
}
friend QDataStream & operator >>(QDataStream &in, FileUserData &myObj){
    in >> myObj.username;
    in >> myObj.password;
    in >> myObj.accessLevel;
    return in;
}
};
Q_DECLARE_METATYPE(FileUserData)
#endif // FILEUSERDATA_H
