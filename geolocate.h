
#ifndef __GEOLOCATE_H__
#define __GEOLOCATE_H__

typedef struct {
        double accuracy;
        double lat;
        double lon;
        int success;
} GeoLocateResult;

GeoLocateResult geolocate_locate(char **macs, int count);

#endif
