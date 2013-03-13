#include <curl/curl.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "geolocate.h"
#include "../cJSON/cJSON.h"
#include <string.h>


#define GOOGLE_BASE_URL "https://maps.googleapis.com/maps/api/browserlocation/json?browser=none&sensor=false"

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
	std::string *response = (std::string *)userp;
	response->append((char*)buffer, size * nmemb);
	return size * nmemb;
}

GeoLocateResult geolocate_locate(std::vector<AccessPoint> aps) {
	CURL *curl = curl_easy_init();

	GeoLocateResult result;
	result.success = 0;

	//std::string url = std::string(GOOGLE_BASE_URL);

	std::ostringstream url;
	url << GOOGLE_BASE_URL;
//s << "select logged from login where id = " << ClientID;
//std::string query(s.str());

	for (int i = 0; i < aps.size(); i++) {
		url << "&wifi=mac:" << aps[i].mac_address;
		if (aps[i].signal != 0) {
			url << "|ss:" << aps[i].signal;
		}
	}

	std::cout << "querying: " << url.str() << std::endl;
	std::string response;

	curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
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
