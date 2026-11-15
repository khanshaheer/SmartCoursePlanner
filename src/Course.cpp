#include "Course.h"
Course::Course()
{
    id = 0;
    code = "";
    name = "";
    creditHours = 0;
    completed = false;
}

Course::Course(int id, string code,
               string name, int credits)
{
    this->id = id;
    this->code = code;
    this->name = name;
    creditHours = credits;
    completed = false;
}

int Course::getID() const
{
    return id;
}

string Course::getCode() const
{
    return code;
}

string Course::getName() const
{
    return name;
}

int Course::getCreditHours() const
{
    return creditHours;
}

bool Course::isCompleted() const
{
    return completed;
}

void Course::setName(string name)
{
    this->name = name;
}

void Course::setCreditHours(int credits)
{
    creditHours = credits;
}

void Course::setCompleted(bool status)
{
    completed = status;
}
