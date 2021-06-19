#include "authentication.h"

Authentication::Authentication(Database *db, GeneralUseScout *gelobal, QWidget *parent):
    generalUse(gelobal)
{
    database = db;
}

void Authentication::setSignUpDataUseraUser(QString userName, QString passWord, QString rePassword)
{
    this->upUsername = userName;
    this->upPassword = passWord;
    this->upRePassword = rePassword;
}

bool Authentication::checkedAdminData(QString userName, QString passWord)
{
    this->adminUsername = userName;
    this->adminPassword = passWord;
    QString whereClaus = "username='" + adminUsername + "' AND password='" + adminPassword + "'";
    QString query = "SELECT id_category,username,password FROM admin_users WHERE " + whereClaus;

    int size = database->execute(query);
    if(size == 1)
    {
        database->nextRecord();
        return true;
    }
    else if (size == 0){
        return false;
    }
    else {
        return false;
    }
}

void Authentication::setSigninDataUserUser(QString userName, QString passWord, QString remoteName)
{
    inUsername = userName;
    inPassword = passWord;
}

bool Authentication::signIn()
{
    QString hashedPassword = QCryptographicHash::hash(inPassword.toUtf8(), QCryptographicHash::Sha1).toHex();

    QString whereClaus = "username='" + inUsername + "' AND password='" + hashedPassword + "'";
    QString query = "SELECT id,access_level,info FROM system_users WHERE " + whereClaus;

    int size = database->execute(query);

    if (size == 1){
        database->nextRecord();

        QDateTime currentDateTime = QDateTime::currentDateTime();

        generalUse->loggedUser.id = database->getValue("id").toInt();
        generalUse->loggedUser.accesLevel = database->getValue("access_level").toInt();
        generalUse->loggedUser.username = inUsername;
        generalUse->loggedUser.loginTime = currentDateTime;
        generalUse->loggedUser.info = database->getValue("info").toByteArray();

        //        QStringList roleList;

        //        if (loggedUser.role == "Ordinary"){
        //            roleList << "Ordinary";

        //        } else if (loggedUser.role == "Admin"){
        //            roleList << "Ordinary" << "Admin";

        //        } else if (loggedUser.role == "Developer"){
        //            roleList << "Ordinary" << "Admin" << "Developer";
        //        }



        inUsername.clear();
        inPassword.clear();
        return true;

    } else if (size == 0){
        emit incorrectLoginData();
        return false;
    } else {
        emit thereIsAnotherError();
        return false;
    }
}

bool Authentication::signUp()
{
    QString username = upUsername;
    QString password1 = upPassword;
    QString password2 = upRePassword;

    QString query;

    if (password1==password2){
        //check if username already exist
        query = "SELECT id FROM system_users WHERE username='" + username + "'";

        int size = database->execute(query);
        if (size==1){//username already exist
            emit userDuplicateError();
            return false;
        }
        else if (size==0){//username does not exist
            QString hashedPassword = QCryptographicHash::hash(password1.toUtf8(), QCryptographicHash::Sha1).toHex();

            query = "INSERT INTO system_users (username,access_level,password) VALUES ('";
            query = query + username + "',0,'" + hashedPassword + "')";

            int size = database->execute(query);
            if (size==1){
                //                        createStatus->setStyleSheet("color: rgb(0, 170, 0);");
                //                        QString text = "Operator " + username + " created";
                //                        createStatus->setText(text);

                //                        editTableAfterCreate(name,username,role);

                return true;
            } else {
                emit thereIsAnotherError();
                return false;
            }
        }
    }
    return false;
}


void Authentication::signOut()
{
    generalUse->loggedUser.id = -1;
    generalUse->loggedUser.accesLevel = -1;
    generalUse->loggedUser.username = "";
    //    loggedUser.loginTime = NULL;
    generalUse->loggedUser.info = NULL;
}

