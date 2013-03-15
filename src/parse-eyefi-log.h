#ifndef __PARSE_EYE_FI_LOG_H__
#define __PARSE_EYE_FI_LOG_H__

#include <string>
#include <vector>
#include "access-point.h"

class LogFileResult {
public:
        int image_found;
        int image_time;
        std::vector<AccessPoint> aps;
};

LogFileResult parseLogFile(std::string &fileName, std::string &imageName);

#endif
