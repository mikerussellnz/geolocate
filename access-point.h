#ifndef __ACCESS_POINT_H__
#define __ACCESS_POINT_H__

#include <string>
#include <vector>

class AccessPoint {
public:
        std::string mac_address;
        int time;
        int signal;
        std::string data;
};

#endif
