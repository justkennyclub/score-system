#ifndef VIEWSTUDENT_H
#define VIEWSTUDENT_H

#include <QDialog>
#include "login.h"

namespace Ui {
class ViewStudent;
}

class ViewStudent : public QDialog
{
    Q_OBJECT

public:
    explicit ViewStudent(QWidget *parent = 0);
    ViewStudent(QWidget *parent, QString, QString);
    ~ViewStudent();

private slots:
    //function for showing/hiding the extension window for adding a new subject
    void on_pushButton_addSubject_clicked();

    //function for when the input on the search field changes
    void on_lineEdit_search_textChanged(const QString &arg1);

    //function for allowing interaction (double-click to select) with the list of a student's results
    void on_tableView_studentResult_activated(const QModelIndex &index);

    //function for back button to go back to previous page
    void on_pushButton_back_clicked();

    //function for save button in "edit/modify" window to save modification on students' results
    void on_pushButton_edit_save_clicked();

    //function for when the input on the regular score field (in edit window) changes
    void on_lineEdit_editRegular_textChanged(QString arg1);

    //function for when the input on the coursework score field (in edit window) changes
    void on_lineEdit_editCoursework_textChanged(QString arg1);

    //function for when the input on the mid term score field (in edit window) changes
    void on_lineEdit_editMidterm_textChanged(QString arg1);

    //function for when the input on the final exam score field (in edit window) changes
    void on_lineEdit_editFinalExam_textChanged(QString arg1);

    //function for when the input on the total score field (in edit window) changes
    void on_lineEdit_editTotal_textChanged(QString arg1);

    //function for button to add a new course/subject that a student takes
    void on_pushButton_addingSubject_clicked();

    //function for button to delete a particular student's results and the course taken
    void on_pushButton_delete_clicked();

    //function for button to show all students entries in order of their registration time
    void on_pushButton_displayAll_clicked();

    //function for button to save a student's results in a text file for printing
    void on_pushButton_print_clicked();

private:
    Ui::ViewStudent *ui;

    //flag for showing/hiding "add subject" extension window
    bool showAdd;

    //student id, fullname, and major details of a student
    QString studentId, fullname, major;

    //temporary course name before modification by users
    QString temp_course;

    //the acceptable strings on the input for regular, coursework, midterm, final, total and grade point fields
    QString correctRegular, correctCoursework, correctMidterm, correctFinal, correctTotal, correctGradePoint;
};

#endif // VIEWSTUDENT_H
