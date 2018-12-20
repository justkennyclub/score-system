#ifndef CHOOSEELECTIVE_H
#define CHOOSEELECTIVE_H

#include <QDialog>
#include "login.h"

namespace Ui {
class ChooseElective;
}

class ChooseElective : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseElective(QWidget *parent = 0);
    ChooseElective(QWidget *parent, QString, QString, QString);
    ~ChooseElective();

private slots:
    //function for back button to go to previous page
    void on_pushButton_back_clicked();

    //function for allowing interaction (double-click to choose) with the list of electives shown
    void on_listView_course_activated(const QModelIndex &index);

    //function for button to confirm and add a selected elective
    void on_pushButton_choose_clicked();

private:
    Ui::ChooseElective *ui;

    //fullname, student id and major details of a student
    QString fullname, studentId, major;
};

#endif // CHOOSEELECTIVE_H
