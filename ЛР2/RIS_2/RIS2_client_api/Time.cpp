#include <string>
#include <ctime>
#include "Time.h"

const char* GetTime() {
    auto* name = new std::string();
    time_t t1;
    tm ttm{};

    t1 = time(&t1);
    localtime_s(&ttm, &t1);

    *name += std::to_string(ttm.tm_mday) + "." + std::to_string(ttm.tm_mon) + "." +
        std::to_string(1900 + ttm.tm_year) + " ";
    *name += std::to_string(ttm.tm_hour) + "." + std::to_string(ttm.tm_min) + "." + std::to_string(ttm.tm_sec);
    return name->c_str();
}
