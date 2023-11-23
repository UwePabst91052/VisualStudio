// stdafx.h : Includedatei für Standardsystem-Includedateien
// oder häufig verwendete, projektspezifische Includedateien,
// die nur selten geändert werden.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Selten verwendete Teile der Windows-Header ausschließen
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // einige CString-Konstruktoren sind explizit

// Deaktiviert das Ausblenden einiger häufiger und oft ignorierter Warnungen durch MFC
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen


#include <afxdisp.h>        // MFC-Automatisierungsklassen



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC-Unterstützung für allgemeine Steuerelemente von Internet Explorer 4
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC-Unterstützung für allgemeine Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afx.h>
#include <afxdisp.h>
#include <afxdisp.h>
#include <afxdisp.h>

TCHAR* indent_in [];
TCHAR* indent_out [];

class CFuncInOut
{
public:
    CFuncInOut(char* func_name)
    {
        m_func_name = func_name;
        CA2T namew(func_name);
		CString name(namew);
        m_out.Format(_T("%s %s\n"), indent_in[s_indent], name);
        OutputDebugString(m_out);
        s_indent++;
    }
    ~CFuncInOut()
    {
        s_indent--;
        CA2T namew(m_func_name);
		CString name(namew);
        m_out.Format(_T("%s %s\n"), indent_out[s_indent], name);
        OutputDebugString(m_out);
    }
    void Trace0(TCHAR* fmt)
    {
        m_out = fmt;
        m_out += _T("\n");
        OutputDebugString(m_out);
    }
    void Trace1(TCHAR* fmt, int arg1)
    {
        m_out.Format(fmt, arg1);
        m_out += _T("\n");
        OutputDebugString(m_out);
    }
    void Trace2(TCHAR* fmt, int arg1, int arg2)
    {
        m_out.Format(fmt, arg1, arg2);
        m_out += _T("\n");
        OutputDebugString(m_out);
    }
    void Trace3(TCHAR* fmt, int arg1, int arg2, int arg3)
    {
        m_out.Format(fmt, arg1, arg2, arg3);
        m_out += _T("\n");
        OutputDebugString(m_out);
    }
private:
    CString m_out;
    char* m_func_name;
    static int s_indent;
};
#if 1
#define TRACE_FUNCTION  CFuncInOut dbg_func(__FUNCTION__);
#define DBG_TRACE0(fmt) dbg_func.Trace0(_T(fmt));
#define DBG_TRACE1(fmt, arg1) dbg_func.Trace1(_T(fmt), arg1);
#define DBG_TRACE2(fmt, arg1, arg2) dbg_func.Trace2(_T(fmt), arg1, arg2);
#define DBG_TRACE3(fmt, arg1, arg2, arg3) dbg_func.Trace3(_T(fmt), arg1, arg2, arg3);
#else
#define TRACE_FUNCTION  
#define DBG_TRACE0(fmt) 
#define DBG_TRACE1(fmt, arg1) 
#define DBG_TRACE2(fmt, arg1, arg2) 
#define DBG_TRACE3(fmt, arg1, arg2, arg3) 
#endif





#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


