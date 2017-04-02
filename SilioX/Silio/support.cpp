//
//  support.cpp
//  project4
//
//  Created by Lawrence Chen on 3/12/17.
//  Copyright © 2017 Lawrence Chen. All rights reserved.
//

#include "support.h"
#include <string>
using namespace std;

//compare latitude, then longitude
bool operator<(const GeoCoord& a, const GeoCoord& b){
    if(a.latitude < b.latitude)
        return true;
    else if(a.latitude == b.latitude && a.longitude < b.longitude)
        return true;
    else
        return false;
}
bool operator==(const GeoCoord& a, const GeoCoord& b){
    if(a.latitude == b.latitude && a.longitude == b.longitude)
        return true;
    else
        return false;
}
bool operator>(const GeoCoord& a, const GeoCoord& b){
    if(a.latitude > b.latitude)
        return true;
    else if(a.latitude == b.latitude && a.longitude > b.longitude)
        return true;
    else
        return false;
}
//compare start and end coordinates
bool operator==(const GeoSegment& a, const GeoSegment& b){
    return (a.start == b.start && a.end == b.end);
}
//compare street name and segments
bool operator==(const StreetSegment& a, const StreetSegment& b){
    return((a.streetName == b.streetName) && (a.segment == b.segment));
}

string directionOfLine(const GeoSegment& gs)
{
    double angle = angleOfLine(gs);
    string direction;
    if(angle >= 0 && angle <= 22.5)
        direction = "east";
    else if(angle > 22.5 && angle <= 67.5)
        direction = "northeast";
    else if(angle > 22.5 && angle <= 67.5)
        direction = "northeast";
    else if(angle > 67.5 && angle <= 112.5)
        direction = "north";
    else if(angle > 112.5 && angle <= 157.5)
        direction = "northwest";
    else if(angle > 157.5 && angle <= 202.5)
        direction = "west";
    else if(angle > 202.5 && angle <= 247.5)
        direction = "southwest";
    else if(angle > 247.5 && angle <= 292.5)
        direction = "south";
    else if(angle > 292.5 && angle <= 337.5)
        direction = "southeast";
    else if(angle > 337.5 && angle < 360)
        direction = "east";
    else
        direction = "";
    
    return direction;
}
