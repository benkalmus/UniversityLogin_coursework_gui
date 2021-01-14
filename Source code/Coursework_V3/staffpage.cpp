#include "staffpage.h"
#include "loginpage.h"
#include "ui_staffpage.h"
#include "QDebug"
#include "QMessageBox"
#include "QFileDialog"
#include "QDesktopServices"

using namespace person;

StaffPage::StaffPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StaffPage)
{
    ui->setupUi(this);
    showNino = false; showSalary=false;

    degreePage = new DegreePage;
    coursePage = new Courses;
    assessmentPage = new AssessmentPage;
    connect(this, &StaffPage::sendEmployee, degreePage, &DegreePage::receiveEmployee);
    connect(this, &StaffPage::sendEmployee, coursePage, &Courses::receiveEmployee);
    connect(this, &StaffPage::sendEmployee, assessmentPage, &AssessmentPage::receiveEmployee);
}

StaffPage::~StaffPage()
{
    delete ui;
}

void StaffPage::receiveEmployee(Employee e)
{
    currentUser = e;
    ui->WelcomeMessage->setText("Welcome back, " + e.getFullName() + ",");
    loadEmployee(&currentUser);
    if (currentUser.getRole() == "admin")
    {
        ui->BtnRestore->setVisible(true);
        ui->BtnBackup->setVisible(true);
    }
    else {
        ui->BtnRestore->setVisible(false);
        ui->BtnBackup->setVisible(false);
    }
}

void StaffPage::loadEmployee(Employee *e)
{
    clearAll();
    QString fname,sname,email,pcode, addr,county, nat, gender, office, role, nino;
    QDate dob;         //date
    int salary;
    e->getDetails(fname,sname,email,pcode,addr,county,dob,gender,nat);      //retrieve data
    office = e->getOfficeNumber();
    role = e->getRole();
    nino = e->getNINO();
    salary = e->getEmplSalary();
    ui->FirstNameBox->setText(fname);
    ui->SurnameBox->setText(sname);
    ui->EmailBox->setText(email);
    ui->PostcodeBox->setText(pcode);
    ui->AddressLine1Box->setText(addr);
    ui->CountyBox->setText(county);
    ui->DateOfBirthBox->setDate(dob);
    ui->GenderBox->setText(gender);
    ui->NationalityBox->setText(nat);

    ui->BoxOfficeNum->setText(office);
    ui->BoxRole->setText(role);
    if (showNino)  ui->BoxNINO->setText(nino);
    if (showSalary)  ui->BoxSalary->setText(QString::number(salary));
}

void StaffPage::clearAll()
{   ui->FirstNameBox->clear();
    ui->SurnameBox->clear();
    ui->EmailBox->clear();
    ui->PostcodeBox->clear();
    ui->AddressLine1Box->clear();
    ui->CountyBox->clear();
    ui->DateOfBirthBox->clear();
    ui->GenderBox->clear();
    ui->NationalityBox->clear();

    ui->BoxOfficeNum->clear();
    ui->BoxRole->clear();
    ui->BoxNINO->clear();
    ui->BoxSalary->clear();
}

void StaffPage::on_BtnShowNINO_clicked()
{
    showNino = !showNino;
    loadEmployee(&currentUser);
}

void StaffPage::on_BtnShowSalary_clicked()
{
    showSalary = !showSalary;
    loadEmployee(&currentUser);
}

void StaffPage::on_SignOutButton_clicked()
{
    emit openLogin();
    hide();
}

void StaffPage::on_SaveButton_clicked()
{
    QString fname,sname,email,pcode, addr,county, nat, gender, office, role, nino, password;
    QDate dob;         //date
    int salary;
    fname = ui->FirstNameBox->text();
    sname = ui->SurnameBox->text();
    email = ui->EmailBox->text();
    pcode = ui->PostcodeBox->text();
    addr = ui->AddressLine1Box->text();
    county = ui->CountyBox->text();
    dob = ui->DateOfBirthBox->date();
    gender = ui->GenderBox->text();
    nat = ui->NationalityBox->text();
    office = ui->BoxOfficeNum->text();
    role = ui->BoxRole->text();
    nino = ui->BoxNINO->text();
    salary = ui->BoxSalary->text().toInt();
    password = ui->PasswordBox->text().trimmed();
    currentUser.setDetails(fname,sname,email,pcode, addr,county, dob, gender, nat);
    if (!nino.isEmpty()) currentUser.setNINO(nino);
    //currentUser.setRole(role);
    currentUser.setOfficeNumber(office);
    if (!password.isEmpty()) {
        //hash the password
        password = currentUser.sha1Hash(password);
        currentUser.setPassword(password);     //if password has been entered, change it
    }
    currentUser.updateFile("../staff.txt");        //update file
    QMessageBox::information(this, "Title", "Your details have been updated.");
}

void StaffPage::on_DegreeButton_clicked()
{
    emit sendEmployee(currentUser);
    degreePage->show();
}

void StaffPage::on_CoursesButton_clicked()
{
    emit sendEmployee(currentUser);
    coursePage->show();
}

void StaffPage::on_AssessmentsButton_clicked()
{
    emit sendEmployee(currentUser);
    assessmentPage->show();
}

void StaffPage::on_BtnBackup_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select a directory to backup to.");
    dir += "/";
    QFile::copy("../assessments.txt", dir+ "assessments.txt");
    QFile::copy("../courses.txt", dir+ "courses.txt");
    QFile::copy("../degreecourselink.txt", dir+ "degreecourselink.txt");
    QFile::copy("../degrees.txt", dir+ "degrees.txt");
    QFile::copy("../staff.txt", dir+ "staff.txt");
    QFile::copy("../studentcourselinks.txt", dir+ "studentcourselinks.txt");
    QFile::copy("../StudentDegreeLink.txt", dir+ "StudentDegreeLink.txt");
    QFile::copy("../students.txt", dir + "students.txt");
    QFile::copy("../submissions.txt", dir+ "submissions.txt");
    QMessageBox::information(this, "Saved", "The database has been backed up to " + dir);
}

void StaffPage::on_BtnRestore_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select a directory where backup files remain to restore a database.");
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Restore", "Are you sure you'd like to restore?\nThis will replace all data in the database, make sure everything is backed up.", QMessageBox::Ok | QMessageBox::Cancel );
    if (reply == QMessageBox::Cancel) return;
    dir += "/";
    QFile::copy(dir+ "assessments.txt", "../assessments.txt");
    QFile::copy(dir+ "courses.txt", "../courses.txt");
    QFile::copy(dir+ "degreecourselink.txt", "../degreecourselink.txt");
    QFile::copy(dir+ "degrees.txt", "../degrees.txt");
    QFile::copy(dir+ "staff.txt","../staff.txt");
    QFile::copy(dir+ "studentcourselinks.txt", "../studentcourselinks.txt");
    QFile::copy(dir+ "StudentDegreeLink.txt", "../StudentDegreeLink.txt");
    QFile::copy(dir + "students.txt", "../students.txt");
    QFile::copy(dir+ "submissions.txt", "../submissions.txt");
    QMessageBox::information(this, "Saved", "The database has been backed up to " + dir);
}
