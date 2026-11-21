#ifndef SAMPLE_DATA_H
#define SAMPLE_DATA_H

#include "CourseDatabase.h"
#include "CourseGraph.h"

// Declarations reserved for optional sample-data loaders. This project currently
// has no matching implementation file and main.cpp does not call these functions.
// db is the destination for sample course records; graph is the destination for
// sample prerequisite edges, stored in prerequisite -> dependent direction.
void loadSampleCourses(CourseDatabase& db);

void loadSamplePrerequisites(CourseGraph& graph);

#endif
