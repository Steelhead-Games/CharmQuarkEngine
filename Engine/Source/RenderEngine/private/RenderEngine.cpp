#include <Camera.h>
#include <Constants.h>
#include <RenderEngine.h>
#include <RenderObject.h>
#include <RHICore.h>
#include <RHIHelper.h>
#include <Material.h>
#include <Mesh.h>
#include <Texture.h>
#include <Technique.h>
#include <Buffer.h>
#include <PipelineStateObject.h>

#include <GUIContext.h>

namespace cqe::Render
{
	namespace
	{
		// TemporalResources supposed to be as a part of Render Pass
		struct TemporalResources
		{
			struct ObjectConstants
			{
				Math::Matrix4x4f WorldViewProj = Math::Matrix4x4f::Identity();
			};

			struct MaterialConstants
			{
				Math::Vector4f Color = Math::Vector4f::Zero();
			};

			struct Vertex
			{
				Math::Vector3f Pos;
			};

			RHI::Buffer::Ptr ObjectCB[RenderCore::g_FrameBufferCount];
			RHI::Buffer::Ptr MaterialCB[RenderCore::g_FrameBufferCount];
			RHI::Technique::Ptr Technique;
			RHI::PipelineStateObject::Ptr PSO;
			RHI::Texture::Ptr DepthStencil;
			std::vector<RHI::Mesh::Ptr> Meshes;
			std::vector<Material*> Materials;
		};

		std::unique_ptr<TemporalResources> m_RenderPassResources;
	}

	RenderEngine::RenderEngine()
	{
		m_rhi = RHI::Helper::CreateRHI("D3D12");

		m_RenderPassResources = std::make_unique<TemporalResources>();

		OnResize();

		GUI::GUIContext::GetInstance()->InitRenderBackend(m_rhi);

		for (int i = 0; i < RenderCore::g_FrameBufferCount; ++i)
		{
			m_RenderPassResources->ObjectCB[i] = m_rhi->CreateBuffer(
				{
					.Count = RenderCore::g_MaximumRenderObjectCount,
					.ElementSize = sizeof(TemporalResources::ObjectConstants),
					.UsageFlag = RHI::Buffer::UsageFlag::ConstantBuffer
				}
			);

			m_RenderPassResources->MaterialCB[i] = m_rhi->CreateBuffer(
				{
					.Count = RenderCore::g_MaximumRenderObjectCount,
					.ElementSize = sizeof(TemporalResources::MaterialConstants),
					.UsageFlag = RHI::Buffer::UsageFlag::ConstantBuffer
				}
			);
		}

		RHI::Technique::ShaderInfo shaderInfo =
		{
			{
				.Type = RHI::Technique::ShaderInfoDescription::ShaderType::VertexShader,
				.ShaderFile = "Object.hlsl",
				.EntryPoint = "VS",
			},
			{
				.Type = RHI::Technique::ShaderInfoDescription::ShaderType::PixelShader,
				.ShaderFile = "Object.hlsl",
				.EntryPoint = "PS",
			}
		};

		RHI::Technique::InputLayout inputLayout =
		{
			{
				.SemanticName = "POSITION",
				.Index = 0,
				.Format = RHI::ResourceFormat::RGB32_FLOAT,
				.InputSlot = 0,
				.InputSlotClass = RHI::Technique::InputLayoutDescription::Classification::PerVertex,
				.InstanceDataStepRate = 0
			}
		};

		RHI::Technique::RootSignatureDescription rootSignature =
		{
			.Type = "BasicRootSignature",
			.SlotIndex = 0,
			.SpaceIndex = 0,
			.IsConstantBuffer = true,
		};

		m_RenderPassResources->Technique = m_rhi->CreateTechnique(shaderInfo, inputLayout, rootSignature);

		RHI::PipelineStateObject::Description desc;
		desc.Technique = m_RenderPassResources->Technique;
		desc.NumRenderTargets = 1;
		desc.RTVFormats[0] = m_rhi->GetSwapChain()->GetCurrentBackBuffer()->GetFormat();
		desc.DSVFormat = m_RenderPassResources->DepthStencil->GetFormat();

		m_RenderPassResources->PSO = m_rhi->CreatePSO(desc);

		m_rhi->GetCommandList()->Close();
		m_rhi->GetCommandQueue()->ExecuteCommandLists({ m_rhi->GetCommandList() });

		m_rhi->GetFence()->Sync(m_rhi->GetCommandQueue());
	}

	RenderEngine::~RenderEngine()
	{
		for (RenderObject* renderObject : m_RenderObjects)
		{
			delete renderObject;
		}

		for (Material* material : m_RenderPassResources->Materials)
		{
			delete material;
		}

		m_RenderPassResources = nullptr;
		GUI::GUIContext::GetInstance()->DeinitRenderBackend(m_rhi);
	}

	void RenderEngine::Update(size_t frame)
	{
		m_rhi->GetCommandList()->GetAllocator()->Reset();

		m_rhi->GetCommandList()->Reset();

		m_rhi->SetDescriptorHeaps();

		OnResize();

		m_rhi->GetCommandList()->SetPipelineStateObject(m_RenderPassResources->PSO);

		RHI::Rect scissorRect(0, Core::g_MainWindowsApplication->GetWidth(), 0, Core::g_MainWindowsApplication->GetHeight());

		RHI::Viewport viewport(
			0.f, static_cast<float>(Core::g_MainWindowsApplication->GetWidth()),
			0.f, static_cast<float>(Core::g_MainWindowsApplication->GetHeight()),
			0.f, 1.f
		);

		m_rhi->GetCommandList()->SetViewport(viewport);
		m_rhi->GetCommandList()->SetScissorRect(scissorRect);

		m_rhi->GetCommandList()->ClearRenderTarget(m_rhi->GetSwapChain()->GetCurrentBackBuffer(), RenderCore::Colors::LightSteelBlue);
		m_rhi->GetCommandList()->ClearDepthStencilView(m_RenderPassResources->DepthStencil, RHI::ClearFlags::Depth | RHI::ClearFlags::Stencil, 1.0f, 0);

		m_rhi->GetCommandList()->SetRenderTargets(1, m_rhi->GetSwapChain()->GetCurrentBackBuffer(), m_RenderPassResources->DepthStencil);

		m_rhi->GetCommandList()->SetTechnique(m_RenderPassResources->Technique);

		for (RenderObject* renderObject : m_RenderObjects)
		{
			assert(renderObject);

			//Draw
			RHI::Mesh::ID meshID = renderObject->GetMeshID();
			Material::ID materialID = renderObject->GetMaterialID();

			assert(meshID >= 0);
			assert(meshID < m_RenderPassResources->Meshes.size());
			assert(meshID != RenderObject::k_invalidMeshID);
			assert(materialID >= 0);
			assert(materialID < m_RenderPassResources->Materials.size());
			assert(materialID != RenderObject::k_invalidMaterialID);

			// Projection and view matrices should be a part of Camera class
			Math::Matrix4x4f view = Core::g_MainCamera->GetViewMatrix();
			Math::Matrix4x4f proj = Math::ProjectionMatrixLH(0.25f * Math::Constants::PI, Core::g_MainWindowsApplication->GetAspectRatio(), 1.0f, 1000.0f);

			Math::Vector3f position = renderObject->GetPosition(frame);
			Math::Matrix4x4f world = Math::Matrix4x4f::Identity();
			world.SetElement(position.x, 3, 0);
			world.SetElement(position.y, 3, 1);
			world.SetElement(position.z, 3, 2);
			Math::Matrix4x4f worldViewProj = world * view * proj;

			TemporalResources::ObjectConstants objConstants;
			objConstants.WorldViewProj = worldViewProj.Transpose();
			m_RenderPassResources->ObjectCB[m_rhi->GetSwapChain()->GetCurrentBackBufferIdx()]->CopyData(meshID, &objConstants, sizeof(TemporalResources::ObjectConstants));

			Material* material = m_RenderPassResources->Materials[materialID];
			TemporalResources::MaterialConstants matConstants;
			matConstants.Color = material->GetAlbedo();
			m_RenderPassResources->MaterialCB[m_rhi->GetSwapChain()->GetCurrentBackBufferIdx()]->CopyData(materialID, &matConstants, sizeof(TemporalResources::MaterialConstants));

			m_rhi->GetCommandList()->SetMesh(m_RenderPassResources->Meshes[meshID]);
			m_rhi->GetCommandList()->SetPrimitiveTopology(RHI::PrimitiveTopology::TriangleList);

			m_rhi->GetCommandList()->SetGraphicsConstantBuffer(0, m_RenderPassResources->ObjectCB[m_rhi->GetSwapChain()->GetCurrentBackBufferIdx()], meshID);
			m_rhi->GetCommandList()->SetGraphicsConstantBuffer(1, m_RenderPassResources->MaterialCB[m_rhi->GetSwapChain()->GetCurrentBackBufferIdx()], materialID);

			m_rhi->GetCommandList()->DrawIndexedInstanced(
				m_RenderPassResources->Meshes[meshID]->GetIndexBuffer()->GetDesc().Count,
				1, 0, 0, 0);
		}
	}

	void RenderEngine::OnEndFrame()
	{
		GUI::GUIContext::GetInstance()->Render();

		m_rhi->GetSwapChain()->MakeBackBufferPresentable(m_rhi->GetCommandList());

		m_rhi->GetCommandList()->Close();
		m_rhi->GetCommandQueue()->ExecuteCommandLists({ m_rhi->GetCommandList() });

		m_rhi->GetSwapChain()->Present();

		m_rhi->GetFence()->Sync(m_rhi->GetCommandQueue());
	}

	void RenderEngine::OnResize()
	{
		if (m_rhi->GetSwapChain()->GetCurrentBackBuffer()->GetWidth() != Core::g_MainWindowsApplication->GetWidth() ||
			m_rhi->GetSwapChain()->GetCurrentBackBuffer()->GetHeight() != Core::g_MainWindowsApplication->GetHeight()) [[unlikely]]
		{
			assert(m_rhi->GetCommandList()->GetAllocator());

			m_rhi->GetFence()->Sync(m_rhi->GetCommandQueue());

			m_rhi->GetSwapChain()->Resize(
				m_rhi->GetDevice(),
				Core::g_MainWindowsApplication->GetWidth(),
				Core::g_MainWindowsApplication->GetHeight()
			);

			m_RenderPassResources->DepthStencil.Reset();

			RHI::Texture::Description description;
			description.Dimension = RHI::Texture::Dimensions::Two;
			description.Width = Core::g_MainWindowsApplication->GetWidth();
			description.Height = Core::g_MainWindowsApplication->GetHeight();
			description.Format = RHI::ResourceFormat::D24S8;
			description.Flags = RHI::Texture::UsageFlags::DepthStencil;

			m_RenderPassResources->DepthStencil = m_rhi->CreateTexture(description);

			m_rhi->GetFence()->Sync(m_rhi->GetCommandQueue());
		}
	}

	void RenderEngine::CreateRenderObject(RenderCore::Geometry* geometry, RenderObject* renderObject)
	{
		assert(geometry);
		assert(renderObject);

		RHI::Mesh::ID meshID = m_RenderPassResources->Meshes.size();
		RHI::Mesh* mesh = m_rhi->CreateMesh(
			{
				.Count = geometry->GetVertexCount(),
				.ElementSize = sizeof(RenderCore::Geometry::VertexType),
				.initData = geometry->GetVertices()
			},
			{
				.Count = geometry->GetIndexCount(),
				.Format = RHI::ResourceFormat::R16_UINT,
				.initData = geometry->GetIndices()
			});

			m_RenderPassResources->Meshes.push_back(mesh);

			Material::ID materialID = m_RenderPassResources->Materials.size();
			Material* material = new Material(materialID);
			m_RenderPassResources->Materials.push_back(material);

			renderObject->SetMeshID(meshID);
			renderObject->SetMaterialID(materialID);
			m_RenderObjects.push_back(renderObject);
	}
}