/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 2014-2015 KiCad Developers, see CHANGELOG.TXT for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include "dialog_3D_view_option_base.h"
#include <3d_viewer/eda_3d_viewer.h>
#include <3d_canvas/cinfo3d_visu.h>
#include <bitmaps.h>

class DIALOG_3D_VIEW_OPTIONS : public DIALOG_3D_VIEW_OPTIONS_BASE
{
public:
    explicit DIALOG_3D_VIEW_OPTIONS( EDA_3D_VIEWER* parent );

private:
    EDA_3D_VIEWER* m_parent;
    CINFO3D_VISU & m_3Dprms;

    void initDialog();

    // Event functions:
    void OnShowAllClick( wxCommandEvent& event ) override;
    void OnShowNoneClick( wxCommandEvent& event ) override;
    void OnOKClick( wxCommandEvent& event ) override;
};


void EDA_3D_VIEWER::Install3DViewOptionDialog( wxCommandEvent& event )
{
    DIALOG_3D_VIEW_OPTIONS dlg( this );

    if( dlg.ShowModal() == wxID_OK )
    {
        SetMenuBarOptionsState();
        NewDisplay();
    }
}


DIALOG_3D_VIEW_OPTIONS::DIALOG_3D_VIEW_OPTIONS( EDA_3D_VIEWER* parent )
     :DIALOG_3D_VIEW_OPTIONS_BASE( parent ), m_3Dprms( parent->GetSettings() )
{
    m_parent = parent;

    initDialog();

    SetDefaultItem( m_sdbSizerOK );
	Layout();
    GetSizer()->SetSizeHints( this );
	Centre();
}


void DIALOG_3D_VIEW_OPTIONS::initDialog()
{
    m_bitmapCuThickness->SetBitmap( KiBitmap( use_3D_copper_thickness_xpm ) );
    m_bitmap3Dshapes->SetBitmap( KiBitmap( shape_3d_xpm ) );
    m_bitmapAreas->SetBitmap( KiBitmap( add_zone_xpm ) );
    m_bitmapSilkscreen->SetBitmap( KiBitmap( text_xpm ) );
    m_bitmapSolderMask->SetBitmap( KiBitmap( pads_mask_layers_xpm ) );
    m_bitmapSolderPaste->SetBitmap( KiBitmap( pads_mask_layers_xpm ) );
    m_bitmapAdhesive->SetBitmap( KiBitmap( tools_xpm ) );
    m_bitmapComments->SetBitmap( KiBitmap( editor_xpm ) );
    m_bitmapECO->SetBitmap( KiBitmap( editor_xpm ) );

    // Check/uncheck checkboxes
    m_checkBoxAreas->SetValue( m_3Dprms.GetFlag( FL_ZONE ) );
    m_checkBoxSilkscreen->SetValue( m_3Dprms.GetFlag( FL_SILKSCREEN ) );
    m_checkBoxSolderMask->SetValue( m_3Dprms.GetFlag( FL_SOLDERMASK ) );
    m_checkBoxSolderpaste->SetValue( m_3Dprms.GetFlag( FL_SOLDERPASTE ) );
    m_checkBoxAdhesive->SetValue( m_3Dprms.GetFlag( FL_ADHESIVE ) );
    m_checkBoxComments->SetValue( m_3Dprms.GetFlag( FL_COMMENTS ) );
    m_checkBoxECO->SetValue( m_3Dprms.GetFlag( FL_ECO ) );
}


void DIALOG_3D_VIEW_OPTIONS::OnShowAllClick( wxCommandEvent& event )
{
    bool state = true;
    m_checkBoxCuThickness->SetValue( state );
    //m_checkBox3Dshapes->SetValue( state );
    m_checkBoxAreas->SetValue( state );
    m_checkBoxSilkscreen->SetValue( state );
    m_checkBoxSolderMask->SetValue( state );
    m_checkBoxSolderpaste->SetValue( state );
    m_checkBoxAdhesive->SetValue( state );
    m_checkBoxComments->SetValue( state );
    m_checkBoxECO->SetValue( state );
}


void DIALOG_3D_VIEW_OPTIONS::OnShowNoneClick( wxCommandEvent& event )
{
    bool state = false;
    m_checkBoxCuThickness->SetValue( state );
    //m_checkBox3Dshapes->SetValue( state );
    m_checkBoxAreas->SetValue( state );
    m_checkBoxSilkscreen->SetValue( state );
    m_checkBoxSolderMask->SetValue( state );
    m_checkBoxSolderpaste->SetValue( state );
    m_checkBoxAdhesive->SetValue( state );
    m_checkBoxComments->SetValue( state );
    m_checkBoxECO->SetValue( state );
}


void DIALOG_3D_VIEW_OPTIONS::OnOKClick( wxCommandEvent& event )
{
    m_3Dprms.SetFlag( FL_ZONE, m_checkBoxAreas->GetValue() );
    m_3Dprms.SetFlag( FL_SILKSCREEN, m_checkBoxSilkscreen->GetValue() );
    m_3Dprms.SetFlag( FL_SOLDERMASK, m_checkBoxSolderMask->GetValue() );
    m_3Dprms.SetFlag( FL_SOLDERPASTE, m_checkBoxSolderpaste->GetValue() );
    m_3Dprms.SetFlag( FL_ADHESIVE, m_checkBoxAdhesive->GetValue() );
    m_3Dprms.SetFlag( FL_COMMENTS, m_checkBoxComments->GetValue() );
    m_3Dprms.SetFlag( FL_ECO, m_checkBoxECO->GetValue( ) );

    EndModal( wxID_OK );
}
