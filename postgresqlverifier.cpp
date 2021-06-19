#include "postgresqlverifier.h"
#include <abstractuser.h>
#include <QHostAddress>
#include <QFile>
#include <QCryptographicHash>
#include "adminuser.h"
#include "guestuser.h"
#include <QtSql/QSqlError>
#include <QMetaEnum>
PostgreSqlVerifier::PostgreSqlVerifier(QString filePath)
{
    if(filePath.length()){
        errFile = new QFile(filePath);

    }
    db = QSqlDatabase::addDatabase("QPSQL");
    if(errFile){
        errFile->open(QIODevice::WriteOnly);
        errFile->write("Driver Loaded :" + db.databaseName().toLatin1());
    }
}

AbstractUser *PostgreSqlVerifier::verifyUser(const QString &username, const QString &password)
{
    if(!(username.length() && password.length())){
        setError(Error::InvlaidInput);
        return nullptr;
    }
    QCryptographicHash hash(QCryptographicHash::Algorithm::Sha1);
    hash.addData(password.toStdString().c_str(),password.size());
    QString passWord = QString(hash.result().toHex());
    QString query = "SELECT id,firstname,lastname,accesslevel FROM userverifier WHERE username = '"  + username +"' AND password = '" + passWord+"'";
    //qDebug()<<"query="<<query;
    int stat =this->execute(query);
    if(!stat){
        setError(Error::DatabaseError);
        return nullptr;
    }
    QMetaEnum metaEnum = QMetaEnum::fromType<AccessLevel>();
    AbstractUser* user = nullptr;
    switch (AccessLevel(metaEnum.keyToValue(getValue("accesslevel").toString().toStdString().data()))) {
        case AbstractUser::AccessLevel::Admin: user = new AdminUser(username,this);setError(Error::NoError);break;
        case AbstractUser::AccessLevel::Guest: user = new GuestUser(username,this);setError(Error::NoError);break;
        case AbstractUser::AccessLevel::Unknown:  user = nullptr;setError(Error::UnknownAccessLevel); return user;
    }
    user->setfName(getValue("firstname").toString());
    user->setlName(getValue("lastname").toString());
    user->setUserId(getValue("id").toString());
    user->setAccessLevel(AccessLevel(metaEnum.keyToValue(getValue("accesslevel").toString().toStdString().data())));
    return user;
}

bool PostgreSqlVerifier::setUpConfig(const QHostAddress &ip, const qint16 &port, const QString &userName, const QString &passWord
                                     , const QString &databaseName)
{
    QString dbName = databaseName;
    db.setPort(port);
    db.setHostName(ip.toString());
    db.setPassword(passWord);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setConnectOptions("connect_timeout=10");
    if(db.open()){
        if(errFile){
            errFile->open(QFile::Append);
            errFile->write("PostgreSQL Opened");
            errFile->close();
            return true;
        }
        else
            return true;
    }
    else{
        db.setDatabaseName("");
        if (db.open()){
            db.exec("CREATE DATABASE "+dbName);
            this->checkErrorDebug();
            if(db.lastError().type() == QSqlError::NoError){
                db.close();
                db.setDatabaseName(dbName);
                if(db.open()){
                    return true;
                }
                else{
                    setError(Error::DataBaseNotOpen);
                    this->checkErrorDebug();
                    return false;
                }
            }
            else{
                this->checkErrorDebug();
                return false;
            }
        }
        else{
              setError(Error::DataBaseNotOpen);
              this->checkErrorDebug();
                }
                return false;
    }
}

QVariant PostgreSqlVerifier::getValue(QString field){
    QVariant value = result.value(field);
    return value;
}

QVariant PostgreSqlVerifier::getValue(int position){
    QVariant value = result.value(position);
    return value;
}


bool PostgreSqlVerifier::nextRecord(){
    return result.next();
}

bool PostgreSqlVerifier::registerUser(const QString &userName, const QString &passWord, const QString &firstName
                                      , const QString &LastName, AbstractUserVerifier::AccessLevel accesslevel, const bool &suspended)
{
    if(!(userName.length() && passWord.length())){
        setError(Error::InvlaidInput);
        return false;
    }
    QCryptographicHash hash(QCryptographicHash::Algorithm::Sha1);
    hash.addData(passWord.toStdString().c_str(),passWord.size());
    QString password = QString(hash.result().toHex());
    QMetaEnum metaEnum = QMetaEnum::fromType<AccessLevel>();
    QString query = "INSERT INTO userverifier (username,password,firstname,lastname,accesslevel,suspended) VALUES ('"
    + userName + "','" + password+"','"+firstName+"','"+LastName+"','"+ metaEnum.valueToKey(accesslevel) +"','"+QString::number(suspended)+"');";
    //qDebug()<<query;
    this->execute(query);
    if(!db.lastError().text().length()){
        this->setError(Error::NoError);
        return true;
    }
    else{
        this->setError(Error::DatabaseError);
        return false;
    }
}

int PostgreSqlVerifier::execute(QString query){
    result = db.exec(query);
    this->checkErrorDebug();
    int size = result.size();
    if(size == -1)
        size = result.numRowsAffected();
    result.first();
    return size;
}

void PostgreSqlVerifier::unregisterUser(AbstractUser *user)
{

}

PostgreSqlVerifier::~PostgreSqlVerifier()
{

}

void PostgreSqlVerifier::checkErrorDebug()
{
    if(db.lastError().type() != QSqlError::NoError){
        qDebug() << Q_FUNC_INFO << db.lastError();
        if(errFile){
        errFile->open(QFile::Append);
        errFile->write("\n"+db.lastError().nativeErrorCode().toLatin1()+ "::\t" +db.lastError().text().toLatin1());
        errFile->close();
        }
    }
}













