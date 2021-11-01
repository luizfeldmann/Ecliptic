#include "DateTimeWindow.h"
#include "EclipticGUI.h"
#include <ctime>
#include "Twilight.h"
#include "MeeusSeasons.h"
#include "SunPosition.h"
#include <wx/dcbuffer.h>
#include "EclipticPlotPanels.h"

// PLOT PALETTE
// ========================================================================================================
static const wxBrush palBrush_NightBlue = wxColor(4, 21, 37);
static const wxBrush palBrush_AstroBlue = wxColor(0, 51, 102);
static const wxBrush palBrush_NautiBlue = wxColor(51, 102, 153);
static const wxBrush palBrush_CivilBlue = wxColor(102, 152, 202);
static const wxBrush palBrush_LightBlue = wxColor(179, 209, 255);

static const wxPen palPen_Grid = wxPen(wxColor(120, 120, 120), 1, wxPENSTYLE_SOLID);
static const wxPen palPen_RedDash = wxPen(*wxRED, 2, wxPENSTYLE_SHORT_DASH);
static const wxPen palPen_RedSolid = wxPen(*wxRED, 2, wxPENSTYLE_SOLID);
static const wxPen palPen_YellowDash = wxPen(*wxYELLOW, 2, wxPENSTYLE_SHORT_DASH);
static const wxPen palPen_YellowSolid = wxPen(*wxYELLOW, 2, wxPENSTYLE_SOLID);

static const wxBrush* polygon_colormap[] = {&palBrush_AstroBlue, &palBrush_NautiBlue, &palBrush_CivilBlue, &palBrush_LightBlue};

// TWILIGHT PLOT
// ========================================================================================================
static inline double x_to_year_day(wxCoord x, wxCoord width) {
    return ((double)x/(double)width)*365.0;
}

static inline wxCoord time_to_x(const struct tm* _tm, wxCoord width) {
    return (wxCoord)( ((double)width * (double)_tm->tm_yday) / 365.0);
}

static inline wxCoord time_to_y(const struct tm* _tm, wxCoord height)
{
    double hour = _tm->tm_hour + ((double)_tm->tm_min)/60.0 + ((double)_tm->tm_sec)/3600;

    return (wxCoord)(hour*(double)height/24.0);
}

static inline wxCoord time_to_y(time_t t, wxCoord height)
{
    return time_to_y(localtime(&t), height);
}

Plot_Twilight::Plot_Twilight(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
        wxPanel(parent, winid, pos, size, style, name),
        polygons{
            {TWILIGHT_ASTRONOMICAL, {}},
            {TWILIGHT_NAUTICAL, {}},
            {TWILIGHT_CIVIL, {}},
            {TWILIGHT_DAYLIGHT, {}},
        }
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void Plot_Twilight::UpdatePlot(const struct tm& newtime, double latitude, double longitude)
{
    if ((newtime.tm_year == 0) || (latitude == 0) || (longitude == 0))
        return;

    const bool year_changed = (time.tm_year != newtime.tm_year);
    time = newtime;

    if (year_changed) // get all the twilights for each day of the year
        twimap = TwilightMapYearly(latitude, longitude, 1900 + time.tm_year);

    Refresh();
}

// events
void Plot_Twilight::OnPaint(wxPaintEvent & evt)
{
    // create context
    wxAutoBufferedPaintDC dc(this);

    if (twimap.empty())
    {
        evt.Skip(true);
        return;
    }

    // background
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBackground(palBrush_NightBlue);
    dc.Clear();

    // get panel dimensions
    wxCoord width, height;
    GetSize(&width, &height);

    unsigned char i = 0;
    const size_t polysize = 2*width;
    for (std::pair<TwilightType, std::vector<wxPoint>>& entry : polygons)
    {
        // fill the polygon vector if required
        if (entry.second.size() != polysize)
        {
            entry.second.resize( polysize );
            for (wxCoord x = 0; x < width; x++)
            {
                const TwilightSpan& twi       = twimap[(int)round(x_to_year_day(x, width))].operator[](entry.first);

                entry.second[x]              = wxPoint(x, time_to_y(twi.first, height));
                entry.second[2*width - 1 -x] = wxPoint(x, time_to_y(twi.second, height));
            }
        }

        // twilight curves
        dc.SetBrush(*polygon_colormap[i++]);
        dc.DrawPolygon(entry.second.size(), entry.second.data(), 0, 0);
    }

    // horizontal grid lines
    dc.SetTextForeground(*wxWHITE);
    dc.SetPen(palPen_Grid);
    for (unsigned char hour = 1; hour < 24; hour += 2)
    {
        wxCoord y = (wxCoord) ((double)hour * (double)height / 24.0);

        dc.DrawLine(0, y, width, y);

        char hourtext[4];
        sprintf(hourtext, "%.2d", hour);
        dc.DrawText(hourtext, 0, y);
    }

    //vertical grid lines
    for (unsigned char month = 0; month < 12; month++)
    {
        struct tm _tm = {0};
        _tm.tm_year = time.tm_year;
        _tm.tm_mon = month;
        _tm.tm_mday = 1;
        _tm.tm_sec = 1;

        // fix the (struct tm) which was not completely defined
        time_t stamp = mktime(&_tm);
        _tm = *localtime(&stamp);

        wxCoord x = time_to_x(&_tm, width);

        dc.DrawLine(x, 0, x, height);

        char montext[5];
        strftime(montext, sizeof(montext), "%b", &_tm);
        dc.DrawText(montext, x + (width/12.0)*0.25, 0);
    }

    // selection cross
    dc.SetPen(palPen_RedDash);
    dc.DrawLine(0, time_to_y(&this->time, height), width, time_to_y(&this->time, height));
    dc.DrawLine(time_to_x(&this->time, width), 0, time_to_x(&this->time, width), height);
}

void Plot_Twilight::OnLeftClick(wxMouseEvent& event)
{
    event.Skip();

    if (!LeftClickCallback)
        return;

    if (twimap.empty())
        return;

    wxCoord x, y;
    event.GetPosition(&x, &y);

    int w, h;
    GetSize(&w, &h);

    // get a reference to a time inside the clicked day
    struct tm _tm = *localtime(&twimap[(int)round(x_to_year_day(x, w))].civil.first);

    // go back to midnight that day
    _tm.tm_hour = 0;
    _tm.tm_min = 0;
    _tm.tm_sec = 1;

    // add seconds based on clicked Y
    time_t stamp = mktime(&_tm);
    stamp += (time_t)(86400.0*(double)y/(double)h);

    LeftClickCallback(stamp);
}

BEGIN_EVENT_TABLE(Plot_Twilight, wxPanel)
EVT_PAINT(Plot_Twilight::OnPaint)
EVT_LEFT_DOWN(Plot_Twilight::OnLeftClick)
END_EVENT_TABLE()

// GENERIC AZIMUTH ZENITH PLOT
// ========================================================================================================
Plot_AzimuthZenith_Cartesian::Plot_AzimuthZenith_Cartesian(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : wxPanel(parent, winid, pos, size, style, name)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

static double linear_interpolate(double in, const double inrange[2], const double outrange[2])
{
    return outrange[0] + ((outrange[1]-outrange[0])/(inrange[1]-inrange[0])) * (in - inrange[0]);
}

int azi_to_x(double azi, double north, int width)
{
    static const double azimuthRange[] = {-180, 180}; // put north (0 deg) in center of screen
    double r[] = {0, (double)width};

    // offset north based on slider
    azi += north;

    // make sure angles wrap around the screen
    if (azi > azimuthRange[1])
        azi -= 360;
    else if (azi < azimuthRange[0])
        azi += 360;

    return (int)linear_interpolate(azi, azimuthRange, r);
}

int zen_to_y(double zen, int height)
{
    static const double zenithRange[] = {0, 90 + 24}; // put noon (90 deg zenith) in center of screen
    double r[] = {0, (double)height};
    return (int)linear_interpolate(zen, zenithRange, r);
}

int Plot_AzimuthZenith_Cartesian::zen_to_y(double zen) const
{
    int w, h;
    GetSize(&w, &h);

    return ::zen_to_y(zen, h);
}

int Plot_AzimuthZenith_Cartesian::azi_to_x(double azi) const
{
    int w, h;
    GetSize(&w, &h);

    return ::azi_to_x(azi, north, w);
}

void Plot_AzimuthZenith_Cartesian::OnPaint(wxPaintEvent & evt)
{
    wxAutoBufferedPaintDC dc(this);

    wxCoord width, height;
    GetSize(&width, &height);

    // background
    dc.SetBackground(palBrush_LightBlue);
    dc.Clear();

    // twilights
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(palBrush_CivilBlue);
    dc.DrawRectangle(0, ::zen_to_y(90 + 0,  height), width, height);

    dc.SetBrush(palBrush_NautiBlue);
    dc.DrawRectangle(0, ::zen_to_y(90 + 6,  height), width, height);

    dc.SetBrush(palBrush_AstroBlue);
    dc.DrawRectangle(0, ::zen_to_y(90 + 12, height), width, height);

    dc.SetBrush(palBrush_NightBlue);
    dc.DrawRectangle(0, ::zen_to_y(90 + 18, height), width, height);

    // vertical grid lines
    dc.SetPen(palPen_Grid);
    dc.SetTextForeground(*wxWHITE);
    dc.SetFont(*wxNORMAL_FONT);

    for (unsigned char grid = 0; grid < 8; grid++)
    {
        wxCoord x = ::azi_to_x(grid * 45, north, width);

        dc.DrawLine(x, 0, x, height);

        static const wxString cardinal[] = {" N", " NE", " E", " SE", " S", " SW", " W", " NW"};
        dc.DrawText(cardinal[grid], x, 0);
    }

    // draw horizontal grid lines
    for (unsigned char gridZen = 0; gridZen < 180; gridZen += 30)
    {
        wxCoord y = ::zen_to_y(gridZen, height);

        dc.DrawLine(0, y, width, y);

        char degtext[6];
        snprintf(degtext, sizeof(degtext), "%dº", gridZen);
        dc.DrawText(degtext, 0, y);
    }

    PostPaint(dc);
}

void Plot_AzimuthZenith_Cartesian::UpdatePlot(const struct tm& newtime, double lat, double lon)
{
    if ((newtime.tm_year == 0) || (lat == 0) || (lon == 0))
        return;

    struct tm changeReport = (struct tm) {
        .tm_sec = (time.tm_sec != newtime.tm_sec),
        .tm_min = (time.tm_min != newtime.tm_min),
        .tm_hour = (time.tm_hour != newtime.tm_hour),
        .tm_mday = (time.tm_mday != newtime.tm_mday),
        .tm_mon = (time.tm_mon != newtime.tm_mon),
        .tm_year = (time.tm_year != newtime.tm_year),
        .tm_wday = (time.tm_wday != newtime.tm_wday),
        .tm_yday = (time.tm_yday != newtime.tm_yday),
        .tm_isdst = (time.tm_isdst != newtime.tm_isdst),
    };

    time = newtime;
    latitude = lat;
    longitude = lon;

    OnUpdate(changeReport);

    Refresh();
}

BEGIN_EVENT_TABLE(Plot_AzimuthZenith_Cartesian, wxPanel)
EVT_PAINT(Plot_AzimuthZenith_Cartesian::OnPaint)
END_EVENT_TABLE()

// ANALEMMA PLOT
// ========================================================================================================
void Plot_Analemma::OnUpdate(struct tm changeReport)
{
    const auto sun = SunPathCalc::GetLocal(time, latitude, longitude);
    curr_azimuth = sun.azimuth;
    curr_zenith  = sun.zenith;

    if (changeReport.tm_year || changeReport.tm_hour || changeReport.tm_min) // analemma is unique for each year and each hour of day, but does not change as the days change
        analemma = SunPathCalc::GetAnalemma(time, latitude, longitude);
}

static void DrawAnalemma(wxDC& dc, int width, int height, double north, bool bMonthlyPoints, const std::vector<std::pair<std::time_t, SunPathPoint>>& analemma)
{
    // temporary static buffers
    static std::vector<wxPoint> polygon1;
    static std::vector<wxPoint> polygon2;

    // prepare
    const size_t numdays = analemma.size();
    polygon1.reserve(numdays);
    polygon1.clear();

    polygon2.reserve(numdays);
    polygon2.clear();

    // draw polygon
    if (bMonthlyPoints)
    {
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(*wxYELLOW_BRUSH);
        dc.SetTextForeground(*wxWHITE);
        dc.SetFont(*wxSMALL_FONT);
    }

    bool split = false;
    wxCoord lastx = azi_to_x(analemma[0].second.azimuth, north, width);

    for (unsigned int i = 0; i < numdays; i++)
    {
        const wxCoord x = azi_to_x(analemma[i].second.azimuth, north, width);
        const wxCoord y = zen_to_y(analemma[i].second.zenith, height);

        if (bMonthlyPoints)
        {
            struct tm* at;
            if ((at = localtime(&analemma[i].first))->tm_mday == 1)
            {
                dc.DrawCircle(x, y, 3);

                char monthtext[5];
                strftime(monthtext, sizeof(monthtext), "%b", at);

                dc.DrawText(monthtext, x, y);
            }
        }

        if (abs(lastx - x) > width/2)
            split = !split;

        if (!split)
            polygon1.emplace_back( wxPoint(x, y) );
        else
            polygon2.emplace_back( wxPoint(x, y) );

        lastx = x;
    }

    // draw analemma
    dc.SetPen(*wxYELLOW_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawPolygon(polygon1.size(), polygon1.data(), 0, 0);
    dc.DrawPolygon(polygon2.size(), polygon2.data(), 0, 0);
}

void Plot_Analemma::PostPaint(wxDC& dc)
{
    if (analemma.empty())
        return;

    wxCoord width, height;
    GetSize(&width, &height);

    DrawAnalemma(dc, width, height, north, true, analemma);

    // draw current day
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(*wxRED_BRUSH);
    dc.DrawCircle(::azi_to_x(curr_azimuth, north, width), ::zen_to_y(curr_zenith, height), 3);
}

// FULL PATH PLOT
// ========================================================================================================
static void get_day_samples(size_t count, std::vector<std::pair<std::time_t, SunPathPoint>>& dst, const struct tm& day, double latitude, double longitude)
{
    dst.reserve(count);
    dst.clear();

    time_t day_start, day_end, resolution;
    {
        struct tm helper = day;

        // get starting midnight of desired day
        helper.tm_hour = 0;
        helper.tm_min = 0;
        helper.tm_sec = 1;
        day_start = mktime(&helper);

         // get ending midnight of desired day
        helper.tm_hour = 23;
        helper.tm_min = 59;
        helper.tm_sec = 59;
        day_end = mktime(&helper);

        resolution = (day_end - day_start)/count;
    }

    // sample multiple times that day
    unsigned int i = 0;
    for (time_t currTime = day_start; (currTime < day_end) && (i < count); currTime += resolution)
        dst.emplace_back( std::pair<std::time_t, SunPathPoint>(
           currTime,
           SunPathCalc::GetLocal(*localtime(&currTime), latitude, longitude)
        ));
}

void Plot_SunPath::OnUpdate(struct tm changeReport)
{
    const static size_t num_samples = 1400;

    if (changeReport.tm_year) // the path is generated for the give year
    {
        // generate 24 analemmas
        for (unsigned char hour = 0; hour < 24; hour++)
        {
            struct tm analtime = time; // each o´clock hour along the day
            time.tm_hour = hour;
            time.tm_min = 0;
            time.tm_sec = 0;

            analemmas[hour] = SunPathCalc::GetAnalemma(analtime, latitude, longitude);
        }

        // get solstices paths (which are the bounds for year-long ecliptic)
        const struct tm june = GetMeeusEvent(JUNE_SOLSTICE, time.tm_year + 1900);
        const struct tm dece = GetMeeusEvent(DECEMBER_SOLSTICE, time.tm_year + 1900);

        get_day_samples(num_samples, solstices[0], june, latitude, longitude);
        get_day_samples(num_samples, solstices[1], dece, latitude, longitude);
    }

    if (changeReport.tm_year || changeReport.tm_mday || changeReport.tm_mon)
        get_day_samples(num_samples, today, time, latitude, longitude);
}


static void DrawPath(wxDC& dc, int width, int height, double north, bool bTagWholeHour, const std::vector<std::pair<std::time_t, SunPathPoint>>& path)
{
    wxCoord lastX = 0, lastY = 0;
    for (unsigned int i = 0; i < path.size(); i++)
    {
        wxCoord x = azi_to_x(path[i].second.azimuth, north, width);
        wxCoord y = zen_to_y(path[i].second.zenith, height);

        if ((i > 0) && (abs(lastX - x) < width/2))
            dc.DrawLine(wxPoint(x, y), wxPoint(lastX, lastY));

        if (bTagWholeHour)
        {
            struct tm* at;
            if ( (at = localtime(&path[i].first))->tm_min == 0 )
            {
                dc.DrawCircle(x, y, 3);

                char hourtext[4];
                strftime(hourtext, sizeof(hourtext), "%H", at);
                dc.DrawText(hourtext, x, y);
            }
        }

        lastX = x;
        lastY = y;
    }
}

void Plot_SunPath::PostPaint(wxDC& dc)
{
    int width, height;
    GetSize(&width, &height);

    // draw 24 analemmas
    for (unsigned char hour = 0; hour < 24; hour++)
        DrawAnalemma(dc, width, height, north, false, analemmas[hour]);

    // draw bounds (solstices)
    dc.SetPen(palPen_YellowSolid);
    DrawPath(dc, width, height, north, false, solstices[0]);
    DrawPath(dc, width, height, north, false, solstices[1]);

    // draw today
    dc.SetPen(palPen_RedSolid);
    dc.SetBrush(*wxRED_BRUSH);
    dc.SetTextForeground(*wxRED);
    DrawPath(dc, width, height, north, true, today);

    // draw current time
    const SunPathPoint currPoint = SunPathCalc::GetLocal(time, latitude, longitude);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(*wxWHITE_BRUSH);
    dc.DrawCircle(::azi_to_x(currPoint.azimuth, north, width), ::zen_to_y(currPoint.zenith, height), 3);
}

// DATE TIME WINDOW
// ========================================================================================================
static void set_text_double(wxTextCtrl* text, double d)
{
    wxString str;
    str.Printf(wxT("%f"), d);
    text->SetValue(str);
}

class my_window_datetime : public window_datetime
{
protected:
    struct window_state {
        double& latitude;
        double& longitude;
        double zenith;
        double azimuth;
        struct tm& time;
    } state;

    void RecalculateSunPosition()
    {
        const SunPathPoint point = SunPathCalc::GetLocal(state.time, state.latitude, state.latitude);
        state.zenith  = point.zenith;
        state.azimuth = point.azimuth;
    }

    void UpdateInterface()
    {
        if (state.time.tm_year == 0 || state.latitude == 0 || state.longitude == 0)
            return;

        char text[256];

        // date text
        strftime(text, sizeof(text), "%d %b %Y", &state.time);
        m_text_date->SetValue(wxString(text));

        // time text
        strftime(text, sizeof(text), "%H : %M", &state.time);
        m_text_time->SetValue(wxString(text));

        // sliders
        m_slider_date->SetValue(state.time.tm_yday);
        m_slider_time->SetValue(state.time.tm_hour*60 + state.time.tm_min);

        // calendar
        m_calendar1->SetDate(wxDateTime(state.time));

        // coordinates
        set_text_double(m_text_Lat, state.latitude);
        set_text_double(m_text_Lon, state.longitude);

        // sun position
        set_text_double(m_text_azi, state.azimuth);
        set_text_double(m_text_zen, state.zenith);

        // twilight
        const struct {
            wxTextCtrl* tex;
            TwilightType event;
        } twimap[4] = {
            {m_text_civil, TWILIGHT_CIVIL},
            {m_text_nautical, TWILIGHT_NAUTICAL},
            {m_text_astronomical, TWILIGHT_ASTRONOMICAL},
            {m_text_daytime, TWILIGHT_DAYLIGHT}};

        for (unsigned char i = 0; i < 4; i++)
        {
            char temp[16];
            strcpy(text, "");

            TwilightSpan twi = TwilightMap::Get(twimap[i].event, state.latitude, state.longitude, state.time.tm_mday, state.time.tm_mon + 1, state.time.tm_year + 1900);

            strftime(temp, sizeof(temp), "%H:%M - ", localtime(&twi.first));
            strcat(text, temp);

            strftime(temp, sizeof(temp), "%H:%M", localtime(&twi.second));
            strcat(text, temp);

            twimap[i].tex->SetValue(wxString(text));
        }

        // plots
        m_daydurationchart->UpdatePlot(state.time, state.latitude, state.longitude);
        m_analemmachart->UpdatePlot(state.time, state.latitude, state.longitude);
        m_pathchart->UpdatePlot(state.time, state.latitude, state.longitude);
    }

    // copy from wxFormBuilder generated code - do not change
    void date_slider_change( wxCommandEvent& event )
    {
        const auto tm_year = state.time.tm_year;
        const auto tm_hour = state.time.tm_hour;
        const auto tm_min  = state.time.tm_min;

        // first day of year
        state.time = { 0 };
        state.time.tm_year = tm_year;
        state.time.tm_mday = 1;

        // add number of days from slider
        time_t t = mktime(&state.time) + (24*60*60)*(std::min( m_slider_date->GetValue(), (tm_year % 4 == 0) ? 365 : 364));
        state.time = *(localtime(&t));

        // add hours and minutes from previous state
        state.time.tm_hour = tm_hour;
        state.time.tm_min  = tm_min;

        RecalculateSunPosition();
        UpdateInterface();
    }

    void time_slider_change( wxCommandEvent& event )
    {
        const int minutes = m_slider_time->GetValue();

        state.time.tm_sec = 0;
        state.time.tm_min = minutes % 60;
        state.time.tm_hour = minutes / 60;

        RecalculateSunPosition();
        UpdateInterface();
    }

    void on_summer_solstice( wxCommandEvent& event ) {
        state.time = GetMeeusEvent((state.latitude > 0) ? JUNE_SOLSTICE : DECEMBER_SOLSTICE, state.time.tm_year + 1900);

        RecalculateSunPosition();
        UpdateInterface();
    }

    void on_spring_equinox( wxCommandEvent& event ) {
        state.time = GetMeeusEvent((state.latitude > 0) ? MARCH_EQUINOX : SEPTEMBER_EQUINOX, state.time.tm_year + 1900);

        RecalculateSunPosition();
        UpdateInterface();
    }

    void on_winter_solstice( wxCommandEvent& event ) {
        state.time = GetMeeusEvent((state.latitude > 0) ? DECEMBER_SOLSTICE : JUNE_SOLSTICE, state.time.tm_year + 1900);

        RecalculateSunPosition();
        UpdateInterface();
    }

    void on_autumn_equinox( wxCommandEvent& event ) {
        state.time = GetMeeusEvent((state.latitude > 0) ? SEPTEMBER_EQUINOX : MARCH_EQUINOX, state.time.tm_year + 1900);

        RecalculateSunPosition();
        UpdateInterface();
    }

    void calendar_changed( wxCalendarEvent& event )
    {
        const wxDateTime newDate = event.GetDate().GetDateOnly(); // get new date from calendar
        const wxDateTime newTime = newDate.Add(wxTimeSpan(state.time.tm_hour, state.time.tm_min, 0, 0 )); // keep the "time of day" unchanged
        const time_t newTime_t   = newTime.GetTicks();

        state.time = *localtime(&newTime_t);

        RecalculateSunPosition();
        UpdateInterface();
    }

    void analemma_north_changed( wxCommandEvent& event ) {
        m_analemmachart->north = m_slider_north_analemma->GetValue();
        m_analemmachart->UpdatePlot(state.time, state.latitude, state.longitude);
    }

    void path_north_changed( wxCommandEvent& event )
    {
        m_pathchart->north = m_slider_north_path->GetValue();
        m_pathchart->UpdatePlot(state.time, state.latitude, state.longitude);
    }
    // ========================================================

public:
    static my_window_datetime* singleton;

    my_window_datetime(wxWindow* parent, double& lat, double& lon, struct tm& _tm) :
        window_datetime(parent),
        state {lat, lon, 0, 0, _tm}
    {
        // icon
        SetIcon(wxIcon("aaaa"));

        // register self
        singleton = this;

        // register panel events
        m_daydurationchart->LeftClickCallback = [&](time_t newtime)->void{
            this->state.time = *localtime(&newtime);
            RecalculateSunPosition();
            UpdateInterface();
        };

        // first update
        RecalculateSunPosition();
        UpdateInterface();
    }

    ~my_window_datetime() {
        singleton = NULL;
    }
};

my_window_datetime* my_window_datetime::singleton = NULL;

#include <wx/app.h>
void DateTimeWindow_Show(wxWindow* parent, double& lat, double& lon, struct tm& time)
{
    if (my_window_datetime::singleton)
        return;


    my_window_datetime* win = new my_window_datetime(parent, lat, lon, time);
    win->Show(true);

    if (!win) wxTheApp->SetTopWindow(win);
}
