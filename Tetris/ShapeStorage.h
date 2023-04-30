#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

class ShapeStorage
{
public:
	ShapeStorage();
	std::vector<std::string> getFigure(uint8_t id);
	char getFigureSymbol();
	char getBorderSymbol();
	char getFieldSymbol();
	char getTrajectorySymbol();
private:
	/*-----------Settings----------*/
	const char _figureSymbol = 219;
	const char _borderSymbol = '#';
	const char _fieldSymbol = ' ';
	const char _trajectorySymbol = '|';
	/*-----------------------------*/

	std::unordered_map<uint8_t, std::vector<std::string>> figureMap;
};