#pragma once

#include <GUI/export.h>

#include <PackedVariables.h>

struct ImGuiContext;
struct ImDrawData;

namespace cqe
{
	namespace Render::RHI
	{
		enum class Type : uint8_t;
		class Context;
	}

	namespace GUI
	{
		class GUI_API GUIContext final
		{
		public:
			static GUIContext* const GetInstance();

		public:
			void PlatformInit();
			void InitRenderBackend(std::shared_ptr<Render::RHI::Context> rhiContext);
			bool UpdateInput(Core::PackedVariables& arguments);
			bool IsUIHovered() const;
			void UnFocus() const;

			void NewFrame();

			// Is called from Render thread to render the generated UI
			void Render();

			// Is called when we generated the UI and ready to render it
			void Draw();

			ImGuiContext* GetImGuiContext();

		private:
			GUIContext() = default;

		private:
			static GUIContext* m_Instance;

		private:
			std::queue<ImDrawData*> m_DrawData;
		};
	}
}