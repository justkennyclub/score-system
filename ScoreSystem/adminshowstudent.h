#ifndef ADMINSHOWSTUDENT_H
#define ADMINSHOWSTUDENT_H

#include <QDialog>

#include "login.h"
#include "adminpage.h"

namespace Ui {
class AdminShowStudent;
}

class AdminShowStudent : public QDialog
{
    Q_OBJECT

public:
    explicit AdminShowStudent(QWidget *parent = 0);
    ~AdminShowStudent();

private slots:
    //function for back button to go back to previous page
    void on_pushButton_back_clicked();

    //function for when the input on the search field changes
    void on_lineEdit_search_textChanged(const QString &arg1);

    //function for edit button to show the extension window for editing a specific student record
    void on_pushButton_edit_clicked();

    //function for advanced search button to show the extension window for allowing the use of advanced search function
    void on_pushButton_advancedSearchMenu_clicked();

    //function for button to use the advanced search feature to search students record
    void on_pushButton_advancedSearching_clicked();

    //function for allowing interaction with the table displaying the students record
    void on_tableView_loadStudent_activated(const QModelIndex &index);

    //function for button to show all students entries in order of their registration time
    void on_pushButton_displayAll_clicked();

    //function for button to save the modification of a student record
    void on_pushButton_edit_save_clicked();

    //function for when the input on the student ID field (in the edit window) changes
    void on_lineEdit_edit_studentId_textChanged(QString arg1);

    //function for when the input on the first name field (in the edit window) changes
    void on_lineEdit_edit_firstName_textChanged(QString arg1);

    //function for when the input on the username field (in the edit winow) changes
    void on_lineEdit_edit_username_textChanged(QString arg1);

    //function for button to delete a selected student record
    void on_pushButton_delete_clicked();

    //function for button to display sorted list of students record alphabetically
    void on_pushButton_sortAlphabet_clicked();

    //function for button to display sorted list of students record based on student ID
    void on_pushButton_sortId_clicked();

    //function for when the input on the middle name field (in the edit window) changes
    void on_lineEdit_edit_midName_textChanged(const QString &arg1);

    //function for when the input on the last name field (in the edit window) changes
    void on_lineEdit_edit_lastName_textChanged(const QString &arg1);

private:
    Ui::AdminShowStudent *ui;
    bool showEdit;                  //flag for showing or hiding "edit" extension window
    bool showAdvancedSearch;        //flag for showing or hiding "advanced search" extension window
    QString temp_Id, temp_fullname; //store temporary id and name of a student before an update on the database for table name purpose
    QString temp_major;             //store temporary major of a student
};

#endif // ADMINSHOWSTUDENT_H
