#include <D3D12Device.h>

#include <Debug/Assertions.h>

namespace cqe
{
	namespace Render::RHI
	{
		D3D12Device::D3D12Device(D3D12Factory::Ptr factory)
		{
			HRESULT hr = D3D12CreateDevice(
				nullptr,
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&m_NativeDevice));

			if (FAILED(hr))
			{
				RefCountPtr<IDXGIAdapter> warpAdapter;
				hr = factory->GetHandle()->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
				assert(SUCCEEDED(hr));

				hr = D3D12CreateDevice(
					warpAdapter.Get(),
					D3D_FEATURE_LEVEL_11_0,
					IID_PPV_ARGS(&m_NativeDevice));

				assert(SUCCEEDED(hr));
			}

			if (!CheckCompatibility())
			{
				ASSERT_NOT_IMPLEMENTED;
			}
		}

		RenderNativeObject D3D12Device::GetNativeObject()
		{
			return RenderNativeObject(m_NativeDevice.Get());
		}

		RefCountPtr<ID3D12Device> D3D12Device::GetHandle() const
		{
			return m_NativeDevice;
		}

		bool D3D12Device::CheckCompatibility() const
		{
			D3D_FEATURE_LEVEL levels[] =
			{
				D3D_FEATURE_LEVEL_12_2,
				D3D_FEATURE_LEVEL_12_1,
			};
			D3D12_FEATURE_DATA_FEATURE_LEVELS supportedLevels;
			supportedLevels.NumFeatureLevels = sizeof(levels) / sizeof(D3D_FEATURE_LEVEL);
			supportedLevels.pFeatureLevelsRequested = levels;
			m_NativeDevice->CheckFeatureSupport(
				D3D12_FEATURE_FEATURE_LEVELS,
				&supportedLevels,
				sizeof(D3D12_FEATURE_DATA_FEATURE_LEVELS)
			);

			D3D12_FEATURE_DATA_SHADER_MODEL shaderModelFeature{ .HighestShaderModel = D3D_SHADER_MODEL_6_6 };
			HRESULT hr = m_NativeDevice->CheckFeatureSupport(
				D3D12_FEATURE_SHADER_MODEL,
				&shaderModelFeature,
				sizeof(D3D12_FEATURE_DATA_SHADER_MODEL)
			);
			assert(SUCCEEDED(hr));

			return shaderModelFeature.HighestShaderModel >= D3D_SHADER_MODEL_6_6
				&& supportedLevels.MaxSupportedFeatureLevel >= D3D_FEATURE_LEVEL_12_1;
		}
	}
}