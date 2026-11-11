#ifndef SAMPLE_DATA_H
#define SAMPLE_DATA_H

#include "CourseDatabase.h"
#include "CourseGraph.h"

void loadSampleCourses(CourseDatabase& db);

void loadSamplePrerequisites(CourseGraph& graph);

#endif