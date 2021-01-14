#ifndef DEGREEPAGE_H
#define DEGREEPAGE_H
#include "student.h"
#include "user.h"
#include "course.h"
#include "studentcourselink.h"
#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class DegreePage;
}

class DegreePage : public QMainWindow
{
    Q_OBJECT

public:
    explicit DegreePage(QWidget *parent = nullptr);
    ~DegreePage();
    void loadDegrees();

public slots:
    void receiveStudent(Student s);
    void receiveEmployee(person::Employee e);

private slots:
    void on_ListDegrees_itemClicked(QListWidgetItem *item);

    void on_viewCoursesBtn_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_addBtn_clicked();

    void on_HomeButton_clicked();

    void on_CoursesButton_clicked();

    void on_AssessmentsButton_clicked();

    void on_SignOutButton_clicked();

private:
    Ui::DegreePage *ui;
    Student currentStudent;
    person::Employee currentUser;
    //store degrees, courses and other..

    std::vector<program::Degree> dList;
    std::vector<Student> students;
    slink::StudentDegreeLink sList[MAX_DEGREES];
    size_t linkCount, degreeCount;

};

#endif // DEGREEPAGE_H
