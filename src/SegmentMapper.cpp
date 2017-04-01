#include "provided.h"
#include <vector>
#include "MyMap.h"
#include "support.h"
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<GeoCoord, vector<StreetSegment> > map;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    for(int i = 0; i < ml.getNumSegments(); i++){ //for each segment loaded by MapLoader
        StreetSegment seg;
        ml.getSegment(i, seg);
        vector<StreetSegment>* segVector;
        GeoCoord geo;
        
        geo = seg.segment.start;    //obtain starting geoCoord of segment
        segVector = map.find(geo);  //find the vector of segments mapped to this geoCoord
        
        if(segVector == nullptr){   //if vector pointer returned is nullptr
            vector<StreetSegment> segments; //create new vector of segments
            segments.push_back(seg);        //push current segment
            map.associate(geo, segments);   //associate this geocoord with new vector of segments
        }else{
            segVector->push_back(seg);  //else, push back current segment to the vector of segments already associated
        }
        
        geo = seg.segment.end;    //obtain starting geoCoord of segment
        segVector = map.find(geo);  //find the vector of segments mapped to this geoCoord
        
        if(segVector == nullptr){   //if vector pointer returned is nullptr
            vector<StreetSegment> segments; //create new vector of segments
            segments.push_back(seg);        //push current segment
            map.associate(geo, segments);   //associate this geocoord with new vector of segments
        }else{
            segVector->push_back(seg);  //else, push back current segment to the vector of segments already associated
        }
        
        for(int j = 0; j < seg.attractions.size(); j++){ //for each attraction within the segment's attraction vector
            geo = seg.attractions[j].geocoordinates;    //get geoCoord of current attraction
            segVector = map.find(geo);                  //get vectors of segments related to the geoCoord
            if(segVector == nullptr){             //if the vector does not exist
                vector<StreetSegment> segments;   //create new segment vector
                segments.push_back(seg);          //push new segment to segment vector
                map.associate(geo, segments);     //associate this geoCoord with the new vector of street segments
            }else{
                segVector->push_back(seg);        //else, push the current segment into the segment vector for the attraction
            }
        }
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segments;
    const vector<StreetSegment>* vectorPtr;

    vectorPtr = map.find(gc);   //use the map to find the pointer to vector of segments mapped by desired geoCoord
    if(vectorPtr == nullptr)    //if nullptr, return empty vector
        return segments;
    else                        //else return the vector indicated by the pointer to vector
        return *vectorPtr;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
