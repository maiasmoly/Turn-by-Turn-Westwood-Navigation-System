
#include <queue>
#include <utility>
#include "router.h"
#include "HashMap.h"


Router::Router(const GeoDatabaseBase& geo_db) : m_geo_db(geo_db) {
}

struct PointAndPath {
    GeoPoint point;
    std::vector<GeoPoint> path;
};

std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {
    HashMap<bool> visitedPoints;
    std::vector<GeoPoint> result;
    
    // Each element is a point and a path that led us to that point.
    std::queue<PointAndPath> geoQueue;
    geoQueue.push({pt1, {pt1}});
    
    while (!geoQueue.empty()) {
        PointAndPath currentPoint = geoQueue.front();
        geoQueue.pop();
        visitedPoints[currentPoint.point.to_string()] = true;

        std::vector<GeoPoint> points = m_geo_db.get_connected_points(currentPoint.point);
        for (const GeoPoint& p : points) {
            if (p.to_string() == pt2.to_string()) {
                // TODO: Success
                result = currentPoint.path;
                result.push_back(p);
                return result;
            } else {
                if (visitedPoints.find(p.to_string()) == nullptr) {
                    std::vector<GeoPoint> newPath = currentPoint.path;
                    newPath.push_back(p);
                    geoQueue.push({p, newPath});
                }
            }
        }
    }
    // Not found...
    return result;
}
