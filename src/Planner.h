//
//  Planner.h
//  Silio
//
//  Created by Lawrence Chen on 4/1/17.
//  Copyright © 2017 Lawrence Chen. All rights reserved.
//

#ifndef Planner_h
#define Planner_h

#include "provided.h"
#include <vector>

class Planner{
    public:
    Planner(){}
    ~Planner(){}
    std::vector<Attraction> getPlan(GeoCoord& start, int maxCost, double maxDist, std::vector<NavSegment> &directions);

    bool loadMapData(std::string mapFile){
        //call member instance of MapLoader to load map data from mapFile text file
        if (loader.load(mapFile) && nav.loadMapData(mapFile)) {
            return true;
        } else
            return false;
    }
    std::vector<GeoCoord> findRoute(GeoCoord start, std::vector<Attraction> visiting);
	int getTotalPrice(std::vector<Attraction> finalPlan);
	double getTotalDistance(std::vector<GeoCoord> route , std::vector<NavSegment> &combinedSegments);

private:
    MapLoader loader;
    Navigator nav;
};

#endif /* Planner_h */
