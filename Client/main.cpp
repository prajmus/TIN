#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QString>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream qout(stdout);
    QTextStream qin(stdin);

    qout << "Hello world 1" << endl;
    QString line = qin.readLine();
    //qout << "Pobrano linie: " << endl;
    qout << line;
    //qDebug() << "Hello world";

    return a.exec();
}
