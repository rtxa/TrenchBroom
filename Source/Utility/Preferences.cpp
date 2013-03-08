/*
 Copyright (C) 2010-2012 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Preferences.h"

#include "View/CommandIds.h"

using namespace TrenchBroom::View::CommandIds;

namespace TrenchBroom {
    namespace Preferences {

        const Preference<float> CameraLookSpeed = Preference<float>(                            "Controls/Camera/Look speed",                                   0.5f);
        const Preference<float> CameraPanSpeed = Preference<float>(                             "Controls/Camera/Pan speed",                                    0.5f);
        const Preference<bool>  CameraLookInvertX = Preference<bool>(                           "Controls/Camera/Look X inverted",                              false);
        const Preference<bool>  CameraLookInvertY = Preference<bool>(                           "Controls/Camera/Look Y inverted",                              false);
        const Preference<bool>  CameraPanInvertX = Preference<bool>(                            "Controls/Camera/Pan X inverted",                               false);
        const Preference<bool>  CameraPanInvertY = Preference<bool>(                            "Controls/Camera/Pan Y inverted",                               false);
        const Preference<float> HandleRadius = Preference<float>(                               "Controls/Vertex handle radius",                                3.0f);
        const Preference<float> MaximumHandleDistance = Preference<float>(                      "Controls/Maximum handle distance",                             1000.0f);
        const Preference<float> HandleScalingFactor = Preference<float>(                        "Controls/Handle scaling factor",                               1.0f / 300.0f);
        const Preference<float> MaximumNearFaceDistance = Preference<float>(                    "Controls/Maximum near face distance",                          8.0f);
        const Preference<float> CameraFieldOfVision = Preference<float>(                        "Renderer/Camera field of vision",                              90.0f);
        const Preference<float> CameraNearPlane = Preference<float>(                            "Renderer/Camera near plane",                                   1.0f);
        const Preference<float> CameraFarPlane = Preference<float>(                             "Renderer/Camera far plane",                                    5000.0f);

        const Preference<float> InfoOverlayFadeDistance = Preference<float>(                    "Renderer/Info overlay fade distance",                          400.0f);
        const Preference<float> SelectedInfoOverlayFadeDistance = Preference<float>(            "Renderer/Selected info overlay fade distance",                 400.0f);
        const Preference<int>   RendererFontSize = Preference<int>(                             "Renderer/Font size",                                           9);
        const Preference<float> RendererBrightness = Preference<float>(                         "Renderer/Brightness",                                          1.0f);
        const Preference<float> GridAlpha = Preference<float>(                                  "Renderer/Grid Alpha",                                          0.25f);
        const Preference<bool>  GridCheckerboard = Preference<bool>(                            "Renderer/Grid Checkerboard",                                   false);

        const Preference<Color> EntityRotationDecoratorColor = Preference<Color>(               "Renderer/Colors/Decorators/Entity rotation",                   Color(1.0f,  1.0f,  1.0f,  1.0f ));
        
        const Preference<Color> BackgroundColor = Preference<Color>(                            "Renderer/Colors/Background",                                   Color(0.0f,  0.0f,  0.0f,  1.0f ));

        const Preference<Color> GuideColor = Preference<Color>(                                 "Renderer/Colors/Guide",                                        Color(1.0f,  0.0f,  0.0f,  0.3f ));
        const Preference<Color> HoveredGuideColor = Preference<Color>(                          "Renderer/Colors/Hovered guide",                                Color(1.0f,  1.0f,  1.0f,  0.5f ));

        const Preference<Color> FaceColor = Preference<Color>(                                  "Renderer/Colors/Face",                                         Color(0.2f,  0.2f,  0.2f,  1.0f ));
        const Preference<Color> SelectedFaceColor = Preference<Color>(                          "Renderer/Colors/Selected face",                                Color(0.6f,  0.35f, 0.35f, 1.0f ));
        const Preference<Color> LockedFaceColor = Preference<Color>(                            "Renderer/Colors/Locked face",                                  Color(0.35f, 0.35f, 0.6f,  1.0f ));
        const Preference<Color> ClippedFaceColor = Preference<Color>(                           "Renderer/Colors/Clipped face",                                 Color(0.6f,  0.3f,  0.0f,  1.0f ));

        const Preference<Color> EdgeColor = Preference<Color>(                                  "Renderer/Colors/Edge",                                         Color(0.7f,  0.7f,  0.7f,  1.0f ));
        const Preference<Color> SelectedEdgeColor = Preference<Color>(                          "Renderer/Colors/Selected edge",                                Color(1.0f,  0.0f,  0.0f,  1.0f ));
        const Preference<Color> OccludedSelectedEdgeColor = Preference<Color>(                  "Renderer/Colors/Occluded selected edge",                       Color(1.0f,  0.0f,  0.0f,  0.5f ));
        const Preference<Color> LockedEdgeColor = Preference<Color>(                            "Renderer/Colors/Locked edge",                                  Color(0.13f, 0.3f,  1.0f,  1.0f ));
        const Preference<Color> ClippedEdgeColor = Preference<Color>(                           "Renderer/Colors/Clipped edge",                                 Color(1.0f,  0.5f,  0.0f,  1.0f ));
        const Preference<Color> OccludedClippedEdgeColor = Preference<Color>(                   "Renderer/Colors/Occluded clipped edge",                        Color(1.0f,  0.5f,  0.0f,  0.5f ));

        const Preference<Color> SelectedEntityColor = Preference<Color>(                        "Renderer/Colors/Selected entity",                              Color(0.6f,  0.35f, 0.35f, 1.0f ));
        const Preference<Color> EntityBoundsColor = Preference<Color>(                          "Renderer/Colors/Entity bounds",                                Color(0.5f,  0.5f,  0.5f,  1.0f ));
        const Preference<Color> SelectedEntityBoundsColor = Preference<Color>(                  "Renderer/Colors/Selected entity bounds",                       Color(1.0f,  0.0f,  0.0f,  1.0f ));
        const Preference<Color> OccludedSelectedEntityBoundsColor = Preference<Color>(          "Renderer/Colors/Occluded selected entity bounds",              Color(1.0f,  0.0f,  0.0f,  0.5f ));
        const Preference<Color> LockedEntityColor = Preference<Color>(                          "Renderer/Colors/Locked entity",                                Color(0.35f, 0.35f, 0.6f,  1.0f ));
        const Preference<Color> LockedEntityBoundsColor = Preference<Color>(                    "Renderer/Colors/Locked entity bounds",                         Color(0.13f, 0.3f,  1.0f,  1.0f ));
        const Preference<Color> EntityBoundsWireframeColor = Preference<Color>(                 "Renderer/Colors/Entity bounds (wireframe mode)",               Color(0.13f, 0.3f,  1.0f,  1.0f ));

        const Preference<Color> SelectionGuideColor = Preference<Color>(                        "Renderer/Colors/Selection guide",                              Color(1.0f,  0.0f,  0.0f,  1.0f ));
        const Preference<Color> OccludedSelectionGuideColor = Preference<Color>(                "Renderer/Colors/Occluded selection guide",                     Color(1.0f,  0.0f,  0.0f,  0.5f ));

        const Preference<Color> InfoOverlayTextColor = Preference<Color>(                       "Renderer/Colors/Info overlay text",                            Color(1.0f,  1.0f,  1.0f,  1.0f ));
        const Preference<Color> InfoOverlayBackgroundColor = Preference<Color>(                 "Renderer/Colors/Info overlay background",                      Color(0.0f,  0.0f,  0.0f,  0.6f ));
        const Preference<Color> OccludedInfoOverlayTextColor = Preference<Color>(               "Renderer/Colors/Occluded info overlay text",                   Color(1.0f,  1.0f,  1.0f,  0.5f ));
        const Preference<Color> OccludedInfoOverlayBackgroundColor = Preference<Color>(         "Renderer/Colors/Occluded info overlay background",             Color(0.0f,  0.0f,  0.0f,  0.3f ));
        const Preference<Color> SelectedInfoOverlayTextColor = Preference<Color>(               "Renderer/Colors/Selected info overlay text",                   Color(1.0f,  1.0f,  1.0f,  1.0f ));
        const Preference<Color> SelectedInfoOverlayBackgroundColor = Preference<Color>(         "Renderer/Colors/Selected info overlay backtround",             Color(1.0f,  0.0f,  0.0f,  0.6f ));
        const Preference<Color> OccludedSelectedInfoOverlayTextColor = Preference<Color>(       "Renderer/Colors/Occluded selected info overlay text",          Color(1.0f,  1.0f,  1.0f,  0.5f ));
        const Preference<Color> OccludedSelectedInfoOverlayBackgroundColor = Preference<Color>( "Renderer/Colors/Occluded selected info overlay background",    Color(1.0f,  0.0f,  0.0f,  0.3f ));
        const Preference<Color> LockedInfoOverlayTextColor = Preference<Color>(                 "Renderer/Colors/Locked info overlay text",                     Color(1.0f,  1.0f,  1.0f,  1.0f ));
        const Preference<Color> LockedInfoOverlayBackgroundColor = Preference<Color>(           "Renderer/Colors/Locked info overlay background",               Color(0.13f, 0.3f,  1.0f,  0.6f ));

        const Preference<Color> VertexHandleColor = Preference<Color>(                          "Renderer/Colors/Vertex handle",                                Color(1.0f,  1.0f,  1.0f,  1.0f ));
        const Preference<Color> OccludedVertexHandleColor = Preference<Color>(                  "Renderer/Colors/Occluded vertex handle",                       Color(1.0f,  1.0f,  1.0f,  0.5f ));
        const Preference<Color> SelectedVertexHandleColor = Preference<Color>(                  "Renderer/Colors/Selected vertex handle",                       Color(1.0f,  0.0f,  0.0f,  1.0f ));
        const Preference<Color> OccludedSelectedVertexHandleColor = Preference<Color>(          "Renderer/Colors/Occluded selected vertex handle",              Color(1.0f,  0.0f,  0.0f,  0.5f ));

        const Preference<Color> SplitHandleColor = Preference<Color>(                           "Renderer/Colors/Split handle",                                 Color(1.0f,  1.0f,  1.0f,  1.0f ));
        const Preference<Color> OccludedSplitHandleColor = Preference<Color>(                   "Renderer/Colors/Occluded split handle",                        Color(1.0f,  1.0f,  1.0f,  0.5f ));
        const Preference<Color> SelectedSplitHandleColor = Preference<Color>(                   "Renderer/Colors/Selected split handle",                        Color(1.0f,  0.0f,  0.0f,  1.0f ));
        const Preference<Color> OccludedSelectedSplitHandleColor = Preference<Color>(           "Renderer/Colors/Occluded selected split handle",               Color(1.0f,  0.0f,  0.0f,  0.5f ));

        const Preference<Color> EdgeHandleColor = Preference<Color>(                            "Renderer/Colors/Edge handle",                                  Color(1.0f,  1.0f,  1.0f,  1.0f ));
        const Preference<Color> OccludedEdgeHandleColor = Preference<Color>(                    "Renderer/Colors/Occluded edge handle",                         Color(1.0f,  1.0f,  1.0f,  0.5f ));
        const Preference<Color> SelectedEdgeHandleColor = Preference<Color>(                    "Renderer/Colors/Selected edge handle",                         Color(1.0f,  0.0f,  0.0f,  1.0f ));
        const Preference<Color> OccludedSelectedEdgeHandleColor = Preference<Color>(            "Renderer/Colors/Occluded selected edge handle",                Color(1.0f,  0.0f,  0.0f,  0.5f ));

        const Preference<Color> FaceHandleColor = Preference<Color>(                            "Renderer/Colors/Face handle",                                  Color(1.0f,  1.0f,  1.0f,  1.0f ));
        const Preference<Color> OccludedFaceHandleColor = Preference<Color>(                    "Renderer/Colors/Occluded face handle",                         Color(1.0f,  1.0f,  1.0f,  0.5f ));
        const Preference<Color> SelectedFaceHandleColor = Preference<Color>(                    "Renderer/Colors/Selected face handle",                         Color(1.0f,  0.0f,  0.0f,  1.0f ));
        const Preference<Color> OccludedSelectedFaceHandleColor = Preference<Color>(            "Renderer/Colors/Occluded selected face handle",                Color(1.0f,  0.0f,  0.0f,  0.5f ));

        const Preference<Color> ClipHandleColor = Preference<Color>(                            "Renderer/Colors/Clip handle",                                  Color(1.0f,  1.0f,  1.0f,  1.0f ));
        const Preference<Color> OccludedClipHandleColor = Preference<Color>(                    "Renderer/Colors/Occluded clip handle",                         Color(1.0f,  1.0f,  1.0f,  0.5f ));
        const Preference<Color> SelectedClipHandleColor = Preference<Color>(                    "Renderer/Colors/Selected clip handle",                         Color(1.0f,  0.0f,  0.0f,  1.0f ));
        const Preference<Color> ClipPlaneColor = Preference<Color>(                             "Renderer/Colors/Clip plane",                                   Color(1.0f,  1.0f,  1.0f,  0.25f ));

        const Preference<Color> ResizeBrushFaceColor = Preference<Color>(                        "Renderer/Colors/Face color when resizing",                    Color(1.0f,  1.0f,  1.0f,  1.0f ));
        const Preference<Color> OccludedResizeBrushFaceColor = Preference<Color>(                "Renderer/Colors/Occluded face color when resizing",           Color(1.0f,  1.0f,  1.0f,  0.5f ));

        const Preference<Color> BrowserTextureColor = Preference<Color>(                        "Texture browser/Texture color",                                Color(1.0f,  1.0f,  1.0f,  1.0f ));
        const Preference<Color> SelectedTextureColor = Preference<Color>(                       "Texture browser/Selected texture color",                       Color(0.8f,  0.0f,  0.0f,  1.0f ));
        const Preference<Color> UsedTextureColor = Preference<Color>(                           "Texture browser/Used texture color",                           Color(0.8f,  0.8f,  0.0f,  1.0f ));
        const Preference<Color> OverriddenTextureColor = Preference<Color>(                     "Texture browser/Overridden texture color",                     Color(0.5f,  0.5f,  0.5f,  1.0f ));
        const Preference<Color> BrowserGroupBackgroundColor = Preference<Color>(                "Texture browser/Group background color",                       Color(0.5f,  0.5f,  0.5f,  1.0f ));
        const Preference<Color> BrowserGroupTextColor = Preference<Color>(                      "Texture browser/Group text color",                             Color(1.0f,  1.0f,  1.0f,  1.0f ));
        const Preference<int>   TextureBrowserFontSize = Preference<int>(                       "Texture browser/Font size",                                    12);
        const Preference<int>   EntityBrowserFontSize = Preference<int>(                        "Entity browser/Font size",                                     12);

#if defined _WIN32
        const Preference<float> CameraMoveSpeed = Preference<float>(                            "Controls/Camera/Move speed",                                   0.3f);
        const Preference<String> QuakePath = Preference<String>(                                "General/Quake path",                                           "C:\\Program Files\\Quake");
        const Preference<String> RendererFontName = Preference<String>(                         "Renderer/Font name",                                           "Arial");
#elif defined __APPLE__
        const Preference<float> CameraMoveSpeed = Preference<float>(                            "Controls/Camera/Move speed",                                   0.3f);
        const Preference<String> QuakePath = Preference<String>(                                "General/Quake path",                                           "/Applications/Quake");
        const Preference<String> RendererFontName = Preference<String>(                         "Renderer/Font name",                                           "LucidaGrande");
#elif defined __linux__
        const Preference<float> CameraMoveSpeed = Preference<float>(                            "Controls/Camera/Move speed",                                   0.5f);
        const Preference<String> QuakePath = Preference<String>(                                "General/Quake path",                                           "/Quake");
        const Preference<String> RendererFontName = Preference<String>(                         "Renderer/Font name",                                           "Arial");
#endif

        const Preference<int>   RendererInstancingMode = Preference<int>(                       "Renderer/Instancing mode",                                     0);
        const int               RendererInstancingModeAutodetect    = 0;
        const int               RendererInstancingModeForceOn       = 1;
        const int               RendererInstancingModeForceOff      = 2;
        
        const Preference<KeyboardShortcut>  FileNew = Preference<KeyboardShortcut>(                             "Menu/File/New",                                        KeyboardShortcut(wxID_NEW, WXK_CONTROL, 'N', "New"));
        const Preference<KeyboardShortcut>  FileOpen = Preference<KeyboardShortcut>(                            "Menu/File/Open",                                       KeyboardShortcut(wxID_OPEN, WXK_CONTROL, 'O', "Open..."));
        const Preference<KeyboardShortcut>  FileSave = Preference<KeyboardShortcut>(                            "Menu/File/Save",                                       KeyboardShortcut(wxID_SAVE, WXK_CONTROL, 'S', "Save"));
        const Preference<KeyboardShortcut>  FileSaveAs = Preference<KeyboardShortcut>(                          "Menu/File/Save as",                                    KeyboardShortcut(wxID_SAVEAS, WXK_CONTROL, WXK_SHIFT, 'S', "Save as..."));
        const Preference<KeyboardShortcut>  FileLoadPointFile = Preference<KeyboardShortcut>(                   "Menu/File/Load Point File",                            KeyboardShortcut(Menu::FileLoadPointFile, "Load Point File..."));
        const Preference<KeyboardShortcut>  FileUnloadPointFile = Preference<KeyboardShortcut>(                 "Menu/File/Unload Point File",                          KeyboardShortcut(Menu::FileUnloadPointFile, "Unload Point File"));
        const Preference<KeyboardShortcut>  FileClose = Preference<KeyboardShortcut>(                           "Menu/File/Close",                                      KeyboardShortcut(wxID_CLOSE, WXK_CONTROL, 'W', "Close"));

        const Preference<KeyboardShortcut>  EditUndo = Preference<KeyboardShortcut>(                            "Menu/File/Undo",                                       KeyboardShortcut(wxID_UNDO, WXK_CONTROL, 'Z', "Undo"));
        const Preference<KeyboardShortcut>  EditRedo = Preference<KeyboardShortcut>(                            "Menu/File/Redo",                                       KeyboardShortcut(wxID_REDO, WXK_CONTROL, WXK_SHIFT, 'Z', "Redo"));
        const Preference<KeyboardShortcut>  EditCut = Preference<KeyboardShortcut>(                             "Menu/File/Cut",                                        KeyboardShortcut(wxID_CUT, WXK_CONTROL, 'X', "Cut"));
        const Preference<KeyboardShortcut>  EditCopy = Preference<KeyboardShortcut>(                            "Menu/File/Copy",                                       KeyboardShortcut(wxID_COPY, WXK_CONTROL, 'C', "Copy"));
        const Preference<KeyboardShortcut>  EditPaste = Preference<KeyboardShortcut>(                           "Menu/File/Paste",                                      KeyboardShortcut(wxID_PASTE, WXK_CONTROL, 'V', "Paste"));
        const Preference<KeyboardShortcut>  EditPasteAtOriginalPosition = Preference<KeyboardShortcut>(         "Menu/File/Paste at Original Position",                 KeyboardShortcut(Menu::EditPasteAtOriginalPosition, WXK_CONTROL, WXK_SHIFT, 'V', "Paste at Original Position"));
        const Preference<KeyboardShortcut>  EditDelete = Preference<KeyboardShortcut>(                          "Menu/File/Delete",                                     KeyboardShortcut(wxID_DELETE, WXK_BACK, "Delete"));

        const Preference<KeyboardShortcut>  EditSelectAll = Preference<KeyboardShortcut>(                       "Menu/Edit/Select All",                                 KeyboardShortcut(Menu::EditSelectAll, WXK_CONTROL, 'A', "Select All"));
        const Preference<KeyboardShortcut>  EditSelectSiblings = Preference<KeyboardShortcut>(                  "Menu/Edit/Select Siblings",                            KeyboardShortcut(Menu::EditSelectSiblings, WXK_CONTROL, WXK_ALT, 'A', "Select Siblings"));
        const Preference<KeyboardShortcut>  EditSelectTouching = Preference<KeyboardShortcut>(                  "Menu/Edit/Select Touching",                            KeyboardShortcut(Menu::EditSelectTouching, WXK_CONTROL, 'T', "Select Touching"));
        const Preference<KeyboardShortcut>  EditSelectNone = Preference<KeyboardShortcut>(                      "Menu/Edit/Select None",                                KeyboardShortcut(Menu::EditSelectTouching, WXK_CONTROL, WXK_SHIFT, 'A', "Select None"));
        const Preference<KeyboardShortcut>  EditHideSelected = Preference<KeyboardShortcut>(                    "Menu/Edit/Hide Selected",                              KeyboardShortcut(Menu::EditHideSelected, WXK_CONTROL, 'H', "Hide Selected"));
        const Preference<KeyboardShortcut>  EditHideUnselected = Preference<KeyboardShortcut>(                  "Menu/Edit/Hide Unselected",                            KeyboardShortcut(Menu::EditHideUnselected, WXK_CONTROL, WXK_ALT, 'H', "Hide Unselected"));
        const Preference<KeyboardShortcut>  EditUnhideAll = Preference<KeyboardShortcut>(                       "Menu/Edit/Unhide All",                                 KeyboardShortcut(Menu::EditUnhideAll, WXK_CONTROL, WXK_SHIFT, 'H', "Unhide All"));
        const Preference<KeyboardShortcut>  EditLockSelected = Preference<KeyboardShortcut>(                    "Menu/Edit/Lock Selected",                              KeyboardShortcut(Menu::EditLockSelected, WXK_CONTROL, 'L', "Lock Selected"));
        const Preference<KeyboardShortcut>  EditLockUnselected = Preference<KeyboardShortcut>(                  "Menu/Edit/Lock Unselected",                            KeyboardShortcut(Menu::EditLockUnselected, WXK_CONTROL, WXK_ALT, 'L', "Lock Unselected"));
        const Preference<KeyboardShortcut>  EditUnlockAll = Preference<KeyboardShortcut>(                       "Menu/Edit/Unlock All",                                 KeyboardShortcut(Menu::EditUnlockAll, WXK_CONTROL, WXK_SHIFT, 'L', "Unlock All"));
        const Preference<KeyboardShortcut>  EditToggleTextureLock = Preference<KeyboardShortcut>(               "Menu/Edit/Toggle Texture Lock",                        KeyboardShortcut(Menu::EditToggleTextureLock, "Texture Lock"));
        const Preference<KeyboardShortcut>  EditShowMapProperties = Preference<KeyboardShortcut>(               "Menu/Edit/Show Map Properties Dialog",                 KeyboardShortcut(Menu::EditShowMapProperties, "Map Properties"));

        const Preference<KeyboardShortcut>  EditToolsToggleClipTool = Preference<KeyboardShortcut>(             "Menu/Edit/Tools/Toggle Clip Tool",                     KeyboardShortcut(Menu::EditToggleClipTool, 'C', "Clip Tool"));
        const Preference<KeyboardShortcut>  EditToolsToggleClipSide = Preference<KeyboardShortcut>(             "Menu/Edit/Tools/Toggle Clip Side",                     KeyboardShortcut(Menu::EditToggleClipSide, WXK_TAB, "Toggle Clip Side"));
        const Preference<KeyboardShortcut>  EditToolsPerformClip = Preference<KeyboardShortcut>(                "Menu/Edit/Tools/Perform Clip",                         KeyboardShortcut(Menu::EditPerformClip, WXK_RETURN, "Perform Clip"));
        const Preference<KeyboardShortcut>  EditToolsToggleVertexTool = Preference<KeyboardShortcut>(           "Menu/Edit/Tools/Toggle Vertex Tool",                   KeyboardShortcut(Menu::EditToggleVertexTool, 'V', "Vertex Tool"));
        const Preference<KeyboardShortcut>  EditToolsToggleRotateTool = Preference<KeyboardShortcut>(           "Menu/Edit/Tools/Toggle Rotate Tool",                   KeyboardShortcut(Menu::EditToggleRotateObjectsTool, 'R', "Rotate Tool"));
        
        const Preference<KeyboardShortcut>  EditActionsMoveTexturesUp = Preference<KeyboardShortcut>(           "Menu/Edit/Actions/Move Textures Up (Coarse)",          KeyboardShortcut(Menu::EditMoveTexturesUp, WXK_UP, "Move Up"));
        const Preference<KeyboardShortcut>  EditActionsMoveTexturesDown = Preference<KeyboardShortcut>(         "Menu/Edit/Actions/Move Textures Down (Coarse)",        KeyboardShortcut(Menu::EditMoveTexturesDown, WXK_DOWN, "Move Down"));
        const Preference<KeyboardShortcut>  EditActionsMoveTexturesLeft = Preference<KeyboardShortcut>(         "Menu/Edit/Actions/Move Textures Left (Coarse)",        KeyboardShortcut(Menu::EditMoveTexturesLeft, WXK_LEFT, "Move Left"));
        const Preference<KeyboardShortcut>  EditActionsMoveTexturesRight = Preference<KeyboardShortcut>(        "Menu/Edit/Actions/Move Textures Right (Coarse)",       KeyboardShortcut(Menu::EditMoveTexturesRight, WXK_RIGHT, "Move Right"));
        const Preference<KeyboardShortcut>  EditActionsRotateTexturesCW = Preference<KeyboardShortcut>(         "Menu/Edit/Actions/Rotate Textures CW (Coarse)",        KeyboardShortcut(Menu::EditRotateTexturesCW, WXK_PAGEUP, "Rotate Clockwise by 15"));
        const Preference<KeyboardShortcut>  EditActionsRotateTexturesCCW = Preference<KeyboardShortcut>(        "Menu/Edit/Actions/Rotate Textures CCW (Coarse)",       KeyboardShortcut(Menu::EditRotateTexturesCW, WXK_PAGEDOWN, "Rotate Counter-clockwise by 15"));
        const Preference<KeyboardShortcut>  EditActionsMoveTexturesUpFine = Preference<KeyboardShortcut>(       "Menu/Edit/Actions/Move Textures Up (Fine)",            KeyboardShortcut(Menu::EditMoveTexturesUp, WXK_CONTROL, WXK_UP, "Move Up by 1"));
        const Preference<KeyboardShortcut>  EditActionsMoveTexturesDownFine = Preference<KeyboardShortcut>(     "Menu/Edit/Actions/Move Textures Down (Fine)",          KeyboardShortcut(Menu::EditMoveTexturesDown, WXK_CONTROL, WXK_DOWN, "Move Down by 1"));
        const Preference<KeyboardShortcut>  EditActionsMoveTexturesLeftFine = Preference<KeyboardShortcut>(     "Menu/Edit/Actions/Move Textures Left (Fine)",          KeyboardShortcut(Menu::EditMoveTexturesLeft, WXK_CONTROL, WXK_LEFT, "Move Left by 1"));
        const Preference<KeyboardShortcut>  EditActionsMoveTexturesRightFine = Preference<KeyboardShortcut>(    "Menu/Edit/Actions/Move Textures Right (Fine)",         KeyboardShortcut(Menu::EditMoveTexturesRight, WXK_CONTROL, WXK_RIGHT, "Move Right by 1"));
        const Preference<KeyboardShortcut>  EditActionsRotateTexturesCWFine = Preference<KeyboardShortcut>(     "Menu/Edit/Actions/Rotate Textures CW (Fine)",          KeyboardShortcut(Menu::EditRotateTexturesCW, WXK_CONTROL, WXK_PAGEUP, "Rotate Clockwise by 1"));
        const Preference<KeyboardShortcut>  EditActionsRotateTexturesCCWFine = Preference<KeyboardShortcut>(    "Menu/Edit/Actions/Rotate Textures CCW (Fine)",         KeyboardShortcut(Menu::EditRotateTexturesCW, WXK_CONTROL, WXK_PAGEDOWN, "Rotate Counter-clockwise by 1"));

        const Preference<KeyboardShortcut>  EditActionsMoveObjectsForward = Preference<KeyboardShortcut>(       "Menu/Edit/Actions/Move Objects Forward",               KeyboardShortcut(Menu::EditMoveObjectsForward, WXK_UP, "Move Forward"));
        const Preference<KeyboardShortcut>  EditActionsMoveObjectsBackward = Preference<KeyboardShortcut>(      "Menu/Edit/Actions/Move Objects Backward",              KeyboardShortcut(Menu::EditMoveObjectsBackward, WXK_DOWN, "Move Backward"));
        const Preference<KeyboardShortcut>  EditActionsMoveObjectsLeft = Preference<KeyboardShortcut>(          "Menu/Edit/Actions/Move Objects Left",                  KeyboardShortcut(Menu::EditMoveObjectsLeft, WXK_LEFT, "Move Left"));
        const Preference<KeyboardShortcut>  EditActionsMoveObjectsRight = Preference<KeyboardShortcut>(         "Menu/Edit/Actions/Move Objects Right",                 KeyboardShortcut(Menu::EditMoveObjectsRight, WXK_RIGHT, "Move Right"));
        const Preference<KeyboardShortcut>  EditActionsMoveObjectsUp = Preference<KeyboardShortcut>(            "Menu/Edit/Actions/Move Objects Up",                    KeyboardShortcut(Menu::EditMoveObjectsUp, WXK_PAGEUP, "Move Up"));
        const Preference<KeyboardShortcut>  EditActionsMoveObjectsDown = Preference<KeyboardShortcut>(          "Menu/Edit/Actions/Move Objects Down",                  KeyboardShortcut(Menu::EditMoveObjectsDown, WXK_PAGEDOWN, "Move Down"));
        const Preference<KeyboardShortcut>  EditActionsRollObjectsCW = Preference<KeyboardShortcut>(            "Menu/Edit/Actions/Roll Objects CW",                    KeyboardShortcut(Menu::EditRollObjectsCW, WXK_CONTROL, WXK_UP, "Rotate Clockwise by 90"));
        const Preference<KeyboardShortcut>  EditActionsRollObjectsCCW = Preference<KeyboardShortcut>(           "Menu/Edit/Actions/Roll Objects CCW",                   KeyboardShortcut(Menu::EditRollObjectsCCW, WXK_CONTROL, WXK_DOWN, "Rotate Counter-clockwise by 90"));
        const Preference<KeyboardShortcut>  EditActionsYawObjectsCW = Preference<KeyboardShortcut>(             "Menu/Edit/Actions/Yaw Objects CW",                     KeyboardShortcut(Menu::EditYawObjectsCW, WXK_CONTROL, WXK_LEFT, "Rotate Left by 90"));
        const Preference<KeyboardShortcut>  EditActionsYawObjectsCCW = Preference<KeyboardShortcut>(            "Menu/Edit/Actions/Yaw Objects CCW",                    KeyboardShortcut(Menu::EditYawObjectsCCW, WXK_CONTROL, WXK_RIGHT, "Rotate Right by 90"));
        const Preference<KeyboardShortcut>  EditActionsPitchObjectsCW = Preference<KeyboardShortcut>(           "Menu/Edit/Actions/Pitch Objects CW",                   KeyboardShortcut(Menu::EditPitchObjectsCW, WXK_CONTROL, WXK_PAGEUP, "Rotate Up by 90"));
        const Preference<KeyboardShortcut>  EditActionsPitchObjectsCCW = Preference<KeyboardShortcut>(          "Menu/Edit/Actions/Pitch Objects CCW",                  KeyboardShortcut(Menu::EditPitchObjectsCCW, WXK_CONTROL, WXK_PAGEDOWN, "Rotate Down by 90"));
        const Preference<KeyboardShortcut>  EditActionsFlipObjectsHorizontally = Preference<KeyboardShortcut>(  "Menu/Edit/Actions/Flip Objects Horizontally",          KeyboardShortcut(Menu::EditFlipObjectsHorizontally, WXK_CONTROL, 'F', "Flip Horizontally"));
        const Preference<KeyboardShortcut>  EditActionsFlipObjectsVertically = Preference<KeyboardShortcut>(    "Menu/Edit/Actions/Flip Objects Vertically",            KeyboardShortcut(Menu::EditFlipObjectsVertically, WXK_CONTROL, WXK_ALT, 'F', "Flip Vertically"));
        const Preference<KeyboardShortcut>  EditActionsDuplicateObjects = Preference<KeyboardShortcut>(         "Menu/Edit/Actions/Duplicate Objects",                  KeyboardShortcut(Menu::EditDuplicateObjects, WXK_CONTROL, 'D', "Duplicate"));
        
        const Preference<KeyboardShortcut>  EditActionsMoveVerticesForward = Preference<KeyboardShortcut>(      "Menu/Edit/Actions/Move Vertices Forward",              KeyboardShortcut(Menu::EditMoveVerticesForward, WXK_UP, "Move Forward"));
        const Preference<KeyboardShortcut>  EditActionsMoveVerticesBackward = Preference<KeyboardShortcut>(     "Menu/Edit/Actions/Move Vertices Backward",             KeyboardShortcut(Menu::EditMoveVerticesBackward, WXK_DOWN, "Move Backward"));
        const Preference<KeyboardShortcut>  EditActionsMoveVerticesLeft = Preference<KeyboardShortcut>(         "Menu/Edit/Actions/Move Vertices Left",                 KeyboardShortcut(Menu::EditMoveVerticesLeft, WXK_LEFT, "Move Left"));
        const Preference<KeyboardShortcut>  EditActionsMoveVerticesRight = Preference<KeyboardShortcut>(        "Menu/Edit/Actions/Move Vertices Right",                KeyboardShortcut(Menu::EditMoveVerticesRight, WXK_RIGHT, "Move Right"));
        const Preference<KeyboardShortcut>  EditActionsMoveVerticesUp = Preference<KeyboardShortcut>(           "Menu/Edit/Actions/Move Vertices Up",                   KeyboardShortcut(Menu::EditMoveVerticesUp, WXK_PAGEUP, "Move Up"));
        const Preference<KeyboardShortcut>  EditActionsMoveVerticesDown = Preference<KeyboardShortcut>(         "Menu/Edit/Actions/Move Vertices Down",                 KeyboardShortcut(Menu::EditMoveVerticesDown, WXK_PAGEDOWN, "Move Down"));

        const Preference<KeyboardShortcut>  EditActionsCorrectVertices = Preference<KeyboardShortcut>(          "Menu/Edit/Actions/Correct Vertices",                   KeyboardShortcut(Menu::EditCorrectVertices, "Correct Vertices"));
        const Preference<KeyboardShortcut>  EditActionsSnapVertices = Preference<KeyboardShortcut>(             "Menu/Edit/Actions/Snap Vertices",                      KeyboardShortcut(Menu::EditSnapVertices, "Snap Vertices"));

        const Preference<KeyboardShortcut>  ViewGridToggleShowGrid = Preference<KeyboardShortcut>(              "Menu/View/Grid/Toggle Show Grid",                      KeyboardShortcut(Menu::ViewToggleShowGrid, WXK_CONTROL, 'G', "Show Grid"));
        const Preference<KeyboardShortcut>  ViewGridToggleSnapToGrid = Preference<KeyboardShortcut>(            "Menu/View/Grid/Toggle Snap to Grid",                   KeyboardShortcut(Menu::ViewToggleSnapToGrid, WXK_CONTROL, WXK_SHIFT, 'G', "Snap to Grid"));
        const Preference<KeyboardShortcut>  ViewGridIncGridSize = Preference<KeyboardShortcut>(                 "Menu/View/Grid/Increase Grid Size",                    KeyboardShortcut(Menu::ViewIncGridSize, WXK_CONTROL, '+', "Increase Grid Size"));
        const Preference<KeyboardShortcut>  ViewGridDecGridSize = Preference<KeyboardShortcut>(                 "Menu/View/Grid/Decrease Grid Size",                    KeyboardShortcut(Menu::ViewDecGridSize, WXK_CONTROL, '-', "Decrease Grid Size"));
        const Preference<KeyboardShortcut>  ViewGridSetSize1 = Preference<KeyboardShortcut>(                    "Menu/View/Grid/Set Grid Size 1",                       KeyboardShortcut(Menu::ViewSetGridSize1, WXK_CONTROL, '1', "Set Grid Size 1"));
        const Preference<KeyboardShortcut>  ViewGridSetSize2 = Preference<KeyboardShortcut>(                    "Menu/View/Grid/Set Grid Size 2",                       KeyboardShortcut(Menu::ViewSetGridSize2, WXK_CONTROL, '2', "Set Grid Size 2"));
        const Preference<KeyboardShortcut>  ViewGridSetSize4 = Preference<KeyboardShortcut>(                    "Menu/View/Grid/Set Grid Size 4",                       KeyboardShortcut(Menu::ViewSetGridSize4, WXK_CONTROL, '3', "Set Grid Size 4"));
        const Preference<KeyboardShortcut>  ViewGridSetSize8 = Preference<KeyboardShortcut>(                    "Menu/View/Grid/Set Grid Size 8",                       KeyboardShortcut(Menu::ViewSetGridSize8, WXK_CONTROL, '4', "Set Grid Size 8"));
        const Preference<KeyboardShortcut>  ViewGridSetSize16 = Preference<KeyboardShortcut>(                   "Menu/View/Grid/Set Grid Size 16",                      KeyboardShortcut(Menu::ViewSetGridSize16, WXK_CONTROL, '5', "Set Grid Size 16"));
        const Preference<KeyboardShortcut>  ViewGridSetSize32 = Preference<KeyboardShortcut>(                   "Menu/View/Grid/Set Grid Size 32",                      KeyboardShortcut(Menu::ViewSetGridSize32, WXK_CONTROL, '6', "Set Grid Size 32"));
        const Preference<KeyboardShortcut>  ViewGridSetSize64 = Preference<KeyboardShortcut>(                   "Menu/View/Grid/Set Grid Size 64",                      KeyboardShortcut(Menu::ViewSetGridSize64, WXK_CONTROL, '7', "Set Grid Size 64"));
        const Preference<KeyboardShortcut>  ViewGridSetSize128 = Preference<KeyboardShortcut>(                  "Menu/View/Grid/Set Grid Size 128",                     KeyboardShortcut(Menu::ViewSetGridSize128, WXK_CONTROL, '8', "Set Grid Size 128"));
        const Preference<KeyboardShortcut>  ViewGridSetSize256 = Preference<KeyboardShortcut>(                  "Menu/View/Grid/Set Grid Size 256",                     KeyboardShortcut(Menu::ViewSetGridSize256, WXK_CONTROL, '9', "Set Grid Size 256"));

        const Preference<KeyboardShortcut>  ViewCameraMoveForward = Preference<KeyboardShortcut>(               "Menu/View/Camera/Move Forward",                        KeyboardShortcut(Menu::ViewMoveCameraForward, WXK_ALT, WXK_UP, "Move Forward"));
        const Preference<KeyboardShortcut>  ViewCameraMoveBackward = Preference<KeyboardShortcut>(              "Menu/View/Camera/Move Backward",                       KeyboardShortcut(Menu::ViewMoveCameraBackward, WXK_ALT, WXK_DOWN, "Move Backward"));
        const Preference<KeyboardShortcut>  ViewCameraMoveLeft = Preference<KeyboardShortcut>(                  "Menu/View/Camera/Move Left",                           KeyboardShortcut(Menu::ViewMoveCameraLeft, WXK_ALT, WXK_LEFT, "Move Left"));
        const Preference<KeyboardShortcut>  ViewCameraMoveRight = Preference<KeyboardShortcut>(                 "Menu/View/Camera/Move Right",                          KeyboardShortcut(Menu::ViewMoveCameraRight, WXK_ALT, WXK_RIGHT, "Move Right"));
        const Preference<KeyboardShortcut>  ViewCameraMoveUp = Preference<KeyboardShortcut>(                    "Menu/View/Camera/Move Up",                             KeyboardShortcut(Menu::ViewMoveCameraUp, WXK_ALT, WXK_PAGEUP, "Move Up"));
        const Preference<KeyboardShortcut>  ViewCameraMoveDown = Preference<KeyboardShortcut>(                  "Menu/View/Camera/Move Down",                           KeyboardShortcut(Menu::ViewMoveCameraUp, WXK_ALT, WXK_PAGEDOWN, "Move Down"));
        const Preference<KeyboardShortcut>  ViewCameraMoveToNextPoint = Preference<KeyboardShortcut>(           "Menu/View/Camera/Move to Next Point",                  KeyboardShortcut(Menu::ViewMoveCameraToNextPoint, WXK_ALT, '+', "Move to Next Point"));
        const Preference<KeyboardShortcut>  ViewCameraMoveToPreviousPoint = Preference<KeyboardShortcut>(       "Menu/View/Camera/Move to Previous Point",              KeyboardShortcut(Menu::ViewMoveCameraToPreviousPoint, WXK_ALT, '-', "Move to Previous Point"));
        const Preference<KeyboardShortcut>  ViewCameraCenterCameraOnSelection = Preference<KeyboardShortcut>(   "Menu/View/Camera/Center on Selection",                 KeyboardShortcut(Menu::ViewCenterCameraOnSelection, WXK_ALT, 'C', "Center on Selection"));
    }
}
