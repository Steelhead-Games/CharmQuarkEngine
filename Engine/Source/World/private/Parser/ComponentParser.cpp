#include <Input/Controller.h>
#include <Parser/ComponentParser.h>

namespace cqe::World
{
	std::unordered_map<ComponentParser::ComponentName, ComponentParser::ComponentConstructor> ComponentParser::s_CustomComponentConstructors =
	{
		{"ControllerPtr", [](const ComponentValue& value) { return reinterpret_cast<uint64_t>(new Core::Controller(Core::g_FileSystem->GetConfigPath(value))); }},
	};

	uint64_t ComponentParser::Parse(const ComponentName& name, const ComponentValue& value)
	{
		assert(s_CustomComponentConstructors.contains(name));

		return s_CustomComponentConstructors[name](value);
	}
}