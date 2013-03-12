#include <curl/curl.h>
#include <iostream>
#include <string>
#include "../../cJSON/cJSON.h"
#include <string.h>
#include <exiv2/exiv2.hpp>

#define GOOGLE_BASE_URL "https://maps.googleapis.com/maps/api/browserlocation/json?browser=none&sensor=false"

typedef struct {
	double accuracy;
	double lat;
	double lon;
	int success;
} GeoLocateResult;

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
	std::string *response = (std::string *)userp;
	response->append((char*)buffer, size * nmemb);
	return size * nmemb;
}

GeoLocateResult geolocate_locate(char **macs, int count) {
	CURL *curl = curl_easy_init();

	GeoLocateResult result;
	result.success = 0;

	std::string url = std::string(GOOGLE_BASE_URL);
	for (int i = 0; i < count; i++) {
		url.append("&wifi=mac:");
		url.append(macs[i]);
	}

	std::cout << "querying: " << url << std::endl;
	std::string response;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	int success = curl_easy_perform(curl);

	std::cout << "received: " << response << std::endl;

	const char *json = response.c_str();

	cJSON *root = cJSON_Parse(json);
        if (root->type != cJSON_Object) {
                fprintf(stderr, "error: root is not an array\n");
                cJSON_Delete(root);
                return result;
        }

	cJSON *status = cJSON_GetObjectItem(root, "status");
	if (status->type != cJSON_String) {
                fprintf(stderr, "error: status is not a string\n");
                cJSON_Delete(root);
                return result;
	}
	char *status_v = status->valuestring;
	if (strcmp("OK", status_v) != 0) {
        	fprintf(stderr, "error: api did not return ok\n");
                cJSON_Delete(root);
                return result;
        }

        cJSON *accuracy = cJSON_GetObjectItem(root, "accuracy");
	if (accuracy->type != cJSON_Number) {
		fprintf(stderr, "error: accuracy is not a real\n");
                cJSON_Delete(root);
		return result;
        }
        result.accuracy = accuracy->valuedouble;

	cJSON *location = cJSON_GetObjectItem(root, "location");
        if (location->type != cJSON_Object) {
                fprintf(stderr, "error: location is not a object\n");
                cJSON_Delete(root);
                return result;
        }
	cJSON *lat = cJSON_GetObjectItem(location, "lat");
        cJSON *lng = cJSON_GetObjectItem(location, "lng");

	if (lat->type != cJSON_Number || lng->type != cJSON_Number) {
		fprintf(stderr, "error: api did not return lat and lon reals\n");
		cJSON_Delete(root);
		return result;
	}

	result.lat = lat->valuedouble;
	result.lon = lng->valuedouble;

	cJSON_Delete(root);

	result.success = 1;
	return result;
}

int main() {
	char *macs[4] = {
                "00-21-29-96-31-e7",
                "00-25-9c-57-de-a2",
                "00-21-91-51-58-6d",
                "00-17-9a-02-4e-05"
        };
	GeoLocateResult result = geolocate_locate(macs, 4);
	std::cout << "lat " << result.lat << " lon " << result.lon << " accuracy " << result.accuracy << " succeeded " << result.success << std::endl;

    	Exiv2::ExifData exifData;
	Exiv2::Image::AutoPtr image;

	std::string file("photo.jpg");

        try {
               image = Exiv2::ImageFactory::open(file);
        } catch(Exiv2::Error e) {
               fprintf(stderr, "Failed to open file %s.\n", file.c_str());
               return 0;
        }
	image->readMetadata();
	if (image.get() == NULL) {
		fprintf(stderr, "Failed to read file %s.\n", file.c_str());
		return 0;
	}
	exifData = image->exifData();
	Exiv2::Exifdatum& tag = exifData["Exif.Photo.DateTimeOriginal"];
	std::string date = tag.toString();
	std::cout << "date is " << date << std::endl;

//	Exiv2::Exifdatum& gpsVer = exifData["Exif.GPSInfo.GPSVersionID"];

	Exiv2::Exifdatum& lat = exifData["Exif.GPSInfo.GPSLatitude"];
	Exiv2::Exifdatum& lon = exifData["Exif.GPSInfo.GPSLongitude"];
	Exiv2::Exifdatum& latRef = exifData["Exif.GPSInfo.GPSLatitudeRef"];
	Exiv2::Exifdatum& lonRef = exifData["Exif.GPSInfo.GPSLongitudeRef"];

//	std::cout << "gps vers " << gpsVer << std::endl;
	std::cout << "lat " << lat.toString() << " ref " << latRef.toString() << " lon " << lon.toString() << " ref " << lonRef.toString() << std::endl;

	Exiv2::ExifData::const_iterator i;
	for (i = exifData.begin(); i != exifData.end(); i++) {
		std::cout << i->key() << ": " << i->value() << std::endl;
	}


	return 0;
}
