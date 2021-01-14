#ifndef ASSESSMENTPAGE_H
#define ASSESSMENTPAGE_H

#include "assessment.h"
#include "student.h"
#include "course.h"
#include "studentcourselink.h"
#include "user.h"

#include "vector"
#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class AssessmentPage;
}

class AssessmentPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit AssessmentPage(QWidget *parent = nullptr);
    ~AssessmentPage();
    void loadStudents();
    void loadCourses();
    void loadAssessments();
    void loadFiles();
    void clearAll();

public slots:
    void receiveStudent(Student s);
    void receiveEmployee(person::Employee e);

private slots:
    void on_ListStudents_itemClicked(QListWidgetItem *item);

    void on_ListCourses_itemClicked(QListWidgetItem *item);

    void on_ListAssessments_itemClicked(QListWidgetItem *item);

    void on_BtnAssessment_clicked();

    void on_BtnAddAssessment_clicked();

    void on_BtnSave_clicked();

    void on_BtnSubmission_clicked();

    void on_BtnAssessmentUpload_clicked();

    void on_BtnSubmissionUpload_clicked();

    void on_BoxMarksSubmission_textChanged(const QString &arg1);

    void on_BtnSubmitMark_clicked();

    void on_HomeButton_clicked();

    void on_ResourcesButton_clicked();

    void on_CourseButton_clicked();

    void on_SignOutButton_clicked();

private:
    Ui::AssessmentPage *ui;
    Student currentStudent;
    person::Employee currentUser;
    size_t studentCount, count, degreeCount, courseCount, sdCount, scCount, aCount, subCount;
    std::vector<Student> students;
    std::vector<program::Course> course;
    std::vector<grade::Assessment> assessments;
    std::vector<grade::Submission> subs;
    slink::StudentDegreeLink sdlink[MAX_DEGREES];
    slink::StudentCourseLink sclink[MAX_DEGREES];
    Student* selectedStudent;
    program::Course *selectedCourse;
    grade::Assessment *selAssessment;
    grade::Submission *subSelected;

};

#endif // ASSESSMENTPAGE_H
