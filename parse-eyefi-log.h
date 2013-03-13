#ifndef __PARSE_EYE_FI_LOG_H__
#define __PARSE_EYE_FI_LOG_H__

#include <string>
#include <vector>

class AccessPoint {
public:
        std::string mac_address;
        int time;
        int signal;
        std::string data;
};

class LogFileResult {
public:
        int image_found;
        int image_time;
        std::vector<AccessPoint> aps;
};

LogFileResult parseLogFile(std::string &fileName, std::string &imageName);

#endif
