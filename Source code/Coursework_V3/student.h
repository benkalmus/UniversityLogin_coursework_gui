#include "user.h"
#include <string>
#include "QString"
#include "QDate"
#include "QObject"

#ifndef STUDENT_H
#define STUDENT_H

//student class inherits from User
class Student : public person::User
{
public:
    Student() : person::User ()
    {this->studentCount++;}
    Student(int idUser, QString FirstName,QString Surname,QString Email,QString PostCode, QString StreetAddress, QString County,
            QDate DOB, QString Gender, QString Nationality, QString Password) : person::User()
    {
        this->studentCount++;
        this->idUser = idUser;
        this->FirstName = FirstName;
        this->Surname = Surname;
        this->Email = Email;
        this->PostCode = PostCode;
        this->StreetAddress = StreetAddress;
        this->County = County;
        this->DOB = DOB;
        this->Gender = Gender;
        this->Nationality = Nationality;
        this->Password = Password;
    }
//functions
    int getStudentCount() {return this->studentCount;}

    void writeToFile(std::string filename);
    void printAllDetails();
    size_t copyFromFile(std::string filename, Student students[], size_t N);
    size_t copyFromFile(std::string filename, std::vector<Student> &students, size_t N);
    void updateFile(std::string filename);
    //operators
    bool operator==(const Student &s);      //compare
    //Student &operator=(const Student &s);      //assignment

protected:

private:
    static int studentCount;        //global count of students created
};

Q_DECLARE_METATYPE(Student)
Q_DECLARE_METATYPE(Student*)


#endif // STUDENT_H
