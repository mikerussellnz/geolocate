#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>

#include "parse-eyefi-log.h"

std::vector<std::string> split(const std::string &input) {
	std::vector<std::string> items;
	std::istringstream ss(input);
	std::string token;

	while(std::getline(ss, token, ',')) {
		items.push_back(token);
	}
	return items;
}

LogFileResult parseLogFile(std::string &fileName, std::string &imageName) {
	LogFileResult result;

	std::ifstream file(fileName.c_str());

	if(file.fail()) {
		std::cout  << "Could not open the file" << std::endl;
		return result;
	}

	std::string line;
	while (std::getline(file, line)) {
		// std::cout << line << std::endl;
		std::vector<std::string> items = split(line);

		if (items.size() < 3) {
			std::cout << "warning, line with less than 3 tokens" << std::endl;
			continue;
		}

		int time =  atoi(items[0].c_str());
		int time_stamp = atoi(items[1].c_str());
		std::string act = items[2];

		if (act == "POWERON") {
			//std::cout << "time " << time << " stamp " << time_stamp << " " <<  act << std::endl;
			result.aps.clear();
		}
		if (act == "NEWAP" || act == "AP") {
			std::string mac = items[3].substr(0, 2) + "-" +
				items[3].substr(2, 2) + "-" +
				items[3].substr(4, 2) + "-" +
				items[3].substr(6, 2) + "-" +
				items[3].substr(8, 2) + "-" +
				items[3].substr(10, 2);

			int signal = atoi(items[4].c_str());
			std::string data = items[5];
                        //std::cout << "time " << time << " stamp " << time_stamp << " " <<  act << " mac " << mac << " sig " << signal << " data " << data << std::endl;
			AccessPoint ap;
			ap.time = time;
			ap.mac_address = mac;
			ap.signal = signal;
			result.aps.push_back(ap);
		}
		if (act == "NEWPHOTO") {
			std::string name = items[3];
			size_t size = atoi(items[4].c_str());
			// std::cout << "time " << time << " secs " << time_stamp << " " <<  act << " name " << name << " size " << size << std::endl;

			if (name == imageName) {
//				std::cout << "found image" << std::endl;
				result.image_found = 1;
				result.image_time = time;
				return result;
			}
		}
	}

	return result;
}
