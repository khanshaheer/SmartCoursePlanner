#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <string>
using namespace std;

class Course
{
private:
    int id;
    string code;
    string name;
    int creditHours;
    bool completed;

public:
    Course();

    Course(int id, string code,
           string name, int credits);

    int getID() const;
    string getCode() const;
    string getName() const;
    int getCreditHours() const;
    bool isCompleted() const;

    void setName(string name);
    void setCreditHours(int credits);
    void setCompleted(bool status);
};

#endif