#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    //set the fixed size of the window
    this->setFixedSize(528,359);

    //FOR DESIGN PURPOSE OF THE BUTTONS IN THE UI
    //set up style sheet and interaction as well as tool tip of the buttons with user's action
    ui->pushButton_close->setStyleSheet("QPushButton#pushButton_close {background-color: transparent;}"
                                         "QPushButton#pushButton_close:hover {background-color: rgb(255,160,122);};");
    ui->pushButton_close->setToolTip("Log out");
    ui->pushButton_login->setStyleSheet("QPushButton#pushButton_login {background-color: transparent;}"
                                       "QPushButton#pushButton_login:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_login->setToolTip("Log in");
    ui->groupBox->setStyleSheet("QGroupBox#groupBox {background-color: rgb(211, 224, 234);};");

    //set the characters and range allowed in the lineEdit field
    QRegExp regExp("[A-Za-z0-9 _]{1,40}");
    ui->lineEdit_username->setValidator(new QRegExpValidator(regExp, this));
}

//destructor for Login
Login::~Login()
{
    delete ui;
}

//function for login button
void Login::on_pushButton_login_clicked()
{
    //get the text from the username and password field filled in by user
    QString username, password;
    username = ui->lineEdit_username->text();
    password = ui->lineEdit_password->text();

    //check for connection to database
    if (!openConnection())
    {
        qDebug()<<"Failed to open database";
        return;
    }

    //connect to database
    openConnection();

    //queries for database to check for username and password
    QSqlQuery query;

    //if admin is checked, only check the database for admin type accounts
    if (ui->radioButton_admin->isChecked())
    {
        //prepare query to database
        query.prepare("select * from admin where username='"+username+"' and password='"+password+"'");

        if(query.exec())
        {
            //initiate a variable count=0 to count the number of accounts with matching username and password
            int count=0;

            //start from beginning of the database, increment 1 entry each time, look for matching username and password for admin account
            //every match will increase count by 1
            while(query.next())
            {
                count++;
            }

            //each account's username and password are unique, so when count=1, there is a matching username and password for the admin account in database
            if (count==1)
            {
                //clear the fields on login page and close connection to database
                ui->lineEdit_username->clear();
                ui->lineEdit_password->clear();
                closeConnection();

                //hide the login page
                this->hide();

                //since username and password are correct, a new object of AdminPage will be created to set up the new interface for the admin home page
                AdminPage adminPage;
                adminPage.setModal(true);
                adminPage.exec();
                this->show();
            }
            if (count<1)
                QMessageBox::critical(this,"Warning","Wrong username and password!");
        }
    }
    else if (ui->radioButton_student->isChecked())
    {
        //prepare query to database which check for matching username and password in student database
        query.prepare("select * from student where Username='"+username+"' and Password='"+password+"'");
        if(query.exec())
        {
            //initiate variable count=0 to count the number of accounts with matching username and password
            int count=0;

            //start from beginning of the database, increment 1 entry each time, look for matching username and password for student account
            //every match will increase count by 1
            while(query.next())
            {
                count++;
            }

            //each account's username and password are unique, so when count=1, there is a matching username and password for the student account in database
            if (count==1)
            {
                //clear the fields on login page and close connection to database
                ui->lineEdit_username->clear();
                ui->lineEdit_password->clear();
                closeConnection();

                //hide the login page
                this->hide();

                //create a new StudentPage object to set up interface for a student's home page
                //the username and password information is also passed so that the student's data is obtained for the next page
                QWidget *parent = 0;
                StudentPage student(parent,username,password);
                student.setModal(true);
                student.exec();
                this->show();
            }
            if (count<1)
                QMessageBox::critical(this,"Warning","Wrong username and password!");
        }
    }

    else if (ui->radioButton_teacher->isChecked())
    {
        //prepare query to database which check for matching username and password in teacher database
        query.prepare("select * from teacher where Username='"+username+"' and Password='"+password+"'");
        if(query.exec())
        {
            //initiate variable count=0 to count the number of accounts with matching username and password
            int count=0;

            //start from beginning of the database, increment 1 entry each time, look for matching username and password for teacher account
            //every match will increase count by 1
            while(query.next())
            {
                count++;
            }

            //each account's username and password are unique, so when count=1, there is a matching username and password for the teacher account in database
            if (count==1)
            {
                //clear the fields on login page and close connection to database
                ui->lineEdit_username->clear();
                ui->lineEdit_password->clear();
                closeConnection();

                //hide the login page
                this->hide();

                //create a new TeacherPage object to set up interface for a teacher's home page
                //the username and password information is also passed so that the teacher's data is obtained for the next page
                QWidget *parent = 0;
                TeacherPage teacher(parent,username,password);
                teacher.setModal(true);
                teacher.exec();
                this->show();
            }
            if (count<1)
                QMessageBox::critical(this,"Warning","Wrong username and password!");
        }
    }
}

//function for when input text on the text field changes
void Login::on_lineEdit_username_textChanged(const QString &arg1)
{
    //validate the text input in the username field by using validator set in constructor
    //if the input is acceptable, only then the login button is enabled
    ui->pushButton_login->setEnabled(ui->lineEdit_username->hasAcceptableInput());
}

//function for close option from toolbar
void Login::on_actionClose_triggered()
{
    //show a pop-up QMessageBox dialog to ask user for confirmation in closing the program using the close menu from toolbar
    int reply;
    reply = QMessageBox::question(this, "Exit", "Are you sure you want to close the program?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        close();
    }
    else
    {
        return;
    }
}

//function for close button
void Login::on_pushButton_close_clicked()
{
    //enable the close button to confirm with user before exiting the program
    int reply;
    reply = QMessageBox::question(this, "Exit", "Are you sure you want to close the program?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        close();
    }
    else
    {
        return;
    }
}
