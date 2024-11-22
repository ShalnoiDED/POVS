#include "Tool.h"

void Tool::addTool(std::any tool)
{
	tools.push_back(tool);
}

void Tool::removeTool(int id)
{
	tools.erase(tools.begin() + id);
}
