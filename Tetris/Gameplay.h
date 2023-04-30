#pragma once
#include <iostream>
#include <string>
#include "FrameBuilder.h"
#include "ShapeStorage.h"
#include <chrono>
#include <windows.h>

class Gameplay
{
public:
    Gameplay();
    ~Gameplay();

	void start();
	void update();
    bool keyboardCheck();
    void move(int8_t deltaX = 0, int8_t deltaY = 0);
    void rotate(int8_t deltaState);
    void setNewFigure(uint8_t x, uint8_t y, uint8_t figureId);

    uint8_t getFigureHeight();
    uint8_t getFigureWidth();

    void checkLine();
    bool checkWall(int8_t deltaX, int8_t deltaY, int8_t deltaState = 0);

    std::vector<char> getSymbols();
private:

/*-----------Settings----------*/
    uint8_t _fieldHeight = 25;
    uint8_t _fieldWidth = 12;

    int16_t _gameSpeed = 750;

    bool _trajectory = true;
/*-----------------------------*/


    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeStart = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeCheckStartRot = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeCheckStartMove = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeCheckStartSpace = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeEnd = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeCheckEndRot = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeCheckEndMove = std::chrono::system_clock::now();
    std::chrono::time_point<struct std::chrono::system_clock, class std::chrono::duration<__int64, struct std::ratio<1, 10000000>>> timeCheckEndSpace = std::chrono::system_clock::now();
    
    FrameBuilder _frameBuilder;
    ShapeStorage _shapeStorage;

    char _borderSymbol;
    char _fieldSymbol;
    char _figureSymbol;

    std::vector<std::string>* _figure = nullptr;

    bool _isNewFigure = false;
    bool _isEnd = false;
    bool _isAction = false;
    bool _isSpace = false;

    uint64_t _score = 0;
    uint16_t _scoreAdd = 10;
    uint16_t _line = 0;

    uint8_t _x = 16;
    uint8_t _y = 1;

    uint8_t _figureId = 7;
    uint8_t _state = 0;
    uint8_t _figureHeight = 0;
    uint8_t _figureWidth = 0;

    const uint16_t _gameSpeedWhenSpace = 20;
    int16_t _currentSpeed = _gameSpeed;

    uint16_t _timeForSpace = 1000;
    uint8_t _timeForMove = 100;
    uint8_t _timeForRotate = 150;

    uint8_t map(int8_t value, uint8_t max, uint8_t min);
    uint8_t clip(int8_t value, uint8_t max, uint8_t min);
    void cheatLine();
    void scoreUpdate(uint8_t upscale);
};

