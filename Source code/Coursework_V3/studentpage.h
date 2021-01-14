#ifndef STUDENTPAGE_H
#define STUDENTPAGE_H
#include "student.h"
#include "degreepage.h"
#include "courses.h"
#include <QMainWindow>
#include "assessmentpage.h"


namespace Ui {

class StudentPage;
}

class StudentPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentPage(QWidget *parent = nullptr);
    ~StudentPage();
    //void loadStudent(Ui::StudentPage  *ui, Student *s);
    void loadStudent(Student *s);       //loads values into textboxes

public slots:
    void receiveStudent(Student s);

private slots:
    void on_SaveButton_clicked();
    void on_SignOutButton_clicked();

    void on_DegreeButton_clicked();

    void on_CoursesButton_clicked();

    void on_AssessmentsButton_clicked();

signals:
    void openLogin();
    void sendStudent(Student s);

private:
    Ui::StudentPage *ui;
    Student currentStudent;
    DegreePage *degreePage;
    Courses *coursePage;
    AssessmentPage *assessmentPage;

};

#endif // STUDENTPAGE_H
