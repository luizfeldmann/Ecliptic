#include "Twilight.h"
#include <functional>
#include "SunPosition.h"

static TwilightSpan& TwilightMapGet(enum TwilightType type, TwilightMap& m)
{
    switch( type )
    {
        case TWILIGHT_CIVIL:
            return m.civil;
        break;

        case TWILIGHT_NAUTICAL:
            return m.nautical;
        break;

        case TWILIGHT_ASTRONOMICAL:
            return m.astronomical;
        break;

        case TWILIGHT_DAYLIGHT:
        default:
            return m.daylight;
        break;
    }
}

TwilightSpan& TwilightMap::operator[](enum TwilightType type)
{
   return TwilightMapGet(type, *this);
}

const TwilightSpan& TwilightMap::operator[](enum TwilightType type) const
{
    return TwilightMapGet(type, const_cast<TwilightMap&>(*this));
}

static double bisection(double a, double b, double err, unsigned int maxiter, std::function<double(double)> func)
{
    unsigned int iter = 0;
    double c = a;

    while ((b - a) >= err)
    {
        // Find middle point
        c = a/2 + b/2;

        double fc;
        // Check if middle point is root
        if ( ((fc = func(c)) == 0.0) || (iter >= maxiter) )
            break;

        // Decide the side to repeat the steps
        else if (fc*func(a) < 0)
            b = c;
        else
            a = c;

        iter++;
    }

    return c;
}



TwilightMap::TwilightMap(double latitude, double longitude, int calendar_day, int calendar_month, int calendar_year) :
    civil(Get(TWILIGHT_CIVIL, latitude, longitude, calendar_day, calendar_month, calendar_year)),
    nautical(Get(TWILIGHT_NAUTICAL, latitude, longitude, calendar_day, calendar_month, calendar_year)),
    astronomical(Get(TWILIGHT_ASTRONOMICAL, latitude, longitude, calendar_day, calendar_month, calendar_year)),
    daylight(Get(TWILIGHT_DAYLIGHT, latitude, longitude, calendar_day, calendar_month, calendar_year))
{

}

TwilightSpan TwilightMap::Get(enum TwilightType type, double latitude, double longitude, int calendar_day, int calendar_month, int calendar_year)
{
    struct tm t = { 0 };

    // midnight
    t.tm_year = calendar_year - 1900;
    t.tm_mon = calendar_month - 1;
    t.tm_mday = calendar_day;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 1;
    std::time_t stamp1 = mktime(&t);

    // noon
    t.tm_hour = 12;
    std::time_t stamp2 = mktime(&t);

    // next midnight
    t.tm_hour = 23;
    t.tm_min = 59;
    t.tm_sec = 59;
    std::time_t stamp3 = mktime(&t);

    auto getZenithDelta = [latitude, longitude, type](double dTime)->double
    {
        std::time_t iTime(dTime);
        return SunPathCalc::GetLocal(*localtime(&iTime), latitude, longitude).zenith - (int)type;
    };

    return TwilightSpan(
        (std::time_t)bisection(stamp1, stamp2, 30, 10, getZenithDelta),
        (std::time_t)bisection(stamp2, stamp3, 30, 10, getZenithDelta)
    );
}

TwilightMapYearly::TwilightMapYearly(double latitude, double longitude, int calendar_year)
{
    reserve(366);
    const YearBounds yb(calendar_year);

    // get noon every day of the year
    for (time_t currtime = yb.first + 43200; currtime < yb.second; currtime += 86400)
    {
        const struct tm* currdate = localtime(&currtime);
        emplace_back( TwilightMap(latitude, longitude, currdate->tm_mday, currdate->tm_mon + 1, 1900 + currdate->tm_year) );
    }
}
