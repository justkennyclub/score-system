#ifndef ADDTEACHER_H
#define ADDTEACHER_H

#include <QDialog>

#include "login.h"
#include "adminpage.h"

namespace Ui {
class AddTeacher;
}

class AddTeacher : public QDialog
{
    Q_OBJECT

public:
    explicit AddTeacher(QWidget *parent = 0);
    ~AddTeacher();

private slots:
    //function for button to save new teacher record
    void on_pushButton_save_clicked();

    //function for back button to go back to previous page
    void on_pushButton_back_clicked();

    //function for when the input on the teacher ID field changes
    void on_lineEdit_teacherId_textChanged(QString input);

    //function for when the input on the teacher name field changes
    void on_lineEdit_teacherName_textChanged(QString input);

    //function for when the input on the username field changes
    void on_lineEdit_username_textChanged(QString input);

    //function for when the input on the middle name field changes
    void on_lineEdit_midName_textChanged(const QString &input);

    //function for when the input on the last name field changes
    void on_lineEdit_lastName_textChanged(const QString &input);

private:
    Ui::AddTeacher *ui;
};

#endif // ADDTEACHER_H
