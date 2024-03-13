#include <sstream>
#include "geodb.h"
#include "geotools.h"


bool GeoDatabase::load(const std::string& map_data_file) {
    
    // Error opening file.
    std::ifstream infile(map_data_file);    // infile is a name of our choosing
    if (!infile ) {
        std::cerr << "Error: Cannot open file: "<< map_data_file << std::endl;
        return false;
    }
    
    //Initialize variables before reading in an entry
    std::string line;
    int lineNumber = 0;
    int numPOIs = 0;
    std::string streetName;
    GeoPoint gp1;
    GeoPoint gp2;
    GeoPoint midPoint;
    
    // Read Every line in the input file
    while (getline(infile, line)) {
        //std::cerr << lineNumber << "-----" << line  << std::endl;
        if (lineNumber == 0) {
            // Reading in the streat name of the street segment, store street name
            streetName = line;
            lineNumber++;
        } else if (lineNumber == 1) {
            // Reading in endpoints of the street segment
            
            splitEndPointsEntry(line, gp1, gp2);
            m_connectedPoints[gp1.to_string()].push_back(gp2);
            m_connectedPoints[gp2.to_string()].push_back(gp1);
            
            midPoint = midpoint(gp1, gp2);
            m_streetsByMidpoint[midPoint.to_string()] = streetName;
            
            
            lineNumber++;
        } else if (lineNumber == 2) {
            // Read in number of Points of Interest on street segement
            numPOIs = std::stod(line);
            
            //If there is at least one point of interest.
            if (numPOIs > 0) {
                // Update Hashmap of connected points to include the midpoint of the street segment
                m_connectedPoints[gp1.to_string()].push_back(midPoint);
                m_connectedPoints[gp2.to_string()].push_back(midPoint);
                m_connectedPoints[midPoint.to_string()].push_back(gp2);
                m_connectedPoints[midPoint.to_string()].push_back(gp1);
                
                // Also we want to find any steet by its part (pt->midpoint), so index by
                // quarter-points too.
                m_streetsByMidpoint[midpoint(gp1, midPoint).to_string()] = streetName;
                m_streetsByMidpoint[midpoint(gp2, midPoint).to_string()] = streetName;

                // Read in the Points of interest
                for (int i = 0; i < numPOIs; ++i) {
                    //read each POI for the entry
                    getline(infile, line);
                    std::string name;
                    GeoPoint gp;
                    splitPOIEntry(line, name, gp);
                    
                    //insert the poi value into the corresponding hashmap
                    m_poiByName.insert(name, gp);
                    
                    // connect the points of interest to the midpoint on the street
                    m_connectedPoints[midPoint.to_string()].push_back(gp);
                    m_connectedPoints[gp.to_string()].push_back(midPoint);
                }
                
            }
            // Reset line number, indicating a new street segment to be read in
            lineNumber = 0;
        }
    }
    return true;
}

// Access point of Interest from hashmap of points of interest
bool GeoDatabase::get_poi_location(const std::string& poi,GeoPoint& point) const {
    const GeoPoint* gp = m_poiByName.find(poi);
    if (gp != nullptr) {
        point = *gp;
        return true;
    }
    return false;
}


// Access connected points from hashmap of string representation of a point to a vector of geopoints connected to that point.
std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const {
    const std::vector<GeoPoint>* points = m_connectedPoints.find(pt.to_string());
    if (points != nullptr) {
        return *points;
    }
    return std::vector<GeoPoint>{};
}

// Access streetname using streetsegment endpoints from a hashmap of string representation of calculated midpoint of the street segment and the streetname.
std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const {
    GeoPoint midPoint = midpoint(pt1, pt2);
    const std::string* street = m_streetsByMidpoint.find(midPoint.to_string());
    if (street != nullptr) {
        return *street;
    }
    return "";
}


//private methods:

// Parse a line containing Point of Interest Information
void GeoDatabase::splitPOIEntry(std::string& entry, std::string& name, GeoPoint& gp) {
    std::string lat;
    std::string lon;
    std::string nameofPOI;
    for (int i = 0; i < entry.size(); ++i) {
        if (entry[i] == '|') {
            nameofPOI = entry.substr(0, i);
            for (int j = i; j < entry.size(); j++) {
                if (entry[j] == ' ') {
                    lat = entry.substr(i + 1, j - i - 1);
                    lon = entry.substr(j + 1);
                }
            }
            
        }
    }
    name = nameofPOI;
    gp = GeoPoint(lat, lon);
    
}

// Parse a line containing endPoints of a street segment
void GeoDatabase::splitEndPointsEntry(std::string& entry, GeoPoint& gp1, GeoPoint& gp2) {
    std::istringstream iss(entry);
    std::string lat;
    std::string lon;
    
    getline( iss, lat, ' ' );
    getline( iss, lon, ' ' );
    gp1 = GeoPoint(lat, lon);
    
    getline( iss, lat, ' ' );
    getline( iss, lon, ' ' );
    gp2 = GeoPoint(lat, lon);
     
   
}
