#ifndef STUDENTPAGE_H
#define STUDENTPAGE_H

#include <QDialog>

#include "login.h"
#include "chooseelective.h"

namespace Ui {
class StudentPage;
}

class StudentPage : public QDialog
{
    Q_OBJECT

public:
    explicit StudentPage(QWidget *parent = 0);
    StudentPage(QWidget *parent, QString, QString);
    ~StudentPage();

private slots:
    //function for logout button to go back to login page
    void on_pushButton_logout_clicked();

    //function for button to allow students to go to choose elective for next semester
    void on_pushButton_chooseElective_clicked();

    //function for button to save results in a text file for printing
    void on_pushButton_print_clicked();

private:
    Ui::StudentPage *ui;

    //fullname, student Id and major details of a student
    QString fullname, studentId, major;
};

#endif // STUDENTPAGE_H
