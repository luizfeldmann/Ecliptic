///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "EclipticGUI.h"

///////////////////////////////////////////////////////////////////////////

window_map::window_map( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxColour( 255, 255, 255 ) );

	wxBoxSizer* map_mainSizer;
	map_mainSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* map_topSizer;
	map_topSizer = new wxBoxSizer( wxHORIZONTAL );

	m_staticText_Lat = new wxStaticText( this, wxID_ANY, wxT("Lat:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_Lat->Wrap( -1 );
	map_topSizer->Add( m_staticText_Lat, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_text_Lat = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_text_Lat->Enable( false );

	map_topSizer->Add( m_text_Lat, 3, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticText_Lon = new wxStaticText( this, wxID_ANY, wxT("Lon:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_Lon->Wrap( -1 );
	map_topSizer->Add( m_staticText_Lon, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_text_Lon = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_text_Lon->Enable( false );

	map_topSizer->Add( m_text_Lon, 3, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	map_topSizer->Add( m_staticline1, 0, wxALL|wxEXPAND, 5 );

	btn_Ok = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	map_topSizer->Add( btn_Ok, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	btn_Cancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxPoint( -1,-1 ), wxDefaultSize, 0 );

	btn_Cancel->SetBitmapPosition( wxRIGHT );
	map_topSizer->Add( btn_Cancel, 2, wxALIGN_CENTER_VERTICAL|wxALL|wxLEFT|wxRIGHT, 5 );

	m_button_fake = new wxButton( this, wxID_DEFAULT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );

	m_button_fake->SetDefault();
	m_button_fake->SetMinSize( wxSize( 0,0 ) );
	m_button_fake->SetMaxSize( wxSize( 0,0 ) );

	map_topSizer->Add( m_button_fake, 0, wxALL, 5 );


	map_mainSizer->Add( map_topSizer, 0, wxEXPAND, 5 );

	map_bottomSizer = new wxBoxSizer( wxVERTICAL );


	map_mainSizer->Add( map_bottomSizer, 1, wxEXPAND, 5 );


	this->SetSizer( map_mainSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( window_map::on_close ) );
	this->Connect( wxEVT_SHOW, wxShowEventHandler( window_map::on_show ) );
	btn_Ok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( window_map::on_Ok ), NULL, this );
	btn_Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( window_map::on_Cancel ), NULL, this );
}

window_map::~window_map()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( window_map::on_close ) );
	this->Disconnect( wxEVT_SHOW, wxShowEventHandler( window_map::on_show ) );
	btn_Ok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( window_map::on_Ok ), NULL, this );
	btn_Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( window_map::on_Cancel ), NULL, this );

}

window_datetime::window_datetime( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxColour( 255, 255, 255 ) );

	wxBoxSizer* sizer_datetime_main;
	sizer_datetime_main = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* sizer_top_info;
	sizer_top_info = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* sizer_datetime;
	sizer_datetime = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Data / Hora") ), wxVERTICAL );

	wxBoxSizer* sizer_date;
	sizer_date = new wxBoxSizer( wxHORIZONTAL );

	m_slider_date = new wxSlider( sizer_datetime->GetStaticBox(), wxID_ANY, 0, 0, 366, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	sizer_date->Add( m_slider_date, 2, wxALL|wxEXPAND, 5 );

	m_text_date = new wxTextCtrl( sizer_datetime->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_text_date->Enable( false );
	m_text_date->SetMinSize( wxSize( 80,-1 ) );

	sizer_date->Add( m_text_date, 1, wxALL|wxEXPAND, 5 );


	sizer_datetime->Add( sizer_date, 0, wxEXPAND, 5 );

	wxBoxSizer* sizer_time;
	sizer_time = new wxBoxSizer( wxHORIZONTAL );

	m_slider_time = new wxSlider( sizer_datetime->GetStaticBox(), wxID_ANY, 0, 0, 1439, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	sizer_time->Add( m_slider_time, 2, wxALL|wxEXPAND, 5 );

	m_text_time = new wxTextCtrl( sizer_datetime->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_text_time->Enable( false );
	m_text_time->SetMinSize( wxSize( 80,-1 ) );

	sizer_time->Add( m_text_time, 1, wxALL|wxEXPAND, 5 );


	sizer_datetime->Add( sizer_time, 0, wxEXPAND, 5 );

	wxFlexGridSizer* sizer_datetime_buttons;
	sizer_datetime_buttons = new wxFlexGridSizer( 2, 2, 0, 0 );
	sizer_datetime_buttons->SetFlexibleDirection( wxBOTH );
	sizer_datetime_buttons->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

	m_btn_solstice_summer = new wxButton( sizer_datetime->GetStaticBox(), wxID_ANY, wxT("Solst. verão"), wxDefaultPosition, wxDefaultSize, 0 );
	sizer_datetime_buttons->Add( m_btn_solstice_summer, 1, wxALL|wxEXPAND, 5 );

	m_btn_equinox_spring = new wxButton( sizer_datetime->GetStaticBox(), wxID_ANY, wxT("Equi. primavera"), wxDefaultPosition, wxDefaultSize, 0 );
	sizer_datetime_buttons->Add( m_btn_equinox_spring, 1, wxALL|wxEXPAND, 5 );

	m_btn_solstice_winter = new wxButton( sizer_datetime->GetStaticBox(), wxID_ANY, wxT("Solst. inverno"), wxDefaultPosition, wxDefaultSize, 0 );
	sizer_datetime_buttons->Add( m_btn_solstice_winter, 1, wxALL|wxEXPAND, 5 );

	m_btn_equinox_autumn = new wxButton( sizer_datetime->GetStaticBox(), wxID_ANY, wxT("Equi. outono"), wxDefaultPosition, wxDefaultSize, 0 );
	sizer_datetime_buttons->Add( m_btn_equinox_autumn, 1, wxALL|wxEXPAND, 5 );


	sizer_datetime->Add( sizer_datetime_buttons, 0, wxEXPAND, 5 );


	sizer_top_info->Add( sizer_datetime, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* sizer_group;
	sizer_group = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sizer_coords;
	sizer_coords = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Coordenadas") ), wxVERTICAL );

	wxFlexGridSizer* sizer_coords_content;
	sizer_coords_content = new wxFlexGridSizer( 0, 2, 0, 0 );
	sizer_coords_content->SetFlexibleDirection( wxBOTH );
	sizer_coords_content->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText11 = new wxStaticText( sizer_coords->GetStaticBox(), wxID_ANY, wxT("Lat:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	sizer_coords_content->Add( m_staticText11, 0, wxALL, 5 );

	m_text_Lat = new wxTextCtrl( sizer_coords->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_text_Lat->Enable( false );
	m_text_Lat->SetMinSize( wxSize( 100,-1 ) );

	sizer_coords_content->Add( m_text_Lat, 0, wxALL, 5 );

	m_staticText12 = new wxStaticText( sizer_coords->GetStaticBox(), wxID_ANY, wxT("Lon:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	sizer_coords_content->Add( m_staticText12, 0, wxALL, 5 );

	m_text_Lon = new wxTextCtrl( sizer_coords->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_text_Lon->Enable( false );
	m_text_Lon->SetMinSize( wxSize( 100,-1 ) );

	sizer_coords_content->Add( m_text_Lon, 0, wxALL, 5 );


	sizer_coords->Add( sizer_coords_content, 1, wxEXPAND, 5 );


	sizer_group->Add( sizer_coords, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* siser_sun;
	siser_sun = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Sol") ), wxVERTICAL );

	wxFlexGridSizer* sizer_sun_content;
	sizer_sun_content = new wxFlexGridSizer( 0, 2, 0, 0 );
	sizer_sun_content->SetFlexibleDirection( wxBOTH );
	sizer_sun_content->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText13 = new wxStaticText( siser_sun->GetStaticBox(), wxID_ANY, wxT("Azimute:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	sizer_sun_content->Add( m_staticText13, 0, wxALL, 5 );

	m_text_azi = new wxTextCtrl( siser_sun->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_text_azi->Enable( false );

	sizer_sun_content->Add( m_text_azi, 0, wxALL, 5 );

	m_staticText14 = new wxStaticText( siser_sun->GetStaticBox(), wxID_ANY, wxT("Zênite:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	sizer_sun_content->Add( m_staticText14, 0, wxALL, 5 );

	m_text_zen = new wxTextCtrl( siser_sun->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_text_zen->Enable( false );

	sizer_sun_content->Add( m_text_zen, 0, wxALL, 5 );


	siser_sun->Add( sizer_sun_content, 1, wxEXPAND, 5 );


	sizer_group->Add( siser_sun, 0, wxALL|wxEXPAND, 5 );


	sizer_top_info->Add( sizer_group, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sizer_twilight;
	sizer_twilight = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Crepúsculo") ), wxVERTICAL );

	wxFlexGridSizer* sizer_twilight_content;
	sizer_twilight_content = new wxFlexGridSizer( 0, 2, 0, 0 );
	sizer_twilight_content->SetFlexibleDirection( wxBOTH );
	sizer_twilight_content->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText15 = new wxStaticText( sizer_twilight->GetStaticBox(), wxID_ANY, wxT("Dia:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	sizer_twilight_content->Add( m_staticText15, 0, wxALL, 5 );

	m_text_daytime = new wxTextCtrl( sizer_twilight->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_text_daytime->Enable( false );

	sizer_twilight_content->Add( m_text_daytime, 0, wxALL, 5 );

	m_staticText16 = new wxStaticText( sizer_twilight->GetStaticBox(), wxID_ANY, wxT("Civil:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	sizer_twilight_content->Add( m_staticText16, 0, wxALL, 5 );

	m_text_civil = new wxTextCtrl( sizer_twilight->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_text_civil->Enable( false );

	sizer_twilight_content->Add( m_text_civil, 0, wxALL, 5 );

	m_staticText17 = new wxStaticText( sizer_twilight->GetStaticBox(), wxID_ANY, wxT("Náutico:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	sizer_twilight_content->Add( m_staticText17, 0, wxALL, 5 );

	m_text_nautical = new wxTextCtrl( sizer_twilight->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_text_nautical->Enable( false );

	sizer_twilight_content->Add( m_text_nautical, 0, wxALL, 5 );

	m_staticText18 = new wxStaticText( sizer_twilight->GetStaticBox(), wxID_ANY, wxT("Astronômico:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	sizer_twilight_content->Add( m_staticText18, 0, wxALL, 5 );

	m_text_astronomical = new wxTextCtrl( sizer_twilight->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_text_astronomical->Enable( false );

	sizer_twilight_content->Add( m_text_astronomical, 0, wxALL, 5 );


	sizer_twilight->Add( sizer_twilight_content, 0, wxEXPAND, 5 );


	sizer_top_info->Add( sizer_twilight, 0, wxALL|wxEXPAND, 5 );

	m_calendar1 = new wxCalendarCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, 0 );
	sizer_top_info->Add( m_calendar1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	sizer_datetime_main->Add( sizer_top_info, 0, wxEXPAND, 5 );

	m_notebook_graph = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_daydurationchart = new Plot_Twilight( m_notebook_graph, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
	m_notebook_graph->AddPage( m_daydurationchart, wxT("Duração do dia"), false );
	m_analemmapage = new wxPanel( m_notebook_graph, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizer_analemma;
	sizer_analemma = new wxBoxSizer( wxVERTICAL );

	m_slider_north_analemma = new wxSlider( m_analemmapage, wxID_ANY, 0, -180, 180, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	sizer_analemma->Add( m_slider_north_analemma, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );

	m_analemmachart = new Plot_Analemma( m_analemmapage, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sizer_analemma->Add( m_analemmachart, 1, wxEXPAND, 5 );


	m_analemmapage->SetSizer( sizer_analemma );
	m_analemmapage->Layout();
	sizer_analemma->Fit( m_analemmapage );
	m_notebook_graph->AddPage( m_analemmapage, wxT("Analema"), true );
	m_pathpage = new wxPanel( m_notebook_graph, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizer_path;
	sizer_path = new wxBoxSizer( wxVERTICAL );

	m_slider_north_path = new wxSlider( m_pathpage, wxID_ANY, 0, -180, 180, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	sizer_path->Add( m_slider_north_path, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );

	m_pathchart = new Plot_SunPath( m_pathpage, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sizer_path->Add( m_pathchart, 1, wxEXPAND, 5 );


	m_pathpage->SetSizer( sizer_path );
	m_pathpage->Layout();
	sizer_path->Fit( m_pathpage );
	m_notebook_graph->AddPage( m_pathpage, wxT("Caminho solar"), false );

	sizer_datetime_main->Add( m_notebook_graph, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( sizer_datetime_main );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_slider_date->Connect( wxEVT_SLIDER, wxCommandEventHandler( window_datetime::date_slider_change ), NULL, this );
	m_slider_time->Connect( wxEVT_SLIDER, wxCommandEventHandler( window_datetime::time_slider_change ), NULL, this );
	m_btn_solstice_summer->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( window_datetime::on_summer_solstice ), NULL, this );
	m_btn_equinox_spring->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( window_datetime::on_spring_equinox ), NULL, this );
	m_btn_solstice_winter->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( window_datetime::on_winter_solstice ), NULL, this );
	m_btn_equinox_autumn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( window_datetime::on_autumn_equinox ), NULL, this );
	m_calendar1->Connect( wxEVT_CALENDAR_SEL_CHANGED, wxCalendarEventHandler( window_datetime::calendar_changed ), NULL, this );
	m_slider_north_analemma->Connect( wxEVT_SLIDER, wxCommandEventHandler( window_datetime::analemma_north_changed ), NULL, this );
	m_slider_north_path->Connect( wxEVT_SLIDER, wxCommandEventHandler( window_datetime::path_north_changed ), NULL, this );
}

window_datetime::~window_datetime()
{
	// Disconnect Events
	m_slider_date->Disconnect( wxEVT_SLIDER, wxCommandEventHandler( window_datetime::date_slider_change ), NULL, this );
	m_slider_time->Disconnect( wxEVT_SLIDER, wxCommandEventHandler( window_datetime::time_slider_change ), NULL, this );
	m_btn_solstice_summer->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( window_datetime::on_summer_solstice ), NULL, this );
	m_btn_equinox_spring->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( window_datetime::on_spring_equinox ), NULL, this );
	m_btn_solstice_winter->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( window_datetime::on_winter_solstice ), NULL, this );
	m_btn_equinox_autumn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( window_datetime::on_autumn_equinox ), NULL, this );
	m_calendar1->Disconnect( wxEVT_CALENDAR_SEL_CHANGED, wxCalendarEventHandler( window_datetime::calendar_changed ), NULL, this );
	m_slider_north_analemma->Disconnect( wxEVT_SLIDER, wxCommandEventHandler( window_datetime::analemma_north_changed ), NULL, this );
	m_slider_north_path->Disconnect( wxEVT_SLIDER, wxCommandEventHandler( window_datetime::path_north_changed ), NULL, this );

}
