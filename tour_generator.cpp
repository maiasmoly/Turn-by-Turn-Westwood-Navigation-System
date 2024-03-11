//
//  TourGenerator.cpp
//  Project_4
//
//  Created by Maia Smolyanov on 3/10/24.
//

#include "tour_generator.h"
#include "geotools.h"

std::string GetDirection(const GeoPoint& from, const GeoPoint& to) {
    double angle = angle_of_line(from, to);
    
    if (0 <= angle && angle < 22.5) return "east";
    else if (22.5 <= angle && angle < 67.5) return "northeast";
    else if (67.5 <= angle && angle < 112.5)  return "north";
    else if (112.5 <= angle  && angle < 157.5) return "northwest";
    else if (157.5 <= angle && angle < 202.5) return "west";
    else if (202.5 <= angle && angle < 247.5) return "southwest";
    else if (247.5 <= angle && angle < 292.5) return "south";
    else if (292.5 <= angle && angle < 337.5) return "southeast";
    else if (angle >= 337.5) return "east";
    else return "UNKNOWN";
}

std::string GetTurnDirection(double angle) {
    if (angle < 180) {
        return "left";
    } else {
        return "right";
    }
}

std::string GetStreetName(const GeoDatabaseBase& m_geodb,
                          const GeoPoint& from, const GeoPoint& to) {
    // TODO: this may not work for every segment, e.g. for a segment from
    // a midpoint to the POI (the last one)
    std::string street = m_geodb.get_street_name(from, to);
    if (street.empty()) {
        return "a path";
    }
    return street;
}



std::vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const {
    std::vector<TourCommand> commands;
    
    for (int i = 0; i < stops.size(); ++i) {
        std::string poi, talking_points;
        if (!stops.get_poi_data(i, poi, talking_points)) {
            std::cerr << "Error: can not find POI " << i << std::endl;
            return std::vector<TourCommand>();
        }
        // Create a commentary command for each POI.
        TourCommand commentary;
        commentary.init_commentary(poi, talking_points);
        commands.push_back(commentary);
        
        std::string next_poi;
        if (!stops.get_poi_data(i + 1, next_poi, talking_points)) {
            break;
        }
        // Find the GeoPoint of the next POI and a route to it.
        GeoPoint poi_point;
        GeoPoint next_poi_point;
        if (!m_geodb.get_poi_location(poi, poi_point) ||
            !m_geodb.get_poi_location(next_poi, next_poi_point)) {
            // Error, no POI location found.
            std::cerr << "Error: can not find POI location for " << poi << " or " << next_poi << std::endl;
            return std::vector<TourCommand>();
        }
        
        // Find the route to the next POI:
        std::vector<GeoPoint> path = m_router.route(poi_point, next_poi_point);
        for (int j = 0; j < path.size() - 1; ++j) {
            const GeoPoint& from = path[j];
            const GeoPoint& to = path[j + 1];
            const GeoPoint* after = nullptr;
            if (j < path.size() - 2) {
                after = &path[j + 2];
            }

            TourCommand proceed;
            std::string direction = GetDirection(from, to);
            std::string streetName = GetStreetName(m_geodb, from, to);
            double distance = distance_earth_miles(from, to);
            proceed.init_proceed(direction, streetName, distance, from, to);
            commands.push_back(proceed);
            
            // Maybe we need a turn command as well, if there is an angle.
            if (after != nullptr) {
                // We have another segment after. Compute the angle.
                double angle = angle_of_turn(from, to, *after);
                std::string streetAfter = GetStreetName(m_geodb, to, *after);
                if (std::abs(angle) > 0.0000001 && streetName != streetAfter) {
                    TourCommand turn;
                    turn.init_turn(GetTurnDirection(angle), GetStreetName(m_geodb, to, *after));
                    commands.push_back(turn);
                }
            }
        }
    }
    return commands;
}
