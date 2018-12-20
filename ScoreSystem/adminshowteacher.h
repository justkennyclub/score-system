#ifndef ADMINSHOWTEACHER_H
#define ADMINSHOWTEACHER_H

#include <QDialog>

#include "login.h"
#include "adminpage.h"

namespace Ui {
class AdminShowTeacher;
}

class AdminShowTeacher : public QDialog
{
    Q_OBJECT

public:
    explicit AdminShowTeacher(QWidget *parent = 0);
    ~AdminShowTeacher();

private slots:
    //function for back button to go back to previous page
    void on_pushButton_back_clicked();

    //function for advanced search button to show the extension window for allowing the use of advanced search function
    void on_pushButton_showAdvSearch_clicked();

    //function for edit button to show the extension window for editing a specific teacher record
    void on_pushButton_Edit_clicked();

    //function for button to use the advanced search feature to search teachers record
    void on_pushButton_advancedSearching_clicked();

    //function for when the input on the search field changes
    void on_lineEdit_search_textChanged(const QString &arg1);

    //function for allowing interaction with the table displaying the teachers record
    void on_tableView_loadTeacher_activated(const QModelIndex &index);

    //function for button to save the modification of a teacher record
    void on_pushButton_edit_save_clicked();

    //function for when the input on the teacher ID field (in the edit window) changes
    void on_lineEdit_edit_teacherId_textChanged(QString arg1);

    //function for when the input on the first name field (in the edit window) changes
    void on_lineEdit_edit_firstName_textChanged(QString arg1);

    //function for when the input on the username field (in the edit winow) changes
    void on_lineEdit_edit_username_textChanged(QString arg1);

    //function for button to delete a selected teacher record
    void on_pushButton_delete_clicked();

    //function for button to show all teachers entries in order of their registration time
    void on_pushButton_displayAll_clicked();

    //function for button to display sorted list of teachers record alphabetically
    void on_pushButton_sortName_clicked();

    //function for button to display sorted list of teachers record based on teacher ID
    void on_pushButton_sortId_clicked();

    //function for when the input on the middle name field (in the edit window) changes
    void on_lineEdit_edit_midName_textChanged(const QString &arg1);

    //function for when the input on the last name field (in the edit window) changes
    void on_lineEdit_edit_lastName_textChanged(const QString &arg1);

private:
    Ui::AdminShowTeacher *ui;

    //flag for showing or hiding "advanced search" and "edit" extension window
    bool showSearch, showEdit;

    //store temporary id and name of a teacher before an update on the database for table name purpose
    //store temporary department of a teacher
    QString temp_teacherId, temp_teacherName, temp_department;
};

#endif // ADMINSHOWTEACHER_H
