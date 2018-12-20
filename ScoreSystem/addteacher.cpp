#include "addteacher.h"
#include "ui_addteacher.h"

#include <QMessageBox>
#include <QToolTip>

AddTeacher::AddTeacher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTeacher)
{
    ui->setupUi(this);

    //set the fixed size of the window
    this->setFixedSize(360,460);

    //FOR DESIGN PURPOSE OF THE FRAME AND BUTTONS IN THE UI
    //set up style sheet and interaction of the buttons with user's action
    ui->frame->setStyleSheet("QFrame#frame {background-color:  rgb(211, 224, 234);};");
    ui->pushButton_save->setStyleSheet("QPushButton#pushButton_save {background-color: transparent;}"
                                       "QPushButton#pushButton_save:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_back->setStyleSheet("QPushButton#pushButton_back {background-color: transparent;}"
                                       "QPushButton#pushButton_back:hover {background-color: rgb(255,160,122);};");
    ui->pushButton_save->setToolTip("Save Record");
    ui->pushButton_back->setToolTip("back");

    //input the items for the combo box for department and gender fields
    ui->comboBox_department->addItem("Architecture");
    ui->comboBox_department->addItem("Business Management");
    ui->comboBox_department->addItem("Computer Science");
    ui->comboBox_department->addItem("Economics");
    ui->comboBox_department->addItem("Mechanical Engineering");
    ui->comboBox_gender->addItem("Male");
    ui->comboBox_gender->addItem("Female");

    //declaration of the connect function to link the signal when there is an input change on teacher Id,
    //teacher name, and username fields to each corrsponding slots for checking of their validity
    QObject::connect(ui->lineEdit_teacherId,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_teacherId_textChanged(QString)));
    QObject::connect(ui->lineEdit_teacherName,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_teacherName_textChanged(QString)));
    QObject::connect(ui->lineEdit_username,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_username_textChanged(QString)));

}

//destructor for AddTeacher
AddTeacher::~AddTeacher()
{
    delete ui;
}

//function for button to save new teacher record
void AddTeacher::on_pushButton_save_clicked()
{
    Login connection;

    if(!connection.openConnection())
    {
        qDebug()<<"Fail to open database";
        return;
    }

    //get all the input from user
    QString id, fullName, username, password, department, gender, email, firstName,midName,lastName;
    id = ui->lineEdit_teacherId->text();
    firstName = ui->lineEdit_teacherName->text();
    midName = ui->lineEdit_midName->text();
    lastName = ui->lineEdit_lastName->text();
    username = ui->lineEdit_username->text();
    password = ui->lineEdit_password->text();
    department = ui->comboBox_department->currentText();
    gender = ui->comboBox_gender->currentText();
    email = ui->lineEdit_email->text();

    //fullname variable is for saving the fullname of a person in database instead of first name, middle name, last name
    fullName = firstName + ((midName!="") ? (" "+midName) : "" ) + ((lastName!="") ? (" "+lastName) : "" );

    //prevent the password field from being empty
    if (password=="")
    {
        QMessageBox::critical(this,tr("Warning"),tr("Please Fill in Password!"));
        return;
    }

    if (email=="")
    {
        email="-";
    }

    connection.openConnection();
    //First check against student record to prevent same ID and username registered
    QSqlQuery checkId, checkUsername;
    checkId.prepare("SELECT * FROM student WHERE StudentID='"+id+"'");
    if (checkId.exec())
    {
        int countId=0;
        while(checkId.next())
        {
            countId++;
        }
        if (countId!=0)
        {
            QMessageBox::critical(this,"Warning","ID already registered. Change TeacherID!");
            return;
        }
    }
    else
    {
        QMessageBox::critical(this,"Error",checkId.lastError().text());
        return;
    }
    checkUsername.prepare("SELECT * FROM student WHERE Username='"+username+"'");
    if (checkUsername.exec())
    {
        int countUsername=0;
        while(checkUsername.next())
        {
            countUsername++;
        }
        if (countUsername!=0)
        {
            QMessageBox::critical(this,"Warning","Username already registered. Change Username!");
            return;
        }
    }
    else
    {
        QMessageBox::critical(this,"Error",checkUsername.lastError().text());
        return;
    }

    //query to insert new database record of a new teacher
    QSqlQuery query;
    query.prepare("INSERT INTO teacher(FullName,TeacherID,Department,Gender,Email,Username,Password,FirstName,MidName,LastName) "
                  "values ('"+fullName+"','"+id+"','"+department+"','"+gender+"','"+email+"','"+username+"','"+password+"','"+firstName+"','"+midName+"','"+lastName+"')");

    if (query.exec())
    {
        //after the new entry is saved, clear all the input fields in the page
        QMessageBox::information(this, tr("Save"),tr("New Account Saved Successfully!"));
        ui->lineEdit_teacherName->clear();
        ui->lineEdit_teacherId->clear();
        ui->lineEdit_midName->clear();
        ui->lineEdit_lastName->clear();
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_email->clear();
    }

    //notify users with pop-up for their specific error in adding the new record to database
    else
    {
        QString error;
        error=query.lastError().text();
        if (error=="UNIQUE constraint failed: teacher.TeacherID Unable to fetch row")
        {
            QMessageBox::critical(this, "Warning","Teacher ID already registered. Change Teacher ID!");
        }
        else if (error=="UNIQUE constraint failed: teacher.Username Unable to fetch row")
        {
            QMessageBox::critical(this, "Warning", "Username already registered. Change Username!");
        }
    }
}

//function for back button to go back to previous page
void AddTeacher::on_pushButton_back_clicked()
{
    //clear all input fields when going back to previous page
    ui->lineEdit_teacherId->clear();
    ui->lineEdit_teacherName->clear();
    ui->lineEdit_midName->clear();
    ui->lineEdit_lastName->clear();
    ui->lineEdit_username->clear();
    this->hide();
    AdminPage adminPage;
    adminPage.setModal(true);
    adminPage.exec();
}

//variables to save the correct text input so far on the input fields in the page
QString id_correct, fullname_correct, username_correct;
QString midName_correct, lastName_correct;

//regular expression to validate the input on each input fields
//as well as to set up requirement for enabling save button
QRegExp valid_id("[0-9]*");QRegExp allowed_button1("[0-9]{1,40}");
QRegExp valid_fullname("[A-Za-z]{0,40}"); QRegExp allowed_button2("[A-Za-z]{1,40}");
QRegExp valid_username("[A-Za-z0-9 _]{0,40}");QRegExp allowed_button3("[A-Za-z0-9 _]{1,40}");

//function for when the input on the teacher ID field changes
void AddTeacher::on_lineEdit_teacherId_textChanged(QString input)
{
    //check if input matches the regular expression for teacher ID input
    if (valid_id.exactMatch(input))
    {
        id_correct = input;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the teacher ID
        QPoint point = QPoint (geometry().left()+ui->lineEdit_teacherId->geometry().left(),
                               geometry().top() + ui->lineEdit_teacherId->geometry().bottom());
        ui->lineEdit_teacherId->setText(id_correct);
        QToolTip::showText(point,"only input number");
    }

    //enable save button when all 3 fields (teacher ID, firstname and username) are filled correctly
    ui->pushButton_save->setEnabled((allowed_button1.exactMatch(ui->lineEdit_teacherId->text()))
                                    &&(allowed_button2.exactMatch(ui->lineEdit_teacherName->text()))
                                       &&(allowed_button3.exactMatch(ui->lineEdit_username->text())));
}

//function for when the input on the teacher name field changes
void AddTeacher::on_lineEdit_teacherName_textChanged(QString input)
{
    //check if input matches the regular expression for teacher firstname input
    if (valid_fullname.exactMatch(input))
    {
        fullname_correct = input;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the teacher firstname
        QPoint point = QPoint (geometry().left()+ui->lineEdit_teacherName->geometry().left(),
                               geometry().top() + ui->lineEdit_teacherName->geometry().bottom());
        ui->lineEdit_teacherName->setText(fullname_correct);
        QToolTip::showText(point,"only alphabets");
    }

    //enable save button when all 3 fields (teacher ID, firstname and username) are filled correctly
    ui->pushButton_save->setEnabled((allowed_button1.exactMatch(ui->lineEdit_teacherId->text()))
                                    &&(allowed_button2.exactMatch(ui->lineEdit_teacherName->text()))
                                       &&(allowed_button3.exactMatch(ui->lineEdit_username->text())));
}

//function for when the input on the username field changes
void AddTeacher::on_lineEdit_username_textChanged(QString input)
{
    //check if input matches the regular expression for teacher username input
    if (valid_username.exactMatch(input))
    {
        username_correct = input;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the teacher username
        QPoint point = QPoint (geometry().left()+ui->lineEdit_username->geometry().left(),
                               geometry().top() + ui->lineEdit_username->geometry().bottom());
        ui->lineEdit_username->setText(username_correct);
        QToolTip::showText(point,"only alphabets, number, <space>, _");
    }

    //enable save button when all 3 fields (teacher ID, firstname and username) are filled correctly
    ui->pushButton_save->setEnabled((allowed_button1.exactMatch(ui->lineEdit_teacherId->text()))
                                    &&(allowed_button2.exactMatch(ui->lineEdit_teacherName->text()))
                                    &&(allowed_button3.exactMatch(ui->lineEdit_username->text())));
}

//function for when the input on the middle name field changes
void AddTeacher::on_lineEdit_midName_textChanged(const QString &input)
{
    //check if input matches the regular expression for teacher midname input
    if (valid_fullname.exactMatch(input))
    {
        midName_correct = input;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the teacher midname
        QPoint point = QPoint (geometry().left()+ui->lineEdit_midName->geometry().left(),
                               geometry().top() + ui->lineEdit_midName->geometry().bottom());
        ui->lineEdit_midName->setText(midName_correct);
        QToolTip::showText(point,"only alphabets");
    }
}

//function for when the input on the last name field changes
void AddTeacher::on_lineEdit_lastName_textChanged(const QString &input)
{
    //check if input matches the regular expression for teacher lastname input
    if (valid_fullname.exactMatch(input))
    {
        lastName_correct = input;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the teacher lastname
        QPoint point = QPoint (geometry().left()+ui->lineEdit_lastName->geometry().left(),
                               geometry().top() + ui->lineEdit_lastName->geometry().bottom());
        ui->lineEdit_lastName->setText(lastName_correct);
        QToolTip::showText(point,"only alphabets");
    }
}
