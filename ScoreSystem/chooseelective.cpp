#include "chooseelective.h"
#include "ui_chooseelective.h"
#include <QMessageBox>

ChooseElective::ChooseElective(QWidget *parent, QString name, QString id, QString majors) :
    QDialog(parent),
    ui(new Ui::ChooseElective)
{
    ui->setupUi(this);

    //FOR DESIGN PURPOSE OF THE BUTTONS IN THE UI
    //set up style sheet and interaction of the buttons with user's action
    ui->pushButton_back->setStyleSheet("QPushButton#pushButton_back {background-color: transparent;}"
                                         "QPushButton#pushButton_back:hover {background-color: rgb(255,160,122);};");
    ui->pushButton_choose->setStyleSheet("QPushButton#pushButton_choose {background-color: transparent;}"
                                       "QPushButton#pushButton_choose:hover {background-color: rgb(152,251,152);};");

    //set the tool tip for displaying information of buttons to user
    ui->pushButton_back->setToolTip("back");
    ui->pushButton_choose->setToolTip("Confirm Elective");

    fullname = name;
    studentId = id;
    major = majors;

    Login connection;
    connection.openConnection();

    QSqlQueryModel *model = new QSqlQueryModel();

    //set query for selecting the elective courses available for certain major
    QSqlQuery *query = new QSqlQuery(connection.mydb);
    query->prepare("SELECT * FROM '"+major+"' WHERE type='elective'");
    query->exec();

    //assign query to model to display in table
    model->setQuery(*query);
    ui->listView_course->setModel(model);

    connection.closeConnection();

    qDebug()<<(model->rowCount());
}

//destructor for ChooseElective
ChooseElective::~ChooseElective()
{
    delete ui;
}

//function for back button to go to previous page
void ChooseElective::on_pushButton_back_clicked()
{
    //close the current window
    close();
}

//function for allowing interaction (double-click to choose) with the list of electives shown
void ChooseElective::on_listView_course_activated(const QModelIndex &index)
{
    //get the index of the selected item in the list display
    QString courseName = ui->listView_course->model()->data(index).toString();

    Login connection;
    if (!connection.openConnection())
    {
        qDebug()<<"Failed to open database";
        return;
    }
    connection.openConnection();

    //prepare a query to database which find a match for the content of the index in the list view
    QSqlQuery query;
    query.prepare("SELECT * FROM '"+major+"' WHERE course='"+courseName+"'");

    if (query.exec())
    {
        //search from the beginning of the database, incrementing one entry/row each time
        //take all information from the entry that match the name of the electives chosen//
        while(query.next())
        {
            //set the label for selected elective with the name of the elective
            ui->label_chosen->setText(query.value(0).toString());

        }
        connection.closeConnection();
    }
    else
    {
        QMessageBox::critical(this, "Error::", query.lastError().text());
    }
}

//function for button to confirm and add a selected elective
void ChooseElective::on_pushButton_choose_clicked()
{
    //get the selected elective name from the label
    QString chosen = ui->label_chosen->text();
    if (chosen=="")
    {
        QMessageBox::critical(this,"Warning","Please select one elective above");
        return;
    }
    Login connection;

    //connect to database
    connection.openConnection();

    QSqlQuery query;

    //prepare query to database to check the electives taken by a student
    query.prepare("select Course from '"+fullname+"_"+studentId+"' WHERE type='elective'");

    if(query.exec())
    {
        //count the number of electives taken
        int count=0;
        //start from beginning of database, incrementing one entry/row each time
        while(query.next())
        {
            count++;
        }

        //depending on the student's major and the number of electives already registered,
        //the student user can either add or not add (not allowed) another elective
        if (major == "Architecture")
        {
            //student are allowed to add if the number of electives is not maximum yet
            if (count<3)
            {
                QSqlQuery check;
                check.prepare("SELECT Course FROM '"+fullname+"_"+studentId+"' WHERE type='elective' and Course='"+chosen+"'");
                if (check.exec())
                {
                    //check if chosen elective is already registered
                    int encounter=0;
                    while(check.next())
                    {
                        encounter++;
                    }

                    //chosen elective is not registered yet
                    if (encounter==0)
                    {
                        //insert the chosen elective into database
                        QSqlQuery add;
                        add.prepare("INSERT INTO '"+fullname+"_"+studentId+"' (Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+chosen+"','','','','','','','elective')");
                        if (add.exec())
                        {
                            QMessageBox::information(this,"Success","Chosen Elective is noted");
                        }
                        else
                            QMessageBox::critical(this,"Error","Fail to choose elective");
                    }

                    //notify user if chosen elective is already registered
                    else
                    {
                        QMessageBox::critical(this,"Warning","Chosen Elective is already registered!");
                        return;
                    }
                }
                else
                {
                    QMessageBox::critical(this,"Error","Fail to access database");
                }

                //if the number of registered electives is already full, disable the button to add electives
                if (count==2)
                {
                    ui->pushButton_choose->setEnabled(false);
                }

            }
            //if number of electives is already full, notify the user
            else
            {
                QMessageBox::critical(this,"Warning","You already have enough electives!");
            }
        }
        else if (major == "Business_Management")
        {
            //student are allowed to add if the number of electives is not maximum yet
            if (count<8)
            {
                QSqlQuery check;
                check.prepare("SELECT Course FROM '"+fullname+"_"+studentId+"' WHERE type='elective' and Course='"+chosen+"'");
                if (check.exec())
                {
                    //check if chosen elective is already registered
                    int encounter=0;
                    while(check.next())
                    {
                        encounter++;
                    }

                    //chosen elective is not registered yet
                    if (encounter==0)
                    {
                        //insert the chosen elective into database
                        QSqlQuery add;
                        add.prepare("INSERT INTO '"+fullname+"_"+studentId+"' (Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+chosen+"','','','','','','','elective')");
                        if (add.exec())
                        {
                            QMessageBox::information(this,"Success","Chosen Elective is noted");
                        }
                        else
                            QMessageBox::critical(this,"Error","Fail to choose elective");
                    }

                    //notify user if chosen elective is already registered
                    else
                    {
                        QMessageBox::critical(this,"Warning","Chosen Elective is already registered!");
                        return;
                    }
                }
                else
                {
                    QMessageBox::critical(this,"Error","Fail to access database");
                }

                 //if the number of registered electives is already full, disable the button to add electives
                if (count==7)
                {
                    ui->pushButton_choose->setEnabled(false);
                }

            }
            //if number of electives is already full, notify the user
            else
            {
                QMessageBox::critical(this,"Warning","You already have enough electives!");
            }
        }
        else if (major == "Computer_Science")
        {
            //student are allowed to add if the number of electives is not maximum yet
            if (count<5)
            {
                QSqlQuery check;
                check.prepare("SELECT Course FROM '"+fullname+"_"+studentId+"' WHERE type='elective' and Course='"+chosen+"'");
                if (check.exec())
                {
                    //check if chosen elective is already registered
                    int encounter=0;
                    while(check.next())
                    {
                        encounter++;
                    }

                    //chosen elective is not registered yet
                    if (encounter==0)
                    {
                        //insert the chosen elective into database
                        QSqlQuery add;
                        add.prepare("INSERT INTO '"+fullname+"_"+studentId+"' (Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+chosen+"','','','','','','','elective')");
                        if (add.exec())
                        {
                            QMessageBox::information(this,"Success","Chosen Elective is noted");
                        }
                        else
                            QMessageBox::critical(this,"Error","Fail to choose elective");
                    }

                    //notify user if chosen elective is already registered
                    else
                    {
                        QMessageBox::critical(this,"Warning","Chosen Elective is already registered!");
                        return;
                    }
                }
                else
                {
                    QMessageBox::critical(this,"Error","Fail to access database");
                }

                //if the number of registered electives is already full, disable the button to add electives
                if (count==4)
                {
                    ui->pushButton_choose->setEnabled(false);
                }

            }

            //if number of electives is already full, notify the user
            else
            {
                QMessageBox::critical(this,"Warning","You already have enough electives!");
            }
        }
        else if (major == "Economics")
        {
            //student are allowed to add if the number of electives is not maximum yet
            if (count<5)
            {
                QSqlQuery check;
                check.prepare("SELECT Course FROM '"+fullname+"_"+studentId+"' WHERE type='elective' and Course='"+chosen+"'");
                if (check.exec())
                {
                    //check if chosen elective is already registered
                    int encounter=0;
                    while(check.next())
                    {
                        encounter++;
                    }

                    //chosen elective is not registered yet
                    if (encounter==0)
                    {
                        //insert the chosen elective into database
                        QSqlQuery add;
                        add.prepare("INSERT INTO '"+fullname+"_"+studentId+"' (Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+chosen+"','','','','','','','elective')");
                        if (add.exec())
                        {
                            QMessageBox::information(this,"Success","Chosen Elective is noted");
                        }
                        else
                            QMessageBox::critical(this,"Error","Fail to choose elective");
                    }

                    //notify user if chosen elective is already registered
                    else
                    {
                        QMessageBox::critical(this,"Warning","Chosen Elective is already registered!");
                        return;
                    }
                }
                else
                {
                    QMessageBox::critical(this,"Error","Fail to access database");
                }

                //if the number of registered electives is already full, disable the button to add electives
                if (count==4)
                {
                    ui->pushButton_choose->setEnabled(false);
                }

            }

            //if number of electives is already full, notify the user
            else
            {
                QMessageBox::critical(this,"Warning","You already have enough electives!");
            }
        }
        else if (major == "Mechanical_Engineering")
        {
            //student are allowed to add if the number of electives is not maximum yet
            if (count<4)
            {
                QSqlQuery check;
                check.prepare("SELECT Course FROM '"+fullname+"_"+studentId+"' WHERE type='elective' and Course='"+chosen+"'");
                if (check.exec())
                {
                    //check if chosen elective is already registered
                    int encounter=0;
                    while(check.next())
                    {
                        encounter++;
                    }

                    //chosen elective is not registered yet
                    if (encounter==0)
                    {
                        //insert the chosen elective into database
                        QSqlQuery add;
                        add.prepare("INSERT INTO '"+fullname+"_"+studentId+"' (Course,Regular,Coursework,Mid_Term,Final_Exam,Total,Grade_Point,Type) values ('"+chosen+"','','','','','','','elective')");
                        if (add.exec())
                        {
                            QMessageBox::information(this,"Success","Chosen Elective is noted");
                        }
                        else
                            QMessageBox::critical(this,"Error","Fail to choose elective");
                    }

                    //notify user if chosen elective is already registered
                    else
                    {
                        QMessageBox::critical(this,"Warning","Chosen Elective is already registered!");
                        return;
                    }
                }
                else
                {
                    QMessageBox::critical(this,"Error","Fail to access database");
                }

                //if the number of registered electives is already full, disable the button to add electives
                if (count==3)
                {
                    ui->pushButton_choose->setEnabled(false);
                }

            }

            //if number of electives is already full, notify the user
            else
            {
                QMessageBox::critical(this,"Warning","You already have enough electives!");
            }
        }
    }
}
