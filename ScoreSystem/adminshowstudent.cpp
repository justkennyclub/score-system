#include "adminshowstudent.h"
#include "ui_adminshowstudent.h"

#include <QMessageBox>
#include <QToolTip>

AdminShowStudent::AdminShowStudent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminShowStudent)
{
    ui->setupUi(this);

    //FOR DESIGN PURPOSE OF THE BUTTONS IN THE UI
    //set up style sheet and interaction of the buttons with user's action
    ui->pushButton_edit->setStyleSheet("QPushButton#pushButton_edit:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_delete->setStyleSheet("QPushButton#pushButton_delete:hover {background-color: rgb(255,160,122);};");
    ui->pushButton_back->setStyleSheet("QPushButton#pushButton_back {background-color: transparent;}"
                                       "QPushButton#pushButton_back:hover {background-color: rgb(255,160,122);};");

    ui->pushButton_sortId->setStyleSheet("QPushButton#pushButton_sortId {background-color: transparent;}"
                                       "QPushButton#pushButton_sortId:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_sortAlphabet->setStyleSheet("QPushButton#pushButton_sortAlphabet {background-color: transparent;}"
                                       "QPushButton#pushButton_sortAlphabet:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_advancedSearching->setStyleSheet("QPushButton#pushButton_advancedSearching {background-color: transparent;}"
                                       "QPushButton#pushButton_advancedSearching:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_displayAll->setStyleSheet("QPushButton#pushButton_displayAll {background-color: transparent;}"
                                       "QPushButton#pushButton_displayAll:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_edit_save->setStyleSheet("QPushButton#pushButton_edit_save {background-color: transparent;}"
                                       "QPushButton#pushButton_edit_save:hover {background-color: rgb(152,251,152);};");

    //set the tool tip for displaying information of buttons to user
    ui->pushButton_edit->setToolTip("Edit Student info");
    ui->pushButton_delete->setToolTip("Delete selected student");
    ui->pushButton_back->setToolTip("Back");
    ui->pushButton_sortId->setToolTip("Sort by ID");
    ui->pushButton_sortAlphabet->setToolTip("Sort by Name");
    ui->pushButton_advancedSearching->setToolTip("Search");
    ui->pushButton_displayAll->setToolTip("Display All");

    //"Edit" and "Advanced search" extension dialog implementation
    //set both extension window for advanced search and edit to be hidden
    showEdit = false;
    showAdvancedSearch = false;
    ui->groupBox_advancedSearch->setVisible(showAdvancedSearch);
    ui->groupBox_modify->setVisible(showEdit);

    //set header font properties of table displayed
    ui->tableView_loadStudent->horizontalHeader()->setStyleSheet ("QHeaderView {font: bold 12pt Arial; color: darkblue;}");

    //input items for combo box of major and gender in the "advanced search" extension dialog
    ui->comboBox_major_advancedSearch->addItem("");
    ui->comboBox_major_advancedSearch->addItem("Architecture");
    ui->comboBox_major_advancedSearch->addItem("Business Management");
    ui->comboBox_major_advancedSearch->addItem("Computer Science");
    ui->comboBox_major_advancedSearch->addItem("Economics");
    ui->comboBox_major_advancedSearch->addItem("Mechanical Engineering");
    ui->comboBox_gender_advancedSearch->addItem("");
    ui->comboBox_gender_advancedSearch->addItem("Male");
    ui->comboBox_gender_advancedSearch->addItem("Female");

    //input items for combo box of major and gender in the "edit" extension dialog
    ui->comboBox_edit_major->addItem("Architecture");
    ui->comboBox_edit_major->addItem("Business Management");
    ui->comboBox_edit_major->addItem("Computer Science");
    ui->comboBox_edit_major->addItem("Economics");
    ui->comboBox_edit_major->addItem("Mechanical Engineering");
    ui->comboBox_edit_gender->addItem("Male");
    ui->comboBox_edit_gender->addItem("Female");

    //declaration of the connect function to link the signal when there is an input change on student Id,
    //student name, and username fields to each corrsponding slots for checking of their validity
    QObject::connect(ui->lineEdit_edit_studentId,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_edit_studentId_textChanged(QString)));
    QObject::connect(ui->lineEdit_edit_firstName,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_edit_firstName_textChanged(QString)));
    QObject::connect(ui->lineEdit_edit_username,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_edit_username_textChanged(QString)));

    Login connection;
    //create a new query model
    QSqlQueryModel *model = new QSqlQueryModel();

    connection.openConnection();
    //prepare query for selecting all the students record
    QSqlQuery *query = new QSqlQuery(connection.mydb);
    query->prepare("SELECT FullName,StudentID,Major,Gender,Email,Username,Password FROM student");
    query->exec();

    //set model to contain query
    model->setQuery(*query);

    //load the model with query into the table to display student list record
    ui->tableView_loadStudent->setModel(model);

    connection.closeConnection();

    qDebug()<<(model->rowCount());
}

//destructor for AdminShowStudent
AdminShowStudent::~AdminShowStudent()
{
    delete ui;
}

//function for back button to go back to previous page
void AdminShowStudent::on_pushButton_back_clicked()
{
    //clear all fields before closing the current window
    ui->lineEdit_edit_studentId->clear();
    ui->lineEdit_edit_firstName->clear();
    ui->lineEdit_edit_midName->clear();
    ui->lineEdit_edit_lastName->clear();
    ui->lineEdit_edit_username->clear();
    close();
    AdminPage back;
    back.setModal(true);
    back.exec();
}

//function to show and hide 'edit' extension dialog
void AdminShowStudent::on_pushButton_edit_clicked()
{
    showEdit = !showEdit;
    ui->groupBox_modify->setVisible(showEdit);
}

//Search function based on one category, updated to table in real time as text input changes
void AdminShowStudent::on_lineEdit_search_textChanged(const QString &input)
{
    Login connection;

    QSqlQueryModel *model = new QSqlQueryModel();

    connection.openConnection();
    QSqlQuery *search = new QSqlQuery(connection.mydb);

    //check which radiobutton is checked, then perform search based on the checked category
    if (ui->radioButton_studentId->isChecked())
    {
        search->exec("SELECT FullName,StudentID,Major,Gender,Email,Username,Password FROM student WHERE StudentID like '"+input+"%' "
                     "or StudentID like '_____%"+input+"%' order by StudentID");
    }
    else if (ui->radioButton_fullname->isChecked())
    {
        search->exec("SELECT FullName,StudentID,Major,Gender,Email,Username,Password FROM student WHERE FullName like '"+input+"%' "
                     "or MidName like '"+input+"%' or LastName like '"+input+"%' order by FullName");
    }
    else if (ui->radioButton_major->isChecked())
    {
        search->exec("SELECT FullName,StudentID,Major,Gender,Email,Username,Password FROM student WHERE Major like '%"+input+"%' order by FullName");
    }
    else if (ui->radioButton_gender->isChecked())
    {
        search->exec("SELECT FullName,StudentID,Major,Gender,Email,Username,Password FROM student WHERE Gender like '"+input+"%' order by FullName");
    }
    else if (ui->radioButton_email->isChecked())
    {
        search->exec("SELECT FullName,StudentID,Major,Gender,Email,Username,Password FROM student WHERE Email like '%"+input+"%' order by FullName");
    }
    else if (ui->radioButton_username->isChecked())
    {
        search->exec("SELECT FullName,StudentID,Major,Gender,Email,Username,Password FROM student WHERE Username like '%"+input+"%' order by FullName");
    }
    else if (ui->radioButton_password->isChecked())
    {
        search->exec("SELECT FullName,StudentID,Major,Gender,Email,Username,Password FROM student WHERE Password like '%"+input+"%' order by FullName");
    }

    //save query to model
    model->setQuery(*search);

    //load the model into the table to display list of student record based on search categories
    ui->tableView_loadStudent->setModel(model);

    connection.closeConnection();

    qDebug()<<(model->rowCount());
}

//function to show and hide 'advanced search' extension dialog
void AdminShowStudent::on_pushButton_advancedSearchMenu_clicked()
{
    showAdvancedSearch = !showAdvancedSearch;
    ui->groupBox_advancedSearch->setVisible(showAdvancedSearch);

    //change the text everytime the "advanced search" button is toggled
    QString text = "Advanced Search";
    text += showAdvancedSearch ? "<<<" : ">>>";
    ui->pushButton_advancedSearchMenu->setText(text);
}

/*Function that allows user to search based on different categories of input
 * or different combination of input to narrow down search results*/
void AdminShowStudent::on_pushButton_advancedSearching_clicked()
{
    //get all the information input by user
    QString studentId, fullname, major, gender, email, username, password;
    studentId = ui->lineEdit_studentId_advancedSearch->text();
    fullname = ui->lineEdit_fullname_advancedSearch->text();
    major = ui->comboBox_major_advancedSearch->currentText();
    gender = ui->comboBox_gender_advancedSearch->currentText();
    email = ui->lineEdit_email_advancedSearch->text();
    username = ui->lineEdit_username_advancedSearch->text();
    password = ui->lineEdit_password_advancedSearch->text();

    //stores query keyword for each category for searching
    QString category[7];
    category[0] = "StudentId like '%"+studentId+"%'";
    category[1] = "(FullName like '"+fullname+"%' or MidName like '"+fullname+"%' or LastName like '"+fullname+"%')";
    category[2] = "Major='"+major+"'";
    category[3] = "Gender='"+gender+"'";
    category[4] = "Email like '%"+email+"%'";
    category[5] = "Username like '%"+username+"%'";
    category[6] = "Password like '%"+password+"%'";

    //stores the query keyword for user-chosen category for searching
    QString category_choice[7];

    //mark which categories input by user as 'true', 'false' if no input from user
    bool data[7];
    for (int i=0; i<7; i++)
    {
        data[i]=false;
    }
    if (studentId!="")
        data[0] = true;
    if (fullname!="")
        data[1] = true;
    if (major!="")
        data[2] = true;
    if (gender!="")
        data[3] = true;
    if (email!="")
        data[4] = true;
    if (username!="")
        data[5] = true;
    if (password!="")
        data[6] = true;

    //initialise all counters
    int count=0, j=0, k=0;

    //fill in the category_choice array with category input by user
    for (int i=0; i<7; i++)
    {
        if (data[i]==true)
        {
            category_choice[count]=category[i];
            count++;
        }
    }

    //appending to all non-last category chosen " and " keyword for query database purpose
    while (j<count-1)
    {
        category_choice[j] = category_choice[j] + " and ";
        j++;
    }

    //concatenates all the category chosen by user into one QString total
    QString total="";
    while (k<count)
    {
        total = total + category_choice[k];
        k++;
    }

    Login connection;

    QSqlQueryModel *model = new QSqlQueryModel();

    connection.openConnection();
    QSqlQuery *query = new QSqlQuery(connection.mydb);

    //create the search query based on categories input by user
    QString search;
    search = "SELECT FullName,StudentID,Major,Gender,Email,Username,Password FROM student WHERE "+total;

    if (query->exec(search))
    {
        //display the search result on the table directly through the model of query
        model->setQuery(*query);
        ui->tableView_loadStudent->setModel(model);

        connection.closeConnection();
    }

    //at least one field needs to be filled to use the advanced search function
    else
    {
        QMessageBox::critical(this, "Warning", "Please fill in at least one field of search");
    }

    qDebug()<<(model->rowCount());
}

//function for allowing interaction with the table displaying the students record
void AdminShowStudent::on_tableView_loadStudent_activated(const QModelIndex &index)
{
    //get the index of the selected row in the table display
    QString value = ui->tableView_loadStudent->model()->data(index).toString();

    Login connection;
    if (!connection.openConnection())
    {
        qDebug()<<"Failed to open database";
        return;
    }
    connection.openConnection();

    //prepare a query to database which find a match for the content of the index in the table
    QSqlQuery query;
    query.prepare("SELECT * FROM student WHERE StudentId='"+value+"' "
                  "or FullName='"+value+"' or Username='"+value+"'");

    if (query.exec())
    {
        //search from the beginning of the database, incrementing one entry/row each time
        /*take all information from the entry that match the choice of user, and put them to the input field
         for user to edit*/
        while(query.next())
        {
            ui->lineEdit_edit_studentId->setText(query.value(1).toString());
            ui->lineEdit_edit_firstName->setText(query.value(9).toString());
            ui->lineEdit_edit_midName->setText(query.value(7).toString());
            ui->lineEdit_edit_lastName->setText(query.value(8).toString());
            ui->comboBox_edit_major->setCurrentText(query.value(2).toString());
            ui->comboBox_edit_gender->setCurrentText(query.value(3).toString());
            ui->lineEdit_edit_email->setText(query.value(4).toString());
            ui->lineEdit_edit_username->setText(query.value(5).toString());
            ui->lineEdit_edit_password->setText(query.value(6).toString());

            //temp variables are for noting the previous version of the database content for a specific student record
            temp_Id = query.value(1).toString();
            temp_fullname = query.value(0).toString();
            temp_fullname.replace(" ","_");
            temp_major = query.value(2).toString();
            //temp_major.replace(" ","_");
        }
        connection.closeConnection();
    }
    else
    {
        QMessageBox::critical(this, "Error::", query.lastError().text());
    }
}

//function for button to show all students entries in order of their registration time
void AdminShowStudent::on_pushButton_displayAll_clicked()
{
    Login connection;

    //make new query model
    QSqlQueryModel *model = new QSqlQueryModel();

    connection.openConnection();

    //preparing query for selecting all student record based on the order they are inserted
    QSqlQuery *query = new QSqlQuery(connection.mydb);
    query->prepare("SELECT FullName,StudentID,Major,Gender,Email,Username,Password FROM student");
    query->exec();

    //set query to a model and load model onto table view
    model->setQuery(*query);
    ui->tableView_loadStudent->setModel(model);

    connection.closeConnection();

    qDebug()<<(model->rowCount());
}

//function for button to save the modification of a student record
void AdminShowStudent::on_pushButton_edit_save_clicked()
{
    Login connection;

    if (!connection.openConnection())
    {
        qDebug()<<"Fail to open database";
        return;
    }

    //get all the input from user
    QString id, fullName, username, password, major, gender, email, firstName, midName, lastName;
    id = ui->lineEdit_edit_studentId->text();
    firstName = ui->lineEdit_edit_firstName->text();
    midName = ui->lineEdit_edit_midName->text();
    lastName = ui->lineEdit_edit_lastName->text();
    username = ui->lineEdit_edit_username->text();
    password = ui->lineEdit_edit_password->text();
    major = ui->comboBox_edit_major->currentText();
    gender = ui->comboBox_edit_gender->currentText();
    email = ui->lineEdit_edit_email->text();

    //fullname variable is for saving the fullname of a person in database instead of first name, middle name, last name
    fullName = firstName + ((midName!="") ? (" "+midName) : "" ) + ((lastName!="") ? (" "+lastName) : "" );

    //prevent the password field from being empty
    if (password=="")
    {
        QMessageBox::critical(this,tr("Warning"),tr("Please Fill in Password!"));
        return;
    }

    //if temp_Id is "", it means user has not double clicked from the table to choose the entry to be edited
    if (temp_Id=="")
    {
        QMessageBox::critical(this, "Error","Please select appropriate account from the table!\nDouble click on a Student ID, Fullname or Username");
        return;
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

    //different queries to database to allow an action to be executed while the other is executed
    QSqlQuery query, query2, destroy, reset, getCourse;

    //prepare the update code to database by comparing with previous version through the temp_Id variable
    query.prepare("UPDATE student SET FullName='"+fullName+"',StudentId='"+id+"',Major='"+major+"',Gender='"+gender+"',Email='"+email+"',"
                                      "Username='"+username+"',Password='"+password+"',MidName='"+midName+"',LastName='"+lastName+"',FirstName='"+firstName+"' WHERE StudentId='"+temp_Id+"'");

    //remove all " " in fullName area for use in database table name
    fullName.replace(" ","_");

    if (query.exec())
    {
        //rename database table for individual student
        query2.exec("ALTER TABLE '"+temp_fullname+"_"+temp_Id+"' RENAME TO '"+fullName+"_"+id+"'");

        //if the student's major changes, the content for the individual student database will be reset according to the new major
        if (temp_major!=major)
        {
            destroy.exec("DELETE FROM '"+fullName+"_"+id+"'");
            //then add the data/courses of corresponding major from the table
            if (major=="Computer Science")
            {
                getCourse.exec("SELECT course,type FROM Computer_Science WHERE type='compulsory'");

                while (getCourse.next())
                {
                    QString name = getCourse.value(0).toString();
                    QString type = getCourse.value(1).toString();
                    reset.exec("INSERT INTO '"+fullName+"_"+id+"'(Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+name+"','','','','','','','"+type+"')");     //fill all with blank string
                }
            }
            else if (major=="Architecture")
            {
                getCourse.exec("SELECT course,type FROM Architecture WHERE type='compulsory'");

                while (getCourse.next())
                {
                    QString name = getCourse.value(0).toString();
                    QString type = getCourse.value(1).toString();
                    reset.exec("INSERT INTO '"+fullName+"_"+id+"'(Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+name+"','','','','','','','"+type+"')");     //fill all with blank string
                }
            }
            else if (major=="Business Management")
            {
                getCourse.exec("SELECT course,type FROM Business_Management WHERE type='compulsory'");

                while (getCourse.next())
                {
                    QString name = getCourse.value(0).toString();
                    QString type = getCourse.value(1).toString();
                    reset.exec("INSERT INTO '"+fullName+"_"+id+"'(Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+name+"','','','','','','','"+type+"')");     //fill all with blank string
                }
            }
            else if (major=="Economics")
            {
                getCourse.exec("SELECT course,type FROM Economics WHERE type='compulsory'");

                while (getCourse.next())
                {
                    QString name = getCourse.value(0).toString();
                    QString type = getCourse.value(1).toString();
                    reset.exec("INSERT INTO '"+fullName+"_"+id+"'(Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+name+"','','','','','','','"+type+"')");     //fill all with blank string
                }
            }
            else if (major=="Mechanical Engineering")
            {
                getCourse.exec("SELECT course,type FROM Mechanical_Engineering WHERE type='compulsory'");

                while (getCourse.next())
                {
                    QString name = getCourse.value(0).toString();
                    QString type = getCourse.value(1).toString();
                    reset.exec("INSERT INTO '"+fullName+"_"+id+"'(Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+name+"','','','','','','','"+type+"')");     //fill all with blank string
                }
            }
        }

        QMessageBox::information(this, tr("Save"),tr("Saved Successfully"));

        //clear the input fields in the "edit" extension window after saving
        ui->lineEdit_edit_studentId->clear();
        ui->lineEdit_edit_firstName->clear();
        ui->lineEdit_edit_midName->clear();
        ui->lineEdit_edit_lastName->clear();
        ui->lineEdit_edit_email->clear();
        ui->lineEdit_edit_username->clear();
        ui->lineEdit_edit_password->clear();

        //clear the temp_Id container so that it will not cause any record to be modified
        //without user choosing (double-click) from the table
        temp_Id = "";

        //prepare the model with query to show the updated database on the table display
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery *show = new QSqlQuery(connection.mydb);
        show->prepare("SELECT FullName,StudentID,Major,Gender,Email,Username,Password FROM student");
        show->exec();
        model->setQuery(*show);
        ui->tableView_loadStudent->setModel(model);

        connection.closeConnection();
    }

    //notify users with pop-up for their specific error in making changes to a specific record in database
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

//variables to save the correct text input so far on the input fields in the page
QString correctId, correctFirstName, correctUsername;
QString correctMidName, correctLastName;

//regular expression to validate the input on each input fields
//as well as to set up requirement for enabling save button
QRegExp id_rule("[0-9]*");QRegExp allow_id("[0-9]{1,40}");
QRegExp fullname_rule("[A-Za-z]{0,40}"); QRegExp allow_fullname("[A-Za-z]{1,40}");
QRegExp username_rule("[A-Za-z0-9 _]{0,40}");QRegExp allow_username("[A-Za-z0-9 _]{1,40}");

//function for when the input on the student ID field (in the edit window) changes
void AdminShowStudent::on_lineEdit_edit_studentId_textChanged(QString input_id)
{
    //check if input matches the regular expression for student ID input
    if (id_rule.exactMatch(input_id))
    {
        correctId = input_id;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the student ID
        QPoint point = QPoint (geometry().left()+ui->groupBox_modify->geometry().left(),
                               geometry().top() + ui->lineEdit_edit_firstName->geometry().bottom());
        ui->lineEdit_edit_studentId->setText(correctId);
        QToolTip::showText(point,"only input number");
    }

    //enable save button when all 3 fields (student ID, firstname and username) are filled correctly
    ui->pushButton_edit_save->setEnabled((allow_id.exactMatch(ui->lineEdit_edit_studentId->text()))
                                    &&(allow_fullname.exactMatch(ui->lineEdit_edit_firstName->text()))
                                       &&(allow_username.exactMatch(ui->lineEdit_edit_username->text())));
}

//function for when the input on the first name field (in the edit window) changes
void AdminShowStudent::on_lineEdit_edit_firstName_textChanged(QString input_firstName)
{
    //check if input matches the regular expression for student firstname input
    if (fullname_rule.exactMatch(input_firstName))
    {
        correctFirstName = input_firstName;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the student firstname
        QPoint point = QPoint (geometry().left()+ui->groupBox_modify->geometry().left(),
                               geometry().top() + ui->lineEdit_edit_lastName->geometry().bottom());
        ui->lineEdit_edit_firstName->setText(correctFirstName);
        QToolTip::showText(point,"only alphabets");
    }

    //enable save button when all 3 fields (student ID, firstname and username) are filled correctly
    ui->pushButton_edit_save->setEnabled((allow_id.exactMatch(ui->lineEdit_edit_studentId->text()))
                                    &&(allow_fullname.exactMatch(ui->lineEdit_edit_firstName->text()))
                                       &&(allow_username.exactMatch(ui->lineEdit_edit_username->text())));
}

//function for when the input on the username field (in the edit winow) changes
void AdminShowStudent::on_lineEdit_edit_username_textChanged(QString input_username)
{
    //check if input matches the regular expression for student username input
    if (username_rule.exactMatch(input_username))
    {
        correctUsername = input_username;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the student username
        QPoint point = QPoint (geometry().left()+ui->groupBox_modify->geometry().left(),
                               geometry().top() + ui->lineEdit_edit_password->geometry().bottom());
        ui->lineEdit_edit_username->setText(correctUsername);
        QToolTip::showText(point,"only alphabets, number, <space>, _");
    }

    //enable save button when all 3 fields (student ID, firstname and username) are filled correctly
    ui->pushButton_edit_save->setEnabled((allow_id.exactMatch(ui->lineEdit_edit_studentId->text()))
                                    &&(allow_fullname.exactMatch(ui->lineEdit_edit_firstName->text()))
                                    &&(allow_username.exactMatch(ui->lineEdit_edit_username->text())));
}

//function for button to delete a selected student record
void AdminShowStudent::on_pushButton_delete_clicked()
{
    //set variable id and name which is taken from the temp variables for both (by double click on table)
    QString id, name, table;
    id = temp_Id;
    name = temp_fullname;
    name.replace("_"," ");

    //ensure user choose by double clicking from table the record to be deleted
    if (id=="")
    {
        QMessageBox::critical(this,"Error","Make sure to double click Student(ID, Fullname, or Username) choice");
        return;
    }

    Login connection;
    connection.openConnection();

    //query for erasing the record from the list of students and deleting the student's individual table
    QSqlQuery eraseAccount, deleteTable;

    //find the record to be deleted
    eraseAccount.prepare("DELETE FROM student WHERE StudentId='"+id+"' and FullName='"+name+"'");

    name.replace(" ","_");
    table = name + "_" + id;

    if (eraseAccount.exec())
    {
        deleteTable.exec("DROP TABLE IF EXISTS '"+table+"'");
        QMessageBox::information(this,"Deleted", "Account deleted");

        //clear the input fields on the "edit" extension window after deleting chosen record
        ui->lineEdit_edit_studentId->clear();
        ui->lineEdit_edit_firstName->clear();
        ui->lineEdit_edit_midName->clear();
        ui->lineEdit_edit_lastName->clear();
        ui->lineEdit_edit_email->clear();
        ui->lineEdit_edit_username->clear();
        ui->lineEdit_edit_password->clear();

        //prepare the model with query to show the updated database on the table display
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery *show = new QSqlQuery(connection.mydb);
        show->prepare("SELECT FullName,StudentID,Major,Gender,Email,Username,Password FROM student");
        show->exec();
        model->setQuery(*show);
        ui->tableView_loadStudent->setModel(model);

        //clear the temp_Id container so that it will not cause any record to be deleted
        //without user choosing (double-click) from the table
        temp_Id = "";
    }
    else
    {
        QMessageBox::critical(this,"Error","Make sure to double click Student(ID, Fullname, or Username) choice");
    }
    connection.closeConnection();
}

//function for button to display sorted list of students record alphabetically
void AdminShowStudent::on_pushButton_sortAlphabet_clicked()
{
    Login connection;
    connection.openConnection();
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *sort = new QSqlQuery(connection.mydb);

    //directly using database query to sort the student list based on fullname/alphabetically
    sort->prepare("SELECT FullName,StudentID,Major,Gender,Email,Username,Password from Student order by FullName");
    sort->exec();

    //set and load the model into the table for display
    model->setQuery(*sort);
    ui->tableView_loadStudent->setModel(model);
    connection.closeConnection();
}

//function for button to display sorted list of students record based on student ID
void AdminShowStudent::on_pushButton_sortId_clicked()
{
    Login connection;
    connection.openConnection();
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *sort = new QSqlQuery(connection.mydb);

    //directly using database query to sort the student list based on ID/numerically
    sort->prepare("SELECT FullName,StudentID,Major,Gender,Email,Username,Password from Student order by StudentId");
    sort->exec();

    //set and load the model into the table for display
    model->setQuery(*sort);
    ui->tableView_loadStudent->setModel(model);
    connection.closeConnection();
}

//function for when the input on the middle name field (in the edit window) changes
void AdminShowStudent::on_lineEdit_edit_midName_textChanged(const QString &input_midName)
{
    //check if input matches the regular expression for student midname input
    if (fullname_rule.exactMatch(input_midName))
    {
        correctMidName = input_midName;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the student midname
        QPoint point = QPoint (geometry().left()+ui->groupBox_modify->geometry().left(),
                               geometry().top() + ui->comboBox_edit_major->geometry().bottom());
        ui->lineEdit_edit_midName->setText(correctMidName);
        QToolTip::showText(point,"only alphabets");
    }
}

//function for when the input on the last name field (in the edit window) changes
void AdminShowStudent::on_lineEdit_edit_lastName_textChanged(const QString &input_lastName)
{
    //check if input matches the regular expression for student lastname input
    if (fullname_rule.exactMatch(input_lastName))
    {
        correctLastName = input_lastName;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the student lastname
        QPoint point = QPoint (geometry().left()+ui->groupBox_modify->geometry().left(),
                               geometry().top() + ui->comboBox_edit_gender->geometry().bottom());
        ui->lineEdit_edit_lastName->setText(correctLastName);
        QToolTip::showText(point,"only alphabets");
    }
}
