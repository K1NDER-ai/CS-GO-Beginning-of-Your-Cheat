#pragma once
#include <Windows.h>
#include  <d3d9.h>

#include "Impl.h"

typedef HRESULT(APIENTRY* PresentFn)(IDirect3DDevice9 *, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);
typedef HRESULT(APIENTRY *ResetFn)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
 
#include <process.h>
#include "Hook\hook.h"   
cDetour<PresentFn>* oPresent;
cDetour<ResetFn>* oReset;
 





      