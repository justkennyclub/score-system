#include "adminshowteacher.h"
#include "ui_adminshowteacher.h"
#include <QMessageBox>
#include <QToolTip>

AdminShowTeacher::AdminShowTeacher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminShowTeacher)
{
    ui->setupUi(this);

    //FOR DESIGN PURPOSE OF THE BUTTONS IN THE UI
    //set up style sheet and interaction of the buttons with user's action
    ui->pushButton_Edit->setStyleSheet("QPushButton#pushButton_Edit:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_delete->setStyleSheet("QPushButton#pushButton_delete:hover {background-color: rgb(255,160,122);};");
    ui->pushButton_back->setStyleSheet("QPushButton#pushButton_back {background-color: transparent;}"
                                       "QPushButton#pushButton_back:hover {background-color: rgb(255,160,122);};");

    ui->pushButton_sortId->setStyleSheet("QPushButton#pushButton_sortId {background-color: transparent;}"
                                       "QPushButton#pushButton_sortId:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_sortName->setStyleSheet("QPushButton#pushButton_sortName {background-color: transparent;}"
                                       "QPushButton#pushButton_sortName:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_advancedSearching->setStyleSheet("QPushButton#pushButton_advancedSearching {background-color: transparent;}"
                                       "QPushButton#pushButton_advancedSearching:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_displayAll->setStyleSheet("QPushButton#pushButton_displayAll {background-color: transparent;}"
                                       "QPushButton#pushButton_displayAll:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_edit_save->setStyleSheet("QPushButton#pushButton_edit_save {background-color: transparent;}"
                                       "QPushButton#pushButton_edit_save:hover {background-color: rgb(152,251,152);};");

    //set the tool tip for displaying information of buttons to user
    ui->pushButton_Edit->setToolTip("Edit teacher info");
    ui->pushButton_back->setToolTip("Back");
    ui->pushButton_delete->setToolTip("Delete selected teacher");
    ui->pushButton_sortId->setToolTip("Sort by ID");
    ui->pushButton_sortName->setToolTip("Sort by Name");
    ui->pushButton_advancedSearching->setToolTip("Search");
    ui->pushButton_displayAll->setToolTip("Display All");

    //"Edit" and "Advanced search" extension dialog implementation
    //set both extension window for advanced search and edit to be hidden
    showSearch = false;
    ui->groupBox_AdvancedSearch->setVisible(showSearch);
    showEdit = false;
    ui->groupBox_showEdit->setVisible(showEdit);

    //set header font properties of table displayed
    ui->tableView_loadTeacher->horizontalHeader()->setStyleSheet ("QHeaderView {font: bold 12pt Arial; color: darkblue;}");

    //input items for combo box of department and gender in the "advanced search" extension dialog
    ui->comboBox_department_advancedSearch->addItem("");
    ui->comboBox_department_advancedSearch->addItem("Architecture");
    ui->comboBox_department_advancedSearch->addItem("Business Management");
    ui->comboBox_department_advancedSearch->addItem("Computer Science");
    ui->comboBox_department_advancedSearch->addItem("Economics");
    ui->comboBox_department_advancedSearch->addItem("Mechanical Engineering");
    ui->comboBox_gender_advancedSearch->addItem("");
    ui->comboBox_gender_advancedSearch->addItem("Male");
    ui->comboBox_gender_advancedSearch->addItem("Female");

    //input items for combo box of department and gender in the "edit" extension dialog
    ui->comboBox_edit_department->addItem("Architecture");
    ui->comboBox_edit_department->addItem("Business Management");
    ui->comboBox_edit_department->addItem("Computer Science");
    ui->comboBox_edit_department->addItem("Economics");
    ui->comboBox_edit_department->addItem("Mechanical Engineering");
    ui->comboBox_edit_gender->addItem("Male");
    ui->comboBox_edit_gender->addItem("Female");

    //declaration of the connect function to link the signal when there is an input change on teacher Id,
    //teacher name, and username fields to each corrsponding slots for checking of their validity
    QObject::connect(ui->lineEdit_edit_teacherId,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_edit_teacherId_textChanged(QString)));
    QObject::connect(ui->lineEdit_edit_firstName,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_edit_firstName_textChanged(QString)));
    QObject::connect(ui->lineEdit_edit_username,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_edit_username_textChanged(QString)));

    Login connection;

    //create a new query model
    QSqlQueryModel *model = new QSqlQueryModel();
    connection.openConnection();

    //prepare query for selecting all the teachers record
    QSqlQuery *query = new QSqlQuery(connection.mydb);
    query->prepare("SELECT FullName,TeacherID,Department,Gender,Email,Username,Password FROM teacher");
    query->exec();

    //set model to contain query
    model->setQuery(*query);

    //load the model with query into the table to display teacher list record
    ui->tableView_loadTeacher->setModel(model);
    connection.closeConnection();

    qDebug()<<(model->rowCount());
}

//destructor for AdminShowTeacher
AdminShowTeacher::~AdminShowTeacher()
{
    delete ui;
}

//function for back button to go back to previous page
void AdminShowTeacher::on_pushButton_back_clicked()
{
    //clear all the input fields before closing the current window
    ui->lineEdit_edit_teacherId->clear();
    ui->lineEdit_edit_firstName->clear();
    ui->lineEdit_edit_midName->clear();
    ui->lineEdit_edit_lastName->clear();
    ui->lineEdit_edit_username->clear();
    close();
    AdminPage back;
    back.setModal(true);
    back.exec();
}

//function to show and hide 'advanced search' extension dialog
void AdminShowTeacher::on_pushButton_showAdvSearch_clicked()
{
    showSearch = !showSearch;
    ui->groupBox_AdvancedSearch->setVisible(showSearch);

    //change the text everytime the "advanced search" button is toggled
    QString text = "Advanced Search";
    text += showSearch ? "<<<" : ">>>";
    ui->pushButton_showAdvSearch->setText(text);
}

//function to show and hide 'edit' extension dialog
void AdminShowTeacher::on_pushButton_Edit_clicked()
{
    showEdit = !showEdit;
    ui->groupBox_showEdit->setVisible(showEdit);
}

//Search function based on one category, updated to table in real time as text input changes
void AdminShowTeacher::on_lineEdit_search_textChanged(const QString &input)
{
    Login connection;

    QSqlQueryModel *model = new QSqlQueryModel();

    connection.openConnection();
    QSqlQuery *search = new QSqlQuery(connection.mydb);

    //check which radiobutton is checked, then perform search based on the checked category
    if (ui->radioButton_teacherId->isChecked())
    {
        search->exec("SELECT FullName,TeacherID,Department,Gender,Email,Username,Password FROM teacher WHERE TeacherID like '"+input+"%' "
                     "or TeacherID like '___%"+input+"%' order by TeacherID");
    }
    else if (ui->radioButton_fullname->isChecked())
    {
        search->exec("SELECT FullName,TeacherID,Department,Gender,Email,Username,Password FROM teacher WHERE FullName like '"+input+"%' "
                     "or MidName like '"+input+"%' or LastName like '"+input+"%' order by FullName");
    }
    else if (ui->radioButton_department->isChecked())
    {
        search->exec("SELECT FullName,TeacherID,Department,Gender,Email,Username,Password FROM teacher WHERE Department like '%"+input+"%' order by FullName");
    }
    else if (ui->radioButton_gender->isChecked())
    {
        search->exec("SELECT FullName,TeacherID,Department,Gender,Email,Username,Password FROM teacher WHERE Gender like '"+input+"%' order by FullName");
    }
    else if (ui->radioButton_email->isChecked())
    {
        search->exec("SELECT FullName,TeacherID,Department,Gender,Email,Username,Password FROM teacher WHERE Email like '%"+input+"%' order by FullName");
    }
    else if (ui->radioButton_username->isChecked())
    {
        search->exec("SELECT FullName,TeacherID,Department,Gender,Email,Username,Password FROM teacher WHERE Username like '%"+input+"%' order by FullName");
    }
    else if (ui->radioButton_password->isChecked())
    {
        search->exec("SELECT FullName,TeacherID,Department,Gender,Email,Username,Password FROM teacher WHERE Password like '%"+input+"%' order by FullName");
    }

    //save query to model
    model->setQuery(*search);

    //load the model into the table to display list of teacher record based on search categories
    ui->tableView_loadTeacher->setModel(model);

    connection.closeConnection();

    qDebug()<<(model->rowCount());
}

//function for button to use the advanced search feature to search teachers record
void AdminShowTeacher::on_pushButton_advancedSearching_clicked()
{
    //get all the information input by user
    QString teacherId, fullname, department, gender, email, username, password;
    teacherId = ui->lineEdit_teacherId_advancedSearch->text();
    fullname = ui->lineEdit_fullname_advancedSearch->text();
    department = ui->comboBox_department_advancedSearch->currentText();
    gender = ui->comboBox_gender_advancedSearch->currentText();
    email = ui->lineEdit_email_advancedSearch->text();
    username = ui->lineEdit_username_advancedSearch->text();
    password = ui->lineEdit_password_advancedSearch->text();

    //stores query keyword for each category for searching
    QString category[7];
    category[0] = "TeacherID like '%"+teacherId+"%'";
    category[1] = "(FullName like '"+fullname+"%' or MidName like '"+fullname+"%' or LastName like '"+fullname+"%')";
    category[2] = "Department='"+department+"'";
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
    if (teacherId!="")
        data[0] = true;
    if (fullname!="")
        data[1] = true;
    if (department!="")
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
    search = "SELECT FullName,TeacherID,Department,Gender,Email,Username,Password FROM teacher WHERE "+total;

    if (query->exec(search))
    {
        //display the search result on the table directly through the model of query
        model->setQuery(*query);
        ui->tableView_loadTeacher->setModel(model);

        connection.closeConnection();
    }

    //at least one field needs to be filled to use the advanced search function
    else
    {
        QMessageBox::critical(this, "Warning", "Please fill in at least one field of search");
    }

    qDebug()<<(model->rowCount());
}

//function for allowing interaction with the table displaying the teachers record
void AdminShowTeacher::on_tableView_loadTeacher_activated(const QModelIndex &index)
{
    //get the index of the selected row in the table display
    QString value = ui->tableView_loadTeacher->model()->data(index).toString();

    Login connection;
    if (!connection.openConnection())
    {
        qDebug()<<"Failed to open database";
        return;
    }
    connection.openConnection();

    //prepare a query to database which find a match for the content of the index in the table
    QSqlQuery query;
    query.prepare("SELECT * FROM teacher WHERE TeacherID='"+value+"' "
                  "or FullName='"+value+"' or Username='"+value+"'");

    if (query.exec())
    {
        //search from the beginning of the database, incrementing one entry/row each time
        /*take all information from the entry that match the choice of user, and put them to the input field
         for user to edit*/
        while(query.next())
        {
            ui->lineEdit_edit_teacherId->setText(query.value(1).toString());
            ui->lineEdit_edit_firstName->setText(query.value(7).toString());
            ui->lineEdit_edit_midName->setText(query.value(8).toString());
            ui->lineEdit_edit_lastName->setText(query.value(9).toString());
            ui->comboBox_edit_department->setCurrentText(query.value(2).toString());
            ui->comboBox_edit_gender->setCurrentText(query.value(3).toString());
            ui->lineEdit_edit_email->setText(query.value(4).toString());
            ui->lineEdit_edit_username->setText(query.value(5).toString());
            ui->lineEdit_edit_password->setText(query.value(6).toString());

            //temp variables are for noting the previous version of the database content for a specific teacher record
            temp_teacherId = query.value(1).toString();
            temp_teacherName = query.value(0).toString();
            temp_department = query.value(2).toString();
        }
        connection.closeConnection();
    }
    else
    {
        QMessageBox::critical(this, "Error::", query.lastError().text());
    }
}

//function for button to save the modification of a teacher record
void AdminShowTeacher::on_pushButton_edit_save_clicked()
{
    Login connection;

    if (!connection.openConnection())
    {
        qDebug()<<"Fail to open database";
        return;
    }

    //get all the input from user
    QString id, fullName, username, password, department, gender, email, firstName, midName, lastName;
    id = ui->lineEdit_edit_teacherId->text();
    firstName = ui->lineEdit_edit_firstName->text();
    midName = ui->lineEdit_edit_midName->text();
    lastName = ui->lineEdit_edit_lastName->text();
    username = ui->lineEdit_edit_username->text();
    password = ui->lineEdit_edit_password->text();
    department = ui->comboBox_edit_department->currentText();
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
    if (temp_teacherId=="")
    {
        QMessageBox::critical(this, "Error","Please select appropriate account from the table!\nDouble click on a Teacher ID, Fullname or Username");
        return;
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

    //different queries to database to allow an action to be executed while the other is executed
    QSqlQuery query;

    //prepare the update code to database by comparing with previous version through the temp_teacherId variable
    query.prepare("UPDATE teacher SET FullName='"+fullName+"',TeacherID='"+id+"',Department='"+department+"',Gender='"+gender+"',Email='"+email+"',"
                                      "Username='"+username+"',Password='"+password+"',FirstName='"+firstName+"',MidName='"+midName+"',LastName='"+lastName+"' WHERE TeacherID='"+temp_teacherId+"'");

    if (query.exec())
    {
        QMessageBox::information(this, tr("Save"),tr("Saved Successfully"));

        //clear the input fields in the "edit" extension window after saving
        ui->lineEdit_edit_teacherId->clear();
        ui->lineEdit_edit_firstName->clear();
        ui->lineEdit_edit_midName->clear();
        ui->lineEdit_edit_lastName->clear();
        ui->lineEdit_edit_email->clear();
        ui->lineEdit_edit_username->clear();
        ui->lineEdit_edit_password->clear();

        //clear the temp_teacherId container so that it will not cause any record to be modified
        //without user choosing (double-click) from the table
        temp_teacherId = "";

        //prepare the model with query to show the updated database on the table display
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery *show = new QSqlQuery(connection.mydb);
        show->prepare("SELECT FullName,TeacherID,Department,Gender,Email,Username,Password FROM teacher");
        show->exec();
        model->setQuery(*show);
        ui->tableView_loadTeacher->setModel(model);

        connection.closeConnection();
    }

    //notify users with pop-up for their specific error in making changes to a specific record in database
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

//variables to save the correct text input so far on the input fields in the page
QString validId, validFullname, validUsername;
QString validMidName, validLastName;

//regular expression to validate the input on each input fields
//as well as to set up requirement for enabling save button
QRegExp teacherId_rule("[0-9]*");QRegExp check_id("[0-9]{1,40}");
QRegExp teacherName_rule("[A-Za-z ]{0,40}"); QRegExp check_fullname("[A-Za-z ]{1,40}");
QRegExp teacherUsername_rule("[A-Za-z0-9 _]{0,40}");QRegExp check_username("[A-Za-z0-9 _]{1,40}");

//function for when the input on the teacher ID field (in the edit window) changes
void AdminShowTeacher::on_lineEdit_edit_teacherId_textChanged(QString input_id)
{
    //check if input matches the regular expression for teacher ID input
    if (teacherId_rule.exactMatch(input_id))
    {
        validId = input_id;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the teacher ID
        QPoint point = QPoint (geometry().left()+ui->groupBox_showEdit->geometry().left(),
                               geometry().top() + ui->lineEdit_edit_firstName->geometry().bottom());
        ui->lineEdit_edit_teacherId->setText(validId);
        QToolTip::showText(point,"only input number");
    }

    //enable save button when all 3 fields (teacher ID, firstname and username) are filled correctly
    ui->pushButton_edit_save->setEnabled((check_id.exactMatch(ui->lineEdit_edit_teacherId->text()))
                                    &&(check_fullname.exactMatch(ui->lineEdit_edit_firstName->text()))
                                       &&(check_username.exactMatch(ui->lineEdit_edit_username->text())));
}

//function for when the input on the first name field (in the edit window) changes
void AdminShowTeacher::on_lineEdit_edit_firstName_textChanged(QString input_fullname)
{
    //check if input matches the regular expression for teacher firstname input
    if (teacherName_rule.exactMatch(input_fullname))
    {
        validFullname = input_fullname;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the teacher firstname
        QPoint point = QPoint (geometry().left()+ui->groupBox_showEdit->geometry().left(),
                               geometry().top() + ui->lineEdit_edit_lastName->geometry().bottom());
        ui->lineEdit_edit_firstName->setText(validFullname);
        QToolTip::showText(point,"only alphabets and <space>");
    }

    //enable save button when all 3 fields (teacher ID, firstname and username) are filled correctly
    ui->pushButton_edit_save->setEnabled((check_id.exactMatch(ui->lineEdit_edit_teacherId->text()))
                                    &&(check_fullname.exactMatch(ui->lineEdit_edit_firstName->text()))
                                       &&(check_username.exactMatch(ui->lineEdit_edit_username->text())));
}

//function for when the input on the username field (in the edit winow) changes
void AdminShowTeacher::on_lineEdit_edit_username_textChanged(QString input_username)
{
    //check if input matches the regular expression for teacher username input
    if (teacherUsername_rule.exactMatch(input_username))
    {
        validUsername = input_username;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the teacher username
        QPoint point = QPoint (geometry().left()+ui->groupBox_showEdit->geometry().left(),
                               geometry().top() + ui->lineEdit_edit_password->geometry().bottom());
        ui->lineEdit_edit_username->setText(validUsername);
        QToolTip::showText(point,"only alphabets, number, <space>, _");
    }

    //enable save button when all 3 fields (teacher ID, firstname and username) are filled correctly
    ui->pushButton_edit_save->setEnabled((check_id.exactMatch(ui->lineEdit_edit_teacherId->text()))
                                    &&(check_fullname.exactMatch(ui->lineEdit_edit_firstName->text()))
                                    &&(check_username.exactMatch(ui->lineEdit_edit_username->text())));
}

//function for button to delete a selected teacher record
void AdminShowTeacher::on_pushButton_delete_clicked()
{
    //set variable id and name which is taken from the temp variables for both (by double click on table)
    QString id, name;
    id = temp_teacherId;
    name = temp_teacherName;

    //ensure user choose by double clicking from table the record to be deleted
    if (id=="")
    {
        QMessageBox::critical(this,"Error","Make sure to double click Teacher(ID, Fullname, or Username) choice");
        return;
    }

    Login connection;
    connection.openConnection();

    //query for erasing the record from the list of teachers
    QSqlQuery eraseAccount;

    //find the record to be deleted
    eraseAccount.prepare("DELETE FROM teacher WHERE TeacherID='"+id+"' and FullName='"+name+"'");

    if (eraseAccount.exec())
    {
        QMessageBox::information(this,"Deleted", "Account deleted");

        //clear the input fields on the "edit" extension window after deleting chosen record
        ui->lineEdit_edit_teacherId->clear();
        ui->lineEdit_edit_firstName->clear();
        ui->lineEdit_edit_midName->clear();
        ui->lineEdit_edit_lastName->clear();
        ui->lineEdit_edit_email->clear();
        ui->lineEdit_edit_username->clear();
        ui->lineEdit_edit_password->clear();

        //prepare the model with query to show the updated database on the table display
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery *show = new QSqlQuery(connection.mydb);
        show->prepare("SELECT * FROM teacher");
        show->exec();
        model->setQuery(*show);
        ui->tableView_loadTeacher->setModel(model);

        //clear the temp_teacherId container so that it will not cause any record to be deleted
        //without user choosing (double-click) from the table
        temp_teacherId = "";
    }
    else
    {
        QMessageBox::critical(this,"Error","Make sure to double click Teacher(ID, Fullname, or Username) choice");
    }
    connection.closeConnection();
}

//function for button to show all teachers entries in order of their registration time
void AdminShowTeacher::on_pushButton_displayAll_clicked()
{
    Login connection;
    connection.openConnection();

    //make new query model
    QSqlQueryModel *model = new QSqlQueryModel();

    //preparing query for selecting all student record based on the order they are inserted
    QSqlQuery *query = new QSqlQuery(connection.mydb);
    query->prepare("SELECT FullName,TeacherID,Department,Gender,Email,Username,Password FROM teacher");
    query->exec();

    //set query to a model and load model onto table view
    model->setQuery(*query);
    ui->tableView_loadTeacher->setModel(model);

    connection.closeConnection();

    qDebug()<<(model->rowCount());
}

//function for button to display sorted list of teachers record alphabetically
void AdminShowTeacher::on_pushButton_sortName_clicked()
{
    Login connection;
    connection.openConnection();
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *sort = new QSqlQuery(connection.mydb);

    //directly using database query to sort the teacher list based on fullname/alphabetically
    sort->prepare("SELECT FullName,TeacherID,Department,Gender,Email,Username,Password from Teacher order by FullName");
    sort->exec();

    //set and load the model into the table for display
    model->setQuery(*sort);
    ui->tableView_loadTeacher->setModel(model);
    connection.closeConnection();
}

//function for button to display sorted list of teachers record based on teacher ID
void AdminShowTeacher::on_pushButton_sortId_clicked()
{
    Login connection;
    connection.openConnection();
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *sort = new QSqlQuery(connection.mydb);

    //directly using database query to sort the teacher list based on ID/numerically
    sort->prepare("SELECT FullName,TeacherID,Department,Gender,Email,Username,Password from Teacher order by TeacherId");
    sort->exec();

    //set and load the model into the table for display
    model->setQuery(*sort);
    ui->tableView_loadTeacher->setModel(model);
    connection.closeConnection();
}

//function for when the input on the middle name field (in the edit window) changes
void AdminShowTeacher::on_lineEdit_edit_midName_textChanged(const QString &input_midName)
{
    //check if input matches the regular expression for teacher midname input
    if (teacherName_rule.exactMatch(input_midName))
    {
        validMidName = input_midName;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the teacher midname
        QPoint point = QPoint (geometry().left()+ui->groupBox_showEdit->geometry().left(),
                               geometry().top() + ui->comboBox_edit_department->geometry().bottom());
        ui->lineEdit_edit_midName->setText(validMidName);
        QToolTip::showText(point,"only alphabets and <space>");
    }
}

//function for when the input on the last name field (in the edit window) changes
void AdminShowTeacher::on_lineEdit_edit_lastName_textChanged(const QString &input_lastName)
{
    //check if input matches the regular expression for teacher lastname input
    if (teacherName_rule.exactMatch(input_lastName))
    {
        validLastName = input_lastName;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the teacher lastname
        QPoint point = QPoint (geometry().left()+ui->groupBox_showEdit->geometry().left(),
                               geometry().top() + ui->comboBox_edit_gender->geometry().bottom());
        ui->lineEdit_edit_lastName->setText(validLastName);
        QToolTip::showText(point,"only alphabets and <space>");
    }
}
