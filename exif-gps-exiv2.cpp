#include <exiv2/exiv2.hpp>
#include <iostream>
#include <string>
#include <string.h>
#include <cstdio>
#include <cmath>

#include "exif-gps.h"


void dumpAllExif(std::string &file) {
	Exiv2::ExifData exifData;
        Exiv2::Image::AutoPtr image;

        try {
               image = Exiv2::ImageFactory::open(file);
        } catch(Exiv2::Error e) {
               fprintf(stderr, "Failed to open file %s.\n", file.c_str());
               return;
        }
        image->readMetadata();
        if (image.get() == NULL) {
                fprintf(stderr, "Failed to read file %s.\n", file.c_str());
		return;
        }
        exifData = image->exifData();

        Exiv2::ExifData::const_iterator i;
        for (i = exifData.begin(); i != exifData.end(); i++) {
                std::cout << i->key() << ": " << i->value() << std::endl;
        }
}

void readDateTime(std::string &file) {
/*     Exiv2::Exifdatum& tag = exifData["Exif.Photo.DateTimeOriginal"];
     std::string date = tag.toString();
     std::cout << "date is " << date << std::endl;
*/
}

void writeGPS(std::string &file, ExifGPS gps)  {
	Exiv2::ExifData exifData;
	Exiv2::Image::AutoPtr image;

        try {
               image = Exiv2::ImageFactory::open(file);
        } catch(Exiv2::Error e) {
               fprintf(stderr, "Failed to open file %s.\n", file.c_str());
//               return 0;
        }
        image->readMetadata();
        if (image.get() == NULL) {
                fprintf(stderr, "Failed to read file %s.\n", file.c_str());
//              return 0;
        }
        exifData = image->exifData();

	char tmp[50];

	snprintf(tmp, 50, "%d/1", (int)gps.altitude);
	exifData["Exif.GPSInfo.GPSAltitude"] = std::string(tmp);
	exifData["Exif.GPSInfo.GPSAltitudeRef"] = (gps.altitude < 0) ? "1" : "0";

	double absLat = fabs(gps.lat);
	int latDegrees = (int)absLat;
	double latRem = absLat - latDegrees;
	int latMinutes = floor((latRem * 60 * 100) + 0.5);
	snprintf(tmp, 50, "%d/1 %d/100 0/1", latDegrees, latMinutes);
	std::cout << "lat " << tmp <<  std::endl;
        exifData["Exif.GPSInfo.GPSLatitude"] = std::string(tmp);
	exifData["Exif.GPSInfo.GPSLatitudeRef"] = (gps.lat < 0) ? "S" : "N";

	double absLon = fabs(gps.lon);
	int lonDegrees = (int)absLon;
	double lonRem = absLon - lonDegrees;
	int lonMinutes = floor((lonRem * 60 * 100) + 0.5);
	snprintf(tmp, 50, "%d/1 %d/100 0/1", lonDegrees, lonMinutes);
        std::cout << "lon " << tmp << std::endl;
	exifData["Exif.GPSInfo.GPSLongitude"] = std::string(tmp);
	exifData["Exif.GPSInfo.GPSLongitudeRef"] = (gps.lon <0) ? "W" : "E";

	image->setExifData(exifData);
	image->writeMetadata();
}

ExifGPS readGPS(std::string &file) {
	Exiv2::ExifData exifData;
        Exiv2::Image::AutoPtr image;

	try {
               image = Exiv2::ImageFactory::open(file);
        } catch(Exiv2::Error e) {
               fprintf(stderr, "Failed to open file %s.\n", file.c_str());
//               return 0;
        }
        image->readMetadata();
        if (image.get() == NULL) {
                fprintf(stderr, "Failed to read file %s.\n", file.c_str());
//              return 0;
        }
        exifData = image->exifData();

//      Exiv2::Exifdatum& gpsVer = exifData["Exif.GPSInfo.GPSVersionID"];

        Exiv2::Exifdatum& latValue = exifData["Exif.GPSInfo.GPSLatitude"];
        Exiv2::Exifdatum& lonValue = exifData["Exif.GPSInfo.GPSLongitude"];
        Exiv2::Exifdatum& latRef = exifData["Exif.GPSInfo.GPSLatitudeRef"];
        Exiv2::Exifdatum& lonRef = exifData["Exif.GPSInfo.GPSLongitudeRef"];

        double lat;
        double lon;

	Exiv2::Rational degrees;
	Exiv2::Rational minutes;
	Exiv2::Rational seconds;

      	degrees = latValue.toRational(0);
	minutes = latValue.toRational(1);
	seconds = latValue.toRational(2);

	lat = ((double)degrees.first / (double)degrees.second) + ((double)minutes.first / (minutes.second * 60)) + ((double)seconds.first / 3600);

        if (strcmp(latRef.toString().c_str(), "S") == 0) {
              lat = -lat;
        }

	degrees = lonValue.toRational(0);
	minutes = lonValue.toRational(1);
	seconds = lonValue.toRational(2);

        lon = ((double)degrees.first / (double)degrees.second) + ((double)minutes.first / (minutes.second * 60)) + ((double)seconds.first / 3600);

        if (strcmp(lonRef.toString().c_str(), "W") == 0) {
                lon = -lon;
        }

	ExifGPS gps;
	gps.lat = lat;
	gps.lon = lon;
	gps.altitude = 0;

        std::cout << "lat " << latValue.toString() << " ref " << latRef.toString() << " lon " << lonValue.toString() << " ref " << lonRef.toString() << std::endl;

	return gps;
}
