#include <RenderCommands.h>
#include <RenderEngine.h>
#include <RenderThread.h>

using namespace cqe::Render;

namespace
{
	// Function to run render thread
	static void RunThisThread(void* thisPtr)
	{
		RenderThread* const self = reinterpret_cast<RenderThread*>(thisPtr);
		self->Run();
	}
}

namespace cqe::Render
{
	RenderThread::RenderThread()
	{
		assert(m_CurMainFrame == 0);

		s_MainThreadId = std::this_thread::get_id();

		m_FrameMutexes[m_CurMainFrame].lock();

		m_Thread = std::make_unique<std::jthread>(RunThisThread, this);
		m_Thread->detach();
	}

	RenderThread::~RenderThread()
	{
		m_FrameMutexes[m_CurMainFrame].unlock();
		m_IsRunning = false;

		while (m_IsRendering)
		{

		}

		delete m_RenderEngine;
	}

	// Render Loop
	void RenderThread::Run()
	{
		s_RenderThreadId = std::this_thread::get_id();

		m_RenderEngine = new RenderEngine();

		m_RenderEngineIsReady.release();

		m_IsRunning = true;

		while (m_IsRunning)
		{
			m_IsRendering = true;
			std::lock_guard<std::mutex> lock(m_FrameMutexes[m_CurrRenderFrame]);

			ProcessCommands();

			m_RenderEngine->Update(m_CurrRenderFrame);

			m_RenderEngine->OnEndFrame();

			OnEndFrame();
			m_IsRendering = false;
		}
	}

	bool RenderThread::IsRenderThread()
	{
		return s_RenderThreadId == std::this_thread::get_id();
	}

	template<typename... Args>
	void RenderThread::EnqueueCommand(ERC command, Args... args)
	{
		switch (command)
		{
		case ERC::CreateRenderObject:
			m_commands[m_CurMainFrame].push_back(
				new EnqueuedRenderCommand(
					[this](RenderCore::Geometry* geometry, RenderObject* renderObject) { m_RenderEngine->CreateRenderObject(geometry, renderObject); },
					std::forward<Args>(args)...)
			);
			break;
		default:
			assert(0);
			break;
		}

		if (IsRenderThread())
		{
			RenderCommand* renderCommand = m_commands[m_CurMainFrame].back();
			renderCommand->DoTask();
			delete renderCommand;
			m_commands[m_CurMainFrame].pop_back();
		}
	}

	void RenderThread::ProcessCommands()
	{
		for (auto& command : m_commands[m_CurrRenderFrame])
		{
			command->DoTask();
		}

		m_commands[m_CurrRenderFrame].clear();
	}

	void RenderThread::OnEndFrame()
	{
		SwitchFrame();
	}

	void RenderThread::SwitchFrame()
	{
		if (IsRenderThread())
		{
			m_CurrRenderFrame = GetNextFrame(m_CurrRenderFrame);
		}
		else
		{
			m_FrameMutexes[m_CurMainFrame].unlock();

			m_CurMainFrame = GetNextFrame(m_CurMainFrame);

			m_FrameMutexes[m_CurMainFrame].lock();
		}
	}

	size_t RenderThread::GetNextFrame(size_t frameNumber) const
	{
		return (frameNumber + 1) % RenderCore::g_FrameBufferCount;
	}

	RenderEngine* RenderThread::GetRenderEngine()
	{
		return m_RenderEngine;
	}

	void RenderThread::WaitForRenderEngineToInit()
	{
		m_RenderEngineIsReady.acquire();
	}
}