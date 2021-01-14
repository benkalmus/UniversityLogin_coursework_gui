#include "courses.h"
#include "ui_courses.h"
#include <QListWidgetItem>
#include <QMessageBox>
#include "QDebug"


#include "student.h"
#include "course.h"
#include "studentcourselink.h"
using namespace program;
using namespace slink;

Courses::Courses(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Courses)
{
    degSelected = new Degree;       //init
    for (size_t i=0; i< MAX_DEGREES; i++) {     //enumerate vectors
        course.push_back(Course());
        degree.push_back(Degree());
        students.push_back(Student());
    }
    loadFiles();
    ui->setupUi(this);
}

Courses::~Courses()
{
    delete ui;
}

void Courses::loadDegrees()
{
    ui->ListDegrees->clear();
    ui->ListCourses->clear();
    //populate degree list with degree from file
    if (!currentStudent.getEmail().isEmpty())
    {
        for (size_t i=0; i< sdCount; i++)
        {
            //if (!degree[i].getTitle().isEmpty())
            if (currentStudent.getID() == sdlink[i].getStudent()->getID())     //only load list of degrees the student is enrolled on.
            {
                QListWidgetItem *item = new QListWidgetItem(sdlink[i].getDegree()->getTitle() + " - " + sdlink[i].getDegree()->getQualification());
                QVariant v;    v.setValue(sdlink[i].getDegree());
                item->setData(Qt::UserRole, v);
                ui->ListDegrees->addItem(item);
            }
        }
    }
    if (currentUser.getID())        //show all degrees.
    {
        for (size_t i=0; i< degreeCount; i++)
        {
            QListWidgetItem *item = new QListWidgetItem(degree[i].getTitle() + " - " + degree[i].getQualification());
            QVariant v;    v.setValue(&degree[i]);
            item->setData(Qt::UserRole, v);
            ui->ListDegrees->addItem(item);
        }
    }
}

void Courses::loadCourses()
{
    ui->ListCourses->clear();       //clear list before adding
    if (!ui->ListDegrees->count())  return;     //if there are no item in the degree list, don't search for courses.
    //get selected degree ID
    if(ui->checkBox->isChecked())   //show only enrolled courses.
    {
        //get degree
        for(size_t i=0; i<scCount; i++)
        {
            if (sclink[i].getStudent()->getID() == currentStudent.getID())
            {
                QString displayText = sclink[i].getCourse()->getTitle() + " - " + sclink[i].getCourse()->getLevel();
                QListWidgetItem *itemCourse = new QListWidgetItem(displayText);
                QVariant v;
                v.setValue(sclink[i].getCourse());
                itemCourse->setData(Qt::UserRole, v);
                ui->ListCourses->addItem(itemCourse);
            }
        }
        return;
    }
    //populate course list given degree from item.
    if (degSelected == nullptr) return;         //check if an item has been seelcted.

    for (size_t i=0; i< count; i++)
    {
        if (link[i].getDegree()->getID() == degSelected->getID())       //found degree
        {
            QString displayText = link[i].getCourse()->getTitle() + " - " + link[i].getCourse()->getLevel();
            QListWidgetItem *itemCourse = new QListWidgetItem(displayText);
            QVariant v;
            v.setValue( link[i].getCourse());
            itemCourse->setData(Qt::UserRole, v);
            ui->ListCourses->addItem(itemCourse);
        }
    }
}

void Courses::loadFiles()
{
    courseCount = course[0].copyFromFile("../courses.txt", course, MAX_COURSES);        //get all courses
    degreeCount = degree[0].copyFromFile("../degrees.txt", degree, MAX_DEGREES);        //get all degrees
    students[0].copyFromFile("../students.txt", students, MAX_DEGREES);
    count = link[0].copyFromFile("../degreecourselink.txt", link, MAX_DEGREES+MAX_COURSES, degree, MAX_DEGREES, course, MAX_COURSES);     //find matches and produce links
    sdCount = sdlink[0].copyFromFile("../StudentDegreeLink.txt", sdlink, MAX_DEGREES+MAX_COURSES, degree, MAX_DEGREES, students, MAX_COURSES);
    scCount = sclink[0].copyFromFile("../studentcourselinks.txt", sclink, MAX_DEGREES+MAX_COURSES, students , MAX_DEGREES, course, MAX_COURSES);
}


void Courses::on_ListDegrees_itemClicked(QListWidgetItem *item)         //list of degrees
{
    //get selected degree ID
    QVariant var = item->data(Qt::UserRole);
    degSelected = var.value<Degree*>();
    loadCourses();
}

void Courses::on_ListCourses_itemClicked(QListWidgetItem *item)     //list of courses
{
    //given course ID, load textboxes with information
    QVariant var = item->data(Qt::UserRole);
    Course *selectedC = var.value<Course*>();
    QString Title;
    QString Description;
    QString Department;
    QString Level;
    QDate StartDate;
    QDate EndDate;
    float Fees;
    selectedC->getDetails(Title, Description, Department, Level, StartDate, EndDate, Fees);
    ui->BoxID->setText(QString::number(selectedC->getID()));
    ui->BoxTitle->setText(Title);
    ui->BoxDetails->setText(Description);
    ui->BoxDepartment->setText(Department);
    ui->BoxLevel->setText(Level);
    ui->BoxStartDate->setDate(StartDate);
    ui->BoxEndDate->setDate(EndDate);
    ui->BoxCourseFees->setText(QString::number(double(Fees)));
}

void Courses::receiveStudent(Student s)
{
    currentStudent = s;
    currentUser = person::Employee();       //empty employee
    ui->ListDegrees->clear();
    ui->BtnEnroll->setVisible(true);
    ui->BtnWithdraw->setVisible(true);
    ui->BtnSaveCourse->setVisible(false);       //should not be visible to students, nor available
    ui->BtnRemoveCourse->setVisible(false);       //should not be visible to students, nor available
    loadFiles();
    loadDegrees();
}

void Courses::receiveEmployee(person::Employee e)
{
    currentUser = e;
    currentStudent = Student();       //empty student
    ui->BtnSaveCourse->setVisible(true);
    ui->BtnRemoveCourse->setVisible(true);
    ui->BtnWithdraw->setVisible(false);
    ui->BtnEnroll->setVisible(false);
    loadFiles();
    loadDegrees();
}

void Courses::on_checkBox_stateChanged(int arg1)
{
    loadCourses();
    //disable button
    ui->BtnEnroll->setEnabled(!arg1);
}

void Courses::on_BtnEnroll_clicked()        //enroll student
{
    int id = ui->BoxID->text().toInt();
    //find course with that id
    for (size_t i=0; i < courseCount; i++)
    {
        if (id == course[i].getID())    //found course
        {
            //check if not already enrolled
            for (size_t j=0; j <scCount; j++) {
                if (sclink[j].getStudent()->getID() == currentStudent.getID() && sclink[j].getCourse()->getID() == course[i].getID())
                {
                    QMessageBox::information(this, "Enrollment", "You are already enrolled on this course.");
                    return;
                }
            }
            //save user
            QDate today; today.currentDate();
            sclink[scCount].setStudent(&currentStudent);
            sclink[scCount].setCourse(&course[i]);
            sclink[scCount].setMark(-1);
            sclink[scCount].setGrade('_');
            sclink[scCount].setDate(today);
            sclink[scCount].writeToFile("../studentcourselinks.txt");      //save user to file
            scCount++;
            QMessageBox::information(this, "Enrollment", "You have been enrolled on this course");
            return;
        }
    }
}

void Courses::on_BtnSaveCourse_clicked()
{
    if (degSelected->getTitle().isEmpty())        return;
    QString title, detail, department, level;
    int id;
    float fees;
    QDate start, end;
    title = ui->BoxTitle->text();
    detail = ui->BoxDetails->toPlainText();
    level = ui->BoxLevel->text();
    fees = ui->BoxCourseFees->text().toFloat();
    end = ui->BoxEndDate->date();
    start = ui->BoxStartDate->date();
    department = ui->BoxDepartment->text();
    id = generate<Course>(course, courseCount);
    level = level.toUpper();
    for (size_t i=0; i<courseCount; i++) {      //check if course already exists
        if (course[i].getTitle() == title)
        {
            QMessageBox::information(this, "Match", "Course has been updated");
            course[i].setDetails(title, detail, department, level, start, end, fees);
            course[i].updateFile("../courses.txt");
            return;
        }
    }
    if (level != "H" && level != "I" && level != "C")
    {
        QMessageBox::information(this, "Validation", "The level for this course must be either H, I or C.");
        return;
    }
    course[courseCount] = Course();
    course[courseCount].setID(id);
    course[courseCount].setDetails(title, detail, department, level, start, end, fees);
    course[courseCount].writeToFile("../courses.txt");
    courseCount++;
    //add course to degree
    link[count].setCourse(&course[courseCount-1]);
    link[count].setDegree(degSelected);
    link[count].writeToFile("../degreecourselink.txt");
    count++;

    QMessageBox::information(this, "Saved", "This course has been successfully saved.");
}

void Courses::on_HomeButton_clicked()
{

    close();
}

void Courses::on_ResourcesButton_clicked()
{
    close();
}

void Courses::on_AccountButton_clicked()
{
    close();
}

void Courses::on_SignOutButton_clicked()
{
    close();
}

void Courses::on_BtnRemoveCourse_clicked()
{
    if (degSelected->getTitle().isEmpty())        return;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Remove", "Are you sure you would like to remove this course?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) return;
    int id = ui->BoxID->text().toInt();
    for (size_t i = 0; i < courseCount; i++) {
        if(course[i].getID() == id)
        {
            course[i].removeFromFile("../courses.txt"); //remove from file!
            loadFiles();
            loadDegrees();
            loadCourses();
            return;
        }
    }
}

void Courses::on_BtnWithdraw_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Remove", "Are you sure you would like to withdraw from this course?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) return;

    int id = ui->BoxID->text().toInt();
     for (size_t i = 0; i < scCount; i++) {
         if(sclink[i].getStudent()->getID() == currentStudent.getID() && sclink[i].getCourse()->getID() == id)
         {
             sclink[i].removeFromFile("../studentcourselinks.txt"); //remove from file!
             loadFiles();
             return ;
         }
     }
}
