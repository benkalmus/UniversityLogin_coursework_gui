#include "user.h"
#include <string>
#include <ios>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QCryptographicHash>

int person::User::userCount=0;
int person::Employee::staffCount=0;

namespace person
{

User::User()
{
    this->userCount++;
    this->idUser = -1;
}

person::User::User(int idUser, QString FirstName, QString Surname, QString Email, QString PostCode, QString StreetAddress,
           QString County, QDate DOB, QString Gender, QString Nationality, QString Password)
{
    Email = Email.replace(" ", "").toLower();       //removes whitespaces and turns to lowercase
    this->userCount++;
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

int person::User::getUserCount()
{
    return User::userCount;
}

void User::printUserDetails()
{
    qDebug() << this->idUser;
    qDebug() << this->FirstName;
    qDebug() << this->Surname;
    qDebug() << this->Email;
    qDebug() << this->PostCode;
    qDebug() << this->StreetAddress;
    qDebug() << this->County;
    qDebug() << this->DOB.toString("yyyy-MM-dd");
    qDebug() << this->Gender;
    qDebug() << this->Nationality;
    qDebug() << this->Password;
}

QString User::sha1Hash(QString text)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);      //declare hashing algorithm
    hash.addData(text.toLocal8Bit());       //convert string to a byte array
    QByteArray res = hash.result();     //return the hash result
    return res.toHex();           //return a hex code (40 characters)
}

void person::User::setID(int id)
{
    this->idUser = id;
}

void person::User::setDetails(QString FirstName, QString Surname, QString Email, QString PostCode, QString StreetAddress, QString County, QDate DOB, QString Gender, QString Nationality)
{
    Email = Email.replace(" ", "").toLower();       //removes whitespaces and turns to lowercase
    this->FirstName = FirstName;
    this->Surname = Surname;
    this->Email = Email;
    this->PostCode = PostCode;
    this->StreetAddress = StreetAddress;
    this->County = County;
    this->DOB = DOB;
    this->Gender = Gender;
    this->Nationality = Nationality;
}

void User::setPassword(QString pass)
{
    this->Password = pass;        //trims whitespace
}

void User::setEmail(QString email)
{
    email = email.replace(" ", "").toLower();       //removes whitespaces and turns to lowercase
    this->Email = email;
}


int person::User::getID()
{
    return this->idUser;
}

void person::User::getDetails(QString &FirstName, QString &Surname, QString &Email, QString &PostCode, QString &StreetAddress, QString &County, QDate &DOB, QString &Gender, QString &Nationality)
{
    FirstName = this->FirstName;
    Surname = this->Surname;
    Email = this->Email;
    PostCode = this->PostCode;
    StreetAddress = this->StreetAddress;
    County = this->County;
    DOB = this->DOB;
    Gender = this->Gender;
    Nationality = this->Nationality;
}

QString User::getFullName()
{
    QString temp = this->FirstName;
    temp += " ";
    temp += this->Surname;
    return temp;
}


bool person::User::operator==(const person::User &u)
{
    bool result = u.idUser == idUser;
    result &= u.FirstName == FirstName;
    result &= u.Surname == Surname;
    result &= u.Email == Email;
    result &= u.PostCode == PostCode;
    result &= u.StreetAddress == StreetAddress;
    result &= u.County == County;
    result &= u.DOB == DOB;
    result &= u.Gender == Gender;
    result &= u.Nationality == Nationality;
    result &= u.Password == Password;
    return result;
}


//  #########################################################           EMPLOYEEE           ####################################################

void person::Employee::printAllDetails()
{
    qDebug() << this->idUser;
    qDebug() << this->OfficeNumber;
    qDebug() << this->NINO;
    qDebug() << this->Role;
    qDebug() << this->AnnualSalary;

    printUserDetails();
}

void Employee::updateFile(std::string filename)
{

    std::ifstream file;
    std::ofstream temp;     //will copy details to this file
    file.open(filename);     //open the file as read only
    temp.open("../temp.txt");
    std::string id, tempStr;
    int lines = 15;
    bool found = false;
    id = std::to_string(this->getID());      //convert id to string
    if (file.is_open() && temp.is_open())     //check if files opened
    {
        while (getline(file, tempStr))          //read line
        {
            if (tempStr == id && found == false)       //check if string matches  id
            {
                id = "___";     //make sure id is never matched again so that no more records get altered
                found = true;
            }

            if (found == true)
            {
                //do not copy lines
                lines--;
                if (lines == 0) found = false;       //continue copying as normal
            }
            else {
                //copy line to temp
                temp << tempStr << "\n";
            }
            tempStr.clear();
        }
    }
    file.close();       //close files
    temp.close();
    //delete original
    std::remove(filename.c_str());
    //rename temp file to original file name
    std::rename("../temp.txt", filename.c_str());
    this->writeToFile(filename);        //add object to end of file
}

void person::Employee::writeToFile(std::string filename)
{
    std::fstream file;
    //std::ios::out means writing to file
    //std::ios::app means appending to the end of the file.
    file.open(filename, std::ios::out | std::ios::app);
    if (file.is_open())
    {
        file << this->idUser << "\n";
        file << this->OfficeNumber.toStdString() << "\n";
        file << this->NINO.toStdString() << "\n";
        file << this->Role.toStdString() << "\n";
        file << this->AnnualSalary << "\n";

        file << this->FirstName.toStdString() << "\n";
        file << this->Surname.toStdString() << "\n";
        file << this->Email.toStdString() << "\n";
        file << this->PostCode.toStdString() << "\n";
        file << this->StreetAddress.toStdString() << "\n";
        file << this->County.toStdString() << "\n";
        file << this->DOB.toString("yyyy-MM-dd").toStdString() << "\n";
        file << this->Gender.toStdString() << "\n";
        file << this->Nationality.toStdString() << "\n";
        file << this->Password.toStdString() << "\n";
    }
    file.close();
}

size_t Employee::copyFromFile(std::string filename, Employee empl[], size_t N)
{
    size_t count=0;
    std::ifstream file;
    file.open(filename);     //open the file as read only
    if (file.is_open())     //check if it's open first
    {
        std::string line; QString qline[16];
        size_t lineNumFile = 1, i = 0;
        //read line
        while (getline(file, line))
        {
            qline[i] = QString::fromStdString(line);
            //qDebug() << qline[i];
            if (lineNumFile % 15 == 0 && count < N)  //every 15 lines, new user starts.
            {
                int idUser = qline[0].toInt();
                int salary = qline[4].toInt();
                QDate date;
                date = date.fromString(qline[11], "yyyy-MM-dd");  //convert qstring to qdate
                //qDebug() << qline[15];
                //line 14-17 string
                //the rest are strings
                empl[count].setDetails(qline[5], qline[6], qline[7], qline[8], qline[9],qline[10], date, qline[12], qline[13]);
                empl[count].setPassword(qline[14]);
                empl[count].setID(idUser);
                empl[count].setOfficeNumber(qline[1]);
                empl[count].setNINO(qline[2]);
                empl[count].setRole(qline[3]);
                empl[count].setEmplSalary(salary);
                count++;
                i = 0;
            }
            else i++;        //increment
            lineNumFile++;
        }
    }
    file.close();   //close the file when no longer needed so that other programs can use it.
    return count;
}

size_t Employee::copyFromFile(std::string filename, std::vector<Employee> &empl, size_t N)
{
    size_t count=0;
    std::ifstream file;
    file.open(filename);     //open the file as read only
    if (file.is_open())     //check if it's open first
    {
        std::string line; QString qline[16];
        size_t lineNumFile = 1, i = 0;
        //read line
        while (getline(file, line))
        {
            qline[i] = QString::fromStdString(line);
            //qDebug() << qline[i];
            if (lineNumFile % 15 == 0 && count < N)  //every 15 lines, new user starts.
            {
                //line 1 and 2 are integers
                int idUser = qline[0].toInt();
                //line 3-5 string
                //line 6 is salary, int
                int salary = qline[4].toInt();
                //line 7-12 string
                //line 13 dob
                QDate date;
                date = date.fromString(qline[11], "yyyy-MM-dd");  //convert qstring to qdate
                //line 14-17 string
                //the rest are strings
                empl.push_back(Employee());
                empl[count].setDetails(qline[5], qline[6], qline[7], qline[8], qline[9],qline[10], date, qline[12], qline[13]);
                empl[count].setPassword(qline[14]);
                empl[count].setID(idUser);
                empl[count].setOfficeNumber(qline[1]);
                empl[count].setNINO(qline[2]);
                empl[count].setRole(qline[3]);
                empl[count].setEmplSalary(salary);
                count++;
                i = 0;
            }
            else i++;        //increment
            lineNumFile++;
        }
    }
    file.close();   //close the file when no longer needed so that other programs can use it.
    return count;
}


}       //namespace person


