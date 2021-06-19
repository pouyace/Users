#ifndef NETADDVERIFIER_H
#define NETADDVERIFIER_H
#pragma comment(lib, "mpr.lib")
#include "abstractuserverifier.h"
#include <QString>
#include <intsafe.h>
#include <minwindef.h>
#include <windows.h>
#include "../Users/abstractuser.h"
#include <winnetwk.h>
#include <tchar.h>

class QHostAddress;
class SharedUser;
class NetAddVerifier:public AbstractUserVerifier
{
    Q_OBJECT
public:
    NetAddVerifier(const QHostAddress &remoteAddress, const QString &remoteGroup, QObject *parent=nullptr);
    virtual AbstractUser * verifyUser(const QString &username, const QString &password) override;
    virtual ~NetAddVerifier();
    virtual bool registerUser(const QString &userName, const QString &passWord, const QString &firstName, const QString &LastName
                              , AccessLevel accesslevel, const bool &suspended = false) override;
    virtual void unregisterUser(AbstractUser *user) override;
    void clearNetPath();

private:
    struct NetResource{
        const QString localName   ;
        const QString remoteName  ;
        const QString userName    ;
        const QString password    ;
        NetResource(const QString &remoteAddress
                    ,const QString &remoteGroup
                    ,const QString &username
                    ,const QString &password);
        ~NetResource();
        NETRESOURCEW *getWinNetResource();

        const wchar_t *wUsername() const;
        const wchar_t *wPassword() const;
    private:
        wchar_t *_wLocalName    = nullptr;
        wchar_t *_wRemoteName   = nullptr;
        wchar_t *_wUserName     = nullptr;
        wchar_t *_wPassword     = nullptr;
        NETRESOURCEW winNetResource;
    };
    QString _remoteAddress        = "192.168.1.100";
    QString _remoteGroup        ;
    QString _remoteName         ;
    SharedUser *signedInUser = nullptr;

private: //Methods
    void wNetCancel();
    SharedUser *wNetAdd(NetResource &netResource);
};

#endif // NETADDVERIFIER_H
