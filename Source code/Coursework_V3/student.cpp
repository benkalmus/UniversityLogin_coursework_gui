#include "student.h"
#include <string>
#include <ios>
#include <iostream>
#include <fstream>
#include <QDebug>

int Student::studentCount = 0;


void Student::writeToFile(std::string filename)
{
    std::fstream file;
    //std::ios::out means writing to file
    //std::ios::app means appending to the end of the file.
    file.open(filename, std::ios::out | std::ios::app);
    if (file.is_open())
    {
        file << this->idUser << "\n";
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

void Student::printAllDetails()
{
    qDebug() << "Printing user " << this->idUser << ": ";
    qDebug() << this->FirstName;
    qDebug() << this->Surname;
    qDebug() << this->Email;
    qDebug() << this->PostCode;
    qDebug() << this->StreetAddress;
    qDebug() << this->County;
    qDebug() << this->DOB;
    qDebug() << this->Gender;
    qDebug() << this->Nationality;
    qDebug() << this->Password;
    qDebug() << "Finished";
}

size_t Student::copyFromFile(std::string filename, Student students[], size_t N)
{
    size_t studentCount=0;
    std::ifstream studentsFile;
    studentsFile.open(filename);     //open the students file as read only
    if (studentsFile.is_open())     //check if it's open first
    {
        std::string line; QString qline[12];
        size_t lineNumFile = 1, i = 0;
        //read line
        while (getline(studentsFile, line))
        {
            qline[i] = QString::fromStdString(line);
            //qDebug() << qline[i];
            if (lineNumFile % 11 == 0 && studentCount < N)  //every 11 lines, new student starts.
            {
                //line 1 and 2 are integers
                int idUser = qline[0].toInt();
                //line 3-8 string
                //line 9 is date
                QDate date;
                date = date.fromString(qline[7], "yyyy-MM-dd");  //convert qstring to qdate
                //the rest are strings
                students[studentCount].setDetails(qline[1], qline[2], qline[3], qline[4], qline[5],qline[6], date, qline[8], qline[9]);
                students[studentCount].setPassword(qline[10]);
                students[studentCount].setID(idUser);

                //students[studentCount].printAllDetails();

                studentCount++;
                i = 0;
            }
            else i++;        //increment
            lineNumFile++;
        }
    }
    studentsFile.close();   //close the file when no longer needed so that other programs can use it.
    return studentCount;
}

size_t Student::copyFromFile(std::string filename, std::vector<Student> &students, size_t N)
{
    size_t studentCount=0;
    std::ifstream studentsFile;
    studentsFile.open(filename);     //open the students file as read only
    if (studentsFile.is_open())     //check if it's open first
    {
        std::string line; QString qline[12];
        size_t lineNumFile = 1, i = 0;
        //read line
        while (getline(studentsFile, line))
        {
            qline[i] = QString::fromStdString(line);
            //qDebug() << qline[i];
            if (lineNumFile % 11 == 0 && studentCount < N)  //every 11 lines, new student starts.
            {
                //line 1 and 2 are integers
                int idUser = qline[0].toInt();
                //line 3-8 string
                //line 9 is date
                QDate date;
                date = date.fromString(qline[7], "yyyy-MM-dd");  //convert qstring to qdate
                students.push_back(Student());
                //the rest are strings
                students[studentCount].setDetails(qline[1], qline[2], qline[3], qline[4], qline[5],qline[6], date, qline[8], qline[9]);
                students[studentCount].setPassword(qline[10]);
                students[studentCount].setID(idUser);

                //students[studentCount].printAllDetails();

                studentCount++;
                i = 0;
            }
            else i++;        //increment
            lineNumFile++;
        }
    }
    studentsFile.close();   //close the file when no longer needed so that other programs can use it.
    return studentCount;
}

void Student::updateFile(std::string filename)
{
    std::ifstream file;
    std::ofstream temp;     //will copy details to this file
    file.open(filename);     //open the students file as read only
    temp.open("../temp.txt");
    std::string id, tempStr;
    int lines = 11;
    bool found = false;
    id = std::to_string(this->getID());      //convert id to string
    if (file.is_open() && temp.is_open())     //check if files opened
    {
        while (getline(file, tempStr))          //read line
        {
            if (tempStr == id)       //check if string matches student id
            {
                id = "____";        //ensures id is not matched more than once
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
                qDebug().noquote() << QString::fromStdString(tempStr) << "\n";
                temp << tempStr << "\n";
            }
        }
    }
    file.close();       //close files
    temp.close();
    //delete original
    std::remove(filename.c_str());
    //rename temp file to original file name
    std::rename("../temp.txt", filename.c_str());
    this->writeToFile(filename);        //append object to file.
}

bool Student::operator==(const Student &s)
{
    bool result = s.idUser == idUser;
    result &= s.FirstName == FirstName;
    result &= s.Surname == Surname;
    result &= s.Email == Email;
    result &= s.PostCode == PostCode;
    result &= s.StreetAddress == StreetAddress;
    result &= s.County == County;
    result &= s.DOB == DOB;
    result &= s.Gender == Gender;
    result &= s.Nationality == Nationality;
    result &= s.Password == Password;
    return result;
}

/*Student& Student::operator=(const Student &s)
{
    return *this;
}*/
