#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile cssFile(":/style.css");
    cssFile.open(QIODevice::ReadOnly);
    a.setStyleSheet(cssFile.readAll());
    cssFile.close();
    MainWindow w;
    w.setStyleSheet("QWidget");
    w.show();

    return a.exec();
}
