#ifndef ADMINSHOWSTUDENT_EXTENSIONSEARCH_H
#define ADMINSHOWSTUDENT_EXTENSIONSEARCH_H

#include <QWidget>

namespace Ui {
class AdminShowStudent_extensionSearch;
}

class AdminShowStudent_extensionSearch : public QWidget
{
    Q_OBJECT

public:
    explicit AdminShowStudent_extensionSearch(QWidget *parent = 0);
    ~AdminShowStudent_extensionSearch();

private:
    Ui::AdminShowStudent_extensionSearch *ui;
};

#endif // ADMINSHOWSTUDENT_EXTENSIONSEARCH_H
