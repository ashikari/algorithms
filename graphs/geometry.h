#pragma once

#include <math.h>
#include "network.h"
#include "graph.h"

const double EARTH_R = 6356.752;

double degToRad(double angle_deg);

double getGCDist(const row& charger1, const row& charger2);
double getGCDist(const GraphNode* charger1, const GraphNode* charger2);
double greatCircleDist(double lat1, double lon1, double lat2, double lon2);