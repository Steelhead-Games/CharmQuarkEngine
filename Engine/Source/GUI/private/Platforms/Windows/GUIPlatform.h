#pragma once

#include <GUI/export.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>

#include <Window/IWindow.h>
#include <PackedVariables.h>

#include <imgui.h>
#include <backends/imgui_impl_win32.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace cqe
{
	namespace Render::RHI
	{
		enum class Type : uint8_t;
		class Context;
	}

	namespace GUI
	{
		class GUI_API GUIContextPlatform final
		{
		public:
			GUIContextPlatform() = delete;
			GUIContextPlatform(GUIContextPlatform&) = delete;
			GUIContextPlatform operator=(GUIContextPlatform&) = delete;

		public:
			static void Init();
			static void Deinit();
			static void InitRenderBackend(std::shared_ptr<Render::RHI::Context> rhiContext);
			static void DeinitRenderBackend(std::shared_ptr<Render::RHI::Context> rhiContext);
			static bool UpdateInput(Core::PackedVariables& arguments);
			static void NewFrame();
			static void Render(ImDrawData* drawData);
		};
	}
}