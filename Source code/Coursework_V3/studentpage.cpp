#include "studentpage.h"
#include "degreepage.h"
#include "courses.h"
#include "ui_studentpage.h"
#include "QDebug"
#include "QMessageBox"
#include "student.h"
#include "assessmentpage.h"



StudentPage::StudentPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StudentPage)
{
    //mainStudent;
    ui->setupUi(this);
    degreePage = new DegreePage;
    coursePage = new Courses;
    assessmentPage = new AssessmentPage;
    connect(this, SIGNAL(sendStudent(Student)), degreePage, SLOT(receiveStudent(Student)));
    connect(this, SIGNAL(sendStudent(Student)), coursePage, SLOT(receiveStudent(Student)));
    connect(this, SIGNAL(sendStudent(Student)), assessmentPage, SLOT(receiveStudent(Student)));
}


StudentPage::~StudentPage()
{
    delete ui;
}

void StudentPage::loadStudent(Student *s)
{
    QString fname,sname,email,pcode, addr,county, nat, gender;
    QDate dob;         //date

    s->getDetails(fname,sname,email,pcode,addr,county,dob,gender,nat);      //retrieve data
    ui->FirstNameBox->setText(fname);
    ui->SurnameBox->setText(sname);
    ui->EmailBox->setText(email);
    ui->PostcodeBox->setText(pcode);
    ui->AddressLine1Box->setText(addr);
    ui->CountyBox->setText(county);
    ui->DateOfBirthBox->setDate(dob);
    ui->GenderBox->setText(gender);
    ui->NationalityBox->setText(nat);
}

void StudentPage::receiveStudent(Student s)
{
    currentStudent = s;     //place current student into member
    QString msg = "Welcome back, ";
    msg += currentStudent.getFullName();
    ui->WelcomeMessage->setText(msg);
    //fill textboxes with information
    loadStudent(&currentStudent);
}


void StudentPage::on_SaveButton_clicked()           //save user
{
    QString fname = ui->FirstNameBox->text().trimmed();     //retrieve values
    QString sname = ui->SurnameBox->text().trimmed();
    QString email = ui->EmailBox->text().trimmed();
    QString pcode = ui->PostcodeBox->text().trimmed();
    QString addr = ui->AddressLine1Box->text().trimmed();
    QString county = ui->CountyBox->text().trimmed();
    QString dob = ui->DateOfBirthBox->text().trimmed();         //date
    QString nat = ui->NationalityBox->text().trimmed();
    QString gender = ui->GenderBox->text().trimmed();
    QString passw = ui->PasswordBox->text().trimmed();
    //validation
    QDate date;
    date = date.fromString(dob, "dd/MM/yyyy");
    //save data
    currentStudent.setDetails(fname, sname, email, pcode, addr, county, date, gender, nat);         //update memory
    if (passw != "")  {
        //hash the password
        passw = currentStudent.sha1Hash(passw);
        currentStudent.setPassword(passw);        //only change password if a new one has been entered
    }
    //save to file.
    currentStudent.updateFile("../students.txt");

    QMessageBox::information(this, "Saved", "Your details have been saved.");

    loadStudent(&currentStudent);
}

void StudentPage::on_SignOutButton_clicked()
{
    currentStudent = Student();         //overwrite the current student with empty
    emit openLogin();      //opens the login page by calling a slot in the login page
    close();        //close this window
}

void StudentPage::on_DegreeButton_clicked()
{
    emit sendStudent(currentStudent);
    degreePage->show(); //open the degree list page.
}

void StudentPage::on_CoursesButton_clicked()        //go to courses page
{
    emit sendStudent(currentStudent);
    coursePage->show();
}

void StudentPage::on_AssessmentsButton_clicked()
{
    emit sendStudent(currentStudent);
    assessmentPage->show();
}
