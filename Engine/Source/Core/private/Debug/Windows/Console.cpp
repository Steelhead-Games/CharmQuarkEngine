#include <Debug/Console.h>
#include <windows.h>

namespace cqe::Core
{
	void Console::PrintDebug(const std::string& message)
	{
#ifdef DEBUG
		OutputDebugStringA(message.c_str());
#endif
	}
}