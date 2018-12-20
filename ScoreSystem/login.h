#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>

#include "adminpage.h"
#include "studentpage.h"
#include "teacherpage.h"

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    //function for login button
    void on_pushButton_login_clicked();

    //function for when input text on the text field changes
    void on_lineEdit_username_textChanged(const QString &arg1);

    //function for close option from toolbar
    void on_actionClose_triggered();

    //function for close button
    void on_pushButton_close_clicked();

public:
    QSqlDatabase mydb;
    void closeConnection()              //function to close connection to database
    {
        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);       //close database and remove connection
    }

    bool openConnection()               //function to open connection to database
    {
        mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("/Users/J/Desktop/Score System NEW/users.db");

        if (!mydb.open())
        {
            qDebug()<<("Failed to open database");
            return false;
        }
        else
        {
            qDebug()<<("Connected to database");
            return true;
        }

    }

private:
    Ui::Login *ui;

};

#endif // LOGIN_H
