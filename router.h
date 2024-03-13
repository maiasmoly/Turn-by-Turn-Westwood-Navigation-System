//
//  Router.hpp
//  Project_4
//
//  Created by Maia Smolyanov on 3/10/24.
//

#ifndef Router_h
#define Router_h

#include "base_classes.h"
#include <vector>

class Router: public RouterBase
{
  public:
    Router(const GeoDatabaseBase& geo_db);
    virtual ~Router(){}
    virtual std::vector<GeoPoint> route(const GeoPoint& pt1,
                                        const GeoPoint& pt2) const;
    
  private:
    const GeoDatabaseBase& m_geo_db;
};

#endif /* Router_hpp */
