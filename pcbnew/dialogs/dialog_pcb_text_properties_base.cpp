///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May 21 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "class_pcb_layer_box_selector.h"

#include "dialog_pcb_text_properties_base.h"

///////////////////////////////////////////////////////////////////////////

DIALOG_PCB_TEXT_PROPERTIES_BASE::DIALOG_PCB_TEXT_PROPERTIES_BASE( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : DIALOG_SHIM( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	wxBoxSizer* bMainSizer;
	bMainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_TextLabel = new wxStaticText( this, wxID_ANY, _("Text:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TextLabel->Wrap( -1 );
	bSizer9->Add( m_TextLabel, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_TextContentCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_TextContentCtrl->SetToolTip( _("Enter the text placed on selected layer.") );
	m_TextContentCtrl->SetMinSize( wxSize( 400,60 ) );
	
	bSizer9->Add( m_TextContentCtrl, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bMainSizer->Add( bSizer9, 1, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizerSetup;
	fgSizerSetup = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizerSetup->AddGrowableCol( 0 );
	fgSizerSetup->AddGrowableCol( 1 );
	fgSizerSetup->AddGrowableCol( 2 );
	fgSizerSetup->AddGrowableCol( 3 );
	fgSizerSetup->SetFlexibleDirection( wxBOTH );
	fgSizerSetup->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_SizeXLabel = new wxStaticText( this, wxID_ANY, _("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SizeXLabel->Wrap( -1 );
	fgSizerSetup->Add( m_SizeXLabel, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 3 );
	
	m_PositionXLabel = new wxStaticText( this, wxID_ANY, _("Position X:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PositionXLabel->Wrap( -1 );
	fgSizerSetup->Add( m_PositionXLabel, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 3 );
	
	m_LayerLabel = new wxStaticText( this, wxID_ANY, _("Layer:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LayerLabel->Wrap( -1 );
	fgSizerSetup->Add( m_LayerLabel, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 3 );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, _("Display:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizerSetup->Add( m_staticText10, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 3 );
	
	m_SizeXCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerSetup->Add( m_SizeXCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3 );
	
	m_PositionXCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerSetup->Add( m_PositionXCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3 );
	
	m_LayerSelectionCtrl = new PCB_LAYER_BOX_SELECTOR( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizerSetup->Add( m_LayerSelectionCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3 );
	
	wxString m_DisplayCtrlChoices[] = { _("Normal"), _("Mirrored") };
	int m_DisplayCtrlNChoices = sizeof( m_DisplayCtrlChoices ) / sizeof( wxString );
	m_DisplayCtrl = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_DisplayCtrlNChoices, m_DisplayCtrlChoices, 0 );
	m_DisplayCtrl->SetSelection( 0 );
	fgSizerSetup->Add( m_DisplayCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3 );
	
	m_SizeYLabel = new wxStaticText( this, wxID_ANY, _("Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SizeYLabel->Wrap( -1 );
	fgSizerSetup->Add( m_SizeYLabel, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 3 );
	
	m_PositionYLabel = new wxStaticText( this, wxID_ANY, _("Position Y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PositionYLabel->Wrap( -1 );
	fgSizerSetup->Add( m_PositionYLabel, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 3 );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, _("Style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizerSetup->Add( m_staticText9, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 3 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, _("Justification:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizerSetup->Add( m_staticText11, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 3 );
	
	m_SizeYCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerSetup->Add( m_SizeYCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3 );
	
	m_PositionYCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerSetup->Add( m_PositionYCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3 );
	
	wxString m_StyleCtrlChoices[] = { _("Normal"), _("Italic") };
	int m_StyleCtrlNChoices = sizeof( m_StyleCtrlChoices ) / sizeof( wxString );
	m_StyleCtrl = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_StyleCtrlNChoices, m_StyleCtrlChoices, 0 );
	m_StyleCtrl->SetSelection( 0 );
	fgSizerSetup->Add( m_StyleCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3 );
	
	wxString m_justifyChoiceChoices[] = { _("Left"), _("Center"), _("Right") };
	int m_justifyChoiceNChoices = sizeof( m_justifyChoiceChoices ) / sizeof( wxString );
	m_justifyChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_justifyChoiceNChoices, m_justifyChoiceChoices, 0 );
	m_justifyChoice->SetSelection( 0 );
	fgSizerSetup->Add( m_justifyChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3 );
	
	m_ThicknessLabel = new wxStaticText( this, wxID_ANY, _("Thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ThicknessLabel->Wrap( -1 );
	fgSizerSetup->Add( m_ThicknessLabel, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 3 );
	
	m_orientationLabel = new wxStaticText( this, wxID_ANY, _("Orientation (deg):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_orientationLabel->Wrap( -1 );
	fgSizerSetup->Add( m_orientationLabel, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 3 );
	
	
	fgSizerSetup->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizerSetup->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_ThicknessCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerSetup->Add( m_ThicknessCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3 );
	
	m_OrientCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerSetup->Add( m_OrientCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3 );
	
	
	fgSizerSetup->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizerSetup->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bMainSizer->Add( fgSizerSetup, 0, wxALL|wxEXPAND, 5 );
	
	m_staticline = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bMainSizer->Add( m_staticline, 0, wxEXPAND | wxALL, 5 );
	
	m_StandardSizer = new wxStdDialogButtonSizer();
	m_StandardSizerOK = new wxButton( this, wxID_OK );
	m_StandardSizer->AddButton( m_StandardSizerOK );
	m_StandardSizerCancel = new wxButton( this, wxID_CANCEL );
	m_StandardSizer->AddButton( m_StandardSizerCancel );
	m_StandardSizer->Realize();
	
	bMainSizer->Add( m_StandardSizer, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bMainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DIALOG_PCB_TEXT_PROPERTIES_BASE::OnInitDlg ) );
}

DIALOG_PCB_TEXT_PROPERTIES_BASE::~DIALOG_PCB_TEXT_PROPERTIES_BASE()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DIALOG_PCB_TEXT_PROPERTIES_BASE::OnInitDlg ) );
	
}
