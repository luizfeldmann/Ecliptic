#ifndef _DATETIME_WINDOW_H_
#define _DATETIME_WINDOW_H_

#include <ctime>

class wxWindow;
void DateTimeWindow_Show(wxWindow* parent, double& lat, double& lon, struct tm& time);

#endif // _DATETIME_WINDOW_H_
