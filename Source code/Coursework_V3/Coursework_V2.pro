#-------------------------------------------------
#
# Project created by QtCreator 2019-03-04T13:16:31
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Coursework_V2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 + static

SOURCES += \
        main.cpp \
        loginpage.cpp \
    studentpage.cpp \
    courses.cpp \
    user.cpp \
    student.cpp \
    course.cpp \
    studentcourselink.cpp \
    degreepage.cpp \
    assessment.cpp \
    assessmentpage.cpp \
    staffpage.cpp

HEADERS += \
        loginpage.h \
    studentpage.h \
    courses.h \
    user.h \
    student.h \
    course.h \
    studentcourselink.h \
    degreepage.h \
    assessment.h \
    assessmentpage.h \
    staffpage.h

FORMS += \
        loginpage.ui \
    studentpage.ui \
    staffpage.ui \
    courses.ui \
    degreepage.ui \
    assessmentpage.ui \
    staffpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
