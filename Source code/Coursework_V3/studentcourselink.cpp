#include "studentcourselink.h"
#include "vector"
#include <string>
#include <ios>
#include <iostream>
#include <fstream>
#include <QDebug>

using namespace slink;

StudentCourseLink::StudentCourseLink()
{
    //empty
}

StudentCourseLink::~StudentCourseLink()
{
    //empty
}

void StudentCourseLink::enroll(Student &student, program::Course &course, QDate dateEnrolled)
{
    this->student = &student;
    this->course = &course;
    this->dateEnrolled = dateEnrolled;
}

void StudentCourseLink::writeToFile(std::string filename)
{
    std::fstream file;
    //std::ios::out means writing to file
    //std::ios::app means appending to the end of the file.
    file.open(filename, std::ios::out | std::ios::app);
    if (file.is_open())
    {
        //save the id links, so that the class can be reconstructed given student and course IDs
        file << this->student->getID() << "\n";
        file << this->course->getID() << "\n";
        file << this->dateEnrolled.toString("yyyy-MM-dd").toStdString() << "\n";
        file << this->TotalMark << "\n";
        file << this->Grade << "\n";
    }
    file.close();
}

int StudentCourseLink::findLink(Student &student, program::Course &course, StudentCourseLink scLink[], int N)
{
    for (int i=0; i < N; i++)
    {
        if (scLink[i].getStudent() == &student && scLink[i].getCourse() == &course)
        {
            return i;
        }
    }
    return -1;
}



int StudentCourseLink::getCourseList(Student &student, StudentCourseLink scLink[], int N, int index[])
{
    int numFound =0;
    //qDebug() << "student passed in ID " << student.getID();
    for (int i=0; i < N; i++)
    {
        //if (scLink[i].getStudent()->getID() == student.getID())
        if (scLink[i].getStudent() == &student)     //comparing the address of two students.
        {
            index[numFound] = i;    //add index to array
            numFound++;             //increment counter
        }
    }
    return numFound;
}

int StudentCourseLink::getStudentList(program::Course &course, StudentCourseLink scLink[], int N, int index[])
{
    int numFound =0;
    for (int i=0; i < N; i++)
    {
        if (scLink[i].getCourse() == &course)
        {
            index[numFound] = i;    //add index to array
            numFound++;             //increment counter
        }
    }
    return numFound;
}

size_t StudentCourseLink::copyFromFile(std::string filename, StudentCourseLink link[], size_t N, std::vector<Student> &sList, size_t sN, std::vector<program::Course> &cList, size_t cN)
{

    size_t count =0;
    std::ifstream file;
    file.open(filename);     //open the students file as read only

    if (file.is_open())     //check if it's open first
    {
        std::string line; QString qline[8];
        size_t lineNumFile = 1, i = 0;
        //read line
        while (getline(file, line))
        {
            qline[i] = QString::fromStdString(line);
            if (lineNumFile % 5 == 0 && count < N)  //every 5 lines, new object starts.
            {
                //line 1,2  and 4 are integers
                int studentID = qline[0].toInt();
                int courseID = qline[1].toInt();
                int Mark = qline[3].toInt();
                //line 3 is date
                QDate date; date.fromString(qline[2], "yyyy-MM-dd");
                link[count].setDate(date);
                link[count].setMark(Mark);
                //line 5 is char
                link[count].setGrade(qline[5].toStdString()[0]);        //set grade

                //find matching course and degree IDs.
                for(size_t d=0; d < sN; d++)
                {
                    if (sList[d].getID() == studentID)      //found degree
                    {
                        link[count].setStudent(&sList[d]);   //add degree to link
                        break;
                    }
                }
                //repeat for course
                for(size_t c=0; c < cN; c++)
                {
                    if (cList[c].getID() == courseID)      //found course
                    {
                        link[count].setCourse(&cList[c]);   //add course to link
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

void StudentCourseLink::removeFromFile(std::string filename)
{
    std::ifstream file;
    std::ofstream temp;     //will copy details to this file
    file.open(filename);     //open the students file as read only
    temp.open("../temp.txt");
    std::string idS, idC, tempStr, prevStr = "_";
    int lines = 5;
    bool found = false;
    idS = std::to_string(this->student->getID());      //convert id to string
    idC = std::to_string(this->course->getID());      //convert id to string
    if (file.is_open() && temp.is_open())     //check if files opened
    {
        while (getline(file, tempStr))          //read line
        {
            if (tempStr == idS && prevStr == idC)       //check if string matches student id
            {
                idS = "____";        //ensures id is not matched more than once
                idC = "____";
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
            prevStr = tempStr;
        }
    }
    file.close();       //close files
    temp.close();
    //delete original
    std::remove(filename.c_str());
    //rename temp file to original file name
    std::rename("../temp.txt", filename.c_str());
}


StudentDegreeLink::StudentDegreeLink()
{
    //empty
}

size_t StudentDegreeLink::copyFromFile(std::string filename, StudentDegreeLink link[], size_t N, std::vector<program::Degree> &dlist, size_t sN, std::vector<Student> &slist, size_t cN)
{

    size_t count =0;
    std::ifstream file;
    file.open(filename);     //open the students file as read only

    if (file.is_open())     //check if it's open first
    {
        std::string line; QString qline[6];
        size_t lineNumFile = 1, i = 0;
        //read line
        while (getline(file, line))
        {
            qline[i] = QString::fromStdString(line);
            if (lineNumFile % 4 == 0 && count < N)  //every 4 lines, new object starts.
            {
                //line 1,2  and 4 are integers
                int studentID = qline[0].toInt();
                int degreeID = qline[1].toInt();
                int Mark = qline[3].toInt();
                QDate date; date.fromString(qline[2], "yyyy-MM-dd");
                link[count].setDate(date);
                link[count].setFinalMark(Mark);

                //find matching course and degree IDs.
                for(size_t d=0; d < sN; d++)
                {
                    if (dlist[d].getID() == degreeID)      //found degree
                    {
                        link[count].setDegree(&dlist[d]);   //add degree to link
                        break;
                    }
                }
                //repeat for course
                for(size_t c=0; c < cN; c++)
                {
                    if (slist[c].getID() == studentID)      //found course
                    {
                        link[count].setStudent(&slist[c]);   //add course to link
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

void StudentDegreeLink::writeToFile(std::string filename)
{
    std::fstream file;
    //std::ios::out means writing to file
    //std::ios::app means appending to the end of the file.
    file.open(filename, std::ios::out | std::ios::app);
    if (file.is_open())
    {
            /*Student *student;
        program::Degree *degree;
        QDate dateEnrolled;
        int FinalMark;*/
        file << this->student->getID() << "\n";
        file << this->degree->getID() << "\n";
        file << this->dateEnrolled.toString("yyyy-MM-dd").toStdString() << "\n";
        file << this->FinalMark << "\n";
    }
    file.close();
}
