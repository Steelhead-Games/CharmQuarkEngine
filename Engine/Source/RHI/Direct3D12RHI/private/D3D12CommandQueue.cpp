#include <D3D12CommandQueue.h>

namespace cqe
{
	namespace Render::RHI
	{
		D3D12CommandQueue::D3D12CommandQueue(D3D12Device::Ptr device)
		{
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			HRESULT hr = device->GetHandle()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_NativeCommandQueue));
			
			assert(SUCCEEDED(hr));
		}

		void D3D12CommandQueue::ExecuteCommandLists(const std::vector<CommandList::Ptr>& cmdLists)
		{
			std::vector<ID3D12CommandList*> d3d12CmdLists(cmdLists.size());

			for (size_t index = 0; index < cmdLists.size(); ++index)
			{
				d3d12CmdLists[index] = (ID3D12CommandList*)(cmdLists[index]->GetNativeObject());
			}

			m_NativeCommandQueue->ExecuteCommandLists(d3d12CmdLists.size(), d3d12CmdLists.data());
		}

		RenderNativeObject D3D12CommandQueue::GetNativeObject()
		{
			return RenderNativeObject(m_NativeCommandQueue.Get());
		}

		RefCountPtr<ID3D12CommandQueue> D3D12CommandQueue::GetHandle() const
		{
			return m_NativeCommandQueue;
		}
	}
}