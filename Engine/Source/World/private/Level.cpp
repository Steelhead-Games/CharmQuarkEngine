#include <Level.h>

namespace cqe::World
{
	Level::Level(std::string& name)
		: m_Name(std::move(name))
	{
	}

	void Level::AddLevelObject(const LevelObject& levelObject)
	{
		m_LevelObjectList.push_back(levelObject);
	}
}