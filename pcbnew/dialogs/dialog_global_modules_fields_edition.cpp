/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2012 Jean-Pierre Charras, jean-pierre.charras@ujf-grenoble.fr
 * Copyright (C) 1992-2012 KiCad Developers, see AUTHORS.txt for contributors.
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

/**
 * @file dialog_global_modules_fields_edition.cpp
 * @brief global module fields edition.
 */

#include <fctsys.h>
#include <common.h>
#include <class_drawpanel.h>
#include <wxBasePcbFrame.h>
#include <base_units.h>
#include <kicad_string.h>
#include <macros.h>
#include <board_commit.h>

#include <pcbnew.h>
#include <wxPcbStruct.h>

#include <class_board.h>
#include <class_module.h>
#include <class_text_mod.h>
#include <dialog_global_modules_fields_edition_base.h>


// The dialog to set options for global fields edition:
// optionas are:
// - edited fields (ref, value, others
// - the footprint filter, for selective edition
class DIALOG_GLOBAL_MODULES_FIELDS_EDITION : public DIALOG_GLOBAL_MODULES_FIELDS_EDITION_BASE
{
    PCB_EDIT_FRAME* m_parent;
    BOARD_DESIGN_SETTINGS* m_brdSettings;
    // Static variable to remember options, withing a session:
    static bool m_refSelection;
    static bool m_valueSelection;
    static bool m_othersSelection;
    static wxString m_filterString;


public:
    DIALOG_GLOBAL_MODULES_FIELDS_EDITION( PCB_EDIT_FRAME* parent )
        : DIALOG_GLOBAL_MODULES_FIELDS_EDITION_BASE( parent )
    {
        m_parent = parent;
        initDialog();
    }

private:
    void initDialog();

    bool TransferDataFromWindow() override;
};

bool DIALOG_GLOBAL_MODULES_FIELDS_EDITION::m_refSelection = false;
bool DIALOG_GLOBAL_MODULES_FIELDS_EDITION::m_valueSelection = false;
bool DIALOG_GLOBAL_MODULES_FIELDS_EDITION::m_othersSelection = false;
wxString DIALOG_GLOBAL_MODULES_FIELDS_EDITION::m_filterString;

void DIALOG_GLOBAL_MODULES_FIELDS_EDITION::initDialog()
{
    m_sdbSizerButtonsOK->SetDefault();

    m_brdSettings = &m_parent->GetDesignSettings();

    m_ReferenceOpt->SetValue(m_refSelection),
    m_ValueOpt->SetValue(m_valueSelection),
    m_OtherFields->SetValue(m_othersSelection);
    m_ModuleFilter->SetValue(m_filterString);
    m_SizeXunit->SetLabel( GetAbbreviatedUnitsLabel() );
    m_SizeYunit->SetLabel( GetAbbreviatedUnitsLabel() );
    m_ThicknessUnit->SetLabel( GetAbbreviatedUnitsLabel() );
    m_SizeX_Value->SetValue(
        StringFromValue( g_UserUnit, m_brdSettings->m_ModuleTextSize.x ) );
    m_SizeY_Value->SetValue(
        StringFromValue( g_UserUnit, m_brdSettings->m_ModuleTextSize.y ) );
    m_ThicknessValue->SetValue(
        StringFromValue( g_UserUnit, m_brdSettings->m_ModuleTextWidth) );

    Layout();
    GetSizer()->SetSizeHints( this );
    Centre();
}


bool DIALOG_GLOBAL_MODULES_FIELDS_EDITION::TransferDataFromWindow()
{
    m_refSelection = m_ReferenceOpt->GetValue();
    m_valueSelection = m_ValueOpt->GetValue();
    m_othersSelection = m_OtherFields->GetValue();
    m_filterString = m_ModuleFilter->GetValue();

    m_brdSettings->m_ModuleTextSize.x = ValueFromTextCtrl( *m_SizeX_Value );
    m_brdSettings->m_ModuleTextSize.y = ValueFromTextCtrl( *m_SizeY_Value );
    m_brdSettings->m_ModuleTextWidth = ValueFromTextCtrl( *m_ThicknessValue );

    // clip m_ModuleTextWidth to the 1/4 of min size, to keep it always readable
    int max_thickness = std::min( m_brdSettings->m_ModuleTextSize.x,
                            m_brdSettings->m_ModuleTextSize.y ) / 4;
    if( m_brdSettings->m_ModuleTextWidth > max_thickness )
        m_brdSettings->m_ModuleTextWidth = max_thickness;

    m_parent->ResetModuleTextSizes( m_filterString, m_refSelection,
                                    m_valueSelection, m_othersSelection );
    return true;
}


void PCB_EDIT_FRAME::OnResetModuleTextSizes( wxCommandEvent& event )
{
    DIALOG_GLOBAL_MODULES_FIELDS_EDITION dlg( this );
    dlg.ShowModal();
}

void PCB_BASE_FRAME::ResetModuleTextSizes( const wxString & aFilter, bool aRef,
                                           bool aValue, bool aOthers )
{
    BOARD_COMMIT commit( this );

    int modTextWidth = GetDesignSettings().m_ModuleTextWidth;
    const wxSize& modTextSize = GetDesignSettings().m_ModuleTextSize;
    bool modified = false;

    // Change fields of footprints with fpid matching the filter
    for( MODULE* module = GetBoard()->m_Modules; module; module = module->Next() )
    {
        if( !aFilter.IsEmpty() )
        {
            if( ! WildCompareString( aFilter, FROM_UTF8( module->GetFPID().Format().c_str() ),
                                     false ) )
                continue;
        }


        if( aRef )
        {
            TEXTE_MODULE* item = &module->Reference();

            if( item->GetTextSize() != GetDesignSettings().m_ModuleTextSize ||
                item->GetThickness() != GetDesignSettings().m_ModuleTextWidth )
            {
                commit.Modify( item );
                item->SetThickness( modTextWidth );
                item->SetTextSize( modTextSize );
                modified = true;
            }
        }

        if( aValue )
        {
            TEXTE_MODULE* item = &module->Value();

            if( item->GetTextSize() != GetDesignSettings().m_ModuleTextSize ||
                item->GetThickness() != GetDesignSettings().m_ModuleTextWidth )
            {
                commit.Modify( item );
                item->SetThickness( modTextWidth );
                item->SetTextSize( modTextSize );
                modified = true;
            }
        }

        if( aOthers )
        {
            // Go through all other module text fields
            for( BOARD_ITEM* boardItem = module->GraphicalItems(); boardItem; boardItem = boardItem->Next() )
            {
                if( boardItem->Type() == PCB_MODULE_TEXT_T )
                {
                    TEXTE_MODULE* item = static_cast<TEXTE_MODULE*>( boardItem );

                    if( item->GetTextSize() != GetDesignSettings().m_ModuleTextSize
                        || item->GetThickness() != GetDesignSettings().m_ModuleTextWidth )
                    {
                        commit.Modify( item );
                        item->SetThickness( modTextWidth );
                        item->SetTextSize( modTextSize );
                        modified = true;
                    }
                }
            }
        }
    }

    if( modified )
    {
        commit.Push( "Reset module text size" );
        GetCanvas()->Refresh();
    }
}
