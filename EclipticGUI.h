///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include "EclipticPlotPanels.h"
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/slider.h>
#include <wx/statbox.h>
#include <wx/calctrl.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class window_map
///////////////////////////////////////////////////////////////////////////////
class window_map : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText_Lat;
		wxTextCtrl* m_text_Lat;
		wxStaticText* m_staticText_Lon;
		wxTextCtrl* m_text_Lon;
		wxStaticLine* m_staticline1;
		wxButton* btn_Ok;
		wxButton* btn_Cancel;
		wxButton* m_button_fake;
		wxBoxSizer* map_bottomSizer;

		// Virtual event handlers, overide them in your derived class
		virtual void on_close( wxCloseEvent& event ) = 0;
		virtual void on_show( wxShowEvent& event ) = 0;
		virtual void on_Ok( wxCommandEvent& event ) = 0;
		virtual void on_Cancel( wxCommandEvent& event ) = 0;


	public:

		window_map( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Mapa"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,600 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );
		~window_map();

};

///////////////////////////////////////////////////////////////////////////////
/// Class window_datetime
///////////////////////////////////////////////////////////////////////////////
class window_datetime : public wxFrame
{
	private:

	protected:
		wxSlider* m_slider_date;
		wxTextCtrl* m_text_date;
		wxSlider* m_slider_time;
		wxTextCtrl* m_text_time;
		wxButton* m_btn_solstice_summer;
		wxButton* m_btn_equinox_spring;
		wxButton* m_btn_solstice_winter;
		wxButton* m_btn_equinox_autumn;
		wxStaticText* m_staticText11;
		wxTextCtrl* m_text_Lat;
		wxStaticText* m_staticText12;
		wxTextCtrl* m_text_Lon;
		wxStaticText* m_staticText13;
		wxTextCtrl* m_text_azi;
		wxStaticText* m_staticText14;
		wxTextCtrl* m_text_zen;
		wxStaticText* m_staticText15;
		wxTextCtrl* m_text_daytime;
		wxStaticText* m_staticText16;
		wxTextCtrl* m_text_civil;
		wxStaticText* m_staticText17;
		wxTextCtrl* m_text_nautical;
		wxStaticText* m_staticText18;
		wxTextCtrl* m_text_astronomical;
		wxCalendarCtrl* m_calendar1;
		wxNotebook* m_notebook_graph;
		Plot_Twilight* m_daydurationchart;
		wxPanel* m_analemmapage;
		wxSlider* m_slider_north_analemma;
		Plot_Analemma* m_analemmachart;
		wxPanel* m_pathpage;
		wxSlider* m_slider_north_path;
		Plot_SunPath* m_pathchart;

		// Virtual event handlers, overide them in your derived class
		virtual void date_slider_change( wxCommandEvent& event ) = 0;
		virtual void time_slider_change( wxCommandEvent& event ) = 0;
		virtual void on_summer_solstice( wxCommandEvent& event ) = 0;
		virtual void on_spring_equinox( wxCommandEvent& event ) = 0;
		virtual void on_winter_solstice( wxCommandEvent& event ) = 0;
		virtual void on_autumn_equinox( wxCommandEvent& event ) = 0;
		virtual void calendar_changed( wxCalendarEvent& event ) = 0;
		virtual void analemma_north_changed( wxCommandEvent& event ) = 0;
		virtual void path_north_changed( wxCommandEvent& event ) = 0;


	public:

		window_datetime( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,700 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_NO_TASKBAR|wxSYSTEM_MENU|wxTAB_TRAVERSAL );

		~window_datetime();

};

