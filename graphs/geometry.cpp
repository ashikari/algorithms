#include "graph.h"
#include "geometry.h"


double degToRad(double angle_deg){
    return angle_deg* (M_PI / 180.0);
}

double getGCDist(const row& charger1, const row& charger2){
    double lat1, lon1, lat2, lon2;
    lat1 = degToRad(charger1.lat);
    lon1 = degToRad(charger1.lon);
    lat2 = degToRad(charger2.lat);
    lon2 = degToRad(charger2.lon);

    return greatCircleDist(lat1, lon1, lat2, lon2);
}

double getGCDist(const GraphNode* charger1, const GraphNode* charger2){
    double lat1, lon1, lat2, lon2;
    lat1 = degToRad(charger1->getLat());
    lon1 = degToRad(charger1->getLon());
    lat2 = degToRad(charger2->getLat());
    lon2 = degToRad(charger2->getLon());

    return greatCircleDist(lat1, lon1,lat2, lon2);
}


double greatCircleDist(double lat1, double lon1, double lat2, double lon2){
    double haversine = pow(sin((lat2-lat1)/2),2) + cos(lat1)*cos(lat2)*pow(sin((lon2-lon1)/2),2);

    double gc_dist = EARTH_R*2*atan2( sqrt(haversine), sqrt(1 - haversine) );

    return gc_dist;
}

