/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2014- Heikki Pulkkinen.
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
 * @file tracknodeitemstools.h
 * @brief Definitions for tracknodeitemstools base class.
 */

#ifndef TRACKITEMS_H
#define TRACKITEMS_H

#include "tracknodeitems.h"
#include "teardrops.h"
#include "roundedtrackscorners.h"

namespace TrackItems
{

    class PADS_SCAN_BASE
    {

    public:
        virtual ~PADS_SCAN_BASE(){};
        void Execute( void );

    protected:
        PADS_SCAN_BASE() {};
        PADS_SCAN_BASE( const BOARD* aBoard );

        virtual bool ExecutePad( const D_PAD* aPad )=0;

    private:
        MODULE* m_first_module;
    };

} //namespace TrackItems

//-----------------------------------------------------------------------------------------------------/
// Track items tools class
//-----------------------------------------------------------------------------------------------------/
class TRACKITEMS
{
public:
    static const wxString TXT_TRACKITEMS;

    TRACKITEMS( const BOARD* aBoard );
    ~TRACKITEMS();

    TEARDROPS* Teardrops( void ) const { return m_Teardrops; }
    ROUNDED_TRACKS_CORNERS* RoundedTracksCorners( void ) const { return m_RoundedTracksCorners; }

#ifdef NEWCONALGO
    void SetEditFrame( const PCB_EDIT_FRAME* aEditFrame );  //Must be done when BOARD is created.
    void SetMenu( wxMenu* aMenu );
#else
    void SetEditFrame( const PCB_EDIT_FRAME* aEditFrame, wxMenu* aMenu );  //Must be done when BOARD is created.
#endif
    PCB_EDIT_FRAME* GetEditFrame( void ) const { return m_EditFrame; }
    BOARD* GetBoard( void ) const { return m_Board; }

    //void TrackItemsMenuAdd( wxMenu* aMenuToAdd );
    void Popup_PickTrackOrViaWidth( wxMenu* aMenu, const TRACK* aTrackSeg ) const;

    //Eliminate items effect when drag track and keep slope.
    TRACK* DragKeepSlopeSegmentTypeCheck( TRACK* aTrackToPoint,
                                          const TRACK* aTrackSeg,
                                          const TRACK* aTrace,
                                          const ENDPOINT_T aEndpoint
                                        );

    bool DragKeepSlopeSegmentsNumCheck( const bool aCurrentError,
                                        TRACK* aTrackToPoint,
                                        const TRACK *aTrackSeg,
                                        const TRACK *aTrace,
                                        const ENDPOINT_T aEndpoint
                                      );

    void PickViaSize( const VIA* aVia );
    void PickTrackSize( const TRACK* aTrack );

private:
    TRACKITEMS(){};

    TEARDROPS* m_Teardrops{nullptr};
    ROUNDED_TRACKS_CORNERS* m_RoundedTracksCorners{nullptr};

    BOARD* m_Board;
    PCB_EDIT_FRAME* m_EditFrame;

//---------------------------------------------------------------------------------------------------
//DRC
//---------------------------------------------------------------------------------------------------
public:
    MARKER_PCB* DRC_AddMarker( const BOARD_CONNECTED_ITEM* aItem1,
                               const BOARD_ITEM* aItem2,
                               const wxPoint aMarkerPos,
                               const int aErrorCode
                             );
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
// Net scan base.
//---------------------------------------------------------------------------------------------------
private:
    class NET_SCAN_BASE : public TrackNodeItem::SCAN_NET_BASE
    {
    public:
        ~NET_SCAN_BASE() {};

    protected:
        NET_SCAN_BASE() {};
        NET_SCAN_BASE( const TRACK* aStartTrack,
                       const TRACKITEMS* aParent
                     ) :
            SCAN_NET_BASE( aStartTrack )
        {
            m_Parent = const_cast<TRACKITEMS*>( aParent );
        }

        TRACKITEMS* m_Parent {nullptr};
    };
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
// Get pads and vias
//---------------------------------------------------------------------------------------------------
public:
    VIA* GetVia( const TRACK* aTrackSegAt, const wxPoint aPosAt ) const;
    VIA* EndPosVia( const TRACK* aTrackSegAt ) const;
    VIA* StartPosVia( const TRACK* aTrackSegAt ) const;
    D_PAD* GetPad( const TRACK* aTrackSegAt, const wxPoint aPosAt ) const;
    D_PAD* EndPosPad( const TRACK* aTrackSegAt ) const;
    D_PAD* StartPosPad( const TRACK* aTrackSegAt ) const;
    std::vector<D_PAD*> GetPads( const int aNetcode ) const;

private:
    class NET_SCAN_GET_VIA : public NET_SCAN_BASE
    {
    public:
        NET_SCAN_GET_VIA( const TRACK* aStartTrack, const wxPoint aPos, const TRACKITEMS* aParent );
        ~NET_SCAN_GET_VIA() {};

        VIA* GetResult( void ) const {
            return m_result_via;
        }

    protected:
        bool ExecuteAt( TRACK* aTrack ) override;

        VIA* m_result_via {nullptr};
    private:
        wxPoint m_pos {0,0};
    };

    class NET_SCAN_GET_ENDPOS_VIA : public NET_SCAN_GET_VIA
    {
    public:
        NET_SCAN_GET_ENDPOS_VIA( const TRACK* aStartTrack, const TRACKITEMS* aParent );
        ~NET_SCAN_GET_ENDPOS_VIA() {};

    protected:
        bool ExecuteAt( TRACK* aTrack ) override;
    };

    class NET_SCAN_GET_STARTPOS_VIA : public NET_SCAN_GET_VIA
    {
    public:
        NET_SCAN_GET_STARTPOS_VIA( const TRACK* aStartTrack, const TRACKITEMS* aParent );
        ~NET_SCAN_GET_STARTPOS_VIA() {};

    protected:
        bool ExecuteAt( TRACK* aTrack ) override;
    };

    class PADS_SCAN_GET_PADS_IN_NET : public TrackItems::PADS_SCAN_BASE
    {
    public:
        PADS_SCAN_GET_PADS_IN_NET( const int aNetCode, std::vector<D_PAD*>& aPadsList, const BOARD* aBoard );

    protected:
        virtual bool ExecutePad( const D_PAD* aPad ) override;

    private:
        int m_netcode;
        std::vector<D_PAD*>* m_pads_list;
    };
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
// Draw target pos net scan
//---------------------------------------------------------------------------------------------------
    class NET_SCAN_DRAW_TARGET_NODE_POS : public NET_SCAN_BASE
    {
    public:
        NET_SCAN_DRAW_TARGET_NODE_POS( const TRACK* aStartTrack,
                                       const wxPoint aPosition,
                                       const std::vector<DRAG_SEGM_PICKER>* aDragSegmentList,
                                       const TRACKITEMS* aParent
                                     );
        ~NET_SCAN_DRAW_TARGET_NODE_POS() {};

        bool GetResult( void ) const {
            return m_result;
        }

    protected:
        bool ExecuteAt( TRACK* aTrack ) override;

    private:
        bool m_result {false};
        wxPoint m_pos {0,0};
        std::vector<DRAG_SEGM_PICKER>* m_drag_segments;
    };
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
// TRACKS_PROGRESS
//---------------------------------------------------------------------------------------------------
    class TRACKITEMS_TRACKS_PROGRESS : public TrackNodeItems::TRACKS_PROGRESS
    {
    protected:
        TRACKITEMS_TRACKS_PROGRESS( const TRACKITEMS* aParent,
                                    const DLIST<TRACK>* aTracks,
                                    PICKED_ITEMS_LIST* aUndoRedoList
                                  ) :
            TRACKS_PROGRESS( aParent->GetEditFrame(), aTracks, aUndoRedoList )
        {
            m_Parent = const_cast<TRACKITEMS*>( aParent );
        }

        TRACKITEMS* m_Parent{nullptr};

    };
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
// Mark Sharp angles
//---------------------------------------------------------------------------------------------------
public:
    void MarkSharpAngles( const DLIST<TRACK>* aTracksAt, DRC* aDRC );

private:
    class TRACKS_PROGRESS_MARK_SHARP_ANGLES : public TRACKITEMS_TRACKS_PROGRESS
    {
    public:
        TRACKS_PROGRESS_MARK_SHARP_ANGLES( const TRACKITEMS* aParent,
                                           const DLIST<TRACK>* aTracks,
                                           DRC* aDRC
                                         );

    protected:
        unsigned int ExecuteItem( const BOARD_ITEM* aItemAt ) override;

    private:
        std::vector<wxPoint> marked_tracks_sharp_angle;
        TRACK* Find_Tracks_Sharp_Angle( const TRACK* aTrackSegAt, const wxPoint aPosAt ) const;
    };

    class NET_SCAN_SHARP_ANGLES : public NET_SCAN_BASE
    {
    public:
        NET_SCAN_SHARP_ANGLES( const TRACK* aStartTrack,
                               const TRACKITEMS* aParent,
                               const wxPoint aPos
                             );
        ~NET_SCAN_SHARP_ANGLES() {};

        TRACK* GetResult( void ) const
        {
            return m_result_track;
        }

    protected:
        bool ExecuteAt( TRACK* aTrack ) override;

    private:
        wxPoint m_pos {0,0};
        double m_angle {0.0};
        PCB_LAYER_ID m_layer_id {F_Cu};
        TRACK* m_result_track;
    };
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
//connect tracks nodes inside via or pad center of them
//---------------------------------------------------------------------------------------------------
public:
    void CentralizeTrackConnection( const DLIST<TRACK>* aTracksAt );

private:
    class TRACKS_PROGRESS_CENTRALIZE_CONNECTION: public TRACKITEMS_TRACKS_PROGRESS
    {
    public:
        TRACKS_PROGRESS_CENTRALIZE_CONNECTION( const TRACKITEMS* aParent,
                                             const DLIST<TRACK>* aTracks,
                                             PICKED_ITEMS_LIST* aUndoRedoList
                                           );
        ~TRACKS_PROGRESS_CENTRALIZE_CONNECTION();

    protected:
        unsigned int ExecuteItem( const BOARD_ITEM* aItemAt ) override;

    private:
        TrackNodeItem::TEARDROP::SHAPES_T m_current_shape;
        TrackNodeItem::TEARDROP::PARAMS m_teardrop_params;
        TrackNodeItem::TEARDROP::PARAMS m_fillet_params;
        TrackNodeItem::TEARDROP::PARAMS m_subland_params;

        std::vector<TRACK*> m_fixed_tracks;
    };

//---------------------------------------------------------------------------------------------------
// Bad connected via.
//---------------------------------------------------------------------------------------------------
public:
    VIA* GetBadConnectedVia( const TRACK* aTrackSeg,
                             const wxPoint aTrackPos,
                             TrackNodeItem::Tracks_Container* aResultList
                           );

private:
    class NET_SCAN_VIA_BAD_CONNECTION : public NET_SCAN_BASE
    {
    public:
        NET_SCAN_VIA_BAD_CONNECTION( const TRACKITEMS* aParent,
                                     const TRACK* aStartTrack,
                                     const wxPoint aTrackPos,
                                     TrackNodeItem::Tracks_Container* aResultList
                                   );
        ~NET_SCAN_VIA_BAD_CONNECTION() {};

        VIA* GetVia( void ) { return m_via; }

    protected:
        bool ExecuteAt( TRACK* aTrack ) override;

    private:
        wxPoint m_track_pos {0,0};
        TrackNodeItem::Tracks_Container* m_result_list;
        VIA* m_via{nullptr};
    };

//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
// Angles / pos helpers.
//---------------------------------------------------------------------------------------------------
public:
    void Edittrack_Init( const TRACK* aTrackSeg, const wxPoint aPosition );
    void Edittrack_Clear( void );
    void Angles( const TRACK* aTrackSeg,
                 const wxPoint aPosition,
                 EDA_DRAW_PANEL* aPanel,
                 wxDC* aDC,
                 const wxPoint& aOffset = BOARD_ITEM::ZeroOffset
               );

    void Angles( const std::vector<DRAG_SEGM_PICKER>* aDragSegmentList,
                 const wxPoint aPosition,
                 EDA_DRAW_PANEL* aPanel,
                 wxDC* aDC,
                 const wxPoint& aOffset = BOARD_ITEM::ZeroOffset
               );

    void Target( const std::vector<DRAG_SEGM_PICKER>* aDragSegmentList,
                 const wxPoint aPosition,
                 EDA_DRAW_PANEL* aPanel,
                 wxDC* aDC,
                 const wxPoint& aOffset = BOARD_ITEM::ZeroOffset
               );

private:
    bool m_target_pos_drawn {false};
    wxPoint m_target_pos{0,0};
    using Angle_Arc_Tuple = std::tuple<wxPoint, wxPoint, wxPoint, wxPoint, bool>;
    enum ANGLE_ARC_TUPLES
    {
        MID_POS,
        START_POS,
        END_POS,
        RECT_POS,
        IS_RECT
    };
    std::vector<Angle_Arc_Tuple> m_draw_help_angle_arc;
    std::set<TRACK*> m_edittrack_start_segments;
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
// Length matching fine tuning
//---------------------------------------------------------------------------------------------------
public:
    double GetNetLength( const TRACK* aTrack );
    void SetMsgPanel( const TRACK* aTrack );

private:
    class NET_SCAN_NET_LENGTH : public NET_SCAN_BASE
    {
    public:
        NET_SCAN_NET_LENGTH( const TRACKITEMS* aParent, const TRACK* aStartTrack );
        ~NET_SCAN_NET_LENGTH() {};

        double GetLength( void ) const{ return m_netlength;}

    protected:
        bool ExecuteAt( TRACK* aTrack ) override;

    private:
        double m_netlength = 0.0;
    };
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
// Speedup m_Track linked list keeping count first item of netcode.
//---------------------------------------------------------------------------------------------------
private:

    class NETCODE_FIRST_TRACKITEM
    {
    public:
        NETCODE_FIRST_TRACKITEM( const BOARD* aBoard )
        {
            m_Board = aBoard;
            m_netcode_first_trackitems.clear();
        }
        ~NETCODE_FIRST_TRACKITEM(){;}

        TRACK* GetBestInsertPoint( const int aNetCode );
        TRACK* GetFirst( const int aNetCode );
        void Insert( const TRACK* aTrackItem );
        void Remove( const TRACK* aTrackItem );

    private:
        NETCODE_FIRST_TRACKITEM(){;}
        const BOARD* m_Board{nullptr};
        std::vector<TRACK*> m_netcode_first_trackitems;
        TRACK* GetAndSync( const int aNetCode );

    };

    NETCODE_FIRST_TRACKITEM* m_NetCodeFirstTrackItem{nullptr};

public:
    NETCODE_FIRST_TRACKITEM* NetCodeFirstTrackItem( void ) const
    {
        return m_NetCodeFirstTrackItem;
    }

//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
// Sort tracks by netcode.
//---------------------------------------------------------------------------------------------------
public:
    void SortTracks( void );

private:
//---------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------/
// Gal canvas route commit push.
//-----------------------------------------------------------------------------------------------------/
public:
    void GalRouteCommitPrepare( void );
    void GalRouteCommitAdd( BOARD_ITEM* aItem, PICKED_ITEMS_LIST* aUndoRedoList );
    void GalRouteCommitRemove( BOARD_ITEM* aItemFrom, PICKED_ITEMS_LIST* aUndoRedoList );
    void GalRouteCommitFinish( void );

private:
    std::set<TRACK*> m_gal_commit_added_tracks;
    void GalRouteCommitFinish( PICKED_ITEMS_LIST* aUndoRedoList );
//-----------------------------------------------------------------------------------------------------/


}; //TRACKITEMS

#endif // TRACKITEMS_H
