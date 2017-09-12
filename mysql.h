#ifndef MYSQL_H
#define MYSQL_H


#include <QApplication>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <iostream>
using namespace std;

class MySQL {
public:
    MySQL();
    ~MySQL();
    bool query(QString username, QString password);

}


#endif // MYSQL_H
