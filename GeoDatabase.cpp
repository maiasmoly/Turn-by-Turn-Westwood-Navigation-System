
#include "GeoDatabase.h"


bool GeoDatabase::load(const std::string& map_data_file) {
    std::ifstream infile(map_data_file);    // infile is a name of our choosing
    if (!infile ) {
        std::cerr << "Error: Cannot open file: "<< map_data_file << std::endl;
        return false;
    }
    std::string line;
    int lineNumber = 0;
    int numPOIs = 0;
    while (getline(infile, line)) {
        //std::cerr << lineNumber << "-----" << line  << std::endl;
        if (lineNumber == 0) {
            //dealing with a street name
            //std::cerr << "Street: " << line << std::endl;
            lineNumber++;
        } else if (lineNumber == 1) {
            //dealing with street endpoints
            lineNumber++;
        } else if (lineNumber == 2) {
            //dealing with number of pois in this street segment
            numPOIs = std::stod(line);
            for (int i = 0; i < numPOIs; ++i) {
                getline(infile, line);
                std::cerr << "POI: " << line << std::endl;
            }
            lineNumber = 0;
        }
    }
    return true;
}

bool GeoDatabase::get_poi_location(const std::string& poi,GeoPoint& point) const {
    return false;
}
std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const {
    return {};
}

std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const {
    return "";
}
