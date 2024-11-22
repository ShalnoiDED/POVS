#include "Header.h"

AsciiDocEditor::AsciiDocEditor() {

}

AsciiDocEditor::AsciiDocEditor(const std::string& _content) {
	content = _content;
}

AsciiDocEditor::~AsciiDocEditor() {

}

void AsciiDocEditor::PrintHeadings() {
	std::regex headerRegex("^={1,6}\\s*(.+)$");
	auto headersBegin = std::sregex_iterator(content.begin(), content.end(), headerRegex);
	auto headersEnd = std::sregex_iterator();
	if (headersBegin == headersEnd) {
		throw "���������� �� �������. �������: PrintHeadings\n";
		return;
	}
	std::cout << "���������:" << std::endl;
	for (std::sregex_iterator i = headersBegin; i != headersEnd; ++i) {
		std::cout << (*i)[1].str() << std::endl;
	}
}

void AsciiDocEditor::PrintParagraphs()
{
	std::regex paragraphRegex("(^[^=\\s].+?(?:\\n{2,}|$))");
	auto paragraphsBegin = std::sregex_iterator(content.begin(), content.end(), paragraphRegex);
	auto paragraphsEnd = std::sregex_iterator();
	if (paragraphsBegin == paragraphsEnd) {
		throw "������� �� �������. �������: PrintParagraphs\n";
		return;
	}
	std::cout << "\n������:" << std::endl;
	for (std::sregex_iterator i = paragraphsBegin; i != paragraphsEnd; ++i) {
		std::cout << (*i)[1].str() << std::endl;
	}
}

void AsciiDocEditor::PrintBoldText()
{
	std::regex boldRegex("\\*(.*?)\\*");
	auto boldsBegin = std::sregex_iterator(content.begin(), content.end(), boldRegex);
	auto boldsEnd = std::sregex_iterator();
	if (boldsBegin == boldsEnd) {
		throw "������� ������ �� �������. �������: PrintBoldText";
		return;
	}
	std::cout << "\n������ �����:" << std::endl;
	for (std::sregex_iterator i = boldsBegin; i != boldsEnd; ++i) {
		std::cout << (*i)[1].str() << std::endl;
	}
}