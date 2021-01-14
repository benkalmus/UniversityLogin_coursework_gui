#ifndef COURSES_H
#define COURSES_H
#include "student.h"
#include "course.h"
#include "user.h"
#include "studentcourselink.h"

#include "vector"
#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class Courses;
}

class Courses : public QMainWindow
{
    Q_OBJECT

public:
    explicit Courses(QWidget *parent = nullptr);
    ~Courses();
    void loadDegrees();
    void loadCourses();
    void loadFiles();
private slots:
    void on_ListDegrees_itemClicked(QListWidgetItem *item);
    void on_ListCourses_itemClicked(QListWidgetItem *item);


    void on_checkBox_stateChanged(int arg1);

    void on_BtnEnroll_clicked();

    void on_BtnSaveCourse_clicked();

    void on_HomeButton_clicked();

    void on_ResourcesButton_clicked();

    void on_AccountButton_clicked();

    void on_SignOutButton_clicked();

    void on_BtnRemoveCourse_clicked();

    void on_BtnWithdraw_clicked();

public slots:
    void receiveStudent(Student s);
    void receiveEmployee(person::Employee e);

private:
    Ui::Courses *ui;
    Student currentStudent;
    person::Employee currentUser;
    size_t count, degreeCount, courseCount, sdCount, scCount;
    program::DegreeCourseLink link[MAX_DEGREES + MAX_COURSES];
    std::vector<program::Course> course;
    std::vector<program::Degree> degree;
    slink::StudentDegreeLink sdlink[MAX_DEGREES];
    slink::StudentCourseLink sclink[MAX_DEGREES];
    std::vector<Student> students;
    program::Degree * degSelected;
};

#endif // COURSES_H
