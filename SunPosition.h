#ifndef _SUN_POSITION_H_
#define _SUN_POSITION_H_

#include <ctime>
#include <utility>
#include <vector>

struct YearBounds : std::pair<time_t, time_t> {
public:
    YearBounds(int calendarYear);
};

struct SunPathPoint {
    double azimuth;
    double zenith;
};

class SunPathCalc
{
public:
    // static utils
    static SunPathPoint GetLocal(const struct tm& localtime, double latitude, double longitude);
    static SunPathPoint GetUTC(const struct tm& utctime, double latitude, double longitude);
    static std::vector<std::pair<std::time_t, SunPathPoint>> GetAnalemma(const struct tm& dtime, double latitude, double longitude);

    // members
    const double latitude;
    const double longitude;
    SunPathCalc(double lat, double lon);


    SunPathPoint operator[](const struct tm& when) const; // presumed UTC
};



#endif // _SUN_POSITION_H_
