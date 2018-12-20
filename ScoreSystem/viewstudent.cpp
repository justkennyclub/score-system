#include "viewstudent.h"
#include "ui_viewstudent.h"
#include <QMessageBox>
#include <QToolTip>

ViewStudent::ViewStudent(QWidget *parent, QString id, QString name) :
    QDialog(parent),
    ui(new Ui::ViewStudent)
{
    ui->setupUi(this);

    //FOR DESIGN PURPOSE OF THE BUTTONS IN THE UI
    //set up style sheet and interaction of the buttons with user's action
    ui->pushButton_print->setStyleSheet("QPushButton#pushButton_print {background-color: transparent;}"
                                       "QPushButton#pushButton_print:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_back->setStyleSheet("QPushButton#pushButton_back {background-color: transparent;}"
                                       "QPushButton#pushButton_back:hover {background-color: rgb(255,160,122);};");
    ui->pushButton_delete->setStyleSheet("QPushButton#pushButton_delete {background-color: transparent;}"
                                       "QPushButton#pushButton_delete:hover {background-color: rgb(255,160,122);};");
    ui->pushButton_displayAll->setStyleSheet("QPushButton#pushButton_displayAll {background-color: transparent;}"
                                       "QPushButton#pushButton_displayAll:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_edit_save->setStyleSheet("QPushButton#pushButton_edit_save {background-color: transparent;}"
                                       "QPushButton#pushButton_edit_save:hover {background-color: rgb(152,251,152);};");
    ui->pushButton_addingSubject->setStyleSheet("QPushButton#pushButton_addingSubject {background-color: transparent;}"
                                       "QPushButton#pushButton_addingSubject:hover {background-color: rgb(152,251,152);};");

    //set the tool tip for displaying information of buttons to user
    ui->pushButton_print->setToolTip("Save to text file for printing");
    ui->pushButton_back->setToolTip("Back");
    ui->pushButton_delete->setToolTip("Delete selected course");
    ui->pushButton_displayAll->setToolTip("Display all");
    ui->pushButton_edit_save->setToolTip("Save");
    ui->pushButton_addingSubject->setToolTip("Add the new course");

    //set header font properties of table displayed
    ui->tableView_studentResult->horizontalHeader()->setStyleSheet ("QHeaderView {font: bold 12pt Arial; color: darkblue;}");

    //"Add Subject" extension dialog implementation
    //set extension window for "add subject" to be hidden
    showAdd = false;
    ui->groupBox_addSubject->setVisible(showAdd);

    //set layout to be unresizable
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    Login connection;
    connection.openConnection();
    QSqlQuery match;
    //search database for record with the same username and password as the one passed to this constructor
    match.exec("SELECT * FROM student WHERE StudentId='"+id+"' and FullName='"+name+"'");

    //search from the first entry
    while (match.next())
    {
        //put the obtained data into the labels for display
        ui->label_studentId->setText(match.value(1).toString());
        studentId = match.value(1).toString();
        ui->label_fullname->setText(match.value(0).toString());
        fullname = match.value(0).toString();
        fullname.replace(" ", "_");
        ui->label_gender->setText(match.value(3).toString());
        ui->label_major->setText(match.value(2).toString());
        major = match.value(2).toString();
        major.replace(" ","_");
        ui->label_email->setText(match.value(4).toString());
        ui->label_username_2->setText(match.value(5).toString());
    }

    //set and load model with query of an individual student's results into the table for display
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *query = new QSqlQuery(connection.mydb);
    query->prepare("SELECT * FROM '"+fullname+"_"+studentId+"'");
    query->exec();
    model->setQuery(*query);
    ui->tableView_studentResult->setModel(model);

    //input items for combo box of "type" in the "Edit" dialog
    ui->comboBox_editType->addItem("");
    ui->comboBox_editType->addItem("compulsory");
    ui->comboBox_editType->addItem("elective");

    //input items for combo box of "type" in the "Add Subject" extension dialog
    ui->comboBox_addType->addItem("compulsory");
    ui->comboBox_addType->addItem("elective");

    connection.closeConnection();

    //declaration of the connect function to link the signal when there is an input change on Regular,
    //Coursework, Midterm, FinalExam and Total fields to each corrsponding slots for checking of their input validity
    QObject::connect(ui->lineEdit_editRegular,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_editRegular_textChanged(QString)));
    QObject::connect(ui->lineEdit_editCoursework,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_editCoursework_textChanged(QString)));
    QObject::connect(ui->lineEdit_editMidterm,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_editMidterm_textChanged(QString)));
    QObject::connect(ui->lineEdit_editFinalExam,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_editFinalExam_textChanged(QString)));
    QObject::connect(ui->lineEdit_editTotal,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_editTotal_textChanged(QString)));
}

//destructor for ViewStudent
ViewStudent::~ViewStudent()
{
    delete ui;
}

//function to show and hide 'Add Subject' extension window
void ViewStudent::on_pushButton_addSubject_clicked()
{
    showAdd = !showAdd;
    ui->groupBox_addSubject->setVisible(showAdd);

    //change the text everytime the "Add Subject" button is toggled
    QString text = "Add Subject";
    text += showAdd ? "<<<" : ">>>";
    ui->pushButton_addSubject->setText(text);
}

//Search function based on one category, updated to table in real time as text input changes
void ViewStudent::on_lineEdit_search_textChanged(const QString &input)
{
    Login connection;
    connection.openConnection();

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *search = new QSqlQuery(connection.mydb);

    //check which radiobutton is checked, then perform search based on the checked category
    if (ui->radioButton_course->isChecked())
    {
        search->exec("SELECT * FROM '"+fullname+"_"+studentId+"' WHERE Course like '%"+input+"%'");
    }
    else if (ui->radioButton_regular->isChecked())
    {
        search->exec("SELECT * FROM '"+fullname+"_"+studentId+"' WHERE Regular like '%"+input+"%'");
    }
    else if (ui->radioButton_coursework->isChecked())
    {
        search->exec("SELECT * FROM '"+fullname+"_"+studentId+"' WHERE Coursework like '%"+input+"%'");
    }
    else if (ui->radioButton_midTerm->isChecked())
    {
        search->exec("SELECT * FROM '"+fullname+"_"+studentId+"' WHERE Mid_Term like '%"+input+"%'");
    }
    else if (ui->radioButton_finalExam->isChecked())
    {
        search->exec("SELECT * FROM '"+fullname+"_"+studentId+"' WHERE Final_Exam like '%"+input+"%'");
    }
    else if (ui->radioButton_total->isChecked())
    {
        search->exec("SELECT * FROM '"+fullname+"_"+studentId+"' WHERE Total like '%"+input+"%'");
    }
    else if (ui->radioButton_gradePoint->isChecked())
    {
        search->exec("SELECT * FROM '"+fullname+"_"+studentId+"' WHERE Grade_Point like '%"+input+"%'");
    }
    else if (ui->radioButton_type->isChecked())
    {
        search->exec("SELECT * FROM '"+fullname+"_"+studentId+"' WHERE Type like '%"+input+"%'");
    }

    //save query to model
    model->setQuery(*search);

    //load the model into the table to display list of student results record based on search categories
    ui->tableView_studentResult->setModel(model);

    connection.closeConnection();

    qDebug()<<(model->rowCount());
}

//function for allowing interaction (double-click to select) with the list of a student's results
void ViewStudent::on_tableView_studentResult_activated(const QModelIndex &index)
{
    //get the index of the selected row in the table display
    QString value = ui->tableView_studentResult->model()->data(index).toString();

    Login connection;
    if (!connection.openConnection())
    {
        qDebug()<<"Failed to open database";
        return;
    }
    connection.openConnection();

    //prepare a query to database which find a match for the content of the index in the table
    QSqlQuery query;
    query.prepare("SELECT * FROM '"+fullname+"_"+studentId+"' WHERE Course='"+value+"'");

    if (query.exec())
    {
        //search from the beginning of the database, incrementing one entry/row each time
        /*take all information from the entry that match the choice of user, and put them to the input field
         for user to edit*/
        while(query.next())
        {
            ui->lineEdit_editCourse->setText(query.value(0).toString());
            if (query.value(1).toString()!="")
                ui->lineEdit_editRegular->setText(QString::number(query.value(1).toDouble()));
            else
                ui->lineEdit_editRegular->setText(query.value(1).toString());

            if (query.value(2).toString()!="")
                ui->lineEdit_editCoursework->setText(QString::number(query.value(2).toDouble()));
            else
                ui->lineEdit_editCoursework->setText(query.value(2).toString());

            if (query.value(3).toString()!="")
                ui->lineEdit_editMidterm->setText(QString::number(query.value(3).toDouble()));
            else
                ui->lineEdit_editMidterm->setText(query.value(3).toString());

            if (query.value(4).toString()!="")
                ui->lineEdit_editFinalExam->setText(QString::number(query.value(4).toDouble()));
            else
                ui->lineEdit_editFinalExam->setText(query.value(4).toString());

            if (query.value(5).toString()!="")
                ui->lineEdit_editTotal->setText(QString::number(query.value(5).toDouble()));
            else
                ui->lineEdit_editTotal->setText(query.value(5).toString());

            ui->comboBox_editType->setCurrentText(query.value(7).toString());

            //temp variables are for noting the previous version of the database content for the course name
            temp_course = query.value(0).toString();
        }
        connection.closeConnection();
    }
    else
    {
        QMessageBox::critical(this, "Error::", query.lastError().text());
    }
}

//function for back button to go back to previous page
void ViewStudent::on_pushButton_back_clicked()
{
    //close the current window and show the previous page/window
    close();
}

//function for save button in "edit/modify" window to save modification on students' results
void ViewStudent::on_pushButton_edit_save_clicked()
{
    //get all the input from user
    QString course, regular, coursework, midTerm, final, total, type;
    course = ui->lineEdit_editCourse->text();
    regular = ui->lineEdit_editRegular->text();
    coursework = ui->lineEdit_editCoursework->text();
    midTerm = ui->lineEdit_editMidterm->text();
    final = ui->lineEdit_editFinalExam->text();
    total = ui->lineEdit_editTotal->text();
    type = ui->comboBox_editType->currentText();

    //prevent the course field from being empty
    if (course=="")
    {
        QMessageBox::critical(this, "Warning", "Please select appropriate course from the table!\nDouble click on the desired course name!");
        return;
    }

    double gradePoint;
    double totalScore;
    QString grade;
    if (total!="")
    {
        totalScore = total.toDouble();
        //determine the gradePoint for the Total Score input
        if (totalScore>=90)
        {
            gradePoint = 4.0;
        }
        else if (totalScore<90 && totalScore>=88)
        {
            gradePoint = 3.7;
        }
        else if (totalScore<88 && totalScore>=85)
        {
            gradePoint = 3.5;
        }
        else if (totalScore<85 && totalScore>=83)
        {
            gradePoint = 3.2;
        }
        else if (totalScore<83 && totalScore>=80)
        {
            gradePoint = 3.0;
        }
        else if (totalScore<80 && totalScore>=77)
        {
            gradePoint = 2.7;
        }
        else if (totalScore<77 && totalScore>=75)
        {
            gradePoint = 2.5;
        }
        else if (totalScore<75 && totalScore>=70)
        {
            gradePoint = 2.3;
        }
        else if (totalScore<70 && totalScore>=60)
        {
            gradePoint = 2.0;
        }
        else if (totalScore<60)
        {
            gradePoint = 0;
        }
        grade = QString::number(gradePoint);
    }


    Login connection;
    if (!connection.openConnection())
    {
        qDebug()<<"Fail to open database";
        return;
    }
    connection.openConnection();

    //if temp_course is "", it means user has not double clicked from the table to choose the entry to be edited
    if (temp_course=="")
    {
        QMessageBox::critical(this, "Error","Please select appropriate course from the table!\nDouble click on the desired course name!");
        return;
    }

    QSqlQuery query;

    //prepare the update code to database by comparing with previous version through the temp_course variable
    query.prepare("UPDATE '"+fullname+"_"+studentId+"' SET Course='"+course+"',Regular='"+regular+"',"
                  "Coursework='"+coursework+"',Mid_Term='"+midTerm+"',Final_Exam='"+final+"',"
                  "Total='"+total+"',Grade_Point='"+grade+"',Type='"+type+"' WHERE Course='"+temp_course+"'");

    if (query.exec())
    {
        QMessageBox::information(this, tr("Save"),tr("Saved Successfully"));

        //clear the input fields in the "edit" window after saving
        ui->lineEdit_editCourse->clear();
        ui->lineEdit_editRegular->clear();
        ui->lineEdit_editCoursework->clear();
        ui->lineEdit_editMidterm->clear();
        ui->lineEdit_editFinalExam->clear();
        ui->lineEdit_editTotal->clear();
        ui->comboBox_editType->setCurrentText("");

        //clear the temp_course container so that it will not cause any record to be modified
        //without user choosing (double-click) from the table
        temp_course = "";

        //update tableview directly
        //prepare the model with query to show the updated database on the table display
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery *show = new QSqlQuery(connection.mydb);
        show->prepare("SELECT * FROM '"+fullname+"_"+studentId+"'");
        show->exec();
        model->setQuery(*show);
        ui->tableView_studentResult->setModel(model);

        connection.closeConnection();
    }

    //notify users with pop-up for their specific error in making changes to a specific record in database
    else
    {
        QMessageBox::critical(this, tr("error::"),query.lastError().text());
    }
}

//regular expression to validate the input on each input fields in "Edit" window
QRegExp score_rule("[0-9.]*");

//function for when the input on the regular score field (in edit window) changes
void ViewStudent::on_lineEdit_editRegular_textChanged(QString input_regular)
{
    //check if input matches the regular expression for regular score input
    if (score_rule.exactMatch(input_regular))
    {
        correctRegular = input_regular;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the regular score input
        QPoint point = QPoint (geometry().left()+ui->groupBox_modify->geometry().left(),
                               geometry().top() + ui->lineEdit_editTotal->geometry().bottom());
        ui->lineEdit_editRegular->setText(correctRegular);
        QToolTip::showText(point,"only input number");
    }
}

//function for when the input on the coursework score field (in edit window) changes
void ViewStudent::on_lineEdit_editCoursework_textChanged(QString input_coursework)
{
    //check if input matches the regular expression for coursework score input
    if (score_rule.exactMatch(input_coursework))
    {
        correctCoursework = input_coursework;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the coursework score input
        QPoint point = QPoint (geometry().left()+ui->groupBox_modify->geometry().left(),
                               geometry().top() + ui->lineEdit_editTotal->geometry().bottom());
        ui->lineEdit_editCoursework->setText(correctCoursework);
        QToolTip::showText(point,"only input number");
    }
}

//function for when the input on the mid term score field (in edit window) changes
void ViewStudent::on_lineEdit_editMidterm_textChanged(QString input_midterm)
{
    //check if input matches the regular expression for midterm score input
    if (score_rule.exactMatch(input_midterm))
    {
        correctMidterm = input_midterm;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the midterm score input
        QPoint point = QPoint (geometry().left()+ui->groupBox_modify->geometry().left(),
                               geometry().top() + ui->lineEdit_editTotal->geometry().bottom());
        ui->lineEdit_editMidterm->setText(correctMidterm);
        QToolTip::showText(point,"only input number");
    }
}

//function for when the input on the final exam score field (in edit window) changes
void ViewStudent::on_lineEdit_editFinalExam_textChanged(QString input_final)
{
    //check if input matches the regular expression for final exam score input
    if (score_rule.exactMatch(input_final))
    {
        correctFinal = input_final;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the final exam score input
        QPoint point = QPoint (geometry().left()+ui->groupBox_modify->geometry().left(),
                               geometry().top() + ui->lineEdit_editTotal->geometry().bottom());
        ui->lineEdit_editFinalExam->setText(correctFinal);
        QToolTip::showText(point,"only input number");
    }
}

//function for when the input on the total score field (in edit window) changes
void ViewStudent::on_lineEdit_editTotal_textChanged(QString input_total)
{
    //check if input matches the regular expression for total score input
    if (score_rule.exactMatch(input_total))
    {
        correctTotal = input_total;
        QToolTip::hideText();
    }
    else
    {
        //set in the input field the last acceptable input before the unacceptable input is pressed and
        //set tool tip to notify user directly what is the acceptable input for the total score input
        QPoint point = QPoint (geometry().left()+ui->groupBox_modify->geometry().left(),
                               geometry().top() + ui->lineEdit_editTotal->geometry().bottom());
        ui->lineEdit_editTotal->setText(correctTotal);
        QToolTip::showText(point,"only input number");
    }
}

//function for button to add a new course/subject that a student takes
void ViewStudent::on_pushButton_addingSubject_clicked()
{
    //get all the input from user
    QString course, regular, coursework, midterm, finalExam, total, type;
    course = ui->lineEdit_addCourse->text();
    regular = ui->lineEdit_addRegular->text();
    coursework = ui->lineEdit_addCoursework->text();
    midterm = ui->lineEdit_addMidterm->text();
    finalExam = ui->lineEdit_addFinalExam->text();
    total = ui->lineEdit_addTotal->text();
    type = ui->comboBox_addType->currentText();

    //prevent the course field from being empty
    if (course=="")
    {
        QMessageBox::critical(this, "Warning", "Please enter the Course name!");
        return;
    }

    double gradePoint;
    double totalScore;
    QString grade;
    if (total!="")
    {
        totalScore = total.toDouble();
        //determine the gradePoint for the Total Score input
        if (totalScore>=90)
        {
            gradePoint = 4.0;
        }
        else if (totalScore<90 && totalScore>=88)
        {
            gradePoint = 3.7;
        }
        else if (totalScore<88 && totalScore>=85)
        {
            gradePoint = 3.5;
        }
        else if (totalScore<85 && totalScore>=83)
        {
            gradePoint = 3.2;
        }
        else if (totalScore<83 && totalScore>=80)
        {
            gradePoint = 3.0;
        }
        else if (totalScore<80 && totalScore>=77)
        {
            gradePoint = 2.7;
        }
        else if (totalScore<77 && totalScore>=75)
        {
            gradePoint = 2.5;
        }
        else if (totalScore<75 && totalScore>=70)
        {
            gradePoint = 2.3;
        }
        else if (totalScore<70 && totalScore>=60)
        {
            gradePoint = 2.0;
        }
        else if (totalScore<60)
        {
            gradePoint = 0;
        }
        grade = QString::number(gradePoint);
    }

    Login connection;
    if (!connection.openConnection())
    {
        qDebug()<<"Fail to open database";
        return;
    }
    connection.openConnection();

    QSqlQuery query;

    //prepare the update code to database to insert new course subject
    query.prepare("INSERT INTO '"+fullname+"_"+studentId+"' (Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) "
                  "values ('"+course+"','"+regular+"','"+coursework+"','"+midterm+"','"+finalExam+"','"+total+"','"+grade+"','"+type+"')");

    if (query.exec())
    {
        QMessageBox::information(this, tr("Add"),tr("Added Successfully"));

        //clear the input fields in the "edit" window after saving
        ui->lineEdit_addCourse->clear();
        ui->lineEdit_addRegular->clear();
        ui->lineEdit_addCoursework->clear();
        ui->lineEdit_addMidterm->clear();
        ui->lineEdit_addFinalExam->clear();
        ui->lineEdit_addTotal->clear();

        //update tableview directly
        //prepare the model with query to show the updated database on the table display
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery *show = new QSqlQuery(connection.mydb);
        show->prepare("SELECT * FROM '"+fullname+"_"+studentId+"'");
        show->exec();
        model->setQuery(*show);
        ui->tableView_studentResult->setModel(model);

        connection.closeConnection();
    }

    //notify users with pop-up for their specific error in making changes to a specific record in database
    else
    {
        QMessageBox::critical(this, tr("error::"),query.lastError().text());
    }
}

//function for button to delete a particular student's results and the course taken
void ViewStudent::on_pushButton_delete_clicked()
{
    //get all the input from user
    QString course, regular, coursework, midTerm, final, total, type;
    course = ui->lineEdit_editCourse->text();
    regular = ui->lineEdit_editRegular->text();
    coursework = ui->lineEdit_editCoursework->text();
    midTerm = ui->lineEdit_editMidterm->text();
    final = ui->lineEdit_editFinalExam->text();
    total = ui->lineEdit_editTotal->text();
    type = ui->comboBox_editType->currentText();

    //prevent the course field from being empty
    if (course=="")
    {
        QMessageBox::critical(this,"Error","Make sure to double click Course name choice");
        return;
    }

    Login connection;
    if (!connection.openConnection())
    {
        qDebug()<<"Fail to open database";
        return;
    }
    connection.openConnection();

    //if temp_course is "", it means user has not double clicked from the table to choose the entry to be edited
    if (temp_course=="")
    {
        QMessageBox::critical(this, "Error","Please select appropriate course from the table!\nDouble click on the desired course name!");
        return;
    }

    QSqlQuery eraseAccount;
    eraseAccount.prepare("DELETE FROM '"+fullname+"_"+studentId+"' WHERE Course='"+temp_course+"' "
                         "and Regular='"+regular+"' and Coursework='"+coursework+"' and Mid_Term='"+midTerm+"' "
                         "and Final_Exam='"+final+"' and Total='"+total+"' and Type='"+type+"'");

    if (eraseAccount.exec())
    {
        QMessageBox::information(this,"Deleted", "Selected Course deleted");

        //clear all the input fields after deletion
        ui->lineEdit_editCourse->clear();
        ui->lineEdit_editRegular->clear();
        ui->lineEdit_editCoursework->clear();
        ui->lineEdit_editMidterm->clear();
        ui->lineEdit_editFinalExam->clear();
        ui->lineEdit_editTotal->clear();
        ui->comboBox_editType->setCurrentText("");

        //clear the temp_course container so that it will not cause any record to be modified
        //without user choosing (double-click) from the table
        temp_course = "";

        //update tableview directly
        //prepare the model with query to show the updated database on the table display
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery *show = new QSqlQuery(connection.mydb);
        show->prepare("SELECT * FROM '"+fullname+"_"+studentId+"'");
        show->exec();
        model->setQuery(*show);
        ui->tableView_studentResult->setModel(model);
    }
    else
    {
        QMessageBox::critical(this,"Error","Make sure to double click Course name choice");
    }
    connection.closeConnection();
}

//function for button to show all students entries in order of their registration time
void ViewStudent::on_pushButton_displayAll_clicked()
{
    Login connection;
    connection.openConnection();

    //make new query model
    QSqlQueryModel *model = new QSqlQueryModel();

    //preparing query for selecting all student results record based on the order they are inserted
    QSqlQuery *query = new QSqlQuery(connection.mydb);
    query->prepare("SELECT * FROM '"+fullname+"_"+studentId+"'");
    query->exec();

    //set query to a model and load model onto table view
    model->setQuery(*query);
    ui->tableView_studentResult->setModel(model);
}

//function for button to save a student's results in a text file for printing
void ViewStudent::on_pushButton_print_clicked()
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
    QFile txtFile("/Users/J/Desktop/Score System NEW/Print Teachers/"+fullname+"_"+studentId+"_Teacher's Copy.txt");
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
                    else if (textData.size()<5)
                    {
                        textData += "\t\t ";
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
    fullname.replace(" ","_");

    connection.closeConnection();
}
