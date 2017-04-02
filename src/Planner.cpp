//
//  Planner.cpp
//  Silio
//
//  Created by Lawrence Chen on 4/1/17.
//  Copyright © 2017 Lawrence Chen. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include "Planner.h"
#include <iostream>
using namespace std;

vector<Attraction> Planner::getPlan(GeoCoord& start, int maxCost, double maxDist, vector<NavSegment> &directions){
    vector<Attraction> currentPlan;
    
    vector<Attraction> cat1 = loader.getCat(1);
    vector<Attraction> cat2 = loader.getCat(2);
    vector<Attraction> cat3 = loader.getCat(3);
    vector<Attraction> cat4 = loader.getCat(4);
    vector<Attraction> cat5 = loader.getCat(5);
    
    vector<vector<int> > catCombo;
    
    int minOfRange[] = {0, 6, 11, 21, 31};
    
    for(int i = 1; i <= 5; i++){
        for(int j = 1; j <= 5; j++){
            for(int k = 1; k<= 5; k++){
                if(minOfRange[i-1] + minOfRange[j-1] + minOfRange[k-1] <= maxCost){
                    vector<int> combo;
                    combo.push_back(i);
                    combo.push_back(j);
                    combo.push_back(k);
                    catCombo.push_back(combo);
                }
            }
        }
    }
    
    for(int i = 0; i <= 30; i++){
        vector<vector<int> >::reverse_iterator it = catCombo.rbegin();
        while(it != catCombo.rend()){
            vector<Attraction> potPlan;
            for(int i = 0; i<3; i++){
                switch ((*it)[i]){
                    case 1:
                        if(cat1.size() == 0)
                            break;
                        potPlan.push_back(cat1[randInt(0, cat1.size()-1)]);
                        break;
                    case 2:
                        if(cat2.size() == 0)
                            break;
                        potPlan.push_back(cat2[randInt(0, cat2.size()-1)]);
                        break;
                    case 3:
                        if(cat3.size() == 0)
                            break;
                        potPlan.push_back(cat3[randInt(0, cat3.size()-1)]);
                        break;
                    case 4:
                        if(cat4.size() == 0)
                            break;
                        potPlan.push_back(cat4[randInt(0, cat4.size()-1)]);
                        break;
                    case 5:
                        if(cat5.size() == 0)
                            break;
                        potPlan.push_back(cat5[randInt(0, cat5.size()-1)]);
                        break;
                    default:
                        break;
                        
                }
                
            }
            if(potPlan.size() != 3)
                return currentPlan;
            if(getTotalPrice(potPlan) <= maxCost){
                vector<NavSegment> potDirections;
                if(getTotalDistance(findRoute(start, potPlan), potDirections) <= maxDist){
                    directions = potDirections;
                    return potPlan;
                }
            }
            it++;
        }
    }
    
    return currentPlan;
}

vector<GeoCoord> Planner::findRoute(GeoCoord start, vector<Attraction> visiting) {
    vector<GeoCoord> finalRoute;
    finalRoute.push_back(start);
    
    for (int i = 0; i < 3; i++) {
        int closestIdx = 0;
        int closestDist = distanceEarthMiles(start, visiting[0].geocoordinates);

        for (int j = 1; j < 3-i; j++) {
            if (distanceEarthMiles(start, visiting[j].geocoordinates) < closestDist) {
                closestIdx = j;
                closestDist = distanceEarthMiles(finalRoute.back(), visiting[j].geocoordinates);
            }
        }
        finalRoute.push_back(visiting[closestIdx].geocoordinates);
        visiting.erase(visiting.begin()+closestIdx);
    }
    
    finalRoute.push_back(start);
    return finalRoute;
}
int Planner::getTotalPrice(vector<Attraction> finalPath) {
	int price = 0;
	for (int i = 0; i < finalPath.size(); i++) {
		price += finalPath[i].price;
	}
	return price;
}
double Planner::getTotalDistance(vector<GeoCoord> route, vector<NavSegment> &combinedSegments) {
	double distance = 0;
	for (int i = 0; i+1 < route.size(); i++) {
		vector<NavSegment> tempSegments;
        NavResult result = nav.navigate(route[i], route[i + 1], tempSegments);
        switch (result)
        {
            case NAV_NO_ROUTE:
                cout << "No route found" << endl;
                break;
            case NAV_BAD_SOURCE:
                cout << "Start attraction not found" <<endl;
                break;
            case NAV_BAD_DESTINATION:
                cout << "End attraction not found"<< endl;
                break;
            default:
                break;
        }
		for (int j = 0; j < tempSegments.size(); j++) {
			combinedSegments.push_back(tempSegments[j]);
			distance += tempSegments[j].m_distance;
		}
	}
	return distance;
}
