#ifndef _MEEUS_SEASONS_H_
#define _MEEUS_SEASONS_H_

#include <ctime>

enum MeeusEvent {
    MARCH_EQUINOX = 'M',
    JUNE_SOLSTICE = 'J',
    SEPTEMBER_EQUINOX = 'S',
    DECEMBER_SOLSTICE = 'D',
};

struct tm GetMeeusEvent(MeeusEvent evt, int calendar_year);

#endif // _MEEUS_SEASONS_H_
