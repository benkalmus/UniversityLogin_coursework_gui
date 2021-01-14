#include "QString"
#include "QDate"
#include <QObject>
#include <string>
#include <ios>
#include <iostream>
#include <fstream>

#ifndef USER_H
#define USER_H



template<class T>
long getUserIndex(QString email, QString password, T object[], size_t N)
{
    for (size_t i=0; i < N; i++)
    {
        if (email == object[i].getEmail() && password == object[i].getPassword())
        {
            //found user
            return long(i);
        }
    }
    return -1;  //return a negative number if item not found in object array
}
//template int getUserIndex<Student>(QString, QString, Student*, int);

namespace person
{
    class User
    {
    public:
        User();
        User(int idUser, QString FirstName,QString Surname,QString Email,QString PostCode, QString StreetAddress, QString County,
             QDate DOB, QString Gender, QString Nationality, QString Password);
        ~User() {this->userCount--;}
        //functions
        int getUserCount();
        int generateID();
        void printUserDetails();
        QString sha1Hash(QString text);
        //template <class T>
        //int getUserIndex(QString email, QString password, T object[], int N);     //god knows how to implement this into a class


    //setters
        void setID(int id);
        void setDetails(QString FirstName,QString Surname,QString Email,QString PostCode, QString StreetAddress, QString County,
                        QDate DOB, QString Gender, QString Nationality);
        void setPassword(QString pass);
        void setEmail(QString email);

    //getters
        int getID();
        void getDetails(QString &FirstName,QString &Surname,QString &Email,QString &PostCode, QString &StreetAddress, QString &County,
                        QDate &DOB, QString &Gender, QString &Nationality);

        QString getPassword() {return this->Password;}
        QString getEmail() {return this->Email;}
        QString getFullName();

        //operators
        bool operator==(const User &u);      //compare
    protected:
        int idUser;
        QString FirstName;
        QString Surname;
        QString Email;
        QString PostCode;
        QString StreetAddress;
        QString County;
        QDate DOB;
        QString Gender;
        QString Nationality;
        QString Password;

    //private:
        static int userCount;
    };

    //  ########################                EMPLOYEE CLASS      ############################################################################
    class Employee : public User
    {
    public:
        Employee() :  User() {this->staffCount++;}
        Employee(QString officeNum, QString nino, QString role, int salary, int idUser, QString FirstName,QString Surname,
              QString Email,QString PostCode, QString StreetAddress, QString County,
              QDate DOB, QString Gender, QString Nationality, QString Password) : User()
        {
            this->staffCount++;
            this->OfficeNumber = officeNum;
            this->NINO = nino;
            this->Role = role;
            this->AnnualSalary = salary;

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
        ~Employee() {this->staffCount--;}
        //functions
        //setters
        void setOfficeNumber(QString office) {this->OfficeNumber = office;}
        void setNINO(QString national_insurance_number) {this->NINO = national_insurance_number;}
        void setRole(QString role) {this->Role = role;}
        void setEmplSalary(int number) {this->AnnualSalary = number;}
        //getters
        QString getOfficeNumber() {return this->OfficeNumber;}
        QString getNINO() {return this->NINO;}
        QString getRole() {return this->Role;}
        int getEmplSalary() {return this->AnnualSalary;}

        void writeToFile(std::string filename);
        size_t copyFromFile(std::string filename, Employee empl[], size_t N);
        size_t copyFromFile(std::string filename, std::vector<Employee> &empl, size_t N);
        void printAllDetails();
        void updateFile(std::string filename);



        //operators
        bool operator==(const Employee &e);      //compare
    protected:
        QString OfficeNumber;
        QString NINO;       //National Insurance Number
        QString Role;
        int AnnualSalary;

    private:
        static int staffCount;
    };

}



Q_DECLARE_METATYPE(person::User)
Q_DECLARE_METATYPE(person::Employee)
Q_DECLARE_METATYPE(person::User*)
Q_DECLARE_METATYPE(person::Employee*)

#endif // USER_H
