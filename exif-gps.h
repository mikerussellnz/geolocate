#ifndef __EXIF_GPS_H__
#define __EXIF_GPS_H__


typedef struct {
	double lat;
	double lon;
	double altitude;
} ExifGPS;

void dumpAllExif(std::string &file);
ExifGPS readGPS(std::string &file);

#endif
