#include "provided.h"
#include <string>
#include "MyMap.h"
#include <iostream>
using namespace std;

class PriceMapperImpl
{
public:
	PriceMapperImpl();
	~PriceMapperImpl();
	void init(const MapLoader& ml);
	bool getPrice(string attraction, int& price) const;
    
private:
    MyMap<string, int> map;
};

PriceMapperImpl::PriceMapperImpl()
{
}

PriceMapperImpl::~PriceMapperImpl()
{
}

void PriceMapperImpl::init(const MapLoader& ml)
{
    for(int i = 0; i < ml.getNumSegments(); i++){   //for each segment loaded by the MapLoader
        StreetSegment seg;
        ml.getSegment(i, seg);
        for(int j = 0; j < seg.attractions.size(); j++){    //for each attraction within the segment's attraction vector
            string name = seg.attractions[j].name;
            for(int n = 0; n < name.size(); n++)    //obtain name of attraction and set to all lower case
                name[n] = tolower(name[n]);
            map.associate(name, seg.attractions[j].price); //associate attraction name with the price of said attraction
        }
    }
}

bool PriceMapperImpl::getPrice(string attraction, int& price) const
{
    string name = attraction;
    for(int n = 0; n < name.size(); n++)    //make name all lowercase to be case insensitive
        name[n] = tolower(name[n]);
    const int* price = map.find(name); //use map to find price mapped to name of attraction
    if(price == nullptr)
        return false;
    else{
        price = *price;
        return true;
    }
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

PriceMapper::PriceMapper()
{
	m_impl = new PriceMapperImpl;
}

PriceMapper::~PriceMapper()
{
	delete m_impl;
}

void PriceMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool PriceMapper::getPrice(string attraction, int& price) const
{
	return m_impl->getPrice(attraction, price);
}
