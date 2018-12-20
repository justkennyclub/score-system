#include "studentpage.h"
#include "ui_studentpage.h"
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPaintEvent>

StudentPage::StudentPage(QWidget *parent, QString username, QString password) :
    QDialog(parent),
    ui(new Ui::StudentPage)
{
    ui->setupUi(this);

    //set header font properties of table displayed
    ui->tableView_result->horizontalHeader()->setStyleSheet ("QHeaderView {font: bold 12pt Arial; color: darkblue;}");

    //FOR DESIGN PURPOSE OF THE BUTTONS IN THE UI
    //set up style sheet and interaction of the buttons with user's action
    ui->pushButton_logout->setStyleSheet("QPushButton#pushButton_logout {background-color: transparent;}"
                                         "QPushButton#pushButton_logout:hover {background-color: rgb(255,160,122);};");
    ui->pushButton_print->setStyleSheet("QPushButton#pushButton_print {background-color: transparent;}"
                                         "QPushButton#pushButton_print:hover {background-color: rgb(211,233,220);};");
    ui->pushButton_chooseElective->setStyleSheet("QPushButton#pushButton_chooseElective {background-color: transparent;}"
                                         "QPushButton#pushButton_chooseElective:hover {background-color: rgb(211,233,220);};");

    //set the tool tip for displaying information of buttons to user
    ui->pushButton_logout->setToolTip("Log out");
    ui->pushButton_chooseElective->setToolTip("Choose Elective");
    ui->pushButton_print->setToolTip("Save to text file for Printing");

    Login connection;
    connection.openConnection();
    QSqlQuery match;

    //search database for record with the same username and password as the one passed to this constructor
    match.exec("SELECT * FROM student WHERE Username='"+username+"' and Password='"+password+"'");

    //search from the first entry
    while (match.next())
    {
        //put the obtained data into the labels for display
        ui->label_id->setText(match.value(1).toString());
        studentId = match.value(1).toString();
        ui->label_fullname->setText(match.value(0).toString());
        fullname = match.value(0).toString();
        fullname.replace(" ", "_");
        ui->label_gender->setText(match.value(3).toString());
        ui->label_major->setText(match.value(2).toString());
        major = match.value(2).toString();
        major.replace(" ","_");
        if (match.value(4).toString() == "")
        {
            ui->label_email->setText("-");
        }
        else
        {
            ui->label_email->setText(match.value(4).toString());
        }
        ui->label_username_2->setText(match.value(5).toString());
    }

    QSqlQuery searchGrade;
    //get all the filled grade point for courses taken
    searchGrade.prepare("SELECT * FROM '"+fullname+"_"+studentId+"' WHERE Grade_Point='0' "
                     "or Grade_Point='2.0' or Grade_Point='2.3' or Grade_Point='2.5' or Grade_Point='2.7' "
                     "or Grade_Point='3.0' or Grade_Point='3.2' or Grade_Point='3.5' or Grade_Point='3.7' or Grade_Point='4.0'");

    //double type for GPA
    double gradeAverage;

    //string type for GPA
    QString GPA;
    if (searchGrade.exec())
    {
        int count=0;
        double total=0;
        while(searchGrade.next())
        {
            //add each grade found to a total variable
            total += searchGrade.value(6).toString().toDouble();
            count++;
        }
        //the GPA is calculated by dividing the total grade by the total course with the grade filled (count)
        gradeAverage = total/count;
    }
    else
    {
        QMessageBox::critical(this, "Error", "Cannot obtain GPA");
    }

    //display GPA to two decimal points if it is not equal to 0 (no course results filled yet)
    GPA = QString::number(gradeAverage,'f',2);      //truncate to 2 dp
    if (GPA!="nan")
    {
        ui->label_GPA->setText(GPA);
    }
    else
    {
        ui->label_GPA->setText("-");
    }

    //set and load model with query of an individual student's results into the table for display
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *query = new QSqlQuery(connection.mydb);
    query->prepare("SELECT * FROM '"+fullname+"_"+studentId+"'");
    query->exec();
    model->setQuery(*query);
    ui->tableView_result->setModel(model);

    connection.closeConnection();
}

//destructor for StudentPage
StudentPage::~StudentPage()
{
    delete ui;
}

//function for logout button to go back to login page
void StudentPage::on_pushButton_logout_clicked()
{
    //close the current window
    close();
}

//function for button to allow students to go to choose elective for next semester
void StudentPage::on_pushButton_chooseElective_clicked()
{
    Login connection;

    if (!connection.openConnection())
    {
        qDebug()<<"Fail to open database";
        return;
    }

    //connect to database
    connection.openConnection();

    fullname.replace(" ","_");
    QSqlQuery query;

    //prepare query to database to look for elective courses
    query.prepare("select Course from '"+fullname+"_"+studentId+"' WHERE type='elective'");

    if(query.exec())
    {
        //count the number of electives already registered by a student
        int count=0;
        while(query.next())
        {
            count++;
        }

        QWidget *parent = 0;
        //depending on the student's major and the number of electives already registered,
        //the student user can either go or not go (not allowed) to "choose electives" page
        if (major == "Architecture")
        {
            if (count<3)
            {
                connection.closeConnection();

                //the student's information and major are also passed to the constructor of the new object (page)
                //for allowing the page to connect to the specific student database as well as displaying the available electives
                ChooseElective page(parent,fullname,studentId, major);
                page.setModal(true);
                page.exec();
            }
            else
            {
                QMessageBox::critical(this,"Warning","You already have enough electives!");
            }
        }
        else if (major == "Business_Management")
        {
            if (count<8)
            {
                connection.closeConnection();

                //the student's information and major are also passed to the constructor of the new object (page)
                //for allowing the page to connect to the specific student database as well as displaying the available electives
                connection.closeConnection();
                ChooseElective page(parent,fullname,studentId, major);
                page.setModal(true);
                page.exec();
            }
            else
            {
                QMessageBox::critical(this,"Warning","You already have enough electives!");
            }
        }
        else if (major == "Computer_Science")
        {
            if (count<5)
            {
                connection.closeConnection();

                //the student's information and major are also passed to the constructor of the new object (page)
                //for allowing the page to connect to the specific student database as well as displaying the available electives
                connection.closeConnection();
                ChooseElective page(parent,fullname,studentId, major);
                page.setModal(true);
                page.exec();
            }
            else
            {
                QMessageBox::critical(this,"Warning","You already have enough electives!");
            }
        }
        else if (major == "Economics")
        {
            if (count<5)
            {
                connection.closeConnection();

                //the student's information and major are also passed to the constructor of the new object (page)
                //for allowing the page to connect to the specific student database as well as displaying the available electives
                connection.closeConnection();
                ChooseElective page(parent,fullname,studentId, major);
                page.setModal(true);
                page.exec();
            }
            else
            {
                QMessageBox::critical(this,"Warning","You already have enough electives!");
            }
        }
        else if (major == "Mechanical_Engineering")
        {
            if (count<4)
            {
                connection.closeConnection();

                //the student's information and major are also passed to the constructor of the new object (page)
                //for allowing the page to connect to the specific student database as well as displaying the available electives
                connection.closeConnection();
                ChooseElective page(parent,fullname,studentId, major);
                page.setModal(true);
                page.exec();
            }
            else
            {
                QMessageBox::critical(this,"Warning","You already have enough electives!");
            }
        }
    }
    else
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
    }
}

//function for button to save results in a text file for printing
void StudentPage::on_pushButton_print_clicked()
{
    Login connection;

    if (!connection.openConnection())
    {
        qDebug()<<"Fail to open database";
        return;
    }

    connection.openConnection();

    QSqlQuery searchGrade;
    //get all the filled grade point for courses taken
    searchGrade.prepare("SELECT * FROM '"+fullname+"_"+studentId+"' WHERE Grade_Point='0' "
                     "or Grade_Point='2.0' or Grade_Point='2.3' or Grade_Point='2.5' or Grade_Point='2.7' "
                     "or Grade_Point='3.0' or Grade_Point='3.2' or Grade_Point='3.5' or Grade_Point='3.7' or Grade_Point='4.0'");
    double gradeAverage;
    QString GPA;
    if (searchGrade.exec())
    {
        int count=0;
        double total=0;
        while(searchGrade.next())
        {
            //add each grade found to a total variable
            total += searchGrade.value(6).toString().toDouble();
            count++;
        }
        //the GPA is calculated by dividing the total grade by the total course which grade is filled (count)
        gradeAverage = total/count;
    }
    else
    {
        QMessageBox::critical(this, "Error", "Cannot obtain GPA");
    }

    //display GPA to two decimal points if it is not equal to 0 (no course results filled yet)
    GPA = QString::number(gradeAverage,'f',2);              //truncate to 2 dp
    if (GPA=="nan")
    {
        GPA = "-";
    }

    //set and load model with query of an individual student's results into the table for display
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *query = new QSqlQuery(connection.mydb);
    query->prepare("SELECT * FROM '"+fullname+"_"+studentId+"'");
    query->exec();
    model->setQuery(*query);

    //textData is for holding course name
    //rowData is for holding the whole string of a row in database
    QString textData, rowData;
    double scores;
    int rows = model->rowCount();
    int columns  = model->columnCount()-1;  //don't print "type" of course

    //open or create a new text file
    QFile txtFile("/Users/J/Desktop/Score System NEW/Print Students/"+fullname+"_"+studentId+".txt");
    if (txtFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QTextStream out(&txtFile);
        out<<"Student ID: "<<studentId<<endl;
        out<<"Fullname  : "<<fullname.replace("_"," ")<<endl;
        out<<"Major     : "<<major<<endl<<endl;
        out<<"GPA: "<<GPA<<endl<<endl;
        out<<"\t\tCourse\t\t\t    Score\t    GradePoint\n";

        //loop through the database table per cell in the same row
        for (int i=0; i<rows; i++)
        {
            rowData = "";
            for (int j=0; j<columns; j++)
            {
                //only considers index column 0, third last and second last column
                //which contains name of course, total score and grade point respectively for printing
                if (j==0)
                {
                    //get the string for course name, which is in index column 0 for every row in database
                    textData = model->data(model->index(i,j)).toString();

                    //add length to synchronize course name length for .txt file arrangement
                    if (textData.size()<40 && textData.size()>4)
                    {
                        for (int i=textData.size(); i<40; i++)
                        {
                            textData += " ";
                        }
                    }

                    //add to rowData the course name
                    rowData += textData;
                }
                else if (j==columns-2 || j==columns-1)
                {
                    //get the string for total score and grade point, and convert them to double type variable
                    scores = model->data(model->index(i,j)).toDouble();

                    //assign back the string type of the score and grade point to the variable 'marks'
                    QString marks = QString::number(scores);
                    if (scores==0)
                    {
                        marks = "-   \t\t ";
                    }
                    else
                    {
                        if (marks.size()==1)
                        {
                            marks += "   \t\t ";
                        }
                        else if (marks.size()==2)
                        {
                            marks += "  \t\t ";
                        }
                        else if (marks.size()==3)
                        {
                            marks += " \t\t ";
                        }
                        else
                            marks += "\t\t ";
                    }
                    //add to rowData the data for total score and grade point saved in 'marks' variable
                    rowData += marks;
                }
            }
            //to align the numbering in text file between unit and tenth
            QString space = (i+1<10) ? " " : "";
            out<<space<<i+1<<". "<<rowData<<"\n";
        }
        out<<"\nThis report is only valid with the official stamp from Springfield University of Science.";
        txtFile.close();
    }
    //notify users for successful printing of results in text file
    QMessageBox::information(this, "Print", "Results saved in .txt file for printing");
    connection.closeConnection();
}
