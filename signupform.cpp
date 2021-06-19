#include "signupform.h"
#include "ui_signupform.h"
#include "usersigner.h"
#include "abstractuser.h"
#include <QMetaEnum>
SignupForm::SignupForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignupForm)
{
    //ui->password_lineEdit->setEchoMode(QLineEdit::Password);
    ui->setupUi(this);
    QMetaEnum accessLevelEnum = QMetaEnum::fromType<AbstractUser::AccessLevel>();
    for(int i= 0;i<accessLevelEnum.keyCount();i++){
        ui->comboBox->addItem(accessLevelEnum.key(i),accessLevelEnum.value(i));
    }
    setProperties();
    this->setWindowFlag(Qt::FramelessWindowHint,true);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->pushButton,&QPushButton::clicked,this,&SignupForm::requestToSignUp);
}

SignupForm::~SignupForm()
{
    delete ui;
}

void SignupForm::setProperties()
{
    ui->pushButton->setProperty("class","GeneralButton");
    ui->pushButton_2->setProperty("class","GeneralButton");
    ui->pushButton->setCursor(Qt::PointingHandCursor);
    ui->pushButton_2->setCursor(Qt::PointingHandCursor);
    ui->comboBox->setCursor(Qt::PointingHandCursor);
    ui->label->setProperty("class","FixedLabels");
    ui->label_2->setProperty("class","FixedLabels");
    ui->label_3->setProperty("class","FixedLabels");
    ui->label_4->setProperty("class","FixedLabels");
    ui->label_5->setProperty("class","FixedLabels");
    ui->label_6->setProperty("class","WelcomeLabel");
}

void SignupForm::requestToSignUp()
{
    AbstractUser::AccessLevel accessLevel = ui->comboBox->currentData().value<AbstractUser::AccessLevel>();
    emit requestToAddUser(ui->username_lineEdit->text(),ui->password_lineEdit->text(),accessLevel
                          ,ui->firstnamelineEdit->text(),ui->lastnamelineedit->text());
}


void SignupForm::showEvent(QShowEvent * event)
{
    ui->stackedWidget->setCurrentIndex(0);
    QDialog::showEvent(event);
    ui->lastnamelineedit->clear();
    ui->firstnamelineEdit->clear();
    ui->password_lineEdit->clear();
    ui->username_lineEdit->clear();
    ui->pushButton->show();
}
void SignupForm::onWelcomePage(QString userName)
{
    ui->stackedWidget->setCurrentIndex(1);
    QString label = "Dear "+userName+" you have successfully signed up\n Sign in to use app";
    ui->label_6->setText(label);
    ui->pushButton->hide();
}
