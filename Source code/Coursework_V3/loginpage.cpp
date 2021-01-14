#include <QDebug>
#include <QMessageBox>
//#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

#include "loginpage.h"
#include "ui_loginpage.h"
#include "studentpage.h"
#include "staffpage.h"

#include "user.h"
#include "student.h"
#include "course.h"
#include "studentcourselink.h"

//file handling libraries
#include <string>
#include <iostream>
#include <fstream>

#define MAX_USERS 50

using namespace std;
using namespace person;
using namespace program;


LoginPage::LoginPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginPage)
{
    //form load
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("acetraining");
    db.setUserName("root");
    db.setPassword("root");
    bool ok = db.open();
    if (!ok)  QMessageBox::information(this, "SQL", "Could not connect to database, make sure the database is online.");
    QSqlQuery query("SELECT FirstName FROM student");
    while (query.next())
    {
        QString FirstName = query.value(0).toString();
    }

    ui->setupUi(this);
    this->setFixedSize(504,576);
    //connect
    //TODO: explain signal slot system
    staffWindow = new StaffPage;
    studentWindow = new StudentPage;        //must init student window with student page.
    connect(this, &LoginPage::sendStudent, studentWindow, &StudentPage::receiveStudent );        //to send student object to the studentWindow
    connect(this, &LoginPage::sendEmployee, staffWindow, &StaffPage::receiveEmployee);        //to send employee object to the staffpage
    //using old syntax (sometimes weird bugs happen and this must be used)
    connect(studentWindow, SIGNAL(openLogin()), this, SLOT(openLogin()));         //to open login again
    connect(staffWindow, SIGNAL(openLogin()), this, SLOT(openLogin()));         //to open login again
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::on_ExitButton_clicked()
{
    close();
}

void LoginPage::on_LoginButton_clicked()    //login
{
    //validation here
    QString user = ui->UsernameBox->text();
    QString pass = ui->PasswordBox->text();
    user = user.replace(" ", "");       //removes all whitespaces
    user = user.toLower();              //lowercase
    User u;
    pass = u.sha1Hash(pass);        //HASH the password using sha1.

    if (user.isEmpty() || pass.isEmpty())       //check if empty
    {
        //display error
        QMessageBox::information(this,"Validation",  "You must enter a username and password.");
        return;     //exit scope
    }

    if (ui->StudentRadioButton->isChecked())
    {
        Student students[MAX_USERS];
        students->copyFromFile("../students.txt", students, MAX_USERS);     //load student list from file
        students->copyFromFile("../students.txt", studentsList, MAX_USERS);     //load student list from file
        //compare username and password.

        //using a template function to find a match
        int index = getUserIndex(user, pass, students, MAX_USERS);
        if (index != -1) //check if found
        {
            ui->UsernameBox->clear();
            ui->PasswordBox->clear();
            studentWindow->show();        //open student page
            emit sendStudent(students[index]);
            hide();             //hides login page
            return;
        }
    }
    else if (ui->TutorRadioButton->isChecked())
    {
        Employee staff[MAX_USERS];
        //staff->copyFromFile("../staff.txt", staff, MAX_USERS);     //load student list from file
        staff->copyFromFile("../staff.txt", staff, MAX_USERS);     //load student list from file
        //compare username and password.
        //using a template function to find a match
        int index = getUserIndex<Employee>(user, pass, staff, MAX_USERS);
        if (index != -1) //check if found
        {
            ui->UsernameBox->clear();
            ui->PasswordBox->clear();
            staffWindow->show();
            emit sendEmployee(staff[size_t(index)]);     //send user object to class
            hide();             //hides login page
            return;
        }
    }
    else {      //no selected user type
        QMessageBox::information(this,"Error",  "You must select user type.");
    }

    //if got to this point, then credentials were incorrect
    QMessageBox::information(this, "Not Found",  "Could not find you, please check your credentials.");


}

void LoginPage::openLogin()
{
    show();
}


