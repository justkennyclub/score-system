#ifndef ADDSTUDENT_H
#define ADDSTUDENT_H

#include <QDialog>

#include "login.h"
#include "adminpage.h"

namespace Ui {
class AddStudent;
}

class AddStudent : public QDialog
{
    Q_OBJECT

public:
    explicit AddStudent(QWidget *parent = 0);
    ~AddStudent();

private slots:
    //function for connecting button signal to its slot to save new student record
    void on_pushButton_save_clicked();

    //function for connecting back button signal to its slot to go back to previous page
    void on_pushButton_back_clicked();

    //function for when the input on the student ID field changes
    void on_lineEdit_studentId_textChanged(QString arg1);

    //function for when the input on the student first name field changes
    void on_lineEdit_studentName_textChanged(QString arg1);

    //function for when the input on the username field changes
    void on_lineEdit_username_textChanged(QString arg1);

    //function for when the input on the middle name field changes
    void on_lineEdit_midName_textChanged(const QString &arg1);

    //function for when the input on the last name field changes
    void on_lineEdit_lastName_textChanged(const QString &arg1);

private:
    Ui::AddStudent *ui;
};

#endif // ADDSTUDENT_H
