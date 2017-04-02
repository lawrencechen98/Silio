#include "provided.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "support.h"

using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(GeoCoord start, GeoCoord end, vector<NavSegment>& directions) const;
    
private:
    
    AttractionMapper attMap;
    SegmentMapper segMap;
    MapLoader loader;
    
    //member function to obtain the same similar street segment shared by two separate vectors of street segments
    void sameStreet(const vector<StreetSegment> &a, const vector<StreetSegment> &b, StreetSegment &c) const{
        for(int i = 0; i < a.size(); i++){
            for(int j = 0; j < b.size(); j++){
                if(a[i] == b[j]){
                    c = a[i]; //set StreetSegment c to the found segment
                }
            }
        }
    }
    
    //container struct to give each coordinate stored in the priority queue a "weight" value to prioritize with
    struct weightedCoord{
        GeoCoord coord;
        double weight; //cost of travel plus heuristic cost
        double distanceTraveled;
        
        weightedCoord(){}
        weightedCoord(GeoCoord c):coord(c){
        }
        
        //operator overload for queue to sort coords to put coords w/ heavier weight last
        bool operator<(const weightedCoord& b) const{
            return weight > b.weight;
        }
        
        bool operator==(const weightedCoord& b) const{
            return coord == b.coord;
        }
    };
    
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    //call member instance of MapLoader to load map data from mapFile text file
    if(loader.load(mapFile)){
        attMap.init(loader);
        segMap.init(loader);
        return true;
    }else
        return false;
}



NavResult NavigatorImpl::navigate(GeoCoord start, GeoCoord end, vector<NavSegment> &directions) const
{
    MyMap<GeoCoord, GeoCoord> prevWaypoint;         //map of each coordinates to its previous waypoint it originates from
    vector<weightedCoord> finishedVector;           //vector of already processed weightedCoords, list of visited nodes
    priority_queue<weightedCoord> weightedVector;   //priority queue of discovered nodes to be process ordered by weight
    MyMap<GeoCoord, double> coordWeights;           //map of each coord to its last cost of travel to keep track of travel distance
    
    //get start coordinate and get all associated StreetSegments associated
    vector<StreetSegment> startSegments = segMap.getSegments(start);
    if(startSegments.empty()){
        return NAV_BAD_SOURCE;
    }
    //get end coordinate and get all associated StreetSegments associated
    vector<StreetSegment> endSegments = segMap.getSegments(end);
    if(endSegments.empty()){
        return NAV_BAD_DESTINATION;
    }
    
    //create a weightedCoord for starting coord
    //set its weight with initial heuristic value, and push onto priority queue
    weightedCoord startWeight(start);
    startWeight.distanceTraveled = 0;
    startWeight.weight = startWeight.distanceTraveled + distanceEarthMiles(start, end);
    weightedVector.push(startWeight);
    
    
    while(!weightedVector.empty()){
        weightedCoord cur = weightedVector.top(); //obtain first weightedCoord of priority queue
        weightedVector.pop();
        
        finishedVector.push_back(cur);  //push current weightedCoord to the vector of finished coordinates
        
        if(cur.coord == end){  //if the current coord is equal to end coordinate, then path found!
            directions.clear();
            stack<NavSegment> path; //stack to use LIFO property to reverse order as nodes backtrack
            
            //start with end coordinateusing the map of previous waypoints, trace and backtrack path taken to arrive at each coordinate
            GeoCoord* cur = &end;
            
            while(cur != nullptr && !(*cur == start)){ //loop until the pointer hits nullptr or backtrack back to the start
        
                vector<StreetSegment> curStreets = segMap.getSegments(*cur);
                
                //using the map of previous waypoints, trace and backtrack path taken to arrive at each coordinate
                GeoCoord* prev = prevWaypoint.find(*cur);
                if(prev == nullptr) //if no previous waypoint, it means we are at the start, so break out of loop
                    break;
                vector<StreetSegment> prevStreets = segMap.getSegments(*prev);
                
                StreetSegment nextStreet;
                sameStreet(curStreets, prevStreets, nextStreet);
                
                GeoSegment newSeg(*prev, *cur);
                string direction =  directionOfLine(newSeg); //call support function to get direction of new segment
                
                //if the last segment has a different name than this new one, create a turn navSegment
                if(!path.empty() && path.top().m_streetName != nextStreet.streetName){
                    string turnDirection;
                    double angle = angleBetween2Lines(newSeg, path.top().m_geoSegment);
                    if(angle >= 0 && angle < 180)
                        turnDirection = "left";
                    else if(angle >= 180 && angle<360)
                        turnDirection = "right";
                    
                    NavSegment newNav(turnDirection, path.top().m_streetName);
                    path.push(newNav);  //push new nav segment to stack
                }
                
                double distance = distanceEarthMiles(*prev, *cur); //get distance of nav segment with helper function
                NavSegment newNav(direction, nextStreet.streetName, distance, newSeg);
                path.push(newNav); //push new nav segment to stack
                cur = prev;
            }
            
            while(!path.empty()){
                directions.push_back(path.top()); //take items of stack and push into final direction vector
                path.pop();
            }
            return NAV_SUCCESS;
        }
        
        vector<StreetSegment> relatedSegments = segMap.getSegments(cur.coord);
        for(auto seg : relatedSegments){    //loop through all segments related to current coordinate
            GeoCoord nextCoord;
            
            for(auto endSeg: endSegments){  //for each of the segments, check
                if(endSeg == seg){          //if they are same as the segments associated with end coordinate
                    prevWaypoint.associate(end, cur.coord);    //associate end coord's previous node to be current coord
                    weightedCoord newWeight(end);
                    newWeight.distanceTraveled = cur.distanceTraveled + distanceEarthMiles(cur.coord, end);
                    newWeight.weight = newWeight.distanceTraveled;
                    coordWeights.associate(newWeight.coord, newWeight.distanceTraveled);
                    weightedVector.push(newWeight); //push end coordinate to vector of to-be processed nodes
                }
            }
            
            if(cur.coord == seg.segment.start){ //if current coord is start of new segment
                nextCoord = seg.segment.end;    //set next coord to be the end of the new segment
                if(prevWaypoint.find(cur.coord) != nullptr && *prevWaypoint.find(cur.coord) == nextCoord){
                    continue;   //if this next coord is the coord we just came from, skip this coord
                }
            }else if(cur.coord == seg.segment.end){ //if current coord is end of new segment
                nextCoord = seg.segment.start;      //set next coord to be start of the new segment
                if(prevWaypoint.find(cur.coord) != nullptr && *prevWaypoint.find(cur.coord) == nextCoord){
                    continue;   //if this next coord is the coord we just came from, skip this coord
                }
            }else{  //else, it means the current coord is in the middle of a segment (start coord)
                //therefore, process BOTH nodes on either end of segment
                nextCoord = seg.segment.start;
            
                /////Processing node: (Core A* implementation)/////
                //update current coord's weight and travel cost
                weightedCoord newWeight(nextCoord);
                newWeight.distanceTraveled = cur.distanceTraveled + distanceEarthMiles(cur.coord, nextCoord);
                newWeight.weight = newWeight.distanceTraveled + distanceEarthMiles(nextCoord, end);
                
                double* oldWeight = coordWeights.find(nextCoord);   //find the weight/cost of visiting this same coord last time
                //if it costs less to visit this coord this time, process it, otherwise ignore it
                if(oldWeight == nullptr || newWeight.distanceTraveled <= *oldWeight){
                    coordWeights.associate(nextCoord, newWeight.distanceTraveled);  //associate new weight for this coord
                    prevWaypoint.associate(nextCoord, cur.coord);   //associate coord with new previous waypoint coord
                    weightedVector.push(newWeight);                 //push new coord/weight into to-be process vector of nodes
                }
                
                //set next coord to other end of segment and process this one as well
                nextCoord = seg.segment.end;
            }
            
            /////Processing node: (Core A* implementation)/////
            //update current coord's weight and travel cost
            weightedCoord newWeight(nextCoord);
            newWeight.distanceTraveled = cur.distanceTraveled + distanceEarthMiles(cur.coord, nextCoord);
            newWeight.weight = newWeight.distanceTraveled + distanceEarthMiles(nextCoord, end);
            
            double* oldWeight = coordWeights.find(nextCoord);   //find the weight/cost of visiting this same coord last time
            //if it costs less to visit this coord this time, process it, otherwise ignore it
            if(oldWeight == nullptr || newWeight.distanceTraveled <= *oldWeight){
                coordWeights.associate(nextCoord, newWeight.distanceTraveled);  //associate new weight for this coord
                prevWaypoint.associate(nextCoord, cur.coord);   //associate coord with new previous waypoint coord
                weightedVector.push(newWeight);                 //push new coord/weight into to-be process vector of nodes
            }
        }
    }

    return NAV_NO_ROUTE;
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(GeoCoord start, GeoCoord end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
