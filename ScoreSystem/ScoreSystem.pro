#-------------------------------------------------
#
# Project created by QtCreator 2017-02-02T00:13:00
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScoreSystem
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    adminpage.cpp \
    addstudent.cpp \
    addteacher.cpp \
    adminshowstudent.cpp \
    adminshowteacher.cpp \
    studentpage.cpp \
    chooseelective.cpp \
    teacherpage.cpp \
    viewstudent.cpp

HEADERS  += login.h \
    adminpage.h \
    addstudent.h \
    addteacher.h \
    adminshowstudent.h \
    adminshowteacher.h \
    studentpage.h \
    chooseelective.h \
    teacherpage.h \
    viewstudent.h

FORMS    += login.ui \
    adminpage.ui \
    addstudent.ui \
    addteacher.ui \
    adminshowstudent.ui \
    adminshowteacher.ui \
    studentpage.ui \
    chooseelective.ui \
    teacherpage.ui \
    viewstudent.ui

RESOURCES += \
    resources.qrc
