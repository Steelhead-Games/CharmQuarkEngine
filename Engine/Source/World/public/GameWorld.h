#pragma once

#include <flecs.h>
#include <World/export.h>

#include <Level.h>

namespace cqe::World
{
	class WORLD_API GameWorld final
	{
	public:
		static GameWorld* GetInstance();

	public:
		GameWorld();
		~GameWorld() = default;

		void LoadLevel(flecs::world& flecs, const std::string& levelPath);
		const Level& GetCurrentLevel() const { assert(m_CurrentLevel.has_value()); return m_CurrentLevel.value(); }

	private:
		void LoadCurrentLevel();
		void UnloadCurrentLevel();

	private:
		flecs::world m_World;

		std::optional<Level> m_CurrentLevel = std::nullopt;

		static std::unique_ptr<GameWorld> m_Instance;
	};
}