#ifndef SIGNINFORM_H
#define SIGNINFORM_H
#include "usersigner.h"
#include <QDialog>

namespace Ui {
class SigninForm;
}

class SigninForm : public QDialog
{
    Q_OBJECT

public:
    explicit SigninForm(QWidget *parent );
    ~SigninForm() override;
    void setProperties();

private slots:
    void requestToSignIn();

public slots:
    void updateSignedUsers(QString);

private:
    Ui::SigninForm *ui;
protected:
    virtual void showEvent(QShowEvent * event) override;
signals:
    void requestToLogin(QString,QString,QObject *parent=nullptr);

};

#endif // SIGNINFORM_H
