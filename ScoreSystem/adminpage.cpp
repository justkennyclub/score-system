#include "adminpage.h"
#include "ui_adminpage.h"
#include<QMessageBox>

AdminPage::AdminPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminPage)
{
    ui->setupUi(this);

    //set the fixed size of the window
    this->setFixedSize(710,436);

    //FOR INTERFACE DESIGN PURPOSE
    //add an image to a label, following the size of the image itself
    QPixmap student_male(":/resource/img/Student.png");
    int w_studentMale = ui->label_student_male->width();
    int h_studentMale = ui->label_student_male->height();
    ui->label_student_male->setPixmap(student_male.scaled(w_studentMale,h_studentMale,Qt::KeepAspectRatio));

    //add an image to a label, following the size of the image itself
    QPixmap student_female(":/resource/img/Student_Female.png");
    int w_studentFemale = ui->label_student_female->width();
    int h_studentFemale = ui->label_student_female->height();
    ui->label_student_female->setPixmap(student_female.scaled(w_studentFemale,h_studentFemale,Qt::KeepAspectRatio));

    //add an image to a label, following the size of the image itself
    QPixmap teacher_female(":/resource/img/Professor_Female.png");
    int w_teacherFemale = ui->label_teacher_female->width();
    int h_teacherFemale = ui->label_teacher_female->height();
    ui->label_teacher_female->setPixmap(teacher_female.scaled(w_teacherFemale,h_teacherFemale,Qt::KeepAspectRatio));

    //add an image to a label, following the size of the image itself
    QPixmap teacher_male(":/resource/img/Professor.png");
    int w_teacherMale = ui->label_teacher_male->width();
    int h_teacherMale = ui->label_teacher_male->height();
    ui->label_teacher_male->setPixmap(teacher_male.scaled(w_teacherMale,h_teacherMale,Qt::KeepAspectRatio));

    //change the properties of a frame for design of UI
    ui->frame_student->setStyleSheet("QFrame#frame_student {background-color: rgb(211, 224, 234);};");
    ui->frame_teacher->setStyleSheet("QFrame#frame_teacher {background-color: rgb(211, 224, 234);};");

    QPalette pal = palette();
    pal.setColor(QPalette::Background,Qt::white);
    ui->label_student->setAutoFillBackground(true);
    ui->label_student->setPalette(pal);
    ui->label_teacher->setAutoFillBackground(true);
    ui->label_teacher->setPalette(pal);
    ui->label_student->show();
    ui->label_teacher->show();

    //FOR THE DESIGN OF THE BUTTONS IN THE PROGRAM
    //changing the style sheet for how the buttons look with and without user interaction (hover)
    //setting tool tip to help guide user with information about the buttons
    ui->pushButton_addStudent->setStyleSheet("QPushButton#pushButton_addStudent {background-color: transparent;}"
                                             "QPushButton#pushButton_addStudent:hover {background-color: rgb(233,220,210);};");
    ui->pushButton_addStudent->setToolTip("Add Student");
    ui->pushButton_loadStudents->setStyleSheet("QPushButton#pushButton_loadStudents {background-color: transparent;}"
                                             "QPushButton#pushButton_loadStudents:hover {background-color: rgb(233,220,210);};");
    ui->pushButton_loadStudents->setToolTip("Show Students List");
    ui->pushButton_deleteAllStudents->setStyleSheet("QPushButton#pushButton_deleteAllStudents {background-color: transparent;}"
                                             "QPushButton#pushButton_deleteAllStudents:hover {background-color: rgb(233,220,210);};");
    ui->pushButton_deleteAllStudents->setToolTip("Delete All Record");


    ui->pushButton_addTeacher->setStyleSheet("QPushButton#pushButton_addTeacher {background-color: transparent;}"
                                             "QPushButton#pushButton_addTeacher:hover {background-color: rgb(211,233,220);};");
    ui->pushButton_addTeacher->setToolTip("Add Teacher");
    ui->pushButton_loadTeachers->setStyleSheet("QPushButton#pushButton_loadTeachers {background-color: transparent;}"
                                             "QPushButton#pushButton_loadTeachers:hover {background-color: rgb(211,233,220);};");
    ui->pushButton_loadTeachers->setToolTip("Show Teachers List");
    ui->pushButton_deleteAllTeachers->setStyleSheet("QPushButton#pushButton_deleteAllTeachers {background-color: transparent;}"
                                             "QPushButton#pushButton_deleteAllTeachers:hover {background-color: rgb(211,233,220);};");
    ui->pushButton_deleteAllTeachers->setToolTip("Delete All Record");

    ui->pushButton_logout->setStyleSheet("QPushButton#pushButton_logout {background-color: transparent;}"
                                         "QPushButton#pushButton_logout:hover {background-color: rgb(255,160,122);};");
    ui->pushButton_logout->setToolTip("Log out");
}

//destructor for AdminPage
AdminPage::~AdminPage()
{
    delete ui;
}

//function for connecting button signal to its slot to add new student
void AdminPage::on_pushButton_addStudent_clicked()
{
    Login connection;

    if (!connection.openConnection())
    {
        qDebug()<<"Fail to open database";
        return;
    }

    //hide the current window and create an object of the next add student window to set up its interface
    this->hide();
    AddStudent addStudent;
    addStudent.setModal(true);
    addStudent.exec();

}

//function for connecting button signal to its slot to add new teacher
void AdminPage::on_pushButton_addTeacher_clicked()
{
    Login connection;

    if(!connection.openConnection())
    {
        qDebug()<<"Fail to open database";
        return;
    }

    //hide the current window and create an object of the next add teacher window to set up its interface
    this->hide();
    AddTeacher addTeacher;
    addTeacher.setModal(true);
    addTeacher.exec();
}

//function for connecting button signal to its slot to show list of students
void AdminPage::on_pushButton_loadStudents_clicked()
{
    //hide the current window and create an object of AdminShowStudent object to set up window for its interface
    this->hide();
    AdminShowStudent showStudent;
    showStudent.setModal(true);
    showStudent.exec();
}

//function for connecting button signal to its slot to show list of teachers
void AdminPage::on_pushButton_loadTeachers_clicked()
{
    //hide the current window and create an object of AdminShowTeacher object to set up window for its interface
    this->hide();
    AdminShowTeacher showTeacher;
    showTeacher.setModal(true);
    showTeacher.exec();
}

//function for connecting logout button signal to its slot to go back to login page
void AdminPage::on_pushButton_logout_clicked()
{
    //close the current window
    close();
}

/*Function to connect signal from Delete All Students button to its slot
  for clearing database of student*/
void AdminPage::on_pushButton_deleteAllStudents_clicked()
{
    //confirming with users of complete deletion of student database
    int reply;
    reply = QMessageBox::question(this,"Confirm","Are you sure you want to delete all students data?",QMessageBox::Yes,QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        Login connection;
        if (!connection.openConnection())
        {
            qDebug()<<"Fail to open database";
            return;
        }
        connection.openConnection();
        //query for searching the content of database and clearing the content of database
        QSqlQuery search, clear;
        QVector<QString> studentData;    //string vector type data to store database table name

        search.exec("SELECT FullName,StudentID FROM student");
        int name_row_id = search.record().indexOf("FullName");  //obtain index for the column FullName in database
        int id_row_id = search.record().indexOf("StudentID");   //obtain index for the column StudentID in database

        //save every student's username and ID on the vector studentData
        while (search.next())
        {
            QString full_name = search.value(name_row_id).toString();
            full_name.replace(" ","_");
            QString student_id = search.value(id_row_id).toString();
            QString studentTableName = full_name + "_" + student_id;
            studentData.append(studentTableName);
        }

        //clear all students' individual data in database
        for (int i=0; i<studentData.size();i++)
        {
            clear.exec("DROP TABLE IF EXISTS '"+studentData[i]+"'");
        }

        //clear student list
        clear.exec("DELETE FROM student");
        connection.closeConnection();
    }
    //if user cancel to delete all students' record
    else
    {
        return;
    }
}

//function for connecting button signal to its slot to delete all teachers record
void AdminPage::on_pushButton_deleteAllTeachers_clicked()
{
    //confirming with user of deletion of teacher database
    int reply;
    reply = QMessageBox::question(this,"Confirm","Are you sure you want to delete all teachers data?",QMessageBox::Yes,QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        Login connection;
        if (!connection.openConnection())
        {
            qDebug()<<"Fail to open database";
            return;
        }
        connection.openConnection();
        //query for clearing the list of teachers in teacher's database
        QSqlQuery clear;

        //clear teacher list
        clear.exec("DELETE FROM teacher");
        connection.closeConnection();
    }
    else
    {
        return;
    }
}
