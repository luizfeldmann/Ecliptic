#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "DateTimeWindow.h"

class mainApp: public wxApp
{
private:
    double lat = -27.85136654;
    double lon = -54.19328445;
    struct tm time;

public:
	virtual bool OnInit()
	{
	    time_t tim = ::time(0);
	    time = *localtime(&tim);

        DateTimeWindow_Show(NULL, lat, lon, time);

        return true;
	}
};

wxIMPLEMENT_APP(mainApp);
