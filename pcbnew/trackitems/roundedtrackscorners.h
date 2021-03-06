/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2017- Heikki Pulkkinen.
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
 * @file rounded_trackscorners.h
 * @brief Definitions for rounded track corners using class.
 */

#ifndef ROUNDED_TRACKS_CORNERS_H
#define ROUNDED_TRACKS_CORNERS_H


#include "tracknodeitem.h"
#include "tracknodeitems.h"
#include "roundedtrackscorner.h"
#include "roundedcornertrack.h"

#include <drag.h>
#include <board_commit.h>


class ROUNDED_TRACKS_CORNERS : public TrackNodeItems::TRACKNODEITEMS
{
public:
    static const wxString TXT_ROUNDEDTRACKSCORNERS;

    ROUNDED_TRACKS_CORNERS( const TRACKITEMS* aParent, const BOARD* aBoard );
    ~ROUNDED_TRACKS_CORNERS();

    inline bool IsOn( void ) const { return m_params.length_ratio; }

    //One aCorner
    TrackNodeItem::ROUNDED_TRACKS_CORNER* Add( TRACK* aTrackTo, const wxPoint& aCurPosAt );
    TrackNodeItem::ROUNDED_TRACKS_CORNER* Add( TRACK* aTrackTo,
                                             const wxPoint aPosition,
                                             PICKED_ITEMS_LIST* aUndoRedoList
                                           );
    TrackNodeItem::ROUNDED_TRACKS_CORNER* Add( TRACK* aTrackTo,
                                             const wxPoint aPosition,
                                             const unsigned int aLength,
                                             PICKED_ITEMS_LIST* aUndoRedoList
                                           );

    std::pair<TrackNodeItem::ROUNDED_TRACKS_CORNER*, TrackNodeItem::ROUNDED_TRACKS_CORNER*>
        Add( TRACK* aTrackTo, PICKED_ITEMS_LIST* aUndoRedoList );
    std::pair<TrackNodeItem::ROUNDED_TRACKS_CORNER*, TrackNodeItem::ROUNDED_TRACKS_CORNER*>
        Add( TRACK* aTrackTo, const unsigned int aLength, PICKED_ITEMS_LIST* aUndoRedoList );
    std::pair<TrackNodeItem::ROUNDED_TRACKS_CORNER*, TrackNodeItem::ROUNDED_TRACKS_CORNER*> Add( TRACK* aTrackTo );

    void Add( std::set<TRACK*>* aTracksAt, PICKED_ITEMS_LIST* aUndoRedoList );

    //All
    void Add( const DLIST<TRACK>* aTracksAt );
    void Add( const int aNetCodeTo, PICKED_ITEMS_LIST* aUndoRedoList );
    void Add( const int aNetCodeTo );

    void Remove( const TRACK* aTrackFrom, const bool aUndo, const bool aLockedToo );
    void Remove( const TRACK* aTrackFrom, PICKED_ITEMS_LIST* aUndoRedoList, const bool aLockedToo );
    void Remove( const TRACK* aTrackFrom, BOARD_COMMIT& aCommit, const bool aLockedToo );
    void Remove( const BOARD_ITEM* aItemFrom, PICKED_ITEMS_LIST* aUndoRedoList, const bool aLockedToo );

    void Remove( DLIST<TRACK>* aTracksAt ); //All
    void Remove( const int aNetCodeFrom, PICKED_ITEMS_LIST* aUndoRedoList, const bool aLockedToo );
    void Remove( const int aNetCodeFrom, const bool aUndo, const bool aLockedToo );

    void Change( const TRACK* aTrackFrom, const bool aUndo, const bool aLockedToo );

    void Recreate( const int aNetCodeTo, PICKED_ITEMS_LIST* aUndoRedoList );

    void Repopulate( const int aNetCodeTo, PICKED_ITEMS_LIST* aUndoRedoList );

    void Plot( const TrackNodeItem::TRACKNODEITEM* aTrackItem,
               PLOTTER* aPlotter,
               const EDA_DRAW_MODE_T* aPlotMode,
               void* aData
             ) override;

    //Save, Load
    void Format( OUTPUTFORMATTER* aOut, const int aNestLevel ) const override;
    TrackNodeItem::TRACKNODEITEM* Parse( PCB_PARSER* aParser ) override;

    void LoadDefaultParams( void );
    TrackNodeItem::ROUNDED_TRACKS_CORNER::PARAMS CopyCurrentParams( const TRACK* aTrackSegAt,
                                                                    const wxPoint& aCurPosAt
                                                                  );

    TrackNodeItem::TRACKNODEITEM* EndPosItem( const TRACK* aTrackSegAt ) const override;
    TrackNodeItem::TRACKNODEITEM* StartPosItem( const TRACK* aTrackSegAt ) const override;

protected:
    ROUNDED_TRACKS_CORNERS(){};
    void CreateMenu( wxMenu* aMenu ) const override;

private:
    //Creation
    TrackNodeItem::ROUNDED_TRACKS_CORNER* Create( const TRACK* aTrackTo,
                                                  const TRACK* aTrackSegSecond,
                                                  const wxPoint aPosition,
                                                  const bool aNullTrackCheck
                                                );
    //Removing
    void Delete( TrackNodeItem::ROUNDED_TRACKS_CORNER* aCorner,
                 DLIST<TRACK>*aTrackListAt,
                 PICKED_ITEMS_LIST* aUndoRedoList
               );

    //Private remove funcs.
    static const bool CAN_RECREATE = true;
    void Remove( TrackNodeItem::ROUNDED_TRACKS_CORNER* aCorner,
                 PICKED_ITEMS_LIST* aUndoRedoList,
                 const bool aSaveRemoved,
                 const bool aLockedToo
               );

    TRACK* FindSecondTrack( const TRACK* aTrackTo, wxPoint aPosition );

    //aTrack is converted to ROUNDED_CORNER_TRACK in m_board->m_Track
    ROUNDED_CORNER_TRACK* Convert( TRACK* aTrack, PICKED_ITEMS_LIST* aUndoRedoList ); //One Track
    void Convert( const int aNetCode, PICKED_ITEMS_LIST* aUndoRedoList ); //One Net
    void Convert( const DLIST<TRACK>* aTracksAt, PICKED_ITEMS_LIST* aUndoRedoList ); //All TRACKs in m_Tracks list.

    TrackNodeItem::ROUNDED_TRACKS_CORNER::PARAMS GetParams( void ) const { return m_params; }
    void SetParams( const TrackNodeItem::ROUNDED_TRACKS_CORNER::PARAMS aParams );
    TrackNodeItem::ROUNDED_TRACKS_CORNER::PARAMS GetDefaultParams( void ) const;
    TrackNodeItem::ROUNDED_TRACKS_CORNER::PARAMS m_params;

//-----------------------------------------------------------------------------------------------------/
// One track segment corner( s ) memory
//-----------------------------------------------------------------------------------------------------/
public:
    //Save
    void ToMemory( const TRACK* aTrackSegFrom );
    void FromMemory( const TRACK* aTrackTo, PICKED_ITEMS_LIST* aUndoRedoList );
    void FromMemory( const TRACK* aTrackTo );
    void FromMemory( const TRACK* aTrackTo, BOARD_COMMIT& aCommit );

private:
    TrackNodeItem::ROUNDED_TRACKS_CORNER* m_next_corner_in_memory;
    TrackNodeItem::ROUNDED_TRACKS_CORNER* m_back_corner_in_memory;
//-----------------------------------------------------------------------------------------------------/

//-----------------------------------------------------------------------------------------------------/
// Todo lists.
//-----------------------------------------------------------------------------------------------------/
public:
    using RoundedTracksCorner_Container = std::set<TrackNodeItem::ROUNDED_TRACKS_CORNER*>;
    using RoundedCornerTrack_Container = std::set<ROUNDED_CORNER_TRACK*>;
    void UpdateListClear( void );
    void UpdateListAdd( const TRACK* aTrackSegFrom );
    void UpdateListAdd( const BOARD_ITEM* aBoardItem );
    void UpdateListDo( void );
    void UpdateListDo( EDA_DRAW_PANEL* aPanel, wxDC* aDC, GR_DRAWMODE aDrawMode, bool aErase );
    void UpdateListDo_Route( EDA_DRAW_PANEL* aPanel, wxDC* aDC, bool aErase );
    void UpdateListDo_UndoRedo( void );
    void UpdateListDo_RemoveBroken( PICKED_ITEMS_LIST* aUndoRedoList );
    void UpdateListDo_BlockRotate( PICKED_ITEMS_LIST* aItemsList );
    void UpdateListDo_BlockDuplicate( const wxPoint aMoveVector, PICKED_ITEMS_LIST* aUndoRedoList );

    void UpdateList_DrawTracks( EDA_DRAW_PANEL* aPanel, wxDC* aDC, GR_DRAWMODE aDrawMode );
    void UpdateList_DrawTracks_Route( EDA_DRAW_PANEL* aPanel, wxDC* aDC, const bool aOnlyChanged );
    RoundedCornerTrack_Container* UpdateList_GetUpdatedTracks( void ) const { return m_update_tracks_list; }

private:
    RoundedTracksCorner_Container* m_update_list;
    RoundedCornerTrack_Container* m_update_tracks_list;
    RoundedTracksCorner_Container* m_recreate_list;
    void UpdateListAdd( const TrackNodeItem::ROUNDED_TRACKS_CORNER* aCorner );
//-----------------------------------------------------------------------------------------------------/

//-----------------------------------------------------------------------------------------------------/
// Update
//-----------------------------------------------------------------------------------------------------/
public:
    void Update( const BOARD_ITEM* aItemAt );
    void Update( const int aNetCode, const TRACK* aStartTrack );
    void Update( const TRACK* aTrackSegAt );
    void Update( TRACK* aTrackSegAt,
                 EDA_DRAW_PANEL* aPanel,
                 wxDC* aDC,
                 GR_DRAWMODE aDrawMode,
                 bool aErase
               );

private:
    void Update( TrackNodeItem::ROUNDED_TRACKS_CORNER* aCorner,
                 EDA_DRAW_PANEL* aPanel,
                 wxDC* aDC, GR_DRAWMODE aDrawMode,
                 bool aErase
               );
//-----------------------------------------------------------------------------------------------------/

//-----------------------------------------------------------------------------------------------------/
// Track Routing ( legacy canvas ) Edit corner
//-----------------------------------------------------------------------------------------------------/
public:
    enum TO_EDIT_T
    {
        EDIT_NULL_T = 0,
        EDIT_LENGTH_RATIO_T,
        EDIT_LENGTH_SET_T,
    };
    void RouteCreate_Start( void );
    void RouteCreate_Stop( void );
    bool CanEdit( void ) const { return m_can_edit;}
    void ToggleEdit( const TO_EDIT_T aEdit );
    bool IsEditOn( void ) const { return ( bool )m_to_edit; }
    TrackNodeItem::ROUNDED_TRACKS_CORNER* UpdateRouteEdit( EDA_DRAW_PANEL* aPanel,
                                                         wxDC* aDC,
                                                         const TRACK* aTrack,
                                                         const TRACK* aTrackSecond,
                                                         const bool aErase,
                                                         bool* aTrack45Only
                                                       );
    void DestroyRouteEdit( void );
    TrackNodeItem::ROUNDED_TRACKS_CORNER* GetEditCorner( void ) const { return m_track_edit_corner; }

private:
    TrackNodeItem::ROUNDED_TRACKS_CORNER* m_track_edit_corner{nullptr};
    TO_EDIT_T m_to_edit;
    bool m_can_edit;
    TrackNodeItem::ROUNDED_TRACKS_CORNER::PARAMS m_edit_params;
    bool m_editparams_drawn{false};
    wxPoint m_edit_start_point;
    bool m_track45Only_before_edit;
    bool* m_track45Only;
    void DrawEditParams( EDA_DRAW_PANEL* aPanel, wxDC* aDC, const wxPoint& aOffset ) const;
//-----------------------------------------------------------------------------------------------------/

//-----------------------------------------------------------------------------------------------------/
//DRC, return false on error.
//-----------------------------------------------------------------------------------------------------/
public:
    void AddToDragList( const TRACK* aTrackFrom, std::vector<DRAG_SEGM_PICKER>& aDragSegList );
    bool DRC_Clearance( const BOARD_CONNECTED_ITEM* aRefFirst,
                        const BOARD_CONNECTED_ITEM* aRefSecond,
                        const int aMinDist, DRC* aDRC
                      );
    bool DRC_Clearance( const BOARD_CONNECTED_ITEM* aRef, const D_PAD* aPad, const int aMinDist, DRC* aDRC );
    void DRC_Clearance( const BOARD_CONNECTED_ITEM* aRef, const TEXTE_PCB* aText, const int aMinDist, DRC* aDRC );

private:
    bool DRC_DoClearanceTest( const TrackNodeItem::ROUNDED_TRACKS_CORNER* aCorner,
                              const wxPoint aTestPoint,
                              const int aMinDist
                            );
    bool DRC_DoClearanceTest( const TrackNodeItem::ROUNDED_TRACKS_CORNER* aCorner,
                              const D_PAD* aPad,
                              const int aMinDist,
                              DRC* aDRC
                            );
    bool DRC_ClearanceTest( const TrackNodeItem::ROUNDED_TRACKS_CORNER* aCorner,
                            const VIA* aVia,
                            const int aMinDist
                          );
    bool DRC_ClearanceTest( const TrackNodeItem::ROUNDED_TRACKS_CORNER* aCorner,
                            const TRACK* aTrackSeg,
                            const int aMinDist
                          );
    bool DRC_ClearanceTest( const TrackNodeItem::ROUNDED_TRACKS_CORNER* aCornerFirst,
                            TrackNodeItem::ROUNDED_TRACKS_CORNER* aCornerSecond,
                            const int aMinDist
                          );
    bool DRC_TestClearance( const TrackNodeItem::ROUNDED_TRACKS_CORNER* aCorner,
                            const TRACK* aTrackSeg,
                            const int aMinDist,
                            DRC* aDRC
                          );

//-----------------------------------------------------------------------------------------------------/


//-----------------------------------------------------------------------------------------------------/
// Menus
//-----------------------------------------------------------------------------------------------------/
public:
    void Popup( wxMenu* aMenu, const TRACK* aTrackSeg, const wxPoint& aPos ) const;
    void MenuToDo_ChangeSize( const int aMenuID );

private:
    void Menu_AddToTrack( wxMenu* aMenu, const TRACK* aTrackSeg, const wxPoint& aPos ) const;
    void Menu_RemoveFromTrack( wxMenu* aMenu, const TRACK* aTrackSeg, const wxPoint& aPos ) const;
    void Menu_ChangeFromTrack( wxMenu* aMenu, const TRACK* aTrackSeg, const wxPoint& aPos ) const;
    void Menu_CopyParamsToCurrent( wxMenu* aMenu, const TRACK* aTrackSeg, const wxPoint& aPos ) const;
    void Menu_AddToNet( wxMenu* aMenu, const TRACK* aTrackSeg, const wxPoint& aPos ) const;
    void Menu_RemoveFromNet( wxMenu* aMenu, const TRACK* aTrackSeg, const wxPoint& aPos ) const;
    void Menu_ConvertSegmentedCorner( wxMenu* aMenu, const TRACK* aTrackSeg, const wxPoint& aPos ) const;
    void Menu_ConvertSegmentedCornersNet( wxMenu* aMenu, const TRACK* aTrackSeg, const wxPoint& aPos ) const;

    wxString ParamsTxtToMenu( const TrackNodeItem::ROUNDED_TRACKS_CORNER::PARAMS aParams ) const;
    inline int MenuToDo_CalcSizeLengthSet( const int aMenuID );
    inline int MenuToDo_CalcSizeLengthRatio( const int aMenuID );
    void Menu_ChangeSize( wxMenu* aMenu ) const;
//-----------------------------------------------------------------------------------------------------/

//-----------------------------------------------------------------------------------------------------/
// Convert segmented arced corners to rounded corner.
//-----------------------------------------------------------------------------------------------------/
public:
    void ConvertSegmentedCorners( TRACK* aTrackFrom, const bool aUndo );
    bool ConvertSegmentedCorners( TRACK* aTrackFrom, PICKED_ITEMS_LIST* aUndoRedoList );
    void ConvertSegmentedCorners( const int aNetCode, const bool aUndo );

private:
    void RemoveArcedSegments( std::set<TRACK*>* aTracksArced, PICKED_ITEMS_LIST* aUndoRedoList );
    std::set<TRACK*> CollectSameLengthConnected( const TRACK* aTrackToConnect,
                                                 const std::set<TRACK*>* aTracksSameLength
                                               );
    std::map<TRACK*, bool> FindSegmentsBothSidesOfArced( const std::set<TRACK*>* aTracksArced,
                                                         const std::set<TRACK*>* aTracksOthers
                                                       );
    bool CreateCorner( std::set<TRACK*>* aTracksArced,
                       const std::map<TRACK*,
                       bool>* aBothSidesTracks,
                       PICKED_ITEMS_LIST* aUndoRedoList
                     );

    class NET_SCAN_TRACK_COLLECT_SAMELENGTH : public NET_SCAN_BASE
    {
    public:
        NET_SCAN_TRACK_COLLECT_SAMELENGTH( const TRACK* aTrackSeg,
                                           const ROUNDED_TRACKS_CORNERS* aParent,
                                           PICKED_ITEMS_LIST* aUndoRedoList
                                         );
        ~NET_SCAN_TRACK_COLLECT_SAMELENGTH() {};

        std::set<TRACK*>* GetSamelengthSegments( void ) { return &m_samelength_segments; }
        std::set<TRACK*>* GetAnotherSegments( void ) { return &m_another_segments; }

    protected:
        bool ExecuteAt( TRACK* aTrack ) override;
        PICKED_ITEMS_LIST* m_picked_items{nullptr};

    private:
        unsigned int m_track_length{0};
        std::set<TRACK*> m_samelength_segments;
        std::set<TRACK*> m_another_segments;
        static constexpr double ROUND_DIVIDER = 100.0;
    };

//-----------------------------------------------------------------------------------------------------/

//-----------------------------------------------------------------------------------------------------/
// NETSCAN OPERATIONS
//-----------------------------------------------------------------------------------------------------/
private:

    class NET_SCAN_NET_ADD : public NET_SCAN_BASE
    {
    public:
        NET_SCAN_NET_ADD( const int aNet,
                          const ROUNDED_TRACKS_CORNERS* aParent,
                          PICKED_ITEMS_LIST* aUndoRedoList
                        );
        ~NET_SCAN_NET_ADD() {};

    protected:
        bool ExecuteAt( TRACK* aTrack ) override;

        PICKED_ITEMS_LIST* m_picked_items {nullptr};
    };

    class NET_SCAN_NET_CONVERT : virtual public NET_SCAN_NET_ADD
    {
    public:
        NET_SCAN_NET_CONVERT( const int aNet,
                              const ROUNDED_TRACKS_CORNERS* aParent,
                              PICKED_ITEMS_LIST* aUndoRedoList
                            );
        ~NET_SCAN_NET_CONVERT() {};

    protected:
        bool ExecuteAt( TRACK* aTrack ) override;
    };

    class NET_SCAN_NET_REMOVE : public NET_SCAN_BASE
    {
    public:
        NET_SCAN_NET_REMOVE( const int aNet,
                             const ROUNDED_TRACKS_CORNERS* aParent,
                             PICKED_ITEMS_LIST* aUndoRedoList,
                             RoundedTracksCorner_Container* aRecreateList,
                             const bool aLockedToo
                           );
        ~NET_SCAN_NET_REMOVE() {};

    protected:
        bool ExecuteAt( TRACK* aTrack ) override;

        PICKED_ITEMS_LIST* m_picked_items {nullptr};
        RoundedTracksCorner_Container* m_recreate_list {nullptr};
        bool m_locked_too {false};
    };

    class NET_SCAN_NET_RECREATE : public NET_SCAN_NET_REMOVE
    {
    public:
        NET_SCAN_NET_RECREATE( const int aNet,
                               const ROUNDED_TRACKS_CORNERS* aParent,
                               PICKED_ITEMS_LIST* aUndoRedoList,
                               RoundedTracksCorner_Container* aRecreateList
                             );
        ~NET_SCAN_NET_RECREATE();

    protected:
        bool ExecuteAt( TRACK* aTrack ) override;

    private:
        TrackNodeItem::ROUNDED_TRACKS_CORNER::PARAMS m_current_params;
    };

    class NET_SCAN_TRACK_UPDATE : public NET_SCAN_BASE
    {
    public:
        NET_SCAN_TRACK_UPDATE( const TRACK* aTrackSeg, const ROUNDED_TRACKS_CORNERS* aParent );
        ~NET_SCAN_TRACK_UPDATE() {};

    protected:
        bool ExecuteAt( TRACK* aTrack ) override;
    };

//-----------------------------------------------------------------------------------------------------/
// PROGRESS OPERATIONS
//-----------------------------------------------------------------------------------------------------/
private:

    class ROUNDEDTRACKSCORNERS_PROGRESS : public TrackNodeItems::TRACKS_PROGRESS
    {
    protected:
        ROUNDEDTRACKSCORNERS_PROGRESS( const ROUNDED_TRACKS_CORNERS* aParent,
                                       const DLIST<TRACK>* aTracks,
                                       PICKED_ITEMS_LIST* aUndoRedoList
                                     ) :
            TRACKS_PROGRESS( aParent->GetEditFrame(), aTracks, aUndoRedoList )
            {
                m_parent = const_cast<ROUNDED_TRACKS_CORNERS*>( aParent );
            }

        ROUNDED_TRACKS_CORNERS* m_parent{nullptr};

    };

    class ROUNDEDTRACKSCORNERS_PROGRESS_ADD_CORNERS : public ROUNDEDTRACKSCORNERS_PROGRESS
    {
    public:
        ROUNDEDTRACKSCORNERS_PROGRESS_ADD_CORNERS( const ROUNDED_TRACKS_CORNERS* aParent,
                                                   const DLIST<TRACK>* aTracks,
                                                   PICKED_ITEMS_LIST* aUndoRedoList
                                                 );
    protected:
        unsigned int ExecuteItem( const BOARD_ITEM* aItemAt ) override;
    };

    //Convert all TRACKs to ROUNDEDCORNERTRACKs.
    class ROUNDEDTRACKSCORNERS_PROGRESS_CONVERT_TRACKS : public ROUNDEDTRACKSCORNERS_PROGRESS
    {
    public:
        ROUNDEDTRACKSCORNERS_PROGRESS_CONVERT_TRACKS( const ROUNDED_TRACKS_CORNERS* aParent,
                                                      const DLIST<TRACK>* aTracks,
                                                      PICKED_ITEMS_LIST* aUndoRedoList
                                                    );
    protected:
        unsigned int ExecuteItem( const BOARD_ITEM* aItemAt ) override;
    };

    class ROUNDEDTRACKSCORNERS_PROGRESS_REMOVE_CORNERS : virtual public ROUNDEDTRACKSCORNERS_PROGRESS
    {
    public:
        ROUNDEDTRACKSCORNERS_PROGRESS_REMOVE_CORNERS( const ROUNDED_TRACKS_CORNERS* aParent,
                                                      const DLIST<TRACK>* aTracks,
                                                      PICKED_ITEMS_LIST* aUndoRedoList
                                                    );
        ~ROUNDEDTRACKSCORNERS_PROGRESS_REMOVE_CORNERS();

    protected:
        unsigned int ExecuteItem( const BOARD_ITEM* aItemAt ) override;
        void ExecuteEnd( void ) override;
        RoundedTracksCorner_Container* m_remove_corners {nullptr};

    private:
        DLIST<TRACK>* m_tracks {nullptr};
    };

//-----------------------------------------------------------------------------------------------------/
//Clean
//-----------------------------------------------------------------------------------------------------/
public:
    bool Clean( const DLIST<TRACK>* aTracksAt, BOARD_COMMIT& aCommit );

private:
    class ROUNDEDTRACKSCORNERS_PROGRESS_CLEAN : public ROUNDEDTRACKSCORNERS_PROGRESS_REMOVE_CORNERS
    {
    public:
        ROUNDEDTRACKSCORNERS_PROGRESS_CLEAN( const ROUNDED_TRACKS_CORNERS* aParent,
                                             const DLIST<TRACK>* aTracks,
                                             PICKED_ITEMS_LIST* aUndoRedoList
                                           );
    protected:
        unsigned int ExecuteItem( const BOARD_ITEM* aItemAt ) override;
    };
//-----------------------------------------------------------------------------------------------------/

};

#endif //ROUNDED_TRACKS_CORNERS_H
