#include "ConsoleOutput.h"

void ConsoleOutput::print(std::vector<std::string>& frame)
{
	uint8_t rows = frame.size();
	uint8_t columns = frame[0].size();
	uint8_t normalizedColumns = (frame[0].size() * horizontalRatio);

	std::string out;

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < normalizedColumns; ++j) {
			out += frame[i][(j * columns) / normalizedColumns];
		}
		out += '\n';
	}

	out += _row;

	std::cout << out;

	SetConsoleCursorPosition(_console, _pos);
}

void ConsoleOutput::print(std::string text) {
	SetConsoleCursorPosition(_console, _pos);
	std::cout << text;
}

void ConsoleOutput::setRow(std::string str)
{
	_row = str;
}

void ConsoleOutput::end(HANDLE console, COORD position)
{
	SetConsoleCursorPosition(console, position);
}
