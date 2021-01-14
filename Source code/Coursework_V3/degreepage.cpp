#include "degreepage.h"
#include "ui_degreepage.h"
#include "course.h"
#include "studentcourselink.h"
#include <QListWidgetItem>
#include <QMessageBox>
#include <QDate>
#include <QDebug>
using namespace slink;      //student link
using namespace program;


DegreePage::DegreePage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DegreePage)
{
    ui->setupUi(this);
    for (size_t var = 0; var < MAX_DEGREES; ++var) {
        dList.push_back(Degree());          //init degree vector
        students.push_back(Student());          //init degree vector
    }
    degreeCount = dList[0].copyFromFile("../degrees.txt", dList, MAX_DEGREES);        //populate degree list vector
    students[0].copyFromFile("../students.txt", students, MAX_DEGREES);
    linkCount = sList[0].copyFromFile("../StudentDegreeLink.txt", sList, MAX_DEGREES, dList, MAX_DEGREES, students, MAX_DEGREES);

}

DegreePage::~DegreePage()
{
    delete ui;
}

void DegreePage::loadDegrees()
{
    ui->ListDegrees->clear();       //clear list
    if (ui->checkBox->isChecked())  //load a list of degrees for this student
    {
        for (size_t var = 0; var < linkCount; var++) {
            if(sList[var].getStudent()->getID() == currentStudent.getID()){
                if (!sList[var].getDegree()->getDetails().isEmpty())     //only add items with a title
                {
                    auto *item = new QListWidgetItem(sList[var].getDegree()->getTitle());       //create a widget item
                    QVariant v;
                    v.setValue(*sList[var].getDegree());      //set id
                    item->setData(Qt::UserRole, v); //add data to item
                    ui->ListDegrees->addItem(item);                                        //add item to list
                }
            }
        }
        ui->DegreeMessageLabel->setText("We are only displaying degrees you are enrolled on:");
        return;
    }
    ui->DegreeMessageLabel->setText("Within the page, you will find a table of  \nall the programmes that we provide students to study");
    for (size_t i = 0; i < MAX_DEGREES; i++)
    {
        if (!dList[i].getDetails().isEmpty())     //only add items with a title
        {
            auto *item = new QListWidgetItem(dList[i].getTitle());       //create a widget item
            QVariant v;
            v.setValue(dList[i]);      //set id
            item->setData(Qt::UserRole, v); //add data to item
            ui->ListDegrees->addItem(item);                                        //add item to list
        }
    }
}

void DegreePage::receiveStudent(Student s)
{
    currentStudent = s;
    ui->addBtn->setVisible(false);  //hides the add degree button, not available to students!
    ui->viewCoursesBtn->setVisible(true);
    loadDegrees();

}

void DegreePage::receiveEmployee(person::Employee e)
{
    currentUser = e;
    ui->addBtn->setVisible(true);
    ui->viewCoursesBtn->setVisible(false);
    loadDegrees();
}

void DegreePage::on_ListDegrees_itemClicked(QListWidgetItem *item)  //selected an item
{

    QVariant v; v = item->data(Qt::UserRole);
    Degree deg = v.value<Degree>();
    for (size_t i=0; i < MAX_DEGREES; i++)
    {
        if (deg.getID() == dList[i].getID() && !dList[i].getTitle().isEmpty())
        {
            ui->idBox->setText(QString::number(dList[i].getID()));
            ui->TitleBox->setText(dList[i].getTitle());
            ui->QualificationsBox->setText(dList[i].getQualification());
            ui->DetailsBox->setText(dList[i].getDetails());
            ui->DurationBox->setText(QString::number(dList[i].getDuration()));
        }
    }
}

void DegreePage::on_viewCoursesBtn_clicked()        //students wishes to enroll. create a link
{
    int selectedID = ui->idBox->text().toInt();
    for (size_t i=0; i< MAX_DEGREES; i++)
    {
        if (selectedID == dList[i].getID())     //create a link
        {
            //check if not already exists
            for (size_t var = 0; var < linkCount; var++) {
                if (sList[var].getStudent()->getID() == currentStudent.getID() && sList[var].getDegree()->getID() == selectedID)
                {
                    QMessageBox::information(this, "Enrollment", "You are already enrolled on this degree programme.");
                    return;     //exit scope
                }
            }

            StudentDegreeLink s;
            QDate today = QDate::currentDate();
            s.setDate(today);
            s.setDegree(&dList[i]);
            s.setStudent(&currentStudent);
            s.setFinalMark(0);      //no mark set yet, as the student just enrolled onthe course.
            s.writeToFile("../StudentDegreeLink.txt");
            QMessageBox::information(this, "Enrollment", "You have been enrolled on this degree programme.");
            return;
        }
    }
}

void DegreePage::on_checkBox_stateChanged(int arg1)
{
    loadDegrees();
}

void DegreePage::on_addBtn_clicked()
{
    //save new degree
    QString Title, Details, qualifications;
    int id, duration;
    Title = ui->TitleBox->text();
    Details = ui->DetailsBox->toPlainText();
    qualifications = ui->QualificationsBox->text();
    duration = ui->DurationBox->text().toInt();
    id = generate<Degree>(dList, degreeCount);
    for (size_t i=0; i<degreeCount; i++) {
        if (dList[i].getTitle() == Title)
        {
            QMessageBox::information(this, "Match", "Details have been modified.");
            dList[i].setTitle(Title);
            dList[i].setQualification(qualifications);
            dList[i].setDetails(Details);
            dList[i].setDuration(duration);
            dList[i].updateFile("../degrees.txt");      //UDPATE FILE
            return;
        }
    }
    dList[degreeCount] = Degree();  //new degree
    dList[degreeCount].setID(id);
    dList[degreeCount].setTitle(Title);
    dList[degreeCount].setQualification(qualifications);
    dList[degreeCount].setDetails(Details);
    dList[degreeCount].setDuration(duration);
    dList[degreeCount].writeToFile("../degrees.txt");       //write to file
    degreeCount++;  //increment count
    QMessageBox::information(this, "Saved", "This degree has been successfully saved.");
}

void DegreePage::on_HomeButton_clicked()
{
    close();
}

void DegreePage::on_CoursesButton_clicked()
{

    close();
}

void DegreePage::on_AssessmentsButton_clicked()
{

    close();
}

void DegreePage::on_SignOutButton_clicked()
{

    close();
}
