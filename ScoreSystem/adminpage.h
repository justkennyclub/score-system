#ifndef ADMINPAGE_H
#define ADMINPAGE_H

#include <QDialog>
#include "login.h"

#include "addstudent.h"
#include "addteacher.h"
#include "adminshowstudent.h"
#include "adminshowteacher.h"

namespace Ui {
class AdminPage;
}

class AdminPage : public QDialog
{
    Q_OBJECT

public:
    explicit AdminPage(QWidget *parent = 0);
    ~AdminPage();

private slots:
    //function for connecting button signal to its slot to add new student
    void on_pushButton_addStudent_clicked();

    //function for connecting button signal to its slot to add new teacher
    void on_pushButton_addTeacher_clicked();

    //function for connecting button signal to its slot to show list of students
    void on_pushButton_loadStudents_clicked();

    //function for connecting logout button signal to its slot to go back to login page
    void on_pushButton_logout_clicked();

    //function for connecting button signal to its slot to show list of teachers
    void on_pushButton_loadTeachers_clicked();

    //function for connecting button signal to its slot to delete all students record
    void on_pushButton_deleteAllStudents_clicked();

    //function for connecting button signal to its slot to delete all teachers record
    void on_pushButton_deleteAllTeachers_clicked();

private:
    Ui::AdminPage *ui;
};

#endif // ADMINPAGE_H
