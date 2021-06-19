#include "signinform.h"
#include "ui_signinform.h"
#include <QDebug>

SigninForm::SigninForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SigninForm)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    this->setWindowFlag(Qt::FramelessWindowHint,true);
    setProperties();
    connect(ui->signin_button,&QPushButton::clicked,this,&SigninForm::requestToSignIn);
    ui->usernamelineEdit->setText("amir");
    ui->passwordlineEdit->setText("p@SS5383");

}

SigninForm::~SigninForm()
{
    delete ui;
}

void SigninForm::setProperties()
{
    ui->signin_button->setProperty("class","GeneralButton");
    ui->close_button->setProperty("class","GeneralButton");
    ui->close_button_2->setProperty("class","GeneralButton");
    ui->close_button->setCursor(Qt::PointingHandCursor);
    ui->close_button_2->setCursor(Qt::PointingHandCursor);
    ui->signin_button->setCursor(Qt::PointingHandCursor);
    ui->label->setProperty("class","FixedLabels");
    ui->label_2->setProperty("class","FixedLabels");
    ui->welcomeLlabel->setProperty("class","WelcomeLabel");

}


void SigninForm::requestToSignIn()
{
    emit requestToLogin(ui->usernamelineEdit->text(),ui->passwordlineEdit->text());
}
void SigninForm::updateSignedUsers(QString userName){
    qDebug()<<"Welcome page updated for " + userName;
    ui->welcomeLlabel->setText("WELCOME "+userName+"!");
    ui->stackedWidget->setCurrentIndex(1);
}


void SigninForm::showEvent(QShowEvent * event)
{
    ui->stackedWidget->setCurrentIndex(0);
    QDialog::showEvent(event);/*
    ui->passwordlineEdit->clear();
    ui->usernamelineEdit->clear();*/
}
