#include "course.h"
#include <string>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QMessageBox>


int program::Course::courseCount =0;
int program::Degree::degreeCount = 0;




namespace program
{

program::Course::Course()
{
    this->courseCount++;
}

program::Course::Course(int idCourse, QString Title, QString Description, QString Department, QString Level, QDate StartDate, QDate EndDate, float Fees)
{
    this->courseCount++;
    this->idCourse = idCourse;
    this->Title = Title;
    this->Description = Description;
    this->Department = Department;
    this->Level = Level;
    this->StartDate = StartDate;
    this->EndDate = EndDate;
    this->Fees = Fees;
}

size_t program::Course::copyFromFile(std::string filename, Course courses[], size_t N)
{
    size_t count=0;
    std::ifstream coursesFile;
    coursesFile.open(filename);     //open the file as read only
    if (coursesFile.is_open())     //check if it's open first
    {
        std::string line; QString qline[9];
        size_t lineNumFile = 1, i = 0;
        //read line
        while (getline(coursesFile, line))
        {
            qline[i] = QString::fromStdString(line);
           // qDebug() << qline[i];
            if (lineNumFile % 8 == 0 && count < N)  //every 8 lines, new object starts.
            {
                //line 1 is integers
                int id = qline[0].toInt();
                //line 2-5 string
                //line 6-7 date
                QDate startDate, endDate;
                startDate = startDate.fromString(qline[5], "yyyy-MM-dd");  //convert qstring to qdate
                endDate =endDate.fromString(qline[6], "yyyy-MM-dd");
                //line 8 float
                float fees = qline[7].toFloat();        //converts to float
                courses[count].setDetails(qline[1], qline[2], qline[3], qline[4], startDate, endDate, fees);
                courses[count].setID(id);
                //courses[count].printAllDetails();
                count++;
                i = 0;
            }
            else i++;        //increment
            lineNumFile++;
        }
    }
    coursesFile.close();   //close the file when no longer needed so that other programs can use it.
    return count;
}

size_t Course::copyFromFile(std::string filename, std::vector<Course> &courses, size_t N)
{
    size_t count =0;
    std::ifstream coursesFile;
    coursesFile.open(filename);     //open the file as read only
    if (coursesFile.is_open())     //check if it's open first
    {
        std::string line; QString qline[9];
        size_t lineNumFile = 1, i = 0;
        //read line
        while (getline(coursesFile, line))
        {
            qline[i] = QString::fromStdString(line);
           // qDebug() << qline[i];
            if (lineNumFile % 8 == 0 && count < N)  //every 8 lines, new object starts.
            {
                //line 1 is integers
                int id = qline[0].toInt();
                //line 2-5 string
                //line 6-7 date
                QDate startDate, endDate;
                startDate = startDate.fromString(qline[5], "yyyy-MM-dd");  //convert qstring to qdate
                endDate =endDate.fromString(qline[6], "yyyy-MM-dd");
                //line 8 float
                float fees = qline[7].toFloat();        //converts to float
                courses.push_back(Course());
                courses[count] = Course();
                courses[count].setDetails(qline[1], qline[2], qline[3], qline[4], startDate, endDate, fees);
                courses[count].setID(id);
                //courses[count].printAllDetails();
                count++;
                i = 0;
            }
            else i++;        //increment
            lineNumFile++;
        }
    }
    coursesFile.close();   //close the file when no longer needed so that other programs can use it.
    return count;
}

void program::Course::writeToFile(std::string filename)
{
    std::fstream file;
    //std::ios::out means writing to file
    //std::ios::app means appending to the end of the file.
    file.open(filename, std::ios::out | std::ios::app);
    if (file.is_open())
    {
        file << this->idCourse << "\n";
        file << this->Title.toStdString() << "\n";
        file << this->Description.toStdString() << "\n";
        file << this->Department.toStdString() << "\n";
        file << this->Level.toStdString() << "\n";
        file << this->StartDate.toString("yyyy-MM-dd").toStdString() << "\n";
        file << this->EndDate.toString("yyyy-MM-dd").toStdString() << "\n";
        file << this->Fees << "\n";
    }
    file.close();
}

void Course::removeFromFile(std::string filename)
{
    std::ifstream file;
    std::ofstream temp;     //will copy details to this file
    file.open(filename);     //open the students file as read only
    temp.open("../temp.txt");
    std::string id, tempStr;
    int lines = 8;
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
}

void Course::updateFile(std::string filename)
{
    std::ifstream file;
    std::ofstream temp;     //will copy details to this file
    file.open(filename);     //open the students file as read only
    temp.open("../temp.txt");
    std::string id, tempStr;
    int lines = 8;
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
    this->writeToFile(filename);
}

bool program::Course::operator==(const Course &c)
{
    bool result = c.idCourse == idCourse;
    result &= c.Title == Title;
    result &= c.Description == Description;
    result &= c.Department == Department;
    result &= c.Level == Level;
    result &= c.EndDate == EndDate;
    //result &= c.Fees == Fees;     //comparing floats with== is unsafe
    return result;
}

void program::Course::printAllDetails()
{
    qDebug() << "Printing course " << this->idCourse << ": ";
    qDebug() << this->Title;
    qDebug() << this->Description;
    qDebug() << this->Department;
    qDebug() << this->Level;
    qDebug() << this->StartDate;
    qDebug() << this->EndDate;
    qDebug() << this->Fees;
    qDebug() << "Finished";
}

void program::Course::setDetails(QString Title, QString Description, QString Department, QString Level, QDate StartDate, QDate EndDate, float Fees)
{
    this->Title=Title;
    this->Title = Title;
    this->Description = Description;
    this->Department = Department;
    this->Level = Level;
    this->StartDate = StartDate;
    this->EndDate = EndDate;
    this->Fees = Fees;
}

void Course::getDetails(QString &Title, QString &Description, QString &Department, QString &Level, QDate &StartDate, QDate &EndDate, float &Fees)
{
    Title = this->Title;
    Description = this->Description;
    Department = this->Department;
    Level = this->Level;
    StartDate = this->StartDate;
    EndDate = this->EndDate;
    Fees = this->Fees;
}


//  ##########################################################  Degree Class     #################################

size_t program::Degree::copyFromFile(std::string filename, program::Degree degrees[], size_t N)
{
    size_t count =0;
    std::ifstream file;
    file.open(filename);     //open the file as read only
    if (file.is_open())     //check if it's open first
    {
        std::string line; QString qline[5];
        size_t lineNumFile = 1, i = 0;
        //read line
        while (getline(file, line))
        {
            qline[i] = QString::fromStdString(line);

            if (lineNumFile % 5 == 0 && count < N)  //every 5 lines, new object starts.
            {
                //line 1 and 5 integers
                int id = qline[0].toInt();
                int duration = qline[4].toInt();
                //line 2-4 string
                degrees[count].setID(id);
                degrees[count].setTitle(qline[1]);
                degrees[count].setQualification(qline[2]);
                degrees[count].setDetails(qline[3]);
                degrees[count].setDuration(duration);
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

size_t Degree::copyFromFile(std::string filename, std::vector<Degree> &degrees, size_t N)
{
    size_t count =0;
    std::ifstream file;
    file.open(filename);     //open the file as read only
    if (file.is_open())     //check if it's open first
    {
        std::string line; QString qline[5];
        size_t lineNumFile = 1, i = 0;
        //read line
        while (getline(file, line))
        {
            qline[i] = QString::fromStdString(line);

            if (lineNumFile % 5 == 0 && count < N)  //every 5 lines, new object starts.
            {
                //line 1 and 5 integers
                int id = qline[0].toInt();
                int duration = qline[4].toInt();
                //line 2-4 string
                degrees.push_back(Degree());
                degrees[count] = Degree();
                degrees[count].setID(id);
                degrees[count].setTitle(qline[1]);
                degrees[count].setQualification(qline[2]);
                degrees[count].setDetails(qline[3]);
                degrees[count].setDuration(duration);
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

void Degree::writeToFile(std::string filename)
{
    std::fstream file;
    //std::ios::out means writing to file
    //std::ios::app means appending to the end of the file.
    file.open(filename, std::ios::out | std::ios::app);
    if (file.is_open())
    {
        file << this->idDegree << "\n";
        file << this->Title.toStdString() << "\n";
        file << this->Qualification.toStdString() << "\n";
        file << this->DegreeDetails.toStdString() << "\n";
        file << this->DurationYears << "\n";
    }
    file.close();
}

void Degree::updateFile(std::string filename)
{
    std::ifstream file;
    std::ofstream temp;     //will copy details to this file
    file.open(filename);     //open the students file as read only
    temp.open("../temp.txt");
    std::string id, tempStr;
    int lines = 5;
    int lineNum = 0;
    bool found = false;
    id = std::to_string(this->getID());      //convert id to string
    if (file.is_open() && temp.is_open())     //check if files opened
    {
        while (getline(file, tempStr))          //read line
        {
            if ( lineNum % 5 == 0 && tempStr == id)       //check if string matches  id
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
                temp << tempStr << "\n";
            }
            lineNum++;
        }
    }
    file.close();       //close files
    temp.close();
    //delete original
    std::remove(filename.c_str());
    //rename temp file to original file name
    std::rename("../temp.txt", filename.c_str());
    this->writeToFile(filename);
}

DegreeCourseLink::DegreeCourseLink()
{
    //empty
}

size_t DegreeCourseLink::copyFromFile(std::string filename, DegreeCourseLink link[], size_t N, std::vector<Degree> &degreeList, size_t dN, std::vector<Course> &courseList, size_t cN)
{
    size_t count =0;
    std::ifstream file;
    file.open(filename);     //open the students file as read only

    if (file.is_open())     //check if it's open first
    {
        std::string line; QString qline[2];
        size_t lineNumFile = 1, i = 0;
        //read line
        while (getline(file, line))
        {
            qline[i] = QString::fromStdString(line);
           // qDebug() << qline[i];

            if (lineNumFile % 2 == 0 && count < N)  //every 2 lines, new object starts.
            {
                //line 1 and 2 are integers
                int degreeID = qline[0].toInt();
                int courseID = qline[1].toInt();
                //find matching course and degree IDs.
                for(size_t d=0; d < dN; d++)
                {
                    if (degreeList[d].getID() == degreeID )      //found degree
                    {
                        link[count].setDegree(&degreeList[d]);   //add degree to link
                        //qDebug() << "d " << link[count].getDegree()->getID();
                        break;      //leave scope
                    }
                }
                //repeat for course
                for(size_t c=0; c < cN; c++)
                {
                    if (courseList[c].getID() == courseID)      //found course
                    {
                        link[count].setCourse(&courseList[c]);   //add course to link
                        //qDebug() << "c " << link[count].getCourse()->getID();
                        break;
                    }
                }
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

void DegreeCourseLink::writeToFile(std::string filename)
{
    std::fstream file;
    //std::ios::out means writing to file
    //std::ios::app means appending to the end of the file.
    file.open(filename, std::ios::out | std::ios::app);
    if (file.is_open())
    {
        file << this->dPtr->getID() << "\n";
        file << this->cPtr->getID() << "\n";
    }
    file.close();
}



}   //namespace
