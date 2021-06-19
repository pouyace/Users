#ifndef SIGNUPFORM_H
#define SIGNUPFORM_H
#include "usersigner.h"
#include <QDialog>


namespace Ui {
class SignupForm;
}

class SignupForm : public QDialog
{
    Q_OBJECT

public:
    explicit SignupForm(QWidget *parent);
    ~SignupForm();
    void setProperties();
    void on_signup_button_clicked();

private:
    Ui::SignupForm *ui;
private slots:
    void requestToSignUp();
public slots:
    void onWelcomePage(QString userName);
protected:
    virtual void showEvent(QShowEvent * event) override;
signals:
    void requestToAddUser(QString,QString,AbstractUser::AccessLevel,QString,QString);
};

#endif // SIGNUPFORM_H
