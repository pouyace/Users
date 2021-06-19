#ifndef MASTERUSERCREATOR_H
#define MASTERUSERCREATOR_H

#include <QDialog>

namespace Ui {
class MasterUserCreator;
}

class MasterUserCreator : public QDialog
{
    Q_OBJECT

public:
    explicit MasterUserCreator(QWidget *parent);
    ~MasterUserCreator();
    QString getFName() const;
    QString getLName() const;
public slots:
    void checkUserData();

private:
    Ui::MasterUserCreator *ui;

};

#endif // MASTERUSERCREATOR_H
