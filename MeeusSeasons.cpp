#include "MeeusSeasons.h"
#include <iostream>
#include <cmath>

static double calcJDE0(int year, MeeusEvent event)
{
    if ((year < 1000) || (year > 3000))
        std::cerr << "Attempt calculating events for year " << year << " which is outside the implemented bounds 1000 < Y < 3000!" << std::endl;

    double Y = (((double)year) - 2000.0f)/1000.0f;

    switch (event)
    {
        case MARCH_EQUINOX:
            return 2451623.80984 + 365242.37404*Y + 0.05169*std::pow(Y, 2) - 0.00411*std::pow(Y, 3) - 0.00057*std::pow(Y, 4);
        break;

        case JUNE_SOLSTICE:
            return 2451716.56767 + 365241.62603*Y + 0.00325*std::pow(Y, 2) + 0.00888*std::pow(Y, 3) - 0.00030*std::pow(Y, 4);
        break;

        case SEPTEMBER_EQUINOX:
            return 2451810.21715 + 365242.01767*Y - 0.11575*std::pow(Y, 2) + 0.00337*std::pow(Y, 3) + 0.00078*std::pow(Y, 4);
        break;

        case DECEMBER_SOLSTICE:
            return 2451900.05952 + 365242.74049*Y - 0.06223*std::pow(Y, 2) - 0.00823*std::pow(Y, 3) + 0.00032*std::pow(Y, 4);
        break;
    }

    return 0;
}

static inline double to_degrees(double radians) {
    return radians * (180.0 / M_PI);
}

static inline double to_radians(double degrees) {
    return degrees * (M_PI / 180.0);
}

static double calcS(double T)
{
    static const double A[] = {485,203,199,182,156,136,77,74,70,58,52,50,45,44,29,18,17,16,14,12,12,12,9,8};

    // values given in degrees ... must convert to radians before using in cossine
	static const double B[] = {324.96,337.23,342.08,27.85,73.14,171.52,222.54,296.72,243.58,119.81,297.17,21.02,
			247.54,325.15,60.93,155.12,288.79,198.04,199.76,95.39,287.11,320.81,227.73,15.45};

	static const double C[] = {1934.136,32964.467,20.186,445267.112,45036.886,22518.443,
			65928.934,3034.906,9037.513,33718.147,150.678,2281.226,
			29929.562,31555.956,4443.417,67555.328,4562.452,62894.029,
			31436.921,14577.848,31931.756,34777.259,1222.114,16859.074};


	double S = 0;
	for(unsigned char i = 0; i < 24; i++)
        S += A[i]*std::cos(to_radians( B[i] + (C[i]*T) ));

	return S;
}

struct tm GetMeeusEvent(MeeusEvent evt, int year)
{
    // calculation of event in JULIAN DAY EPHEMERIS
    double JDE;
    {
        const double JDE0 = calcJDE0(year, evt);
        const double T = ( JDE0 - 2451545.0) / 36525;
        const double W_deg = 35999.373*T - 2.47;
        const double W_rad = to_radians(W_deg);
        const double lambda = 1 + 0.0334*cos(W_rad) + 0.0007*cos(2*W_rad);
        const double S = calcS( T );
        JDE = JDE0 + ( (0.00001*S) / lambda );
    }

    // conversion to calendar day
    JDE += 0.5;
    const double Z = floor(JDE);
    const double F = JDE  - Z;

    double A = Z;
    if (Z >= 2299161)
    {
        double alpha = floor( (Z - 1867216.25) / 36524.25 );
    	A = Z + 1 + alpha - floor( alpha / 4 );
    }

    const double B = A + 1524;
    const double C = floor( (B - 122.1) / 365.25 );
    const double D = floor(365.25*C);
    const double E = floor( ( B - D )/30.6001 );

    // generate TM struct
    struct tm utc = {0};

    const double day_decimals = B - D - floor(30.6001*E) + F;
    utc.tm_mday = std::floor(day_decimals);

    const double hour_decimals = 24*(day_decimals - utc.tm_mday);
    utc.tm_hour = std::floor(hour_decimals);

    const double min_decimals = 60*(hour_decimals - utc.tm_hour);
    utc.tm_min = std::floor(min_decimals);

    utc.tm_sec = std::floor(60*(min_decimals - utc.tm_min));

    utc.tm_mon  = (E - ((E <13.5) ? 1 : 13)) - 1;
    utc.tm_year = (C - ((utc.tm_mon > 1) ? 4716 : 4715)) - 1900;

    const time_t stamp_utc = mktime( &utc );
    const time_t stamp_loc = stamp_utc - timezone + ( daylight ? 3600 : 0 );

    return *(localtime(&stamp_loc));
}
