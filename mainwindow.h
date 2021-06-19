#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "signupform.h"
#include "masterusercreator.h"
#include "usersigner.h"
#include <QMainWindow>
#include "signinform.h"
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void userSignedOut();
    void setProperties();

private slots:
    void setUpConnections();
    void updateTime();
public slots:
    void createMasterUserFile();
    void updateUsersList();

private:
    Ui::MainWindow *ui;
    SignupForm* _signupForm;
    UserSigner* _userSigner;
    SigninForm* _signinForm;
    QTimer *timer;

signals:
    void signOutCalled();
    void masterUserFileCreatorCalled();
};


#endif // MAINWINDOW_H
