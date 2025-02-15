#include <GUIContext.h>
#include <GUIPlatform.h>
#include <RHIContext.h>

namespace cqe::GUI
{
	GUIContext* GUIContext::m_Instance = nullptr;

	GUIContext* const GUIContext::GetInstance()
	{
		if (m_Instance == nullptr) [[unlikely]]
		{
			m_Instance = new GUIContext();
		}

		return m_Instance;
	}

	void GUIContext::PlatformInit()
	{
		GUIContextPlatform::Init();
	}

	void GUIContext::PlatformDeinit()
	{
		GUIContextPlatform::Deinit();

		delete m_Instance;
	}

	void GUIContext::InitRenderBackend(std::shared_ptr<Render::RHI::Context> rhiContext)
	{
		GUIContextPlatform::InitRenderBackend(rhiContext);
	}

	void GUIContext::DeinitRenderBackend(std::shared_ptr<Render::RHI::Context> rhiContext)
	{
		GUIContextPlatform::DeinitRenderBackend(rhiContext);
	}

	bool GUIContext::UpdateInput(Core::PackedVariables& arguments)
	{
		return GUIContextPlatform::UpdateInput(arguments);
	}

	void GUIContext::NewFrame()
	{
		GUIContextPlatform::NewFrame();
		ImGui::NewFrame();
	}

	ImGuiContext* GUIContext::GetImGuiContext()
	{
		return ImGui::GetCurrentContext();
	}

	void GUIContext::Render()
	{
		if (m_DrawData.size() > 0) [[likely]]
		{
			GUIContextPlatform::Render(m_DrawData.front());

			// Does it clear and deinit the memory correctly?
			m_DrawData.front()->Clear();
			m_DrawData.pop();
		}
	}

	bool GUIContext::IsUIHovered() const
	{
		return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
	}

	void GUIContext::UnFocus() const
	{
		ImGui::SetWindowFocus(nullptr);
	}

	void GUIContext::Draw()
	{
		ImGui::Render();

		ImDrawData* renderData = new ImDrawData(*ImGui::GetDrawData());

		for (uint32_t i = 0; i < ImGui::GetDrawData()->CmdListsCount; ++i)
		{
			renderData->CmdLists[i] = ImGui::GetDrawData()->CmdLists[i]->CloneOutput();
		}

		m_DrawData.push(renderData);
	}
}