#pragma once
#include <iostream>
#include "ShapeStorage.h"
#include "ConsoleOutput.h"

class FrameBuilder
{
public:
	FrameBuilder();
	FrameBuilder(std::vector<std::string>& frame);
	~FrameBuilder();

	std::vector<std::string>& getFigure();
	std::vector<std::string> getFigure(uint8_t figureId, uint8_t state);
	void setFigure(uint8_t x, uint8_t y, uint8_t figureId, uint8_t state);
	void removeFigure(uint8_t x, uint8_t y, uint8_t figureId, uint8_t state);

	void setTrajectory(uint8_t x, int8_t y);
	void removeTrajectory(uint8_t x, uint8_t y);

	std::vector<std::string>& getFrame();
	void setFrame(std::vector<std::string>& frame);
	void setRow(std::string str);
	void showMessage(std::string message);

	void print();
	void theEnd();

private:
	ShapeStorage _shape;
	ConsoleOutput _output;

	char _fieldSymbol;
	char _trajectorySymbol;

	std::vector<std::string>* _frame;
	std::vector<std::string> _figure;

	std::vector<std::string>& rotate(std::vector<std::string>& _figureSymbol, uint8_t state);
	std::vector<std::string>& rotate(std::vector<std::string>&& _figureSymbol, uint8_t state);
};