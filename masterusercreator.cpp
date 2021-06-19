#include "masterusercreator.h"
#include "ui_masterusercreator.h"
#include <QMessageBox>

MasterUserCreator::MasterUserCreator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MasterUserCreator)
{
    ui->setupUi(this);
    connect(ui->createButoon,&QPushButton::clicked,this,&MasterUserCreator::checkUserData);
    this->setWindowFlag(Qt::FramelessWindowHint,true);
    ui->cancelButton->setProperty("class","GeneralButton");
    ui->createButoon->setProperty("class","GeneralButton");
    ui->createButoon->setCursor(Qt::PointingHandCursor);
    ui->cancelButton->setCursor(Qt::PointingHandCursor);
    ui->label->setProperty("class","FixedLabels");
    ui->label_2->setProperty("class","FixedLabels");
}

MasterUserCreator::~MasterUserCreator()
{
    delete ui;
}

QString MasterUserCreator::getFName() const
{
    return ui->firstNameLineEdit->text();
}

QString MasterUserCreator::getLName() const
{
    return ui->lastNameLineEdit->text();
}

void MasterUserCreator::checkUserData()
{
    if(!(ui->firstNameLineEdit->text().length() && ui->lastNameLineEdit->text().length())){
        QMessageBox::warning(this,"Operation Failed","Empty name not allowed");
    }
    else{

        done(1);
    }
}
