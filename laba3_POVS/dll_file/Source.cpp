#include "Header.h"

extern "C" std::string ReadFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open())
		throw "������ �������� �����.";

	std::string content = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	if (content.empty())
		throw "������ ������ ����.";
	file.close();
	return content;
}