#ifndef _TWILIGHT_H_
#define _TWILIGHT_H_

#include <ctime>
#include <utility>
#include <vector>

enum TwilightType {
    TWILIGHT_CIVIL = 90 + 6,
    TWILIGHT_NAUTICAL = 90 + 12,
    TWILIGHT_ASTRONOMICAL = 90 + 18,
    TWILIGHT_DAYLIGHT = 90,
};

using TwilightSpan = std::pair<std::time_t, std::time_t>; // times when sun rises and sets below given angle

struct TwilightMap
{
    // member
    TwilightSpan civil;
    TwilightSpan nautical;
    TwilightSpan astronomical;
    TwilightSpan daylight;

    TwilightSpan& operator[](enum TwilightType);
    const TwilightSpan& operator[](enum TwilightType) const;

    TwilightMap(double latitude, double longitude, int calendar_day, int calendar_month, int calendar_year);

    // static
    static TwilightSpan Get(enum TwilightType, double latitude, double longitude, int calendar_day, int calendar_month, int calendar_year);
};

class TwilightMapYearly : public std::vector<TwilightMap>
{
public:
    TwilightMapYearly() = default;
    TwilightMapYearly(double latitude, double longitude,int calendar_year);
};

#endif // _TWILIGHT_H_
