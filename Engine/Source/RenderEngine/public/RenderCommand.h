#pragma once

#include <RenderEngine/export.h>

namespace cqe::Render
{
	class RENDER_ENGINE_API RenderCommand final
	{
	public:
		using Task = std::function<void()>;
		using Ptr = std::unique_ptr<RenderCommand>;

	public:
		RenderCommand() = delete;

		RenderCommand(Task&& renderTask) :
			m_Task(std::forward<Task>(renderTask))
		{
		}

		void DoTask()
		{
			m_Task();
		}

	private:
		Task m_Task;
	};
}