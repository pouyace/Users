#include "abstractuserverifier.h"

AbstractUserVerifier::AbstractUserVerifier(QObject *parent)
    :QObject(parent)
{

}

AbstractUserVerifier::~AbstractUserVerifier(){

}

AbstractUserVerifier::Error AbstractUserVerifier::error() const
{
    return _lastError;
}



QString AbstractUserVerifier::errorString() const
{
    switch (_lastError) {
    case Error::NoError:                return "No Error";
    case Error::UserNotFoundError:      return "Username Not Found";
    case Error::WrongPasswordError:     return "Password is wrong";
    case Error::UnknownError:           return "Unknown Error";
    case Error::RemoteNameError:        return "Remote name is not valid(error code 66)";
    case Error::ServerConnectionError:  return "server connection error";
    case Error::MultipleUsersNotAllowed:return "Multiple Users are not allowed";
    case Error::BadNameOrPassword:      return "Bad Username or Password";
    case Error::DataBaseNotOpen:        return "Couldn't open Database";
    case Error::DatabaseError:          return "Authentucation Failed";
    case Error::InvlaidInput:           return "Input valuse are invalid";
    case Error::UnknownAccessLevel:     return "Unknown Accesslevel is not allowed";
    default:return "";
    }
}

void AbstractUserVerifier::setError(AbstractUserVerifier::Error error)
{
    _lastError = error;
}
