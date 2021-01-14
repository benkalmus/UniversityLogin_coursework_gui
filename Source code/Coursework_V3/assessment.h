#ifndef ASSESSMENT_H
#define ASSESSMENT_H
#include "student.h"
#include "course.h"
#include "QString"
#include "QDate"
#include "QObject"

namespace grade
{

class Assessment
{
public:
    Assessment();

    void setTitle(QString t) {this->Title = t;}
    void setWeighing(int w) {this->Weighing = w;}
    void setDate(QDate d) {this->DeadlineDate = d;}
    void setID(int id) {this->idAssessment = id;}
    void setCourse(program::Course* course) {this->course = course;}
    void setDocument(QString doc) {this->document = doc;}

    QString getTitle() {return this->Title;}
    int getWeighing() {return this->Weighing;}
    QDate getDate() {return this->DeadlineDate;}
    int getID() {return this->idAssessment;}
    program::Course* getCourse() {return this->course;}
    QString getDocument() {return this->document;}

    size_t copyFromFile(std::string filename, std::vector<Assessment> &alist, size_t N, std::vector<program::Course> &clist, size_t cN);
    void writeToFile(std::string filename);
    int generateID(std::vector<Assessment> alist, size_t N);
private:
    int idAssessment;
    QString Title;
    int Weighing;
    QDate DeadlineDate;
    QString document;
    program::Course *course;        //needs a course to link to

};


class Submission
{
public:
    Submission();

    void setID(int id) {this->idSubmission = id;}
    void setTitle(QString t) {this->Title = t;}
    void setDate(QDate d) {this->SubmissionDate = d;}
    void setMark(int m) {this->Mark = m;}
    void setConcCode(QString c) {this->ConcessionalCode = c;}
    void setProgCode(QString c) {this->ProgressionCode = c;}
    void setAssessment(Assessment *t) {this->assessment =t;}
    void setStudent(Student *t) {this->student = t;}
    void setDocument(QString doc) {this->document = doc;}

    int getID() {return this->idSubmission;}
    QString getTitle() {return this->Title;}
    QDate getDate() {return this->SubmissionDate;}
    int getMark() {return this->Mark;}
    QString getConcCode() {return this->ConcessionalCode;}
    QString getProgCode() {return this->ProgressionCode;}
    Assessment* getAssessment() {return this->assessment;}
    Student* getStudent() {return this->student;}
    QString getDocument() {return this->document;}

    size_t copyFromFile(std::string filename, std::vector<Submission> &subm, size_t N, std::vector<Assessment> &alist, std::vector<Student> &slist);
    void writeToFile(std::string filename);
    void updateFile(std::string filename);
private:
    int idSubmission;
    QString Title;
    QDate SubmissionDate;
    int Mark;
    QString ConcessionalCode;
    QString ProgressionCode;

    Assessment *assessment;
    Student *student;

    QString document;

};


} //namespace

Q_DECLARE_METATYPE(grade::Assessment)
Q_DECLARE_METATYPE(grade::Assessment*)
Q_DECLARE_METATYPE(grade::Submission)
Q_DECLARE_METATYPE(grade::Submission*)


#endif // ASSESSMENT_H
