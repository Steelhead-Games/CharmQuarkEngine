#include <GUIPlatform.h>
#include <RHIContext.h>
#include <RHIHelper.h>
#include <D3D12GUIRenderBackend.h>
#include <PackedVariables.h>

#include <imgui.h>
#include <backends/imgui_impl_win32.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace cqe
{
	namespace GUI
	{
		namespace
		{
			ImGuiContext* g_Context = nullptr;
		}

		void GUIContextPlatform::Init()
		{
			IMGUI_CHECKVERSION();
			g_Context = ImGui::CreateContext();
			ImGui::SetCurrentContext(g_Context);
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

			ImGui::StyleColorsDark();

			ImGui_ImplWin32_Init(cqe::Core::g_MainWindowsApplication->GetWindowHandle());
		}

		void GUIContextPlatform::Deinit()
		{
			ImGui_ImplWin32_Shutdown();
			D3D12RenderBackend::Deinit();
			ImGui::DestroyContext(g_Context);
		}

		void GUIContextPlatform::InitRenderBackend(std::shared_ptr<Render::RHI::Context> rhiContext)
		{
			switch (Render::RHI::Helper::GetRHIType())
			{
			case Render::RHI::Type::D3D12:
				D3D12RenderBackend::Init(rhiContext);
				break;
			default:
				assert(false && "Couldn't init the GUI");
				break;
			}
		}

		void GUIContextPlatform::DeinitRenderBackend(std::shared_ptr<Render::RHI::Context> rhiContext)
		{
			switch (Render::RHI::Helper::GetRHIType())
			{
			case Render::RHI::Type::D3D12:
				D3D12RenderBackend::Deinit(rhiContext);
				break;
			default:
				assert(false && "Couldn't deinit the GUI");
				break;
			}
		}

		bool GUIContextPlatform::UpdateInput(Core::PackedVariables& arguments)
		{
			HWND hWnd; UINT msg; WPARAM wParam; LPARAM lParam;
			arguments.Decode(hWnd, msg, wParam, lParam);
			return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam) != 0;
		}

		void GUIContextPlatform::NewFrame()
		{
			ImGui_ImplWin32_NewFrame();

			switch (Render::RHI::Helper::GetRHIType())
			{
			case Render::RHI::Type::D3D12:
				D3D12RenderBackend::NewFrame();
				break;
			default:
				assert(false && "Couldn't acquire the new frame");
				break;
			}
		}

		void GUIContextPlatform::Render(ImDrawData* drawData)
		{
			switch (Render::RHI::Helper::GetRHIType())
			{
			case Render::RHI::Type::D3D12:
				D3D12RenderBackend::Render(drawData);
				break;
			default:
				assert(false && "Couldn't render GUI");
				break;
			}
		}
	}
}