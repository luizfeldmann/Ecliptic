#ifndef _ECLIPTIC_PLOT_PANELS_h_
#define _ECLIPTIC_PLOT_PANELS_h_

#include <wx/panel.h>
#include "Twilight.h"
#include "SunPosition.h"

class Plot_Twilight : public wxPanel
{
private:
    // points defining the polygon
    std::vector<std::pair<TwilightType, std::vector<wxPoint>>> polygons;
    TwilightMapYearly twimap; // the sunset and sunrise times for every day of the year
    struct tm time;

public:
    Plot_Twilight(wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);


    void UpdatePlot(const struct tm& newtime, double latitude, double longitude);
    void OnPaint(wxPaintEvent & evt);

    std::function<void(time_t)> LeftClickCallback;
    void OnLeftClick(wxMouseEvent& event);

    DECLARE_EVENT_TABLE()
};

class Plot_AzimuthZenith_Cartesian : public wxPanel
{
public:
    Plot_AzimuthZenith_Cartesian(wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);

    double north = 0;

    void UpdatePlot(const struct tm& newtime, double latitude, double longitude);
    void OnPaint(wxPaintEvent & evt);

    DECLARE_EVENT_TABLE()

protected:
    virtual void OnUpdate(struct tm changeReport) = 0;
    virtual void PostPaint(wxDC& dc) = 0;

    int zen_to_y(double zen) const;
    int azi_to_x(double azi) const;

    struct tm time = { 0 };
    double latitude = 0, longitude = 0;
};

class Plot_Analemma : public Plot_AzimuthZenith_Cartesian
{
public:
    using Plot_AzimuthZenith_Cartesian::Plot_AzimuthZenith_Cartesian;

private:
    double curr_azimuth, curr_zenith;
    std::vector<std::pair<std::time_t, SunPathPoint>> analemma;

    std::vector<wxPoint> polygon1;
    std::vector<wxPoint> polygon2;

protected:
    void OnUpdate(struct tm changeReport);
    void PostPaint(wxDC& dc);
};

class Plot_SunPath : public Plot_AzimuthZenith_Cartesian
{
public:
    using Plot_AzimuthZenith_Cartesian::Plot_AzimuthZenith_Cartesian;

private:
    std::vector<std::pair<std::time_t, SunPathPoint>> analemmas[24]; // one analemma for each hour of the year
    std::vector<wxPoint> polygon1;                                   // used to draw analemmas
    std::vector<wxPoint> polygon2;                                   // used to draw analemmas

    std::vector<std::pair<std::time_t, SunPathPoint>> today;         // sun path position sampled along the current day
    std::vector<std::pair<std::time_t, SunPathPoint>> solstices[2];  // path position sampled along the solstices days

protected:
    void OnUpdate(struct tm changeReport);
    void PostPaint(wxDC& dc);
};

#endif
