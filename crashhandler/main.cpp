#include "cInclude.h"
#include <cstdint>
#include <string>
bool Create = false;

// Тут и будет проходить наш рендер на экран
HRESULT APIENTRY myPresent(IDirect3DDevice9* m_pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	if (Create == false)
	{
		ImGui_Init(hWnd, m_pDevice);
		Create = true;
	}
	else
	{
		ImGui_NewFrame();
		if (bShowWindow)
		{
			// Рисуем ImGui
			// И ВООБЩЕ ЧТО ЛИБО РИСУЕТСЯ ТУТ
			DWORD dwFlag = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoSavedSettings;
			ImGui::Begin(u8"Консоль разработчика", &bShowWindow, ImVec2(450, 400), 1.0f, dwFlag);
			{
				ImGui::Text(u8"Информация о модулях:");
				ImGui::Text(u8"client_panorama.dll - успешно загружен.");
				ImGui::Text(u8"engine.dll - успешно загружен.");
				ImGui::Text(u8"shaderapidx9.dll - успешно загружен.");
			}
			ImGui::End();
		}
		ImGui::Render();
	}
	return oPresent->GetTrampoline()(m_pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

#include "cFunction.h"

DWORD WINAPI goto_start(LPVOID base)
{
	// Проверяем загрузились ли либы игры, подстраивайте под свою игру
	if (CheckModules(L"client_panorama.dll") && CheckModules(L"engine.dll") && CheckModules(L"shaderapidx9.dll") == true)
	{
		// Название процесса игры
		hWnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");
		if (hWnd)
			m_pWindowProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)myWndProc);

		// Sleep, чтобы процессор не нагружался.
		while (!Init())
			Sleep(200);
	}
	return 0L;
}

// Если ты не знаешь, что это, то закрой проект, пожалуйста.
BOOL APIENTRY DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved) 
{
	DisableThreadLibraryCalls(hinstDLL);
	switch (fdwReason) 
	{
		case DLL_PROCESS_ATTACH: 
		{
			CreateThread(nullptr, 0, goto_start, hinstDLL, 0, nullptr);
			break;
		}
		case DLL_THREAD_ATTACH:
		{
			break;
		}
		case DLL_THREAD_DETACH:
		{
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)m_pWindowProc);
			break;
		}
	}
	return TRUE;
}