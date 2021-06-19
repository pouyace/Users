#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "netaddverifier.h"
#include <QHostAddress>
#include <QDebug>
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    PostgreSqlVerifier* userVerifier = new PostgreSqlVerifier();
    NetAddVerifier *userVerifier = new NetAddVerifier(QHostAddress("192.168.1.100"),"Fajr05",this);
    _userSigner = new UserSigner(userVerifier);
    _signinForm = new SigninForm(this);
    _signupForm = new SignupForm(this);
    timer = new QTimer(this);
//    userVerifier->setUpConfig(QHostAddress::LocalHost,5432,"postgres","12345","userVerifier");


    setProperties();
    setUpConnections();
    timer->start(1000);
    timer->setTimerType(Qt::VeryCoarseTimer);
    _userSigner->masterPCsLogin();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setProperties()
{
    this->setProperty("class","sample1");
    this->setWindowFlag(Qt::FramelessWindowHint,true);
    this->setFixedSize(this->size());
    ui->comboBox->hide();
    ui->pushButton->setProperty("class","GeneralButton");
    ui->signuppushButton->setProperty("class","GeneralButton");
    ui->signinpushButton->setProperty("class","GeneralButton");
    ui->pushButton_2->setProperty("class","GeneralButton");
    ui->pushButton_3->setProperty("class","GeneralButton");
    ui->pushButton->setCursor(Qt::PointingHandCursor);
    ui->pushButton_2->setCursor(Qt::PointingHandCursor);
    ui->pushButton_3->setCursor(Qt::PointingHandCursor);
    ui->signinpushButton->setCursor(Qt::PointingHandCursor);
    ui->signuppushButton->setCursor(Qt::PointingHandCursor);
    ui->comboBox->setCursor(Qt::PointingHandCursor);
    ui->label->setProperty("class","GeneralLabel");
    ui->timerLabel->setProperty("class","timeLabel");
    ui->dateLabel->setProperty("class","timeLabel");
}
void MainWindow::setUpConnections()
{
    // Build up all connections
    connect(_signinForm,&SigninForm::requestToLogin,_userSigner,&UserSigner::signIn,Qt::UniqueConnection);
    connect(_signupForm,&SignupForm::requestToAddUser,_userSigner,&UserSigner::signUp,Qt::UniqueConnection);

    connect(ui->pushButton,&QPushButton::clicked,_userSigner,&UserSigner::signOut);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&MainWindow::createMasterUserFile,Qt::UniqueConnection);
    connect(ui->signinpushButton,&QPushButton::clicked,_signinForm,&SigninForm::show);
    connect(ui->signuppushButton,&QPushButton::clicked,_signupForm,&SignupForm::show);

    connect(_userSigner,&UserSigner::userSignedOut,this,&MainWindow::updateUsersList,Qt::UniqueConnection);
    connect(_userSigner,&UserSigner::userSwitched,this,&MainWindow::updateUsersList,Qt::UniqueConnection);
    connect(_userSigner,&UserSigner::UserSignedIn,this,&MainWindow::updateUsersList,Qt::UniqueConnection);
    connect(_userSigner,&UserSigner::stackedWidgetSwitched,_signinForm,&SigninForm::updateSignedUsers,Qt::UniqueConnection);
    connect(_userSigner,&UserSigner::userSignedUp,_signupForm,&SignupForm::onWelcomePage,Qt::UniqueConnection);

    connect(timer,&QTimer::timeout,this,&MainWindow::updateTime);

}

void MainWindow::updateTime()
{
    ui->timerLabel->setText(QTime::currentTime().toString("h:mm:ss"));
    ui->dateLabel->setText(QDate::currentDate().toString());
}

void MainWindow::createMasterUserFile()
{
   MasterUserCreator masterUserCreator(this);
   int number = masterUserCreator.exec();
   if(number){
       _userSigner->createMasterFile(masterUserCreator.getFName(),masterUserCreator.getLName());
   }
}

void MainWindow::updateUsersList()
{
    // Updating MainWindow Label and ComboBox
    qDebug()<<"MainWindow::setFrontUser -- Updating ComboBox and Label --";
    ui->comboBox->clear();
    ui->comboBox->addItems(_userSigner->signedInUserNames());
    if(_userSigner->usersListCount()== 0)
        ui->label->setText("");
    else{
        AbstractUser *frontUser = _userSigner->frontUser();
        QString label;
        if(frontUser){
            label = "Current User is now " + frontUser->prettyName();
        }else{
            label = "No user is signed in";
        }
        ui->label->setText(label);
    }
    if(_userSigner->signedInUserNames().count() == 0)
        ui->comboBox->hide();
    else
        ui->comboBox->show();
}




