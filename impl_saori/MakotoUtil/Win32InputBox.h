#ifndef __03022006__WIN32INPUTBOX__
#define __03022006__WIN32INPUTBOX__

/*

This library is (c) Elias Bachaalany aka lallous <lallousx86@yahoo.com>
You may use this library under the following license agreement:

The zlib/libpng License.
---------------------------
This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, 
and to alter it and redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; 
   you must not claim that you wrote the original software. 
   If you use this software in a product, an acknowledgment in the product 
   documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, 
   and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution. 

*/

#include <windows.h>
#include <tchar.h>

// Use alignment if supported by the compiler
#ifdef _MSC_VER
  #if _MSC_VER > 1200
    __declspec(align(4)) 
  #endif
#endif

// per the MSDN, the DLGTEMPLATE must be DWORD aligned
// this was generated by the DlgResToDlgTemplate tool
static unsigned char definputbox_dlg[] =
{
  0x01,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc8,0x00,0xc8,0x00,0x06,
    0x00,0x16,0x00,0x11,0x00,0xe7,0x00,0x6d,0x00,0x00,0x00,0x00,0x00,0x57,0x00,0x69,
    0x00,0x6e,0x00,0x33,0x00,0x32,0x00,0x49,0x00,0x6e,0x00,0x70,0x00,0x75,0x00,0x74,
    0x00,0x42,0x00,0x6f,0x00,0x78,0x00,0x00,0x00,0x08,0x00,0xbc,0x02,0x00,0x00,0x4d,
    0x00,0x53,0x00,0x20,0x00,0x53,0x00,0x68,0x00,0x65,0x00,0x6c,0x00,0x6c,0x00,0x20,
    0x00,0x44,0x00,0x6c,0x00,0x67,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x80,0x00,0x02,0x50,0x06,0x00,0x04,0x00,0x9d,0x00,0x21,0x00,0xe8,
    0x03,0x00,0x00,0xff,0xff,0x82,0x00,0x50,0x00,0x72,0x00,0x6f,0x00,0x6d,0x00,0x70,
    0x00,0x74,0x00,0x3a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x80,0x00,0x81,0x50,0x06,0x00,0x25,0x00,0xd8,0x00,0x0e,0x00,0xe9,
    0x03,0x00,0x00,0xff,0xff,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x84,0x10,0xa1,0x50,0x06,0x00,0x37,0x00,0xd8,0x00,0x31,0x00,0xea,
    0x03,0x00,0x00,0xff,0xff,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x01,0x00,0x03,0x50,0xab,0x00,0x04,0x00,0x33,0x00,0x0e,0x00,0x01,
    0x00,0x00,0x00,0xff,0xff,0x80,0x00,0x4f,0x00,0x4b,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x50,0xab,0x00,0x15,0x00,0x33,
    0x00,0x0e,0x00,0x02,0x00,0x00,0x00,0xff,0xff,0x80,0x00,0x43,0x00,0x41,0x00,0x4e,
    0x00,0x43,0x00,0x45,0x00,0x4c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x02,0x40,0x00,0x00,0x27,0x00,0x08,0x00,0x08,0x00,0xff,
    0xff,0xff,0xff,0xff,0xff,0x82,0x00,0x00,0x00,0x00,0x00
};

class CWin32InputBox;

// Structure used to orient the inputbox behavior
struct WIN32INPUTBOX_PARAM
{
  friend class CWin32InputBox;

  //
  IN OPTIONAL bool bMultiline;

  // Pass this as none zero so to use this memory dlg template
  IN OPTIONAL LPVOID DlgTemplateData;

  // Pass this as none ZERO so to load DLGTEMPLATE from resources
  IN OPTIONAL LPCTSTR DlgTemplateName;

  // passing both "DlgTemplateName" and "DlgTemplateData" ZERO will cause
  // the dialog to use his default embedded resource

  // Center on monitor or owner window?
  IN OPTIONAL bool bCenter;

  // Want to add more styles to the dialog?
  IN OPTIONAL DWORD dwStylesPlus, dwStylesMinus;
  IN OPTIONAL DWORD dwExStylesPlus, dwExStylesMinus;

  IN LPCTSTR szTitle, szPrompt;

  // Return buffer
  OUT LPTSTR szResult;
  IN DWORD nResultSize;

  // Owner window
  HWND hwndOwner;
  HINSTANCE hInstance;

  short xPos, yPos;

  WIN32INPUTBOX_PARAM();
private:
  HWND hDlg;
};

class CWin32InputBox
{
private:
  WIN32INPUTBOX_PARAM *_param;
  static LRESULT CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
  HWND _hwndEditCtrl;

  void InitDialog();
  void SetParam(WIN32INPUTBOX_PARAM *);
  WIN32INPUTBOX_PARAM * GetParam();

public:

  CWin32InputBox(WIN32INPUTBOX_PARAM *);
  ~CWin32InputBox();

  static INT_PTR InputBoxEx(WIN32INPUTBOX_PARAM *);
  static INT_PTR InputBox(
    LPCTSTR szTitle, 
    LPCTSTR szPrompt, 
    LPTSTR szResult, 
    DWORD nResultSize,
    int flags = 0,
    HWND hwndParent = 0);
};

#endif