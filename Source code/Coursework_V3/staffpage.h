#ifndef ADMINPAGE_H
#define ADMINPAGE_H

#include "user.h"
#include "degreepage.h"
#include "courses.h"
#include "assessmentpage.h"
#include <QMainWindow>

namespace Ui {
class StaffPage;
}

class StaffPage : public QMainWindow
{

    Q_OBJECT

public:
    explicit StaffPage(QWidget *parent = nullptr);
    ~StaffPage();
    void loadEmployee(person::Employee *e);
    void clearAll();

public slots:
    void receiveEmployee(person::Employee e);


private slots:
    void on_BtnShowNINO_clicked();
    void on_BtnShowSalary_clicked();

    void on_SignOutButton_clicked();
    void on_SaveButton_clicked();

    void on_DegreeButton_clicked();

    void on_CoursesButton_clicked();

    void on_AssessmentsButton_clicked();

    void on_BtnBackup_clicked();

    void on_BtnRestore_clicked();

signals:
    void openLogin();
    void sendEmployee(person::Employee e);


private:
    Ui::StaffPage *ui;
    person::Employee currentUser;
    bool showNino, showSalary;
    //windows
    DegreePage *degreePage;
    Courses *coursePage;
    AssessmentPage *assessmentPage;
};

#endif // StaffPage
