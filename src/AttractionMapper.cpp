#include "provided.h"
#include <string>
#include "MyMap.h"
#include <iostream>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
    
private:
    MyMap<string, GeoCoord> map;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    for(int i = 0; i < ml.getNumSegments(); i++){   //for each segment loaded by the MapLoader
        StreetSegment seg;
        ml.getSegment(i, seg);
        for(int j = 0; j < seg.attractions.size(); j++){    //for each attraction within the segment's attraction vector
            string name = seg.attractions[j].name;
            for(int n = 0; n < name.size(); n++)    //obtain name of attraction and set to all lower case
                name[n] = tolower(name[n]);
            map.associate(name, seg.attractions[j].geocoordinates); //associate attraction name with the coordinates of said attraction
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    string name = attraction;
    for(int n = 0; n < name.size(); n++)    //make name all lowercase to be case insensitive
        name[n] = tolower(name[n]);
    const GeoCoord* coord = map.find(name); //use map to find coordinate mapped to name of attraction
    if(coord == nullptr)
        return false;
    else{
        gc = *coord;
        return true;
    }
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
