#include "teacherpage.h"
#include "ui_teacherpage.h"
#include <QMessageBox>

TeacherPage::TeacherPage(QWidget *parent, QString username, QString password) :
    QDialog(parent),
    ui(new Ui::TeacherPage)
{
    ui->setupUi(this);

    ui->tableView_student->resizeRowsToContents();
    ui->tableView_student->resizeColumnsToContents();

    //set header font properties of table displayed
    ui->tableView_student->horizontalHeader()->setStyleSheet ("QHeaderView {font: bold 12pt Arial; color: darkblue;}");

    //FOR DESIGN PURPOSE OF THE BUTTONS IN THE UI
    //set up style sheet and interaction of the buttons with user's action
    //set the tool tip for displaying information of buttons to user
    ui->pushButton_logout->setStyleSheet("QPushButton#pushButton_logout {background-color: transparent;}"
                                         "QPushButton#pushButton_logout:hover {background-color: rgb(255,160,122);};");
    ui->pushButton_logout->setToolTip("Log out");
    ui->pushButton_view->setStyleSheet("QPushButton#pushButton_view {background-color: transparent;}"
                                       "QPushButton#pushButton_view:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_view->setToolTip("View Record");

    Login connection;
    connection.openConnection();
    QSqlQuery match;
    //search database for record with the same username and password as the one passed to this constructor
    match.exec("SELECT * FROM teacher WHERE Username='"+username+"' and Password='"+password+"'");

    //search from the first entry
    while (match.next())
    {
        //put the obtained data into the labels for display
        ui->label_teacherId->setText(match.value(1).toString());
        teacherId = match.value(1).toString();
        ui->label_fullname->setText(match.value(0).toString());
        name = match.value(0).toString();
        name.replace(" ", "_");
        ui->label_gender->setText(match.value(3).toString());
        ui->label_department->setText(match.value(2).toString());
        department = match.value(2).toString();
        if (match.value(4).toString() == "")
        {
            ui->label_email->setText("-");
        }
        else
        {
            ui->label_email->setText(match.value(4).toString());
        }
        ui->label_username->setText(match.value(5).toString());
    }

    //create a new query model
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *query = new QSqlQuery(connection.mydb);
    //prepare query for selecting all the students record with the same major as the teacher's department
    query->prepare("SELECT Fullname,StudentID,Major,Gender,Email FROM student WHERE Major='"+department+"'");
    query->exec();

    //set model to contain query
    model->setQuery(*query);

    //load the model with query into the table to display student list record
    ui->tableView_student->setModel(model);

    connection.closeConnection();
}

//destructor for TeacherPage
TeacherPage::~TeacherPage()
{
    delete ui;
}

//function for logout button to go back to login function
void TeacherPage::on_pushButton_logout_clicked()
{
    //close the current window
    close();
}

//Search function based on one category, updated to table in real time as text input changes
void TeacherPage::on_lineEdit_search_textChanged(const QString &input)
{
    Login connection;
    connection.openConnection();

    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery *search = new QSqlQuery(connection.mydb);

    //check which radiobutton is checked, then perform search based on the checked category
    if (ui->radioButton_studentId->isChecked())
    {
        search->exec("SELECT Fullname,StudentID,Major,Gender,Email FROM student WHERE Major='"+department+"' and (StudentID like '"+input+"%' "
                     "or StudentID like '_____%"+input+"%') order by StudentID");
    }
    else if (ui->radioButton_fullname->isChecked())
    {
        search->exec("SELECT Fullname,StudentID,Major,Gender,Email FROM student WHERE Major='"+department+"' and (FullName like '"+input+"%' "
                     "or MidName like '"+input+"%' or LastName like '"+input+"%') order by Fullname");
    }
    else if (ui->radioButton_gender->isChecked())
    {
        search->exec("SELECT Fullname,StudentID,Major,Gender,Email FROM student WHERE Gender like '"+input+"%' "
                     "AND Major='"+department+"'");
    }
    else if (ui->radioButton_email->isChecked())
    {
        search->exec("SELECT Fullname,StudentID,Major,Gender,Email FROM student WHERE Email like '%"+input+"%' "
                     "AND Major='"+department+"'");
    }

    //save query to model
    model->setQuery(*search);

    //load the model into the table to display list of student record based on search categories
    ui->tableView_student->setModel(model);

    connection.closeConnection();

    qDebug()<<(model->rowCount());
}

//function for allowing interaction (double-click to select) with the list of students in the table
void TeacherPage::on_tableView_student_activated(const QModelIndex &index)
{
    //get the index of the selected row in the table display
    QString value = ui->tableView_student->model()->data(index).toString();

    Login connection;
    if (!connection.openConnection())
    {
        qDebug()<<"Failed to open database";
        return;
    }
    connection.openConnection();

    //prepare a query to database which find a match for the content of the index in the table
    QSqlQuery query;
    query.prepare("SELECT Fullname,StudentID,Major,Gender,Email FROM student WHERE StudentId='"+value+"' "
                  "or FullName='"+value+"'");

    if (query.exec())
    {
        //search from the beginning of the database, incrementing one entry/row each time
        /*take the ID and name information from the entry that match the choice of user,
          and put them to the selected label*/
        while(query.next())
        {
            ui->label_selected->setText(query.value(1).toString() + "_" + query.value(0).toString());
            //the selected ID and name are noted to pass to the next page for access to the student's results database
            selectedId = query.value(1).toString();
            selectedName = query.value(0).toString();
        }
        connection.closeConnection();
    }
    else
    {
        QMessageBox::critical(this, "Error::", query.lastError().text());
    }
}

//function for view button to view the detail information of a selected student record
void TeacherPage::on_pushButton_view_clicked()
{
    //if a student record is selected (double-clicked) from table, new object of ViewStudent will be created
    //to set up window for its interface
    if (ui->label_selected->text()!="-")
    {
        QWidget *parent = 0;
        //selectedId and selectedName are passed to the constructor for access to the student's results database
        ViewStudent page(parent,selectedId, selectedName);
        page.setModal(true);
        page.exec();
    }

    //notify user to double click from table for viewing a specific student record
    else
    {
        QMessageBox::critical(this, "Warning", "Please select a student from table to view!\nDouble click on the desired student ID or name!");
        return;
    }
}
