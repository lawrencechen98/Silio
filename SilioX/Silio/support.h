//
//  support.h
//  project4
//
//  Created by Lawrence Chen on 3/11/17.
//  Copyright © 2017 Lawrence Chen. All rights reserved.
//

#ifndef support_h
#define support_h

#include "provided.h"
#include <string>

bool operator<(const GeoCoord& a, const GeoCoord& b);               //operator to compare GeoCoord is lesser
bool operator==(const GeoCoord& a, const GeoCoord& b);              //operator to compare GeoCoord is equal
bool operator>(const GeoCoord& a, const GeoCoord& b);               //operator to compare GeoCoord is greater
bool operator==(const GeoSegment& a, const GeoSegment& b);          //operator to compare GeoSegment equality
bool operator==(const StreetSegment& a, const StreetSegment& b);    //operator to compare StreetSegment equality
std::string directionOfLine(const GeoSegment& gs);                  //helper function to get direction of any geoSegment
#endif /* support_h */
