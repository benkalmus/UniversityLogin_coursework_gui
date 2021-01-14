#include "assessment.h"


grade::Assessment::Assessment()
{
    //empty
}

size_t grade::Assessment::copyFromFile(std::string filename, std::vector<Assessment> &alist, size_t N, std::vector<program::Course> &clist, size_t cN)
{
    size_t count =0;
    std::ifstream file;
    file.open(filename);     //open the file as read only
    if (file.is_open())     //check if it's open first
    {
        std::string line; QString qline[7];
        size_t lineNumFile = 1, i = 0;
        //read line
        while (getline(file, line))
        {
            qline[i] = QString::fromStdString(line);
            if (lineNumFile % 6 == 0 && count < N)  //every 6 lines, new object starts.
            {
                int id = qline[0].toInt();
                int idCourse = qline[1].toInt();

                int weighing = qline[3].toInt();
                QDate date;
                date = date.fromString(qline[4], "yyyy-MM-dd");
                //line 2-4 string
                alist[count].setID(id);
                alist[count].setTitle(qline[2]);
                alist[count].setWeighing(weighing);
                alist[count].setDate(date);
                alist[count].setDocument(qline[5]);
                for (size_t c=0; c<cN; c++)
                {
                    if (idCourse == clist[c].getID())
                    {
                        alist[count].setCourse(&clist[c]);      //link to course
                        break;  //stop search
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

void grade::Assessment::writeToFile(std::string filename)
{
    std::fstream file;
    //std::ios::out means writing to file
    //std::ios::app means appending to the end of the file.
    file.open(filename, std::ios::out | std::ios::app);
    if (file.is_open())
    {
        file << this->idAssessment << "\n";
        file << this->course->getID() << "\n";
        file << this->Title.toStdString() << "\n";
        file << this->Weighing << "\n";
        file << this->DeadlineDate.toString("yyyy-MM-dd").toStdString() << "\n";
        file << this->document.toStdString() << "\n";
    }
    file.close();
}


int grade::Assessment::generateID(std::vector<grade::Assessment> alist, size_t N)
{
    int random = std::rand();
    for (size_t i=0; i<N; i++) {
        if (alist[i].getID() == random){
            i = 0;      //restart counter
            random = std::rand(); //generate new id
        }
    }
    //if got to here, return id
    return random;
}

grade::Submission::Submission()
{
    //empty

}

size_t grade::Submission::copyFromFile(std::string filename, std::vector<grade::Submission> &subm, size_t N, std::vector<grade::Assessment> &alist, std::vector<Student> &slist)
{
    size_t count =0;
    std::ifstream file;
    file.open(filename);     //open the students file as read only

    if (file.is_open())     //check if it's open first
    {
        std::string line; QString qline[10];
        size_t lineNumFile = 1, i = 0;
        //read line
        while (getline(file, line))
        {
            qline[i] = QString::fromStdString(line);
           // qDebug() << qline[i];

            if (lineNumFile % 9 == 0 && count < N)  //every 9 lines
            {
                //line 1 and 2 are integers
                int idSubm = qline[0].toInt();
                int mark = qline[3].toInt();
                int aID = qline[6].toInt();
                int sID = qline[7].toInt();
                QDate date;
                date = date.fromString(qline[2], "yyyy-MM-dd");
                subm[count].setID(idSubm);
                subm[count].setTitle(qline[1]);
                subm[count].setDate(date);
                subm[count].setMark(mark);
                subm[count].setConcCode(qline[4]);
                subm[count].setProgCode(qline[5]);
                subm[count].setDocument(qline[8]);
                //find matching course and degree IDs.
                for(size_t d=0; d < N; d++)
                {
                    if (alist[d].getID() == aID)      //found degree
                    {
                        subm[count].setAssessment(&alist[d]);   //add degree to link
                        break;      //leave scope
                    }
                }
                //repeat for course
                for(size_t c=0; c < N; c++)
                {
                    if (slist[c].getID() == sID)      //found course
                    {
                        subm[count].setStudent(&slist[c]);   //add course to link
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

void grade::Submission::writeToFile(std::string filename)
{
    std::fstream file;
    //std::ios::out means writing to file
    //std::ios::app means appending to the end of the file.
    file.open(filename, std::ios::out | std::ios::app);
    if (file.is_open())
    {
        file << this->idSubmission << "\n";
        file << this->Title.toStdString() << "\n";
        file << this->SubmissionDate.toString("yyyy-MM-dd").toStdString() << "\n";
        file << this->Mark << "\n";
        file << this->ConcessionalCode.toStdString() << "\n";
        file << this->ProgressionCode.toStdString() << "\n";
        file << this->assessment->getID() << "\n";
        file << this->student->getID() << "\n";
        file << this->document.toStdString() << "\n";
    }
    file.close();
}

void grade::Submission::updateFile(std::string filename)
{

    std::ifstream file;
    std::ofstream temp;     //will copy details to this file
    file.open(filename);     //open the file as read only
    temp.open("../temp.txt");
    std::string id, tempStr;
    int lines = 9;
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
                qDebug() << "liens " << lines;
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
    this->writeToFile(filename);        // add modified record to end of file
}
