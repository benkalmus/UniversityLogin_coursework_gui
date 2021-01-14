#include "QString"
#include "QDate"
#include "student.h"
#include <QDebug>

#ifndef COURSE_H
#define COURSE_H

#define MAX_DEGREES 50
#define MAX_COURSES 50

template<class T>       //ID GENERATING FUNCTION TEMPLATE
int generate(std::vector<T> list, size_t sizeOfList)
{
    int random = std::rand();
    for (size_t i=0; i<sizeOfList; i++) {
        if (list[i].getID() == random){
            i = 0;      //restart counter
            random = std::rand(); //generate new id
        }
    }
    //if got to here, return id
    return random;
}

namespace program
{

class Course
{
public:
    Course();
    Course(int idCourse, QString Title, QString Description,QString Department,QString Level,QDate StartDate, QDate EndDate, float Fees );
    ~Course()
    {
        if(this->courseCount != 0)  this->courseCount--;
    }
    //list of students on this course

    size_t copyFromFile(std::string filename, Course courses[], size_t N);
    size_t copyFromFile(std::string filename, std::vector<Course> &courses, size_t N);
    void writeToFile(std::string filename);
    void removeFromFile(std::string filename);
    void updateFile(std::string filename);
    bool operator==(const Course &c);
    void printAllDetails();

    void setDetails(QString Title, QString Description,QString Department,QString Level,QDate StartDate, QDate EndDate, float Fees );
    void setID(int id) {this->idCourse = id;}

    void getDetails(QString &Title, QString &Description,QString &Department,QString &Level,QDate &StartDate, QDate &EndDate, float &Fees );
    QString getLevel() {return this->Level;}
    QString getDepartment() {return this->Department;}
    int getCourseCount() {return this->courseCount;}
    int getID() {return this->idCourse;}
    QString getTitle() {return this->Title;}


protected:
    int idCourse;
    QString Title;
    QString Description;
    QString Department;
    QString Level;
    QDate StartDate;
    QDate EndDate;
    float Fees;

private:
    static int courseCount;

};

//  ############################################  DEGREE Class####################################################
class Degree
{
public:
    Degree() {this->degreeCount++;}
    Degree(int idDegree, QString Title, QString Qualification, QString DegreeDetails, int DurationYears)
    {
        this->degreeCount++;
        this->idDegree = idDegree;
        this->Title = Title;
        this->Qualification = Qualification;
        this->DegreeDetails  = DegreeDetails;
        this->DurationYears = DurationYears;
    }
    ~Degree()
    {
        if (this->degreeCount > 0)   this->degreeCount--;
    }
    //getters
    int getID() {return this->idDegree;}
    QString getTitle() {return this->Title;}
    QString getQualification() {return this->Qualification;}
    QString getDetails() {return this->DegreeDetails;}
    int getDuration() { return this->DurationYears;}
    int getDegreeCount() {return this->degreeCount;}
    //setters
    void setID(int number) {this->idDegree = number;}
    void setTitle(QString string) {this->Title = string;}
    void setQualification(QString string) {this->Qualification = string;}
    void setDetails(QString string) {this->DegreeDetails = string;}
    void setDuration(int number) {this->DurationYears = number;}


    size_t copyFromFile(std::string filename, Degree degrees[], size_t N);
    size_t copyFromFile(std::string filename, std::vector<Degree> &degrees, size_t N);
    void writeToFile(std::string filename);
    void updateFile(std::string filename);

protected:
    int idDegree;
    QString Title;
    QString Qualification;
    QString DegreeDetails;
    int DurationYears;


private:
    static int degreeCount;
};


//  #   #   #   #   #   #   #   #   #   #   #   #   Degree Course Link   #   #   #   #  #   #   #   #   #   #   #   #   #   #

//purpose of this class is to create a link between degree programmes and individual courses.
//for instance, two degrees may share a course, an examploe of this would be Computuring degree sharing Networks course with the IT degree
//additionally, a degree may have courses for 3 or more different years of study. This would involve a set of courses for Level C, I and H.
class DegreeCourseLink
{
public:
    DegreeCourseLink();
    DegreeCourseLink(Degree* degree, Course* course)
    {
        this->dPtr = degree;
        this->cPtr = course;
    }
    ~DegreeCourseLink() {}

    Degree* getDegree(){return this->dPtr;}       //returns a pointer to where the degree is stored in memory.
    Course* getCourse(){return this->cPtr;}       //same ^

    void setDegree(Degree *d){this->dPtr = d;}       //sets a pointer to where the degree is stored in memory.
    void setCourse(Course *c){this->cPtr = c;}      //same ^

    //copies links to array and returns number found
    size_t copyFromFile(std::string filename, DegreeCourseLink link[], size_t N, std::vector<Degree> &degreeList, size_t dN, std::vector<Course> &courseList, size_t cN);
    void writeToFile(std::string filename);

private:
    Degree * dPtr;
    Course * cPtr;

};


} //namespace

Q_DECLARE_METATYPE(program::Course)
Q_DECLARE_METATYPE(program::Course*)
Q_DECLARE_METATYPE(program::Degree)
Q_DECLARE_METATYPE(program::Degree*)
Q_DECLARE_METATYPE(program::DegreeCourseLink)
Q_DECLARE_METATYPE(program::DegreeCourseLink*)

#endif // COURSE_H
