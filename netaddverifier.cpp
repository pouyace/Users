#include "netaddverifier.h"
#include <QDebug>
#include <QMetaEnum>
#include <QHostAddress>
#include "shareduser.h"
#include <QMessageBox>
#include <lmshare.h>
#include "netconnectionenumerator.h"
#include <QSysInfo>
void cpyByteToWChar(wchar_t *dst, const QString &str)
{
    str.toWCharArray(dst);
    wchar_t *valPtr = dst;
    valPtr += str.length();
    *valPtr = 0;
}

wchar_t *getWChar(const QString &str){
    wchar_t *retval = (wchar_t *)malloc((str.length()+1)*sizeof(wchar_t));
    int mLen = str.toWCharArray(retval);
    *(retval + mLen) = 0;
    return retval;
}

NetAddVerifier::NetAddVerifier(const QHostAddress &remoteAddress, const QString &remoteGroup, QObject *parent)
    :AbstractUserVerifier(parent)
    ,_remoteAddress(remoteAddress.toString())
    ,_remoteGroup(remoteGroup)
    ,_remoteName("\\\\" + _remoteAddress + "\\" + _remoteGroup)
{

}

AbstractUser* NetAddVerifier::verifyUser(const QString &username, const QString &password)
{
    if(!(username.length() && password.length())){
        setError(Error::InvlaidInput);
        return nullptr;
    }
    if(signedInUser){
        unregisterUser(signedInUser);
        signedInUser->deleteLater();
    }

    NetResource nr(_remoteAddress,_remoteGroup,username,password);
    return wNetAdd(nr);
}

NetAddVerifier::~NetAddVerifier()
{
    if(signedInUser){
        delete signedInUser;
    }
}

void NetAddVerifier::wNetCancel()
{
    qDebug()<<"in WnetCancel";
    if(!signedInUser) return;
    DWORD dwRetValCancel;
    wchar_t *wrn = getWChar(signedInUser->path().path());
    dwRetValCancel =  WNetCancelConnection2W(wrn,CONNECT_UPDATE_PROFILE,true);
    if (dwRetValCancel == NO_ERROR){
    }else{
        qDebug() << "ConnectionCanceled Error:" << dwRetValCancel;
    }
    free(wrn);
//    signedInUser = nullptr;
}

SharedUser *NetAddVerifier::wNetAdd(NetResource &netResource)
{
    qDebug() << "in Addnet connection";
    DWORD dwRetVal = ERROR_ALREADY_ASSIGNED;
    DWORD dwFlags = CONNECT_UPDATE_PROFILE;

    clearNetPath();
    int tryTimes = 0;
    while(tryTimes<1){
        NETRESOURCEW * tempNetRec = netResource.getWinNetResource();
        const wchar_t * tempWPassword = netResource.wPassword();
        const wchar_t * tempUserName = netResource.wUsername();
        QString compName = QSysInfo::machineHostName();
        NetConnectionEnumerator b("10.10.10.10", "\\\\"+compName);
        dwRetVal = WNetAddConnection2W(tempNetRec, tempWPassword, tempUserName, dwFlags);
        if (dwRetVal == NO_ERROR){
            this->setError(Error::NoError);
            break;
        }
        else if (dwRetVal == ERROR_BAD_DEV_TYPE){
            this->setError(Error::RemoteNameError);
            break;
        }
        else if (dwRetVal == ERROR_SESSION_CREDENTIAL_CONFLICT){
            this->setError(Error::MultipleUsersNotAllowed);
            break;
        }
        else if(dwRetVal==ERROR_NOT_CONNECTED){
            this->setError(Error::AlreadyAssignedError);
            wNetCancel();
        }
        else if(dwRetVal==ERROR_LOGON_FAILURE){
            this->setError(Error::BadNameOrPassword);
            break;
        }
        else if(dwRetVal == ERROR_ALREADY_ASSIGNED){
            this->setError(Error::AlreadyAssignedError);
            QMessageBox::warning(nullptr,"sfsd","sgdfg");
            clearNetPath();
        }
        else if(dwRetVal == ERROR_INVALID_PASSWORD){
            this->setError(Error::WrongPasswordError);
            break;
        }
        else{
            qDebug()<<dwRetVal;
            this->setError(Error::ServerConnectionError);
            break;
        }
        tryTimes++;
    }
    if(dwRetVal) return nullptr;

    SharedUser *retVal = new SharedUser(netResource.userName,this);
    QMetaEnum metaEnum = QMetaEnum::fromType<AccessLevel>();
    retVal->setAccessLevel(AccessLevel(metaEnum.keyToValue("User")));
    retVal->setPath(QDir(netResource.localName));
    this->signedInUser = retVal;
    qDebug() << "user was recognised" << netResource.userName;
    qDebug()<<"-----------------------------------";
    qDebug()<<"username:"<<netResource.userName;
    qDebug()<<"localname:"<<netResource.localName;
    qDebug()<<"remotename:"<<netResource.remoteName;
    qDebug()<<"pass:"<<netResource.password;
    qDebug()<<"-----------------------------------";
    return retVal;
}

QString getAFreeDriveLetter()
{
    for(char i='Z'; i>'C'; i--){
        char dLet[3] = {i , ':' , '\0'};
        DWORD dVal = GetDriveTypeA(dLet);
        if(dVal == DRIVE_NO_ROOT_DIR){
            return QString::fromLatin1(dLet);
        }
    }
    return QString();
}

bool NetAddVerifier::registerUser(const QString &userName, const QString &passWord, const QString &firstName, const QString &LastName, AbstractUserVerifier::AccessLevel accesslevel, const bool &suspended)
{
    return 0;
}

void NetAddVerifier::unregisterUser(AbstractUser *user)
{
    qDebug()<<"unregisterUser in netAddVerifier";
    wNetCancel();
}

void NetAddVerifier::clearNetPath()
{
    QDir dir;
    char myI = 'C';

    DWORD remoteNameBufferLength = 2048;
    REMOTE_NAME_INFOW *rNameInfo = (REMOTE_NAME_INFOW*)malloc(remoteNameBufferLength);
    do{
        myI++;
        QString path;
        path.append(QChar(myI));
        path.append(":");
        dir.setPath(path);

        wchar_t *localPath_w = getWChar(path);

        if(dir.exists()){
            DWORD universalNameRetVal = 0;

            memset(rNameInfo,0,remoteNameBufferLength);
            universalNameRetVal = WNetGetUniversalNameW(localPath_w,REMOTE_NAME_INFO_LEVEL,(LPVOID) rNameInfo,&remoteNameBufferLength);
            while(universalNameRetVal == ERROR_MORE_DATA){
                try{
                    free(rNameInfo);
                    remoteNameBufferLength *= 2;
                    rNameInfo = (REMOTE_NAME_INFOW*)malloc(remoteNameBufferLength);
                    memset(rNameInfo,0,remoteNameBufferLength);
                    universalNameRetVal = WNetGetUniversalNameW(localPath_w,REMOTE_NAME_INFO_LEVEL,(LPVOID) rNameInfo,&remoteNameBufferLength);
                }catch(const std::bad_alloc &baException){
                    qDebug() << Q_FUNC_INFO << "Error in memory allocation at line" << __LINE__ << "bufferSize:" << remoteNameBufferLength;
                    remoteNameBufferLength /= 2;
                }catch(...){
                    qDebug() << Q_FUNC_INFO << "Unknown error at" << __LINE__ << "bufferSize:" << remoteNameBufferLength;
                }
            }

            QString universalNameReturned = QString::fromWCharArray(rNameInfo->lpUniversalName);
            switch(universalNameRetVal){
            case NO_ERROR:
            case ERROR_NOT_CONNECTED:
            case ERROR_NO_NETWORK:
                if(universalNameReturned == _remoteName && universalNameReturned.length()){
                    DWORD updateProfileStrategy = CONNECT_UPDATE_PROFILE;
                    DWORD dwCancaelRetVal = WNetCancelConnection2W(localPath_w,updateProfileStrategy,true);
                    qDebug() << "Cancel retval:" << dwCancaelRetVal;
                    if(dwCancaelRetVal){
                        updateProfileStrategy = 0;
                        dwCancaelRetVal = WNetCancelConnection2W(localPath_w,updateProfileStrategy,true);
                        qDebug() << "trying again - Cancel retval:" << dwCancaelRetVal;
                    }
                }
                break;
            default:
                qDebug() << Q_FUNC_INFO << "Unhandled Error for universal name" << universalNameRetVal;
            }
        }
        free(localPath_w);
    }while(myI < 'Z');
    free(rNameInfo);
}

NetAddVerifier::NetResource::NetResource(const QString &remoteAddress, const QString &remoteGroup, const QString &username, const QString &password)
    :localName(getAFreeDriveLetter())
    ,remoteName("\\\\" + remoteAddress + "\\" + remoteGroup)
    ,userName(username)
    ,password(password)
{
    _wLocalName  = getWChar(this->localName);
    _wRemoteName = getWChar(this->remoteName);
    _wUserName   = getWChar(remoteAddress + "\\" + username);
    _wPassword   = getWChar(this->password);

    memset(&winNetResource, 0, sizeof (NETRESOURCEW));

    winNetResource.dwType = RESOURCETYPE_ANY;
    winNetResource.lpLocalName = _wLocalName;
    winNetResource.lpRemoteName = _wRemoteName;
    winNetResource.lpProvider = NULL;
}

NetAddVerifier::NetResource::~NetResource()
{
    free(_wLocalName     );
    free(_wRemoteName    );
    free(_wUserName      );
    free(_wPassword      );
}

NETRESOURCEW *NetAddVerifier::NetResource::getWinNetResource()
{
    return &winNetResource;
}

const wchar_t *NetAddVerifier::NetResource::wUsername() const
{
    return _wUserName;
}

const wchar_t *NetAddVerifier::NetResource::wPassword() const
{
    return _wPassword;
}
