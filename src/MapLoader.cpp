#include "provided.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
    vector<Attraction> getCat(int num) const;
    
private:
    vector<StreetSegment> streetSegments;

    vector<Attraction> cat1;
    vector<Attraction> cat2;
    vector<Attraction> cat3;
    vector<Attraction> cat4;
    vector<Attraction> cat5;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
    string line;

    ifstream myfile (mapFile);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            StreetSegment newStreet;    //initialize a new street
            newStreet.streetName = line; //first line will be street name
            
            string coordinates;
            string numAttractions;
            
            getline (myfile,coordinates);
            string firstlat;
            string firstlong;
            string secondlat;
            string secondlong;
            
            //parse map data text for coordinates, eliminating whitespaces and commas
            while(coordinates[0] == ' ')
                coordinates.erase(0, 1);
            firstlat = coordinates.substr(0, coordinates.find(","));
            coordinates = coordinates.substr(coordinates.find(",") + 1);
            while(coordinates[0] == ' ' || coordinates[0] == ',')
                coordinates.erase(0, 1);
            firstlong = coordinates.substr(0, coordinates.find(" "));
            coordinates = coordinates.substr(coordinates.find(" ") + 1);
            while(coordinates[0] == ' ')
                coordinates.erase(0, 1);
            secondlat = coordinates.substr(0, coordinates.find(","));
            coordinates = coordinates.substr(coordinates.find(",") + 1);
            while(coordinates[0] == ' ' || coordinates[0] == ',')
                coordinates.erase(0, 1);
            secondlong = coordinates;
            while(coordinates[coordinates.size()-1] == ' ')
                coordinates.erase(coordinates.size()-1, 1);
            
            //create segment using parsed coordinates
            GeoSegment segment(GeoCoord(firstlat, firstlong), GeoCoord(secondlat, secondlong));
            newStreet.segment = segment;    //assign to a new street
            
            getline (myfile,numAttractions);    //get number of attractions for each street
            int count = stoi(numAttractions);
            for(int i = 0; i < count; i++){     //for each number of attraction stated
                Attraction newAttraction;
                string attraction;
                getline(myfile, attraction);    //get attraction detail line
                size_t pos = attraction.find("|");  //name is up to "|" character
                newAttraction.name = attraction.substr(0, pos);
                attraction = attraction.substr(pos + 1);
                //parse rest of line for coordinates
                while(attraction[0] == ' ')
                    attraction.erase(0, 1);
                string lat = attraction.substr(0, attraction.find(","));
                attraction = attraction.substr(attraction.find(",") + 1);
                while(attraction[0] == ' ' || attraction[0] == ',')
                    attraction.erase(0, 1);
                std::size_t index = attraction.find("|");
                if(index == std::string::npos){
                    while(attraction[attraction.size()-1] == ' ')
                        attraction.erase(attraction.size()-1, 1);
                    string lon = attraction;
                    newAttraction.geocoordinates = GeoCoord(lat, lon);
                    newAttraction.price = 0;
                    cat1.push_back(newAttraction);

                }else{
                    string lon = attraction.substr(0, attraction.find("|"));
                    while(lon[lon.size()-1] == ' ')
                        lon.erase(lon.size()-1, 1);
                    attraction = attraction.substr(attraction.find("|") + 1);
                    while(attraction[0] == ' ')
                        attraction.erase(0, 1);
                    newAttraction.geocoordinates = GeoCoord(lat, lon);
                    newAttraction.price = stoi(attraction);
                    newStreet.attractions.push_back(newAttraction); //push new attraction into vector of attraction of new street
                    if (newAttraction.price >= 31) 
                        cat5.push_back(newAttraction);
                    else if (newAttraction.price  > 20) 
                        cat4.push_back(newAttraction);
                    else if (newAttraction.price > 10) 
                        cat3.push_back(newAttraction);
                    else if (newAttraction.price > 5)
                        cat2.push_back(newAttraction);
                    else
                        cat1.push_back(newAttraction);
                }
            }
            streetSegments.push_back(newStreet);    //push new street into vectors of street
        }
        myfile.close();
        return true;
    }
    else{
        return false;
    }
}

vector<Attraction> MapLoaderImpl::getCat(int num) const {
    switch(num) {
        case 1: {
            return cat1;
            break;
        }
        case 2: {
            return cat2;
            break;
        }
        case 3: {
            return cat3;
            break;
        }
        case 4: {
            return cat4;
            break;
        }
        case 5: {
            return cat5;
            break;
        }
    }
    vector<Attraction> a;
    return a;
}

size_t MapLoaderImpl::getNumSegments() const
{
    return streetSegments.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if(segNum >= getNumSegments())
        return false;
    else{
        seg = streetSegments[segNum];   //set seg to be equal to the street segment at indicated index segNum
        return true;
    }
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
