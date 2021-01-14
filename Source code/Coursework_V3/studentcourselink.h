#ifndef STUDENTCOURSELINK_H
#define STUDENTCOURSELINK_H

#include <QDate>
#include "student.h"
#include "course.h"

namespace   slink   //student links to courses and degrees and other files..
{


class StudentCourseLink
{
public:
    StudentCourseLink();
    StudentCourseLink(Student &student, program::Course &course, QDate dateEnrolled)
    {
        this->student = &student;
        this->course = &course;
        this->dateEnrolled = dateEnrolled;
        this->TotalMark = 0;
        this->Grade = '_';
    }
    ~StudentCourseLink();

    void enroll(Student &student, program::Course &course, QDate dateEnrolled);
    void writeToFile(std::string filename);

    void setStudent(Student *s) {this->student = s;}
    void setCourse(program::Course *c) {this->course = c;}
    void setDate(QDate d) {this->dateEnrolled = d;}
    void setMark(int d) {this->TotalMark = d;}
    void setGrade(char grade) {this->Grade = grade;}

    Student* getStudent() {return this->student;}
    program::Course* getCourse()  {return this->course;}
    QDate getDate() {return this->dateEnrolled;}
    int getMark() {return this->TotalMark;}
    char getGrade() {return this->Grade;}

    int findLink(Student &student, program::Course &course, StudentCourseLink scLink[], int N);       //find a matching object given student and course, in array. returns index if found, else -1.

    int getCourseList(Student &student, StudentCourseLink scLink[], int N, int index[]);       //returns an array of indexes for course where a student match was found
    int getStudentList(program::Course &course, StudentCourseLink scLink[], int N, int index[]);       //returns an array of indexes for student where a course match was found

    //copies links to array and returns number found
    size_t copyFromFile(std::string filename, StudentCourseLink link[], size_t N, std::vector<Student> &sList, size_t sN, std::vector<program::Course> &cList, size_t cN);
    void removeFromFile(std::string filename);

private:
    Student *student;       //storing pointers to memory of where the actual student is stored, saves on memory as opposed to copying the entire student.
    program::Course *course;         //same for course, pointers.
    QDate dateEnrolled;
    int TotalMark;
    char Grade;
};

class StudentDegreeLink
{
public:
    StudentDegreeLink();

    Student* getStudent() {return this->student;}
    program::Degree* getDegree() {return this->degree;}
    QDate getDate() {return this->dateEnrolled;}
    int getFinalMark() {return  this->FinalMark;}

    void setStudent(Student *student) {this->student = student;}
    void setDegree(program::Degree *d) {this->degree = d;}
    void setDate(QDate date) {this->dateEnrolled = date;}
    void setFinalMark(int mark) {this->FinalMark = mark;}


    //copies links to array and returns number found
    size_t copyFromFile(std::string filename, StudentDegreeLink link[], size_t N, std::vector<program::Degree> &dlist, size_t dN, std::vector<Student> &slist, size_t cN);
    void writeToFile(std::string filename);

private:
    Student *student;
    program::Degree *degree;
    QDate dateEnrolled;
    int FinalMark;
};

}   //namespace

#endif // STUDENTCOURSELINK_H
