#include "provided.h"
#include "support.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "Planner.h"
using namespace std;


void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments);
void printDirections(/*string start, string end, */vector<NavSegment>& navSegments);

int main(int argc, char *argv[])
{
    bool raw = false;
    if (argc == 7  &&  strcmp(argv[6], "-raw") == 0)
    {
        raw = true;
        argc--;
    }
    if (argc != 6)
    {
        cout << "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\"" << endl
        << "or" << endl
        << "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\" -raw" << endl;
        return 1;
    }
    
    Planner planner;
    
    if ( ! planner.loadMapData(argv[1]))
    {
        cout << "Map data file was not found or has bad format: " << argv[1] << endl;
        return 1;
    }
    
    if ( ! raw)
        cout << "Routing..." << flush;
    
    string start1 = argv[2];
    string start2 = argv[3];
    GeoCoord start(start1, start2);

    
    vector<NavSegment> navSegments;
    vector<Attraction> planAttractions;
    
    planAttractions = planner.getPlan(start, stoi(argv[4]), stod(argv[5]), navSegments);
    cout<<endl;
    for(int i = 0; i < planAttractions.size(); i++){
        cout<<planAttractions[i].name<<": $"<<planAttractions[i].price<<endl;
    }
    cout<<endl<<endl;
    printDirections(navSegments);
}
    /*
    NavResult result = nav.navigate(start, end, navSegments);
    if ( ! raw)
        cout << endl;
    
    switch (result)
    {
        case NAV_NO_ROUTE:
            cout << "No route found between " << start << " and " << end << endl;
            break;
        case NAV_BAD_SOURCE:
            cout << "Start attraction not found: " << start << endl;
            break;
        case NAV_BAD_DESTINATION:
            cout << "End attraction not found: " << end << endl;
            break;
        case NAV_SUCCESS:
            if (raw)
                printDirectionsRaw(start, end, navSegments);
            else
                printDirections(start, end, navSegments);
            break;
    }
}

void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments)
{
    cout << "Start: " << start << endl;
    cout << "End:   " << end << endl;
    cout.setf(ios::fixed);
    cout.precision(4);
    for (auto ns : navSegments)
    {
        switch (ns.m_command)
        {
            case NavSegment::PROCEED:
                cout << ns.m_geoSegment.start.latitudeText << ","
                << ns.m_geoSegment.start.longitudeText << " "
                << ns.m_geoSegment.end.latitudeText << ","
                << ns.m_geoSegment.end.longitudeText << " "
                << ns.m_direction << " "
                << ns.m_distance << " "
                << ns.m_streetName << endl;
                break;
            case NavSegment::TURN:
                cout << "turn " << ns.m_direction << " " << ns.m_streetName << endl;
                break;
        }
    }
}
*/
void printDirections(/*string start, string end, */vector<NavSegment>& navSegments)
{
    cout.setf(ios::fixed);
    cout.precision(2);
    
    //cout << "You are starting at: " << start << endl;
    
    double totalDistance = 0;
    string thisStreet;
    GeoSegment effectiveSegment;
    double distSinceLastTurn = 0;
    
    for (auto ns : navSegments)
    {
        switch (ns.m_command)
        {
            case NavSegment::PROCEED:
                if (thisStreet.empty())
                {
                    thisStreet = ns.m_streetName;
                    effectiveSegment.start = ns.m_geoSegment.start;
                }
                effectiveSegment.end = ns.m_geoSegment.end;
                distSinceLastTurn += ns.m_distance;
                totalDistance += ns.m_distance;
                break;
            case NavSegment::TURN:
                if (distSinceLastTurn > 0)
                {
                    cout << "Proceed " << distSinceLastTurn << " miles "
                    << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
                    thisStreet.clear();
                    distSinceLastTurn = 0;
                }
                cout << "Turn " << ns.m_direction << " onto " << ns.m_streetName << endl;
                break;
        }
    }
    
    if (distSinceLastTurn > 0)
        cout << "Proceed " << distSinceLastTurn << " miles "
        << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
    //cout << "You have reached your destination: " << end << endl;
    cout.precision(1);
    cout << "Total travel distance: " << totalDistance << " miles" << endl;
}
