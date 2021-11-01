#include "SunPosition.h"
#include <cmath>

#define twopi (2.0*M_PI)
#define rad   (M_PI/180.0)
#define dEarthMeanRadius     6371.01	    // In km
#define dAstronomicalUnit    149597890.0	// In km

YearBounds::YearBounds(int calendar_year)
{
    struct tm start = { 0 }; // JAN 1st 00:00:00
    start.tm_year = calendar_year - 1900;
    start.tm_mday = 1;
    first = mktime(&start);

    struct tm end = { 0 };  // DEC 31st 23:59:59
    end.tm_year = calendar_year - 1900;
    end.tm_mon = 11;
    end.tm_mday = 31;
    end.tm_hour = 23;
    end.tm_min = 59;
    end.tm_sec = 59;
    end.tm_yday = (calendar_year % 4 == 0) ? 365 : 364;
    second = mktime(&end);
}


SunPathPoint SunPathCalc::GetLocal(const struct tm& localtime, double latitude, double longitude)
{
    struct tm localtime_nonconst = localtime;
    std::time_t stamp_loc = std::mktime(&localtime_nonconst);

    return GetUTC(*gmtime(&stamp_loc), latitude, longitude);
}

SunPathPoint SunPathCalc::GetUTC(const struct tm& utctime, double latitude, double longitude)
{
    SunPathPoint udtSunCoordinates;

    // the calculations use different representation than the language standard
    struct tm udtTime = utctime;
    udtTime.tm_year += 1900;
    udtTime.tm_mon += 1;

    // Main variables
	double dElapsedJulianDays;
	double dDecimalHours;
	double dEclipticLongitude;
	double dEclipticObliquity;
	double dRightAscension;
	double dDeclination;

	// Auxiliary variables
	double dY;
	double dX;

	// Calculate difference in days between the current Julian Day
	// and JD 2451545.0, which is noon 1 January 2000 Universal Time
	{
		double dJulianDate;
		long int liAux1;
		long int liAux2;

		// Calculate time of the day in UT decimal hours
		dDecimalHours = udtTime.tm_hour + (udtTime.tm_min + udtTime.tm_sec / 60.0 ) / 60.0;

		// Calculate current Julian Day
		liAux1 =(udtTime.tm_mon - 14)/12;

		liAux2=(1461*(udtTime.tm_year + 4800 + liAux1))/4 + (367*(udtTime.tm_mon - 2-12*liAux1))/12- (3*((udtTime.tm_year + 4900 + liAux1)/100))/4+udtTime.tm_mday-32075;

		dJulianDate=(double)(liAux2)-0.5+dDecimalHours/24.0;
		// Calculate difference between current Julian Day and JD 2451545.0
		dElapsedJulianDays = dJulianDate-2451545.0;
	}

	// Calculate ecliptic coordinates (ecliptic longitude and obliquity of the
	// ecliptic in radians but without limiting the angle to be less than 2*Pi
	// (i.e., the result may be greater than 2*Pi)
	{
		double dMeanLongitude;
		double dMeanAnomaly;
		double dOmega;
		dOmega=2.1429-0.0010394594*dElapsedJulianDays;
		dMeanLongitude = 4.8950630+ 0.017202791698*dElapsedJulianDays; // Radians
		dMeanAnomaly = 6.2400600+ 0.0172019699*dElapsedJulianDays;
		dEclipticLongitude = dMeanLongitude + 0.03341607*std::sin( dMeanAnomaly )
			+ 0.00034894*std::sin( 2*dMeanAnomaly )-0.0001134
			-0.0000203*std::sin(dOmega);
		dEclipticObliquity = 0.4090928 - 6.2140e-9*dElapsedJulianDays
			+0.0000396*std::cos(dOmega);
	}

	// Calculate celestial coordinates ( right ascension and declination ) in radians
	// but without limiting the angle to be less than 2*Pi (i.e., the result may be
	// greater than 2*Pi)
	{
		double dSin_EclipticLongitude;
		dSin_EclipticLongitude= sin( dEclipticLongitude );
		dY = cos( dEclipticObliquity ) * dSin_EclipticLongitude;
		dX = cos( dEclipticLongitude );
		dRightAscension = atan2( dY,dX );

		if( dRightAscension < 0.0 ) dRightAscension = dRightAscension + twopi;
		dDeclination = asin( sin( dEclipticObliquity )*dSin_EclipticLongitude );
	}

	// Calculate local coordinates ( azimuth and zenith angle ) in degrees
	{
		double dGreenwichMeanSiderealTime;
		double dLocalMeanSiderealTime;
		double dLatitudeInRadians;
		double dHourAngle;
		double dCos_Latitude;
		double dSin_Latitude;
		double dCos_HourAngle;
		double dParallax;

		dGreenwichMeanSiderealTime = 6.6974243242 + 0.0657098283*dElapsedJulianDays + dDecimalHours;
		dLocalMeanSiderealTime = (dGreenwichMeanSiderealTime*15 + longitude)*rad;
		dHourAngle = dLocalMeanSiderealTime - dRightAscension;
		dLatitudeInRadians = latitude*rad;
		dCos_Latitude = cos( dLatitudeInRadians );
		dSin_Latitude = sin( dLatitudeInRadians );
		dCos_HourAngle= cos( dHourAngle );
		udtSunCoordinates.zenith = (acos( dCos_Latitude*dCos_HourAngle *cos(dDeclination) + sin( dDeclination )*dSin_Latitude));

		dY = -sin( dHourAngle );
		dX = tan( dDeclination )*dCos_Latitude - dSin_Latitude*dCos_HourAngle;

		if ( (udtSunCoordinates.azimuth = atan2( dY, dX )) < 0.0 )
			udtSunCoordinates.azimuth += twopi;

		udtSunCoordinates.azimuth /= rad;

		// Parallax Correction
		dParallax=(dEarthMeanRadius/dAstronomicalUnit)*sin(udtSunCoordinates.zenith);

		udtSunCoordinates.zenith = (udtSunCoordinates.zenith+ dParallax)/rad;
	}

	return udtSunCoordinates;
}

SunPathCalc::SunPathCalc(double lat, double lon) :
    latitude(lat),
    longitude(lon)
{

}

SunPathPoint SunPathCalc::operator[](const struct tm& when) const
{
    return GetUTC(when, latitude, longitude);
}


std::vector<std::pair<std::time_t, SunPathPoint>> SunPathCalc::GetAnalemma(const struct tm& dtime, double latitude, double longitude)
{
    std::vector<std::pair<std::time_t, SunPathPoint>> ann;
    ann.reserve(366);
    const YearBounds yb(dtime.tm_year + 1900);

    for (std::time_t basetime = yb.first + 43200; basetime < yb.second; basetime += 86400)
    {
        struct tm analemma_time = *localtime(&basetime);
        analemma_time.tm_hour = dtime.tm_hour;
        analemma_time.tm_min = dtime.tm_min;
        analemma_time.tm_sec = dtime.tm_sec;

        std::time_t sample_time = mktime(&analemma_time);

        ann.emplace_back(
            std::pair<std::time_t, SunPathPoint>(
                sample_time,
                GetLocal(analemma_time, latitude, longitude)
            )
        );
    }

    return ann;
}
