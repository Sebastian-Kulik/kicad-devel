

/**
 * @file move-drag_pads.cpp
 * @brief Edit footprint pads.
 */

#include <fctsys.h>
#include <gr_basic.h>
#include <common.h>
#include <class_drawpanel.h>
#include <trigo.h>
#include <block_commande.h>
#include <wxBasePcbFrame.h>

#include <class_board.h>
#include <class_module.h>

#include <pcbnew.h>
#include <drag.h>

#ifdef PCBNEW_WITH_TRACKITEMS
#include "trackitems/trackitems.h"
static PICKED_ITEMS_LIST pick_list;
#endif


static D_PAD*  s_CurrentSelectedPad;
static wxPoint Pad_OldPos;


/* Cancel move pad command.
 */
static void Abort_Move_Pad( EDA_DRAW_PANEL* Panel, wxDC* DC )
{
    D_PAD* pad = s_CurrentSelectedPad;

    Panel->SetMouseCapture( NULL, NULL );

    if( pad == NULL )
        return;

    pad->Draw( Panel, DC, GR_XOR );
    pad->ClearFlags();
    pad->SetPosition( Pad_OldPos );
    pad->Draw( Panel, DC, GR_XOR );

#ifdef PCBNEW_WITH_TRACKITEMS
    PCB_EDIT_FRAME* edit_frame = static_cast<PCB_EDIT_FRAME*>( Panel->GetParent() );
    BOARD* pcb = edit_frame->GetBoard();
    pcb->TrackItems()->Teardrops()->UpdateListClear();
    pcb->TrackItems()->RoundedTracksCorners()->UpdateListClear();
    for( unsigned jj=0 ; jj < g_DragSegmentList.size(); jj++ )
    {
        TRACK* tr = g_DragSegmentList[jj].m_Track;
        pcb->TrackItems()->RoundedTracksCorners()->UpdateListAdd( tr );
    }
#endif

    // Pad move in progress: restore origin of dragged tracks, if any.
    for( unsigned ii = 0; ii < g_DragSegmentList.size(); ii++ )
    {
        TRACK* track = g_DragSegmentList[ii].m_Track;
        track->Draw( Panel, DC, GR_XOR );
        track->SetState( IN_EDIT, false );
        track->ClearFlags();
        g_DragSegmentList[ii].RestoreInitialValues();
        track->Draw( Panel, DC, GR_OR );
#ifdef PCBNEW_WITH_TRACKITEMS
        pcb->TrackItems()->Teardrops()->UpdateListAdd(track);
#endif
    }
#ifdef PCBNEW_WITH_TRACKITEMS
    if( !g_DragSegmentList.size() )
        pcb->TrackItems()->Teardrops()->Recreate( pad, false );
    pcb->TrackItems()->RoundedTracksCorners()->UpdateListDo();
    pcb->TrackItems()->Teardrops()->UpdateListAdd( pcb->TrackItems()->RoundedTracksCorners()->UpdateList_GetUpdatedTracks() );
    pcb->TrackItems()->Teardrops()->UpdateListDo();
    Panel->Refresh();
#endif

    EraseDragList();
    s_CurrentSelectedPad = NULL;
}


/* Draw in drag mode when moving a pad.
 */
static void Show_Pad_Move( EDA_DRAW_PANEL* aPanel, wxDC* aDC, const wxPoint& aPosition,
                           bool aErase )
{
    TRACK*       Track;
    D_PAD*       pad    = s_CurrentSelectedPad;

    if( pad == NULL )       // Should not occur
        return;

    if( aErase )
        pad->Draw( aPanel, aDC, GR_XOR );

    pad->SetPosition( aPanel->GetParent()->GetCrossHairPosition() );
    pad->Draw( aPanel, aDC, GR_XOR );

#ifdef PCBNEW_WITH_TRACKITEMS
    BOARD * pcb = static_cast<PCB_EDIT_FRAME*>(aPanel->GetParent())->GetBoard();
    pcb->TrackItems()->Teardrops()->UpdateListClear();
    pcb->TrackItems()->RoundedTracksCorners()->UpdateListClear();
    for( unsigned ii = 0; ii < g_DragSegmentList.size(); ii++ )
    {
        TRACK* tr = g_DragSegmentList[ii].m_Track;
        pcb->TrackItems()->RoundedTracksCorners()->UpdateListAdd( tr );
    }
    pcb->TrackItems()->RoundedTracksCorners()->UpdateList_DrawTracks( aPanel, aDC, GR_XOR );
#endif

    for( unsigned ii = 0; ii < g_DragSegmentList.size(); ii++ )
    {
        Track = g_DragSegmentList[ii].m_Track;

#ifdef PCBNEW_WITH_TRACKITEMS
        if( !dynamic_cast<ROUNDED_CORNER_TRACK*>(Track) )
#endif
        if( aErase )
            Track->Draw( aPanel, aDC, GR_XOR );

        g_DragSegmentList[ii].SetTrackEndsCoordinates( wxPoint(0, 0) );

#ifdef PCBNEW_WITH_TRACKITEMS
        pcb->TrackItems()->Teardrops()->UpdateListAdd( Track );
        if( !dynamic_cast<ROUNDED_CORNER_TRACK*>(Track) )
#endif
        Track->Draw( aPanel, aDC, GR_XOR );
    }
#ifdef PCBNEW_WITH_TRACKITEMS
    pcb->TrackItems()->RoundedTracksCorners()->UpdateListDo( aPanel, aDC, GR_XOR, true );
    pcb->TrackItems()->RoundedTracksCorners()->UpdateList_DrawTracks( aPanel, aDC, GR_XOR );
    pcb->TrackItems()->Teardrops()->UpdateListAdd( pcb->TrackItems()->RoundedTracksCorners()->UpdateList_GetUpdatedTracks() );
    pcb->TrackItems()->Teardrops()->UpdateListDo( aPanel, aDC, GR_XOR, true );
#endif
}


// Function to initialize the "move pad" command
void PCB_BASE_FRAME::StartMovePad( D_PAD* aPad, wxDC* aDC, bool aDragConnectedTracks )
{
    if( aPad == NULL )
        return;

    s_CurrentSelectedPad = aPad;

    Pad_OldPos = aPad->GetPosition();

    SetMsgPanel( aPad );
    m_canvas->SetMouseCapture( Show_Pad_Move, Abort_Move_Pad );

    // Draw the pad, in SKETCH mode
    aPad->Draw( m_canvas, aDC, GR_XOR );
    aPad->SetFlags( IS_MOVED );
    aPad->Draw( m_canvas, aDC, GR_XOR );

    EraseDragList();

#ifdef PCBNEW_WITH_TRACKITEMS
    pick_list.ClearItemsList();
#endif

    // Build the list of track segments to drag if the command is a drag pad
    if( aDragConnectedTracks )
    {
        DRAG_LIST drglist( GetBoard() );
        drglist.BuildDragListe( aPad );

#ifdef PCBNEW_WITH_TRACKITEMS
        ITEM_PICKER itemWrapper( NULL, UR_CHANGED );
        for( unsigned ii = 0; ii < g_DragSegmentList.size(); ii++ )
        {
            TRACK* segm = g_DragSegmentList[ii].m_Track;
            itemWrapper.SetItem( segm );
            itemWrapper.SetLink( segm->Clone() );
            itemWrapper.GetLink()->SetState( IN_EDIT, false );
            pick_list.PushItem( itemWrapper );
        }
#endif

        UndrawAndMarkSegmentsToDrag( m_canvas, aDC );
    }
#ifdef PCBNEW_WITH_TRACKITEMS
    else
        GetBoard()->TrackItems()->Teardrops()->Remove( aPad, false, true );
#endif
}


// Routine to place a moved pad.
void PCB_BASE_FRAME::PlacePad( D_PAD* aPad, wxDC* DC )
{
    int     dX, dY;
    TRACK*  track;

    if( aPad == NULL )
        return;

    MODULE* module = aPad->GetParent();

    ITEM_PICKER       picker( NULL, UR_CHANGED );

#ifndef PCBNEW_WITH_TRACKITEMS
    PICKED_ITEMS_LIST pickList;

    // Save dragged track segments in undo list
    for( unsigned ii = 0; ii < g_DragSegmentList.size(); ii++ )
    {
        track = g_DragSegmentList[ii].m_Track;

        // Set the old state
        if( g_DragSegmentList[ii].m_Pad_Start )
            track->SetStart( Pad_OldPos );

        if( g_DragSegmentList[ii].m_Pad_End )
            track->SetEnd( Pad_OldPos );

        picker.SetItem( track );
        pickList.PushItem( picker );
    }
#endif

    // Save old module and old items values
    aPad->ClearFlags();
    wxPoint pad_curr_position = aPad->GetPosition();

    aPad->SetPosition( Pad_OldPos );

    if( g_DragSegmentList.size() == 0 )
#ifdef PCBNEW_WITH_TRACKITEMS
    {
        GetBoard()->TrackItems()->Teardrops()->Recreate( aPad, false );
        GetBoard()->TrackItems()->Teardrops()->Remove( aPad, &pick_list, true );
    }
    picker.SetItem( module );
    pick_list.PushItem( picker );
    SaveCopyInUndoList( pick_list, UR_CHANGED );
#else
        SaveCopyInUndoList( module, UR_CHANGED );
    else
    {
        picker.SetItem( module );
        pickList.PushItem( picker );
        SaveCopyInUndoList( pickList, UR_CHANGED );
    }
#endif

    aPad->SetPosition( pad_curr_position );
    aPad->Draw( m_canvas, DC, GR_XOR );

    // Redraw dragged track segments
    for( unsigned ii = 0; ii < g_DragSegmentList.size(); ii++ )
    {
        track = g_DragSegmentList[ii].m_Track;

#ifdef PCBNEW_WITH_TRACKITEMS
        g_DragSegmentList[ii].SetTrackEndsCoordinates( wxPoint(0, 0) );
#else
        // Set the new state
        if( g_DragSegmentList[ii].m_Pad_Start )
            track->SetStart( aPad->GetPosition() );

        if( g_DragSegmentList[ii].m_Pad_End )
            track->SetEnd( aPad->GetPosition() );

        if( DC )
            track->Draw( m_canvas, DC, GR_XOR );

        track->SetState( IN_EDIT, false );
        track->ClearFlags();
#endif

        if( DC )
            track->Draw( m_canvas, DC, GR_OR );
    }

    // Compute local coordinates (i.e refer to module position and for module orient = 0)
    dX = aPad->GetPosition().x - Pad_OldPos.x;
    dY = aPad->GetPosition().y - Pad_OldPos.y;

    RotatePoint( &dX, &dY, -module->GetOrientation() );

    aPad->SetX0( dX + aPad->GetPos0().x );
    aPad->SetY0( dY + aPad->GetPos0().y );

    if( DC )
        aPad->Draw( m_canvas, DC, GR_OR );

#ifdef PCBNEW_WITH_TRACKITEMS
    GetBoard()->TrackItems()->Teardrops()->Update( aPad, m_canvas, DC, GR_XOR, true );
    m_canvas->Refresh();
#endif

    module->CalculateBoundingBox();
    module->SetLastEditTime();

    EraseDragList();

    OnModify();
    m_canvas->SetMouseCapture( NULL, NULL );
    m_Pcb->m_Status_Pcb &= ~( LISTE_RATSNEST_ITEM_OK | CONNEXION_OK );
}
