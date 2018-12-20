#include "adminshowstudent_extensionsearch.h"
#include "ui_adminshowstudent_extensionsearch.h"

AdminShowStudent_extensionSearch::AdminShowStudent_extensionSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminShowStudent_extensionSearch)
{
    ui->setupUi(this);
}

AdminShowStudent_extensionSearch::~AdminShowStudent_extensionSearch()
{
    delete ui;
}
