//
//  GeoDatabase.hpp
//  Project_4
//
//  Created by Maia Smolyanov on 3/10/24.
//

#ifndef GeoDatabase_h
#define GeoDatabase_h
#include <iostream>
#include <fstream>
#include <vector>
#include "base_classes.h"
#include "HashMap.h"
#include "geopoint.h"

class GeoDatabase: public GeoDatabaseBase
{
public:
    GeoDatabase() {}
    virtual ~GeoDatabase() {}
    
     virtual bool load(const std::string& map_data_file);
     virtual bool get_poi_location(const std::string& poi,
     GeoPoint& point) const;
     virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt)
     const;
     virtual std::string get_street_name(const GeoPoint& pt1,
     const GeoPoint& pt2) const;
    
private:
    void splitPOIEntry(std::string& entry, std::string& name, GeoPoint& gp);
    void splitEndPointsEntry(std::string& entry, GeoPoint& gp1, GeoPoint& gp2);
    HashMap<GeoPoint> m_poiByName;
    HashMap<std::vector<GeoPoint>> m_connectedPoints;
    HashMap<std::string> m_streetsByMidpoint;
    
    
};

#endif /* GeoDatabase_hpp */
