#include "sharedspacedconnection.h"

SharedSpacedConnection::SharedSpacedConnection(QObject *parent)
{

}

SharedSpacedConnection::~SharedSpacedConnection()
{
    signOut();
}

void SharedSpacedConnection::cpyByteToWChar(wchar_t *dst, const QString &str)
{
    str.toWCharArray(dst);
    wchar_t *valPtr = dst;
    valPtr += str.length();
    *valPtr = 0;
}

void SharedSpacedConnection::setSigninDataUserUser(QString userName, QString passWord, QString remoteName)
{
    _remoteName = remoteName;
    _netResource.remoteName = "\\\\" + _hostAddress + "\\" + _remoteName;

    _userName = userName;
    _netResource.userName = _hostAddress + "\\" + _userName;

    _password = passWord;
    _netResource.password = passWord;

}

void SharedSpacedConnection::setSignUpDataUseraUser(QString userName, QString passWord, QString rePassword)
{

}

bool SharedSpacedConnection::signIn()
{
    bool retVal;
    retVal =  wNetAdd();
    return retVal;
}

void SharedSpacedConnection::signOut()
{
    wNetCancel();
}

bool SharedSpacedConnection::signUp()
{

}

bool SharedSpacedConnection::checkedAdminData(QString userName, QString passWord)
{

}

void SharedSpacedConnection::setHostAddress(QString hostAddress)
{
    this->_hostAddress = hostAddress;
}

QString SharedSpacedConnection::retLocalName()
{
    return _localName;
}

QString SharedSpacedConnection::retRemoteName()
{
    return  _remoteName;
}

QString SharedSpacedConnection::retUserName()
{
    return _userName;
}

QString SharedSpacedConnection::retPassword()
{
    return _password;
}

QString SharedSpacedConnection::hostAddress()
{
    return _hostAddress;
}

void SharedSpacedConnection::wNetCancel()
{
    DWORD dwRetValCancel;
    wchar_t wrn[512];
    cpyByteToWChar(wrn,_netResource.localName);
    wchar_t * arguments[5];
    arguments[0] = wrn;

    dwRetValCancel =  WNetCancelConnection2(arguments[0],0,true);
    if (dwRetValCancel == NO_ERROR)
        qDebug() << "Connection Canceled.";
    else
        qDebug() << "ConnectionCanceled Error:" << dwRetValCancel;
}

bool SharedSpacedConnection::wNetAdd()
{
    DWORD dwRetVal;
    NETRESOURCE nr;
    DWORD dwFlags;

    memset(&nr, 0, sizeof (NETRESOURCE));

    wchar_t wln[512];
    wchar_t wrn[512];
    wchar_t wun[512];
    wchar_t wpw[512];

    cpyByteToWChar(wln,_netResource.localName);
    cpyByteToWChar(wrn,_netResource.remoteName);
    cpyByteToWChar(wun,_netResource.userName);
    cpyByteToWChar(wpw,_netResource.password);

    nr.dwType = RESOURCETYPE_ANY;
    nr.lpLocalName = wln;
    nr.lpRemoteName = wrn;
    nr.lpProvider = NULL;

    wchar_t * arguments[5];
    arguments[0] = wpw;
    arguments[1] = wun;
    arguments[2] = wln;
    arguments[3] = wrn;

    dwFlags = CONNECT_REDIRECT;

    wNetCancel();

    dwRetVal = WNetAddConnection2(&nr,arguments[0] ,arguments[1] , dwFlags);
    if (dwRetVal == NO_ERROR)
    {
        qDebug() << "Connection Added.";
        emit this->sendLocalName(_netResource.localName);
        return true;
    }
    else
        qDebug() << "ConnectionAdded Error:" << dwRetVal;
        return false;
}
