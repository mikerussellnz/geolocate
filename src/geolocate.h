
#ifndef __GEOLOCATE_H__
#define __GEOLOCATE_H__

#include <vector>
#include "access-point.h"

typedef struct {
        double accuracy;
        double lat;
        double lon;
        int success;
} GeoLocateResult;

GeoLocateResult geolocate_locate(std::vector<AccessPoint> aps);

#endif
