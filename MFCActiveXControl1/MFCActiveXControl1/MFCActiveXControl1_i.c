

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Wed Apr 03 23:18:05 2013
 */
/* Compiler settings for MFCActiveXControl1.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_MFCActiveXControl1Lib,0xCD0E8D21,0xD5B7,0x4574,0xAF,0x1C,0x6D,0x05,0xFD,0xEB,0x56,0xB8);


MIDL_DEFINE_GUID(IID, DIID__DMFCActiveXControl1,0x128B1BE4,0xF9BF,0x4BF4,0xB9,0x11,0xF2,0x28,0x32,0x7A,0x5B,0x64);


MIDL_DEFINE_GUID(IID, DIID__DMFCActiveXControl1Events,0x8CB960EB,0x3788,0x4DF0,0x9E,0x15,0x07,0xEC,0xA0,0xBA,0x75,0xD5);


MIDL_DEFINE_GUID(CLSID, CLSID_MFCActiveXControl1,0x046DEA3C,0x55C0,0x4C75,0x82,0x9E,0x94,0xC5,0x51,0x05,0xF3,0xB3);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



