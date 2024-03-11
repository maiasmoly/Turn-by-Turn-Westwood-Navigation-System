//
//  TourGenerator.hpp
//  Project_4
//
//  Created by Maia Smolyanov on 3/10/24.
//

#ifndef TourGenerator_h
#define TourGenerator_h

#include "base_classes.h"

class TourGenerator: public TourGeneratorBase
{
  public:
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router)
        : m_geodb(geodb), m_router(router) {}
    virtual ~TourGenerator() {}
    
    virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
  
  private:
    const GeoDatabaseBase& m_geodb;
    const RouterBase& m_router;
};

#endif /* TourGenerator_h */
