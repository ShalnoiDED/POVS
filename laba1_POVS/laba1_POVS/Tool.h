#ifndef TOOL_H
#define TOOL_H

#include <vector>
#include <string>
#include "AsciiDocParser.h"
#include <any>

class Tool {
private:
	std::vector<std::any> tools;

public:
	void addTool(std::any tool);

	void removeTool(int id);
};

#endif