/////////////////////////////////////////////////////////////////////////////
// Name:        msvc/wx/setup.h
// Purpose:     wrapper around the real wx/setup.h for Visual C++
// Author:      Vadim Zeitlin
// Modified by:
// Created:     2004-12-12
// Copyright:   (c) 2004 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MSC_VER
    #error "This file should only be included when using Microsoft Visual C++"
#endif

// VC++ IDE predefines _DEBUG and _UNICODE for the new projects itself, but
// the other symbols (WXUSINGDLL, __WXUNIVERSAL__, ...) should be defined
// explicitly!

#include "wx/version.h"
#include "wx/cpp.h"

// notice that wxSUFFIX_DEBUG is a string but wxSUFFIX itself must be an
// identifier as string concatenation is not done inside #include where we
// need it
#ifdef _DEBUG
    #define wxSUFFIX_DEBUG "d"
    #ifdef _UNICODE
        #define wxSUFFIX ud
    #else // !_UNICODE
        #define wxSUFFIX d
    #endif // _UNICODE/!_UNICODE
#else
    #define wxSUFFIX_DEBUG ""
    #ifdef _UNICODE
        #define wxSUFFIX u
    #else // !_UNICODE
        // don't define wxSUFFIX at all as preprocessor operations don't work
        // with empty values so we need to check for this case specially below
    #endif // _UNICODE/!_UNICODE
#endif

// compiler-specific prefix: by default it's always just "vc" for compatibility
// reasons but if you use multiple MSVC versions you probably build them with
// COMPILER_PREFIX=vcXX and in this case you may want to either predefine
// wxMSVC_VERSION as "XX" or define wxMSVC_VERSION_AUTO to use the appropriate
// version depending on the compiler used
#ifdef wxMSVC_VERSION
    #define wxCOMPILER_PREFIX wxCONCAT(vc, wxMSVC_VERSION)
#elif defined(wxMSVC_VERSION_AUTO)
    #if _MSC_VER == 1200
        #define wxCOMPILER_PREFIX vc60
    #elif _MSC_VER == 1300
        #define wxCOMPILER_PREFIX vc70
    #elif _MSC_VER == 1310
        #define wxCOMPILER_PREFIX vc71
    #elif _MSC_VER == 1400
        #define wxCOMPILER_PREFIX vc80
    #elif _MSC_VER == 1500
        #define wxCOMPILER_PREFIX vc90
    #elif _MSC_VER == 1600
        #define wxCOMPILER_PREFIX vc100
    #elif _MSC_VER == 1700
        #define wxCOMPILER_PREFIX vc110
    #elif _MSC_VER == 1800
        #define wxCOMPILER_PREFIX vc120
    #elif _MSC_VER == 1900
        #define wxCOMPILER_PREFIX vc140
    #else
        #error "Unknown MSVC compiler version, please report to wx-dev."
    #endif
#else
    #define wxCOMPILER_PREFIX vc
#endif

// architecture-specific part: not used (again, for compatibility), for x86
#if defined(_M_X64)
    #define wxARCH_SUFFIX _x64
#elif defined(_M_IA64)
    #define wxARCH_SUFFIX _ia64
#else // assume _M_IX86
    #define wxARCH_SUFFIX
#endif

// Ensure the library configuration is defined
#ifndef wxCFG
    #define wxCFG
#endif

// Construct the path for the subdirectory under /lib/ that the included setup.h
// will be used from
#ifdef WXUSINGDLL
    #define wxLIB_SUBDIR \
        wxCONCAT4(wxCOMPILER_PREFIX, wxARCH_SUFFIX, _dll, wxCFG)
#else // !DLL
    #define wxLIB_SUBDIR \
        wxCONCAT4(wxCOMPILER_PREFIX, wxARCH_SUFFIX, _lib, wxCFG)
#endif // DLL/!DLL

// The user can predefine a different prefix if not using the default MSW port
// with MSVC.
#ifndef wxTOOLKIT_PREFIX
    #if defined(__WXGTK__)
        #define wxTOOLKIT_PREFIX gtk2
    #else
        #define wxTOOLKIT_PREFIX msw
    #endif
#endif // wxTOOLKIT_PREFIX

// the real setup.h header file we need is in the build-specific directory,
// construct the path to it
#ifdef wxSUFFIX
    #define wxSETUPH_PATH \
        wxCONCAT6(../../../lib/, wxLIB_SUBDIR, /, wxTOOLKIT_PREFIX, wxSUFFIX, /wx/setup.h)
#else // suffix is empty
    #define wxSETUPH_PATH \
        wxCONCAT5(../../../lib/, wxLIB_SUBDIR, /, wxTOOLKIT_PREFIX, /wx/setup.h)
#endif

#define wxSETUPH_PATH_STR wxSTRINGIZE(wxSETUPH_PATH)

#if _MSC_VER < 1900

#include wxSETUPH_PATH_STR


// the library names depend on the build, these macro builds the correct
// library name for the given base name
#ifdef wxSUFFIX
    #define wxSUFFIX_STR wxSTRINGIZE(wxSUFFIX)
#else // suffix is empty
    #define wxSUFFIX_STR ""
#endif
#define wxSHORT_VERSION_STRING \
    wxSTRINGIZE(wxMAJOR_VERSION) wxSTRINGIZE(wxMINOR_VERSION)

#define wxWX_LIB_NAME(name, subname) \
    "wx" name wxSHORT_VERSION_STRING wxSUFFIX_STR subname

#define wxBASE_LIB_NAME(name) wxWX_LIB_NAME("base", "_" name)
#define wxTOOLKIT_LIB_NAME(name) wxWX_LIB_NAME(wxSTRINGIZE(wxTOOLKIT_PREFIX), "_" name)

// This one is for 3rd party libraries: they don't have the version number
// in their names and usually exist in ANSI version only (except for regex)
//
// 3rd party libraries are also are not linked in when using DLLs as they're
// embedded inside our own DLLs and don't need to be linked with the user code.
#define wx3RD_PARTY_LIB_NAME(name) "wx" name wxSUFFIX_DEBUG

// special version for regex as it does have a Unicode version
#define wx3RD_PARTY_LIB_NAME_U(name) "wx" name wxSUFFIX_STR

#pragma comment(lib, wxWX_LIB_NAME("base", ""))

#ifndef wxNO_NET_LIB
    #ifndef WXUSINGDLL
        #pragma comment(lib, "wsock32")
    #endif
    #pragma comment(lib, wxBASE_LIB_NAME("net"))
#endif
#if wxUSE_XML && !defined(wxNO_XML_LIB)
    #pragma comment(lib, wxBASE_LIB_NAME("xml"))
#endif
#if wxUSE_REGEX && !defined(wxNO_REGEX_LIB) && !defined(WXUSINGDLL)
    #pragma comment(lib, wx3RD_PARTY_LIB_NAME_U("regex"))
#endif

#if wxUSE_GUI
    #if wxUSE_XML && !defined(wxNO_EXPAT_LIB) && !defined(WXUSINGDLL)
        #pragma comment(lib, wx3RD_PARTY_LIB_NAME("expat"))
    #endif
    #if wxUSE_LIBJPEG && !defined(wxNO_JPEG_LIB) && !defined(WXUSINGDLL)
        #pragma comment(lib, wx3RD_PARTY_LIB_NAME("jpeg"))
    #endif
    #if wxUSE_LIBPNG && !defined(wxNO_PNG_LIB) && !defined(WXUSINGDLL)
        #pragma comment(lib, wx3RD_PARTY_LIB_NAME("png"))
    #endif
    #if wxUSE_LIBTIFF && !defined(wxNO_TIFF_LIB) && !defined(WXUSINGDLL)
        #pragma comment(lib, wx3RD_PARTY_LIB_NAME("tiff"))
    #endif
    #if wxUSE_ZLIB && !defined(wxNO_ZLIB_LIB) && !defined(WXUSINGDLL)
        #pragma comment(lib, wx3RD_PARTY_LIB_NAME("zlib"))
    #endif

    #pragma comment(lib, wxTOOLKIT_LIB_NAME("core"))

    #ifndef wxNO_ADV_LIB
        #pragma comment(lib, wxTOOLKIT_LIB_NAME("adv"))
    #endif

    #if wxUSE_HTML && !defined(wxNO_HTML_LIB)
        #pragma comment(lib, wxTOOLKIT_LIB_NAME("html"))
    #endif
    #if wxUSE_GLCANVAS && !defined(wxNO_GL_LIB)
        #pragma comment(lib, wxTOOLKIT_LIB_NAME("gl"))
    #endif
    #if wxUSE_DEBUGREPORT && !defined(wxNO_QA_LIB)
        #pragma comment(lib, wxTOOLKIT_LIB_NAME("qa"))
    #endif
    #if wxUSE_XRC && !defined(wxNO_XRC_LIB)
        #pragma comment(lib, wxTOOLKIT_LIB_NAME("xrc"))
    #endif
    #if wxUSE_AUI && !defined(wxNO_AUI_LIB)
        #pragma comment(lib, wxTOOLKIT_LIB_NAME("aui"))
    #endif
    #if wxUSE_PROPGRID && !defined(wxNO_PROPGRID_LIB)
        #pragma comment(lib, wxTOOLKIT_LIB_NAME("propgrid"))
    #endif
    #if wxUSE_RIBBON && !defined(wxNO_RIBBON_LIB)
        #pragma comment(lib, wxTOOLKIT_LIB_NAME("ribbon"))
    #endif
    #if wxUSE_RICHTEXT && !defined(wxNO_RICHTEXT_LIB)
        #pragma comment(lib, wxTOOLKIT_LIB_NAME("richtext"))
    #endif
    #if wxUSE_MEDIACTRL && !defined(wxNO_MEDIA_LIB)
        #pragma comment(lib, wxTOOLKIT_LIB_NAME("media"))
    #endif
    #if wxUSE_STC && !defined(wxNO_STC_LIB)
        #pragma comment(lib, wxTOOLKIT_LIB_NAME("stc"))
        #ifndef WXUSINGDLL
            #pragma comment(lib, wx3RD_PARTY_LIB_NAME("scintilla"))
        #endif
    #endif
    #if wxUSE_WEBVIEW && !defined(wxNO_WEBVIEW_LIB)
        #pragma comment(lib, wxTOOLKIT_LIB_NAME("webview"))
    #endif
#endif // wxUSE_GUI


#ifndef WXUSINGDLL
    // Make sure all required system libraries are added to the linker too when
    // using static libraries.
    #pragma comment(lib, "kernel32")
    #pragma comment(lib, "user32")
    #pragma comment(lib, "gdi32")
    #pragma comment(lib, "comdlg32")
    #pragma comment(lib, "winspool")
    #pragma comment(lib, "winmm")
    #pragma comment(lib, "shell32")
    #pragma comment(lib, "comctl32")
    #pragma comment(lib, "ole32")
    #pragma comment(lib, "oleaut32")
    #pragma comment(lib, "uuid")
    #pragma comment(lib, "rpcrt4")
    #pragma comment(lib, "advapi32")
    #if wxUSE_URL_NATIVE
        #pragma comment(lib, "wininet")
    #endif

    #ifdef __WXGTK__
        #pragma comment(lib, "gtk-win32-2.0.lib")
        #pragma comment(lib, "gdk-win32-2.0.lib")
        #pragma comment(lib, "pangocairo-1.0.lib")
        #pragma comment(lib, "gdk_pixbuf-2.0.lib")
        #pragma comment(lib, "cairo.lib")
        #pragma comment(lib, "pango-1.0.lib")
        #pragma comment(lib, "gobject-2.0.lib")
        #pragma comment(lib, "gthread-2.0.lib")
        #pragma comment(lib, "glib-2.0.lib")
    #endif
#endif // !WXUSINGDLL

#else // _MSC_VER == 1900


#ifndef _WX_SETUP_H_
#define _WX_SETUP_H_

#ifndef wxUSE_GUI
#define wxUSE_GUI            1
#endif // wxUSE_GUI

#define WXWIN_COMPATIBILITY_2_6 0
#define WXWIN_COMPATIBILITY_2_8 1

#define wxDIALOG_UNIT_COMPATIBILITY   0
#define wxUSE_ON_FATAL_EXCEPTION 1
#define wxUSE_STACKWALKER 1
#define wxUSE_DEBUGREPORT 1
#define wxUSE_DEBUG_CONTEXT 0
#define wxUSE_MEMORY_TRACING 0
#define wxUSE_GLOBAL_MEMORY_OPERATORS 0
#define wxUSE_DEBUG_NEW_ALWAYS 0
#ifndef wxUSE_UNICODE
#define wxUSE_UNICODE 1
#endif
#define wxUSE_WCHAR_T 1
#define wxUSE_EXCEPTIONS    1
#define wxUSE_EXTENDED_RTTI 0
#define wxUSE_LOG 1
#define wxUSE_LOGWINDOW 1
#define wxUSE_LOGGUI 1
#define wxUSE_LOG_DIALOG 1
#define wxUSE_CMDLINE_PARSER 1
#define wxUSE_THREADS 0
#define wxUSE_STREAMS       1
#define wxUSE_PRINTF_POS_PARAMS      1
#define wxUSE_COMPILER_TLS 1
#define wxUSE_STL 0
#if defined(__DMC__) || defined(__WATCOMC__) \
        || (defined(_MSC_VER) && _MSC_VER < 1200)
#define wxUSE_STD_DEFAULT  0
#else
#define wxUSE_STD_DEFAULT  1
#endif
#define wxUSE_STD_CONTAINERS 0
#define wxUSE_STD_IOSTREAM  wxUSE_STD_DEFAULT
#define wxUSE_STD_STRING  wxUSE_STD_DEFAULT
#define wxUSE_STD_STRING_CONV_IN_WXSTRING wxUSE_STL
#define wxUSE_IOSTREAMH     0
#define wxUSE_LONGLONG      1
#define wxUSE_BASE64        1
#define wxUSE_CONSOLE_EVENTLOOP 1
#define wxUSE_FILE          1
#define wxUSE_FFILE         1
#define wxUSE_FSVOLUME      1
#define wxUSE_STDPATHS      1
#define wxUSE_TEXTBUFFER    1
#define wxUSE_TEXTFILE      1
#define wxUSE_INTL          1
#define wxUSE_XLOCALE       1
#define wxUSE_DATETIME      1
#define wxUSE_TIMER         1
#define wxUSE_STOPWATCH     1
#define wxUSE_FSWATCHER     0
#define wxUSE_CONFIG        1
#define wxUSE_CONFIG_NATIVE   1
#define wxUSE_DIALUP_MANAGER   1
#define wxUSE_DYNLIB_CLASS    1
#define wxUSE_DYNAMIC_LOADER  1
#define wxUSE_SOCKETS       1
#define wxUSE_IPV6          0
#define wxUSE_FILESYSTEM    1
#define wxUSE_FS_ZIP        1
#define wxUSE_FS_ARCHIVE    1
#define wxUSE_FS_INET       1
#define wxUSE_ARCHIVE_STREAMS     1
#define wxUSE_ZIPSTREAM     1
#define wxUSE_TARSTREAM     1
#define wxUSE_ZLIB          1
#define wxUSE_APPLE_IEEE          1
#define wxUSE_JOYSTICK            1
#define wxUSE_FONTENUM 1
#define wxUSE_FONTMAP 1
#define wxUSE_MIMETYPE 1
#define wxUSE_PROTOCOL 1
#define wxUSE_PROTOCOL_FILE 1
#define wxUSE_PROTOCOL_FTP 1
#define wxUSE_PROTOCOL_HTTP 1
#define wxUSE_URL 1
#define wxUSE_URL_NATIVE 0
#define wxUSE_VARIANT 1
#define wxUSE_ANY 1
#define wxUSE_REGEX       1
#define wxUSE_SYSTEM_OPTIONS 1
#define wxUSE_SOUND      1
#define wxUSE_MEDIACTRL     1
#define wxUSE_XRC       1
#define wxUSE_XML       wxUSE_XRC
#define wxUSE_AUI       1
#define wxUSE_RIBBON    1
#define wxUSE_PROPGRID  1
#define wxUSE_STC 1
#define wxUSE_WEBVIEW 1
#ifdef __WXMSW__
#define wxUSE_WEBVIEW_IE 1
#else
#define wxUSE_WEBVIEW_IE 0
#endif
#if defined(__WXGTK__) || defined(__WXOSX__)
#define wxUSE_WEBVIEW_WEBKIT 1
#else
#define wxUSE_WEBVIEW_WEBKIT 0
#endif
#ifdef _MSC_VER
#   if _MSC_VER >= 1310
#       define wxUSE_GRAPHICS_CONTEXT 1
#   else
#       define wxUSE_GRAPHICS_CONTEXT 0
#   endif
#else
#   define wxUSE_GRAPHICS_CONTEXT 0
#endif
#define wxUSE_CAIRO 0
#define wxUSE_CONTROLS     1
#define wxUSE_MARKUP       1
#define wxUSE_POPUPWIN     1
#define wxUSE_TIPWINDOW    1
#define wxUSE_ANIMATIONCTRL 1   // wxAnimationCtrl
#define wxUSE_BANNERWINDOW  1   // wxBannerWindow
#define wxUSE_BUTTON        1   // wxButton
#define wxUSE_BMPBUTTON     1   // wxBitmapButton
#define wxUSE_CALENDARCTRL  1   // wxCalendarCtrl
#define wxUSE_CHECKBOX      1   // wxCheckBox
#define wxUSE_CHECKLISTBOX  1   // wxCheckListBox (requires wxUSE_OWNER_DRAWN)
#define wxUSE_CHOICE        1   // wxChoice
#define wxUSE_COLLPANE      1   // wxCollapsiblePane
#define wxUSE_COLOURPICKERCTRL 1    // wxColourPickerCtrl
#define wxUSE_COMBOBOX      1   // wxComboBox
#define wxUSE_COMMANDLINKBUTTON 1   // wxCommandLinkButton
#define wxUSE_DATAVIEWCTRL  1   // wxDataViewCtrl
#define wxUSE_DATEPICKCTRL  1   // wxDatePickerCtrl
#define wxUSE_DIRPICKERCTRL 1   // wxDirPickerCtrl
#define wxUSE_EDITABLELISTBOX 1 // wxEditableListBox
#define wxUSE_FILECTRL      1   // wxFileCtrl
#define wxUSE_FILEPICKERCTRL 1  // wxFilePickerCtrl
#define wxUSE_FONTPICKERCTRL 1  // wxFontPickerCtrl
#define wxUSE_GAUGE         1   // wxGauge
#define wxUSE_HEADERCTRL    1   // wxHeaderCtrl
#define wxUSE_HYPERLINKCTRL 1   // wxHyperlinkCtrl
#define wxUSE_LISTBOX       1   // wxListBox
#define wxUSE_LISTCTRL      1   // wxListCtrl
#define wxUSE_RADIOBOX      1   // wxRadioBox
#define wxUSE_RADIOBTN      1   // wxRadioButton
#define wxUSE_RICHMSGDLG    1   // wxRichMessageDialog
#define wxUSE_SCROLLBAR     1   // wxScrollBar
#define wxUSE_SEARCHCTRL    1   // wxSearchCtrl
#define wxUSE_SLIDER        1   // wxSlider
#define wxUSE_SPINBTN       1   // wxSpinButton
#define wxUSE_SPINCTRL      1   // wxSpinCtrl
#define wxUSE_STATBOX       1   // wxStaticBox
#define wxUSE_STATLINE      1   // wxStaticLine
#define wxUSE_STATTEXT      1   // wxStaticText
#define wxUSE_STATBMP       1   // wxStaticBitmap
#define wxUSE_TEXTCTRL      1   // wxTextCtrl
#define wxUSE_TIMEPICKCTRL  1   // wxTimePickerCtrl
#define wxUSE_TOGGLEBTN     1   // requires wxButton
#define wxUSE_TREECTRL      1   // wxTreeCtrl
#define wxUSE_TREELISTCTRL  1   // wxTreeListCtrl
#define wxUSE_STATUSBAR    1
#define wxUSE_NATIVE_STATUSBAR        1
#define wxUSE_TOOLBAR 1
#define wxUSE_TOOLBAR_NATIVE 1
#define wxUSE_NOTEBOOK 1
#define wxUSE_LISTBOOK 1
#define wxUSE_CHOICEBOOK 1
#define wxUSE_TREEBOOK 1
#define wxUSE_TOOLBOOK 1
#define wxUSE_TASKBARICON 1
#define wxUSE_GRID         1
#define wxUSE_MINIFRAME 1
#define wxUSE_COMBOCTRL 1
#define wxUSE_ODCOMBOBOX 1
#define wxUSE_BITMAPCOMBOBOX 1
#define wxUSE_REARRANGECTRL 1
#define wxUSE_ACCEL 1
#define wxUSE_ARTPROVIDER_STD 1
#define wxUSE_ARTPROVIDER_TANGO 0
#define wxUSE_HOTKEY 1
#define wxUSE_CARET         1
#define wxUSE_DISPLAY       1
#define wxUSE_GEOMETRY            1
#define wxUSE_IMAGLIST      1
#define wxUSE_INFOBAR       1
#define wxUSE_MENUS         1
#define wxUSE_NOTIFICATION_MESSAGE 1
#define wxUSE_PREFERENCES_EDITOR 1
#define wxUSE_RICHTOOLTIP 1
#define wxUSE_SASH          1
#define wxUSE_SPLITTER      1
#define wxUSE_TOOLTIPS      1
#define wxUSE_VALIDATORS 1
#ifdef __WXMSW__
#define wxUSE_AUTOID_MANAGEMENT 1
#else
#define wxUSE_AUTOID_MANAGEMENT 0
#endif
#define wxUSE_COMMON_DIALOGS 1
#define wxUSE_BUSYINFO      1
#define wxUSE_CHOICEDLG     1
#define wxUSE_COLOURDLG     1
#define wxUSE_DIRDLG 1
#define wxUSE_FILEDLG       1
#define wxUSE_FINDREPLDLG       1
#define wxUSE_FONTDLG       1
#define wxUSE_MSGDLG        1
#define wxUSE_PROGRESSDLG 1
#define wxUSE_STARTUP_TIPS 1
#define wxUSE_TEXTDLG 1
#define wxUSE_NUMBERDLG 1
#define wxUSE_SPLASH 1
#define wxUSE_WIZARDDLG 1
#define wxUSE_ABOUTDLG 1
#define wxUSE_FILE_HISTORY 1
#define wxUSE_METAFILE              1
#define wxUSE_ENH_METAFILE          1
#define wxUSE_WIN_METAFILES_ALWAYS  0
#define wxUSE_MDI 1
#define wxUSE_DOC_VIEW_ARCHITECTURE 1
#define wxUSE_MDI_ARCHITECTURE    1
#define wxUSE_PRINTING_ARCHITECTURE  1
#define wxUSE_HTML          1
#define wxUSE_GLCANVAS       1
#define wxUSE_RICHTEXT       1
#define wxUSE_CLIPBOARD     1
#define wxUSE_DATAOBJ       1
#define wxUSE_DRAG_AND_DROP 1
#define wxUSE_ACCESSIBILITY 0
#define wxUSE_SNGLINST_CHECKER  1
#define wxUSE_DRAGIMAGE 1
#define wxUSE_IPC         1
#define wxUSE_HELP        1
#define wxUSE_MS_HTML_HELP 1
#define wxUSE_WXHTML_HELP 1
#define wxUSE_CONSTRAINTS 1
#define wxUSE_SPLINES     1
#define wxUSE_MOUSEWHEEL        1
#define wxUSE_UIACTIONSIMULATOR 1
#define wxUSE_POSTSCRIPT  0
#define wxUSE_AFM_FOR_POSTSCRIPT 1
#define wxUSE_SVG 1
#define wxUSE_DC_TRANSFORM_MATRIX 1
#define wxUSE_IMAGE         1
#define wxUSE_LIBPNG        1
#define wxUSE_LIBJPEG       1
#define wxUSE_LIBTIFF       1
#define wxUSE_TGA           1
#define wxUSE_GIF           1
#define wxUSE_PNM           1
#define wxUSE_PCX           1
#define wxUSE_IFF           0
#define wxUSE_XPM           1
#define wxUSE_ICO_CUR       1
#define wxUSE_PALETTE       1
#define wxUSE_ALL_THEMES    1
#define wxUSE_THEME_GTK     0
#define wxUSE_THEME_METAL   0
#define wxUSE_THEME_MONO    0
#define wxUSE_THEME_WIN32   0
#ifndef wxUSE_UNICODE_MSLU
#define wxUSE_UNICODE_MSLU 0
#endif
#define wxUSE_MFC           0
#define wxUSE_OLE           1
#define wxUSE_OLE_AUTOMATION 1
#define wxUSE_ACTIVEX 1
#define wxUSE_DC_CACHEING 1
#define wxUSE_WXDIB 1
#define wxUSE_POSTSCRIPT_ARCHITECTURE_IN_MSW 1
#define wxUSE_REGKEY 1
#define wxUSE_RICHEDIT  1
#define wxUSE_RICHEDIT2 1
#define wxUSE_OWNER_DRAWN 1
#define wxUSE_TASKBARICON_BALLOONS 1
#define wxUSE_UXTHEME           1
#define wxUSE_INKEDIT  0
#define wxUSE_INICONF 0
#define wxUSE_DATEPICKCTRL_GENERIC 0
#define wxUSE_TIMEPICKCTRL_GENERIC 0
#define wxUSE_CRASHREPORT 1

#endif // _WX_SETUP_H_

#endif // _MSC_VER < 1900