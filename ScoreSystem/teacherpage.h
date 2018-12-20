#ifndef TEACHERPAGE_H
#define TEACHERPAGE_H

#include <QDialog>
#include "login.h"
#include "viewstudent.h"

namespace Ui {
class TeacherPage;
}

class TeacherPage : public QDialog
{
    Q_OBJECT

public:
    explicit TeacherPage(QWidget *parent = 0);
    TeacherPage(QWidget *parent,QString,QString);
    ~TeacherPage();

private slots:
    //function for logout button to go back to login function
    void on_pushButton_logout_clicked();

    //function for when the input on the search field changes
    void on_lineEdit_search_textChanged(const QString &arg1);

    //function for allowing interaction (double-click to select) with the list of students in the table
    void on_tableView_student_activated(const QModelIndex &index);

    //function for view button to view the detail information of a selected student record
    void on_pushButton_view_clicked();

private:
    Ui::TeacherPage *ui;

    //teacher id , name and department details of a teacher
    QString teacherId, name, department;

    //the id and name of a selected student to be viewed
    QString selectedId,selectedName;
};

#endif // TEACHERPAGE_H
