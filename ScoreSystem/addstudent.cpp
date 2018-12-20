#include "addstudent.h"
#include "ui_addstudent.h"
#include <QMessageBox>
#include <QToolTip>

AddStudent::AddStudent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStudent)
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

    //input the items for the combo box for major and gender fields
    ui->comboBox_major->addItem("Architecture");
    ui->comboBox_major->addItem("Business Management");
    ui->comboBox_major->addItem("Computer Science");
    ui->comboBox_major->addItem("Economics");
    ui->comboBox_major->addItem("Mechanical Engineering");
    ui->comboBox_gender->addItem("Male");
    ui->comboBox_gender->addItem("Female");

    //declaration of the connect function to link the signal when there is an input change on student Id,
    //student name, and username fields to each corrsponding slots for checking of their validity
    QObject::connect(ui->lineEdit_studentId,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_studentId_textChanged(QString)));
    QObject::connect(ui->lineEdit_studentName,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_studentName_textChanged(QString)));
    QObject::connect(ui->lineEdit_username,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_username_textChanged(QString)));

}

//destructor for AddStudent
AddStudent::~AddStudent()
{
    delete ui;
}

//function for connecting button signal to its slot to save new student record
void AddStudent::on_pushButton_save_clicked()
{
    Login connection;

    if (!connection.openConnection())
    {
        qDebug()<<"Fail to open database";
        return;
    }

    //get all the input from user
    QString id, fullName, username, password, major, gender, email,midName,lastName, firstName;
    id = ui->lineEdit_studentId->text();
    firstName = ui->lineEdit_studentName->text();
    username = ui->lineEdit_username->text();
    password = ui->lineEdit_password->text();
    major = ui->comboBox_major->currentText();
    gender = ui->comboBox_gender->currentText();
    email = ui->lineEdit_email->text();
    midName = ui->lineEdit_midName->text();
    lastName = ui->lineEdit_lastName->text();

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

    //First check against teacher record to prevent same ID and username registered
    QSqlQuery checkId, checkUsername;
    checkId.prepare("SELECT * FROM teacher WHERE TeacherID='"+id+"'");
    if (checkId.exec())
    {
        int countId=0;
        while(checkId.next())
        {
            countId++;
        }
        if (countId!=0)
        {
            QMessageBox::critical(this,"Warning","ID already registered. Change StudentID!");
            return;
        }
    }
    else
    {
        QMessageBox::critical(this,"Error",checkId.lastError().text());
        return;
    }
    checkUsername.prepare("SELECT * FROM teacher WHERE Username='"+username+"'");
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

    //two different query to database to allow an action to be executed while the other is being executed
    QSqlQuery query, query2;

    //prepare the insert code to database to add the new entry of student
    query.prepare("INSERT INTO student(FullName,StudentID,Major,Gender,Email,Username,Password,MidName,LastName,FirstName) values ('"+fullName+"','"+id+"','"+major+"','"+gender+"','"+email+"','"+username+"','"+password+"','"+midName+"','"+lastName+"','"+firstName+"')");

    //remove all " " in fullName area for use in database table name
    fullName.replace(" ","_");

    if (query.exec())
    {
        QMessageBox::information(this, tr("Save"),tr("New Account Saved Successfully"));

        //create another table to store the newly added student's data
        query.exec("CREATE TABLE '"+fullName+"_"+id+"'(Course VARCHAR,Regular DOUBLE,Coursework DOUBLE,Mid_Term DOUBLE,Final_Exam DOUBLE,Total DOUBLE,Grade_Point DOUBLE,Type VARCHAR)");

        //based on the new student's major, the list of courses will be added to his/her own database
        //the list of courses for a major is obtained from the database of each of the 5 major
        if (major=="Architecture")
        {
            query2.exec("SELECT course,type FROM Architecture WHERE type='compulsory'");

            while (query2.next())
            {
                QString name = query2.value(0).toString();
                QString type = query2.value(1).toString();
                query.exec("INSERT INTO '"+fullName+"_"+id+"'(Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+name+"','','','','','','','"+type+"')");     //fill all with blank string
            }
        }
        else if (major=="Business Management")
        {
            query2.exec("SELECT course,type FROM Business_Management WHERE type='compulsory'");

            while (query2.next())
            {
                QString name = query2.value(0).toString();
                QString type = query2.value(1).toString();
                query.exec("INSERT INTO '"+fullName+"_"+id+"'(Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+name+"','','','','','','','"+type+"')");     //fill all with blank string
            }
        }
        else if (major=="Computer Science")
        {
            query2.exec("SELECT course,type FROM Computer_Science WHERE type='compulsory'");

            while (query2.next())
            {
                QString name = query2.value(0).toString();
                QString type = query2.value(1).toString();
                query.exec("INSERT INTO '"+fullName+"_"+id+"'(Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+name+"','','','','','','','"+type+"')");     //fill all with blank string
            }
        }
        else if (major=="Economics")
        {
            query2.exec("SELECT course,type FROM Economics WHERE type='compulsory'");

            while (query2.next())
            {
                QString name = query2.value(0).toString();
                QString type = query2.value(1).toString();
                query.exec("INSERT INTO '"+fullName+"_"+id+"'(Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+name+"','','','','','','','"+type+"')");     //fill all with blank string
            }
        }
        else if (major=="Mechanical Engineering")
        {
            query2.exec("SELECT course,type FROM Mechanical_Engineering WHERE type='compulsory'");

            while (query2.next())
            {
                QString name = query2.value(0).toString();
                QString type = query2.value(1).toString();
                query.exec("INSERT INTO '"+fullName+"_"+id+"'(Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+name+"','','','','','','','"+type+"')");     //fill all with blank string
            }
        }

        //clear all the input fields in the page after saving a new record
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_studentId->clear();
        ui->lineEdit_studentName->clear();
        ui->lineEdit_midName->clear();
        ui->lineEdit_lastName->clear();
        ui->lineEdit_email->clear();


        connection.closeConnection();
    } 

    //notify users with pop-up for their specific error in adding the new record to database
    else
    {
        QString error;
        error=query.lastError().text();
        if (error=="UNIQUE constraint failed: student.StudentID Unable to fetch row")
        {
            QMessageBox::critical(this, "Warning","Student ID already registered. Change Student ID!");
        }
        else if (error=="UNIQUE constraint failed: student.Username Unable to fetch row")
        {
            QMessageBox::critical(this, "Warning", "Username already registered. Change Username!");
        }

    }

}

//function for connecting back button signal to its slot to go back to previous page
void AddStudent::on_pushButton_back_clicked()
{
    //clear all input fields when going back to previous page
    ui->lineEdit_studentId->clear();
    ui->lineEdit_studentName->clear();
    ui->lineEdit_midName->clear();
    ui->lineEdit_lastName->clear();
    ui->lineEdit_username->clear();
    this->hide();
    AdminPage adminPage;
    adminPage.setModal(true);
    adminPage.exec();
}

//variables to save the correct text input so far on the input fields in the page
QString id_correctText, firstName_correctText, username_correctText;
QString midName_correctText, lastName_correctText;

//regular expression to validate the input on each input fields
//as well as to set up requirement for enabling save button
QRegExp for_id("[0-9]*");QRegExp allow_button1("[0-9]{1,40}");
QRegExp for_fullname("[A-Za-z]{0,40}"); QRegExp allow_button2("[A-Za-z ]{1,40}");
QRegExp for_username("[A-Za-z0-9 _]{0,40}");QRegExp allow_button3("[A-Za-z0-9 _]{1,40}");

//function for when the input on the student ID field changes
void AddStudent::on_lineEdit_studentId_textChanged( QString inputId)
{
    //check if input matches the regular expression for student ID input
    if (for_id.exactMatch(inputId))
    {
        id_correctText = inputId;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the student ID
        QPoint point = QPoint (geometry().left()+ui->lineEdit_studentId->geometry().left(),
                               geometry().top() + ui->lineEdit_studentId->geometry().bottom());
        ui->lineEdit_studentId->setText(id_correctText);
        QToolTip::showText(point,"only input number");
    }

    //enable save button when all 3 fields (student ID, firstname and username) are filled correctly
    ui->pushButton_save->setEnabled((allow_button1.exactMatch(ui->lineEdit_studentId->text()))
                                    &&(allow_button2.exactMatch(ui->lineEdit_studentName->text()))
                                       &&(allow_button3.exactMatch(ui->lineEdit_username->text())));
}

//function for when the input on the student first name field changes
void AddStudent::on_lineEdit_studentName_textChanged(QString inputName)
{
    //check if input matches the regular expression for student firstname input
    if (for_fullname.exactMatch(inputName))
    {
        firstName_correctText = inputName;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the student firstname
        QPoint point = QPoint (geometry().left()+ui->lineEdit_studentName->geometry().left(),
                               geometry().top() + ui->lineEdit_studentName->geometry().bottom());
        ui->lineEdit_studentName->setText(firstName_correctText);
        QToolTip::showText(point,"only alphabets");
    }

    //enable save button when all 3 fields (student ID, firstname and username) are filled correctly
    ui->pushButton_save->setEnabled((allow_button1.exactMatch(ui->lineEdit_studentId->text()))
                                    &&(allow_button2.exactMatch(ui->lineEdit_studentName->text()))
                                       &&(allow_button3.exactMatch(ui->lineEdit_username->text())));
}

//function for when the input on the username field changes
void AddStudent::on_lineEdit_username_textChanged(QString arg1)
{
    //check if input matches the regular expression for student username input
    if (for_username.exactMatch(arg1))
    {
        username_correctText = arg1;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the student username
        QPoint point = QPoint (geometry().left()+ui->lineEdit_username->geometry().left(),
                               geometry().top() + ui->lineEdit_username->geometry().bottom());
        ui->lineEdit_username->setText(username_correctText);
        QToolTip::showText(point,"only alphabets, number, <space>, _");
    }

    //enable save button when all 3 fields (student ID, firstname and username) are filled correctly
    ui->pushButton_save->setEnabled((allow_button1.exactMatch(ui->lineEdit_studentId->text()))
                                    &&(allow_button2.exactMatch(ui->lineEdit_studentName->text()))
                                    &&(allow_button3.exactMatch(ui->lineEdit_username->text())));
}

//function for when the input on the middle name field changes
void AddStudent::on_lineEdit_midName_textChanged(const QString &midName)
{
    //check if input matches the regular expression for student midname input
    if (for_fullname.exactMatch(midName))
    {
        midName_correctText = midName;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the student midname
        QPoint point = QPoint (geometry().left()+ui->lineEdit_midName->geometry().left(),
                               geometry().top() + ui->lineEdit_midName->geometry().bottom());
        ui->lineEdit_midName->setText(midName_correctText);
        QToolTip::showText(point,"only alphabets");
    }
}

//function for when the input on the last name field changes
void AddStudent::on_lineEdit_lastName_textChanged(const QString &lastName)
{
    //check if input matches the regular expression for student lastname input
    if (for_fullname.exactMatch(lastName))
    {
        lastName_correctText = lastName;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the student lastname
        QPoint point = QPoint (geometry().left()+ui->lineEdit_lastName->geometry().left(),
                               geometry().top() + ui->lineEdit_lastName->geometry().bottom());
        ui->lineEdit_lastName->setText(lastName_correctText);
        QToolTip::showText(point,"only alphabets");
    }
}
