#include <RHIHelper.h>
#include <D3D12Context.h>

namespace cqe
{
	namespace Render::RHI
	{
		Type Helper::s_rhiType = Type::D3D12;

		Context::Ptr Helper::CreateRHI(const std::string& RHIName)
		{
			const auto& it = k_RHITypeMap.find(RHIName);

			assert(it != k_RHITypeMap.end());

			s_rhiType = it->second;

			switch (s_rhiType)
			{
			case Type::D3D12:
				return std::make_shared<D3D12Context>();
			default:
				assert(false && std::format("{} is not supported on the current OS", RHIName).c_str());
				return std::make_shared<D3D12Context>();
			}
		}

		Type Helper::GetRHIType()
		{
			return s_rhiType;
		}
	}
}