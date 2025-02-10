#include <RenderObject.h>
#include <Mesh.h>

namespace cqe::Render
{
	RenderObject::RenderObject(MeshID meshID, Material::ID materialID)
		: m_MeshID(meshID)
		, m_MaterialID(materialID)
	{
		Init();
	}

	RenderObject::RenderObject(std::nullptr_t)
	{
		Init();
	}

	void RenderObject::Init()
	{
		static_assert(std::is_same_v<MeshID, RHI::Mesh::ID> == true, "RHI::Mesh::ID type should be the same as ID type in RenderObject");
	}

	RenderObject::~RenderObject()
	{

	}
}