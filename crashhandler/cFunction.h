#pragma once

HRESULT APIENTRY myReset(IDirect3DDevice9* m_pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (!Create)
		return m_pDevice->Reset(pPresentationParameters);

	ImGui_InvalidateDeviceObjects();

	auto   result = oReset->GetTrampoline()(m_pDevice, pPresentationParameters);

	ImGui_CreateDeviceObjects();
	return result;
}

bool Init()
{
	bool    bResult = false;
	HMODULE hD3d9 = NULL;
	if (hD3d9 = GetModuleHandleA("d3d9.dll"))
	{
		using oDirect3DCreate9Ex = HRESULT(WINAPI*)(UINT, IDirect3D9Ex**);
		oDirect3DCreate9Ex pDirect3DCreate9Ex = (oDirect3DCreate9Ex)GetProcAddress(hD3d9, "Direct3DCreate9Ex");
		if (pDirect3DCreate9Ex)
		{
			HRESULT hr = D3D_OK;
			LPDIRECT3D9EX d3d9ex = nullptr;
			if (SUCCEEDED(hr = pDirect3DCreate9Ex(D3D_SDK_VERSION, &d3d9ex)))
			{
				D3DPRESENT_PARAMETERS dp;
				ZeroMemory(&dp, sizeof(dp));
				dp.Windowed = 1;
				dp.SwapEffect = D3DSWAPEFFECT_FLIP;
				dp.BackBufferFormat = D3DFMT_A8R8G8B8;
				dp.BackBufferCount = 1;
				dp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

				IDirect3DDevice9Ex* mDevice = nullptr;
				if (SUCCEEDED(hr = d3d9ex->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dp, NULL, &mDevice)))
				{
					bResult = true;
					PVOID* vtbl = *reinterpret_cast<PVOID**>(mDevice);
					auto& pContext = cContext::GetInstance();
					pContext.ApplyDetour<PresentFn>(static_cast<PresentFn>(vtbl[17]), reinterpret_cast<PresentFn>(myPresent), &oPresent);
					pContext.ApplyDetour<ResetFn>(static_cast<ResetFn>(vtbl[16]), reinterpret_cast<ResetFn>(myReset), &oReset);

					mDevice->Release();

				}
				d3d9ex->Release();
			}
		}
	}
	ImFontConfig font_config;
	font_config.OversampleH = 1; 
	font_config.OversampleV = 1;
	font_config.PixelSnapH = 1;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, 
		0x0400, 0x044F, 
		0,
	};
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 14.0f, &font_config, ranges);
	return bResult;
}

int CheckModules(LPCWSTR modules)
{
	bool success = false;
	while (true)
	{
		if (GetModuleHandleW(modules) != NULL)
		{
			success = true;
			goto exit;
		}
	}
exit:
	return success;
}

