#include <iostream>
#include <string>
#include "geolocate.h"
#include "exif-gps.h"
#include <string.h>
#include <cstdio>

int main() {
        char *macs[4] = {
                "00-21-29-96-31-e7",
                "00-25-9c-57-de-a2",
                "00-21-91-51-58-6d",
                "00-17-9a-02-4e-05"
        };
        GeoLocateResult result = geolocate_locate(macs, 4);
        std::cout << "lat " << result.lat << " lon " << result.lon << " accuracy " << result.accuracy << " succeeded " << result.success << std::endl;

	std::string file("photo.jpg");


	dumpAllExif(file);
	ExifGPS gps = readGPS(file);

	std::cout << "gps lat " << gps.lat << " gps lon " << gps.lon << std::endl;
	return 0;
}

