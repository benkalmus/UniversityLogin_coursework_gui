#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QMainWindow>
#include "studentpage.h"
#include "staffpage.h"
#include "student.h"
#include "user.h"


namespace Ui {
class LoginPage;
}

class LoginPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();
    std::vector<person::Employee> employeeList;


private slots:
    void on_ExitButton_clicked();
    void on_LoginButton_clicked();
    void openLogin();

signals:
    void sendEmployee(person::Employee e);
    void sendStudent(Student s);

private:
    Ui::LoginPage *ui;
    std::vector<Student> studentsList;
    StudentPage *studentWindow;
    StaffPage *staffWindow;
};

#endif // LOGINPAGE_H
