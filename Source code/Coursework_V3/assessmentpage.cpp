#include "assessmentpage.h"
#include "ui_assessmentpage.h"
#include <QListWidgetItem>
#include <QMessageBox>
#include "QDebug"
#include "QFileDialog"
#include "QDesktopServices"


#include "assessment.h"
#include "student.h"
#include "course.h"
#include "studentcourselink.h"
using namespace program;
using namespace slink;
using namespace grade;

AssessmentPage::AssessmentPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AssessmentPage)
{
    selAssessment = new Assessment();
    selectedCourse = new Course();
    selectedStudent = new Student();
    ui->setupUi(this);
    for (size_t i=0; i< MAX_DEGREES; i++) {     //enumerate vectors
        course.push_back(Course());
        students.push_back(Student());
        assessments.push_back(Assessment());
        subs.push_back(Submission());
    }
    loadFiles();

}


AssessmentPage::~AssessmentPage()
{
    delete ui;
}

void AssessmentPage::loadStudents()
{
    ui->ListStudents->clear();
    ui->ListCourses->clear();
    ui->ListAssessments->clear();
    for (size_t i = 0; i < studentCount; i++) {     //add students
        QListWidgetItem *item = new QListWidgetItem(students[i].getFullName());
        QVariant v; v.setValue<Student*>(&students[i]);
        item->setData(Qt::UserRole, v);
        ui->ListStudents->addItem(item);
    }
}

void AssessmentPage::loadCourses()
{
    ui->ListAssessments->clear();
    ui->ListCourses->clear();
    if (!currentStudent.getEmail().isEmpty()) //if no student is selected, load a list of courses belonging to currentStudent
    {
        for (size_t i=0; i < scCount; i++)
        {
            if (sclink[i].getStudent()->getID() == currentStudent.getID())
            {
                QListWidgetItem *item = new QListWidgetItem(sclink[i].getCourse()->getTitle() + " - " + sclink[i].getCourse()->getLevel());
                QVariant v; v.setValue<Course *>(sclink[i].getCourse());
                item->setData(Qt::UserRole, v);
                ui->ListCourses->addItem(item);
            }
        }
        return;
    }
    for (size_t i=0; i < scCount; i++)
    {
        if (sclink[i].getStudent()->getID() == selectedStudent->getID())
        {
            QListWidgetItem *item = new QListWidgetItem(sclink[i].getCourse()->getTitle() + " - " + sclink[i].getCourse()->getLevel());
            QVariant v; v.setValue<Course *>(sclink[i].getCourse());
            item->setData(Qt::UserRole, v);
            ui->ListCourses->addItem(item);
        }
    }
}

void AssessmentPage::loadAssessments()
{
    ui->ListAssessments->clear();
    for (size_t i=0; i < aCount; i++) {
        if (selectedCourse->getID() == assessments[i].getCourse()->getID())     //given course, find all assessments for that course
        {
            QListWidgetItem *item = new QListWidgetItem(assessments[i].getTitle() + " - " + assessments[i].getDate().toString("yyyy-MM-dd"));
            QVariant v;
            v.setValue<Assessment*>(&assessments[i]);
            item->setData(Qt::UserRole, v);
            ui->ListAssessments->addItem(item);
        }
    }
}

void AssessmentPage::loadFiles()
{
    courseCount = course[0].copyFromFile("../courses.txt", course, MAX_COURSES);        //get all courses
    studentCount = students[0].copyFromFile("../students.txt", students, MAX_DEGREES);
    scCount = sclink[0].copyFromFile("../studentcourselinks.txt", sclink, MAX_DEGREES+MAX_COURSES, students , MAX_DEGREES, course, MAX_COURSES);
    aCount = assessments[0].copyFromFile("../assessments.txt", assessments, MAX_DEGREES, course, MAX_COURSES);
    subCount = subs[0].copyFromFile("../submissions.txt", subs, MAX_DEGREES, assessments, students);

}

void AssessmentPage::clearAll()
{
    ui->BoxIDAssessment->clear();
    ui->BoxTitleAssessment->clear();
    ui->BoxWeighing->clear();
    ui->BoxDeadlineDate->clear();
    ui->BoxFilenameAssessment->clear();

    ui->BoxIDSubmission->clear();
    ui->BoxTitleSubmission->clear();
    ui->BoxSubmissionDate->clear();
    ui->BoxMarksSubmission->clear();
    ui->BoxProgression->clear();
    ui->BoxConcessional->clear();
    ui->BoxSubmissionFilename->clear();
}

void AssessmentPage::receiveStudent(Student s)
{
    currentStudent = s;
    loadFiles();
    //hide student list
    ui->ListStudents->setVisible(false);
    ui->label->setVisible(false);
    ui->BtnAddAssessment->setVisible(false);
    ui->BtnSubmitMark->setVisible(false);
    ui->BtnSave->setVisible(true);
    ui->BoxMarksSubmission->setEnabled(false);
    ui->BoxProgression->setEnabled(false);
    ui->BoxConcessional->setEnabled(false);
    loadCourses();
}

void AssessmentPage::receiveEmployee(person::Employee e)
{
    currentUser = e;
    loadFiles();
    ui->ListStudents->setVisible(true);
    ui->label->setVisible(true);
    ui->BtnAddAssessment->setVisible(true);
    ui->BtnSave->setVisible(false);
    ui->BtnSubmitMark->setVisible(true);
    ui->BoxMarksSubmission->setEnabled(true);
    ui->BoxProgression->setEnabled(true);
    ui->BoxConcessional->setEnabled(true);
    loadStudents();
}

void AssessmentPage::on_ListStudents_itemClicked(QListWidgetItem *item)
{
    clearAll();
    ui->ListCourses->clear();
    ui->ListAssessments->clear();
    QVariant v;
    v = item->data(Qt::UserRole);
    selectedStudent = v.value<Student*>();
    loadCourses();
}

void AssessmentPage::on_ListCourses_itemClicked(QListWidgetItem *item)
{
    clearAll();
    QVariant v = item->data(Qt::UserRole);
    selectedCourse = v.value<Course*>();
    loadAssessments();
}

void AssessmentPage::on_ListAssessments_itemClicked(QListWidgetItem *item)
{
    clearAll();
    QVariant v;
    v = item->data(Qt::UserRole);
    selAssessment = v.value<Assessment*>();
    //load values into textboxes
    ui->BoxIDAssessment->setText(QString::number(selAssessment->getID()));
    ui->BoxTitleAssessment->setText(selAssessment->getTitle());
    ui->BoxWeighing->setText(QString::number(selAssessment->getWeighing()));
    ui->BoxDeadlineDate->setDate(selAssessment->getDate());
    ui->BoxFilenameAssessment->setText(selAssessment->getDocument());

    size_t foundID  = 0;
    bool found = false;
    int matchID = -1;
    if (!selectedStudent->getEmail().isEmpty()) matchID = selectedStudent->getID();
    else matchID = currentStudent.getID();
    //load submission (if exists)
    for (size_t i=0; i<subCount; i++)
    {
        if (subs[i].getAssessment()->getID() == selAssessment->getID()) //if submission is found
        {
            //now check if student id matches
            if (subs[i].getStudent()->getID() == matchID)
            {
                foundID = i;            //FOUND
                subSelected = &subs[i];
                found = true;
                break;      //exit loop
            }
        }
    }
    if (found == false)
    {
        ui->BoxIDSubmission->setText("Submission not found");
        return;
    }
    ui->BoxIDSubmission->setText(QString::number(subs[foundID].getID()));
    ui->BoxTitleSubmission->setText(subs[foundID].getTitle());
    ui->BoxSubmissionDate->setDate(subs[foundID].getDate());
    ui->BoxMarksSubmission->setText(QString::number(subs[foundID].getMark()));
    ui->BoxProgression->setText((subs[foundID].getProgCode()));
    ui->BoxConcessional->setText(subs[foundID].getConcCode());
    ui->BoxSubmissionFilename->setText(subs[foundID].getDocument());
}

void AssessmentPage::on_BtnAssessment_clicked()
{
    QString urlStr;
    urlStr = ui->BoxFilenameAssessment->text();
    bool openFile = false;
    try {
        openFile = QDesktopServices::openUrl(QUrl("file:" + urlStr));      //serve file (opens using default application)
        if (!openFile) throw openFile;      //if file was not opened, throw an exception
    }
    catch (bool result)
    {
        if (!result)        //check the result
        {
            QMessageBox::information(this, "Exception Caught", "This file could not be opened.");
        }else {
            QMessageBox::information(this, "Exception Caught", "Error 01: \nPlease report this bug to the administrator.");
        }
    }

}

void AssessmentPage::on_BtnAddAssessment_clicked()
{
    if (selectedCourse == nullptr)
    {
        QMessageBox::information(this, "Error ", "You must select a course");
        qDebug() << selectedCourse->getID();
        return;
    }
    int id = generate<Assessment>(assessments, aCount);
    //int id = assessments[0].generateID(assessments, aCount);
    qDebug() <<"id "<< id;
    QString title;
    int weighing;
    QDate deadline;
    QString filename;

    //load values into textboxes
    //id = ui->BoxIDAssessment->text().toInt();
    title = ui->BoxTitleAssessment->text();
    weighing = ui->BoxWeighing->text().toInt();
    //TO DO
    //LOAD UP ALL ASSESSMENTS for this course
    //ADD UP WEIGHING
    //IF OVER > 100
    //REDUCE TO SATISFY
    //ELSE MESSAGE BOX

    deadline = ui->BoxDeadlineDate->date();
    filename = ui->BoxFilenameAssessment->text();

    assessments[aCount].setID(id);
    assessments[aCount].setTitle(title);
    assessments[aCount].setDocument(filename);
    assessments[aCount].setWeighing(weighing);
    assessments[aCount].setDate(deadline);
    assessments[aCount].setCourse(selectedCourse);
    assessments[aCount].writeToFile("../assessments.txt");      //save to file
    aCount++;   //increment counter
    QMessageBox::information(this, "Success", "Assessment has been successfully saved");
}

void AssessmentPage::on_BtnSave_clicked()
{
    if (selAssessment == nullptr)
    {
        QMessageBox::information(this, "Error ", "You must select an assessment to add a submission");
        return;
    }
    int id = generate<Submission>(subs, subCount);
    qDebug() <<"id "<< id;

    QString title;
    QDate submissiondate;
    submissiondate =  submissiondate.currentDate();
    QString filename;
    int Mark =0;
    QString cCode= "_";
    QString pCode= "_";

    filename = ui->BoxSubmissionFilename->text();
    title = ui->BoxTitleSubmission->text();
    ui->BoxSubmissionDate->setDate(submissiondate);
    ui->BoxMarksSubmission->setText("Unmarked");
    ui->BoxConcessional->setText("Unmarked");
    ui->BoxProgression->setText("Unmarked");

    subs[subCount].setID(id);
    subs[subCount].setTitle(title);
    subs[subCount].setDocument(filename);
    subs[subCount].setDate(submissiondate);
    subs[subCount].setMark(Mark);
    subs[subCount].setConcCode(cCode);
    subs[subCount].setProgCode(pCode);
    subs[subCount].setAssessment(selAssessment);
    subs[subCount].setStudent(&currentStudent);
    subs[subCount].writeToFile("../submissions.txt");      //save to file
    subCount++;   //increment counter
    QMessageBox::information(this, "Success", "Submission has been successfully saved");
}

void AssessmentPage::on_BtnSubmission_clicked()
{
    QString urlStr;
    urlStr = ui->BoxSubmissionFilename->text();
    //exceptions
    bool openFile = false;
    try {
        openFile = QDesktopServices::openUrl(QUrl("file:" + urlStr));      //serve file (opens using default application)
        if (!openFile) throw openFile;      //if file was not opened, throw an exception
    }
    catch (bool result)
    {
        if (!result)        //check the result
        {
            QMessageBox::information(this, "Exception Caught", "This file could not be opened.");
        }else {
            QMessageBox::information(this, "Exception Caught", "Error 01: \nPlease report this bug to the administrator.");
        }
    }
}

void AssessmentPage::on_BtnAssessmentUpload_clicked()
{
    QUrl url;
    QString urlStr = "../resources/";

    url = QFileDialog::getOpenFileUrl(this, "Select a file to upload");     //open afile dialog box and get file
    urlStr += url.fileName().replace(" ", "_");        //convert filename to qstring
    //copy file to a new location
    bool check = QFile::copy(url.toString().replace("file:///", ""), urlStr);
    if (!check )
    {
        QMessageBox::warning(this, "Failure", "Failed to copy file, check permissions");
        return;
    }
    //to test, open the file
    ui->BoxFilenameAssessment->setText(urlStr);
}

void AssessmentPage::on_BtnSubmissionUpload_clicked()
{
    //assume we are uploading a file
    QUrl url;
    QString urlStr = "../resources/";

    url = QFileDialog::getOpenFileUrl(this, "Select a file to upload");     //open afile dialog box and get file
    urlStr += url.fileName();        //convert filename to qstring
    //copy file to a new location
    bool check = QFile::copy(url.toString().replace("file:///", ""), urlStr);
    if (!check )
    {
        QMessageBox::warning(this, "Failure", "Failed to copy file, check permissions");
        return;
    }
    ui->BoxSubmissionFilename->setText(urlStr);
}

void AssessmentPage::on_BoxMarksSubmission_textChanged(const QString &arg1)
{
    int marks = ui->BoxMarksSubmission->text().toInt();
    QString grade;
    if (marks < 40)    grade = "F";
    else if (40 <= marks && marks < 45)   grade = "E";
    else if (45 <= marks && marks < 50)   grade = "D";
    else if (50 <= marks && marks < 55)   grade = "C";
    else if (55 <= marks && marks < 60)   grade = "C+";
    else if (60 <= marks && marks < 65)   grade = "B";
    else if (65 <= marks && marks < 70)   grade = "B+";
    else if (70 <= marks && marks < 75)   grade = "A";
    else if (marks >= 80)   grade = "A+";
    ui->BoxGrade->setText(grade);
}

void AssessmentPage::on_BtnSubmitMark_clicked()
{
    //save mark
    QString pCode, cCode; int Mark;
    Mark = ui->BoxMarksSubmission->text().toInt();
    pCode = ui->BoxProgression->text();
    cCode = ui->BoxConcessional->text();
    pCode.replace(" ", "_");
    cCode.replace(" ", "_");
    subSelected->setMark(Mark);
    subSelected->setConcCode(cCode);
    subSelected->setProgCode(pCode);
    //update file
    subSelected->updateFile("../submissions.txt");
    QMessageBox::information(this, "Saved", "Mark has been saved.");
}

void AssessmentPage::on_HomeButton_clicked()
{
    close();
}

void AssessmentPage::on_ResourcesButton_clicked()
{
    close();
}

void AssessmentPage::on_CourseButton_clicked()
{
    close();
}

void AssessmentPage::on_SignOutButton_clicked()
{
    close();
}
