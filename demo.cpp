#include <iostream>
#include <string>
#include "geolocate.h"
#include "exif-gps.h"
#include "parse-eyefi-log.h"
#include <string.h>
#include <cstdio>
#include <cstdlib>

int main() {
/*        const char *macs[4] = {
                "00-21-29-96-31-e7",
                "00-25-9c-57-de-a2",
                "00-21-91-51-58-6d",
                "00-17-9a-02-4e-05"
        };
*/

	std::string file("../pics/IMG_0020.JPG.log");
	std::string imageName("IMG_0020.JPG");
	LogFileResult result = parseLogFile(file, imageName);

	std::cout << "result found " << result.image_found << " at " << result.image_time << std::endl;
	std::cout << result.aps.size() << " aps." << std::endl;

	const char **macs2 = (const char**)malloc(sizeof(const char *) * result.aps.size());

	for (int i = 0; i < result.aps.size(); i++) {
		AccessPoint point = result.aps[i];
		std::cout << "ap " << point.mac_address << " with signal " << point.signal << " at "  << point.time << std::endl;
		macs2[i] = point.mac_address.c_str();
	}

        GeoLocateResult gresult = geolocate_locate(macs2, result.aps.size());
	std::cout << "lat " << gresult.lat << " lon " << gresult.lon << " accuracy " <<  gresult.accuracy << " succeeded " << gresult.success << std::endl;



//	dumpAllExif(file);
//	ExifGPS gps = readGPS(file);

//	std::cout << "gps lat " << gps.lat << " gps lon " << gps.lon << std::endl;
	return 0;
}

