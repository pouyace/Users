#ifndef ABSTRACTUSER_H
#define ABSTRACTUSER_H

#include <QObject>
#include <QDir>
class AbstractUserVerifier;

class AbstractUser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString firstName READ firstName WRITE setfName NOTIFY firstnameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setlName NOTIFY lastnameChanged)

public:
    enum AccessLevel{Unknown,Guest,Admin};
    Q_ENUM(AccessLevel)
    explicit AbstractUser(QString userName,AbstractUserVerifier *parent);
    virtual ~AbstractUser();

private:
    QString _userName;
    QDir    _sharedDir;
    QString _firstName;
    AbstractUserVerifier* _parentVerifier;
    QString _lastName;
        QDir    _path;
    int _userId;
protected:
    AccessLevel _accessLevel ;

public: /*Methods*/
    virtual QString prettyName() const;
    void setAccessLevel(enum AccessLevel accessLevel);
    void setSharedDir(const QDir &sharedDir);
    void setfName(const QString &fName);
    void setPath(const QDir &sharedDir);
    void setlName(const QString &lName);
    void setUserId(const QString &userid);
    QString userName() const;
    QDir    path() const;

    AccessLevel accessLevel() const;
    QDir    sharedDir() const;
    QString firstName() const;
    QString lastName() const;
    int userId();

signals:
    void firstnameChanged(const QString&);
    void lastnameChanged(const QString&);


};

#endif // ABSTRACTUSER_H
