#include "Gameplay.h"

Gameplay::Gameplay() {
    _figure = nullptr;
}

Gameplay::~Gameplay()
{
    if (_figure) {
        _figure->clear();
        delete _figure;
    }
}

void Gameplay::start()
{
    _borderSymbol = getSymbols()[0];
    _fieldSymbol = getSymbols()[1];
    _figureSymbol = getSymbols()[2];

    std::vector<std::string> frame;

    frame.emplace_back(std::string(_fieldWidth, _borderSymbol));
    for (int i = 0; i < _fieldHeight - 2; i++)
        frame.emplace_back(std::string(1, _borderSymbol) + std::string(_fieldWidth - 2, _fieldSymbol) + std::string(1, _borderSymbol));
    frame.emplace_back(std::string(_fieldWidth, _borderSymbol));

    srand(time(0));
    _frameBuilder.setFrame(frame);
    update();
}

void Gameplay::update()
{
    while (!_isEnd)
    {
        if (!_isNewFigure) {
            _figureId = rand()%7;
            checkLine();
            try {
                setNewFigure(_frameBuilder.getFrame()[0].size() / 2 - 1, 1, _figureId);
            }
            catch (int ex) {
                switch (ex)
                {
                case 0:
                    _isEnd = true;
                    _frameBuilder.showMessage("This figure is not suitable in 'X' size!");
                    continue;
                    break;
                case 1:
                    _isEnd = true;
                    _frameBuilder.showMessage("This figure is not suitable in 'Y' size!");
                    continue;
                    break;
                default:
                    break;
                }
            }
            if (!checkWall(0, 1)) {
                _isEnd = false;
                _frameBuilder.theEnd();
                break;
            }
        }

        keyboardCheck();

        _frameBuilder.setRow("X: " + std::to_string(_x) + " Y: " + std::to_string(_y) + " Width: " + std::to_string(_figureWidth) + " Height: " + std::to_string(_figureHeight) + "  \n" + "Score: " + std::to_string(_score) + "\nLine: " + std::to_string(_line));

        timeEnd = std::chrono::system_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count() > _currentSpeed) {
            if (checkWall(0, 1)) {
                move(0, 1);
            }
            timeStart = std::chrono::system_clock::now();
        }

        if(_trajectory)
            _frameBuilder.setTrajectory(_x,_y);

        _frameBuilder.print();
        
        if(_trajectory)
            _frameBuilder.removeTrajectory(_x, _y);
    }
}

bool Gameplay::keyboardCheck() {
    _isAction = false;
    timeCheckEndMove = std::chrono::system_clock::now();
    if (!_isSpace) {
        if (GetAsyncKeyState(VK_RIGHT) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndMove - timeCheckStartMove).count() > _timeForMove)) {
            if (checkWall(1, 0)) {
                move(1);
                _isAction = true;
            }
            timeCheckStartMove = std::chrono::system_clock::now();
        }
        if (GetAsyncKeyState(VK_LEFT) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndMove - timeCheckStartMove).count() > _timeForMove))
        {
            if (checkWall(-1, 0)) {
                move(-1);
                _isAction = true;
            }
            timeCheckStartMove = std::chrono::system_clock::now();
        }
        if (GetAsyncKeyState(VK_UP) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndMove - timeCheckStartMove).count() > _timeForMove))
        {
            if (checkWall(0, -1)) {
                move(0, -1);
                _isAction = true;
            }
            timeCheckStartMove = std::chrono::system_clock::now();
        }
        if (GetAsyncKeyState(VK_DOWN) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndMove - timeCheckStartMove).count() > _timeForMove))
        {
            if (checkWall(0, 1)) {
                move(0, 1);
                _isAction = true;
            }
            timeCheckStartMove = std::chrono::system_clock::now();
        }

        timeCheckEndRot = std::chrono::system_clock::now();
        if (GetAsyncKeyState(65) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndRot - timeCheckStartRot).count() > _timeForRotate))
        {
            if (checkWall(0, 0, -1)) {
                rotate(-1);
                _isAction = true;
            }
            timeCheckStartRot = std::chrono::system_clock::now();
        }
        if (GetAsyncKeyState(68) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndRot - timeCheckStartRot).count() > _timeForRotate))
        {
            if (checkWall(0, 0, 1)) {
                rotate(1);
                _isAction = true;
            }
            timeCheckStartRot = std::chrono::system_clock::now();
        }
    }
    timeCheckEndSpace = std::chrono::system_clock::now();
    if ((GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_CONTROL)) && !_isSpace && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndSpace - timeCheckStartSpace).count() > _timeForSpace))
    {
        if (GetAsyncKeyState(VK_CONTROL)) {
            move(0, -1);
            cheatLine();
            timeCheckStartSpace = std::chrono::system_clock::now();
            return _isAction;
        }
        _isSpace = true;
        _currentSpeed = _gameSpeedWhenSpace;
        _isAction = true;
        timeCheckStartSpace = std::chrono::system_clock::now();
    }
    return _isAction;
}

void Gameplay::move(int8_t deltaX, int8_t deltaY)
{
    if (deltaX > 1 && deltaX < -1)
        return;
    if (deltaY > 1 && deltaY < -1)
        return;

    _frameBuilder.removeFigure(_x,_y, _figureId, _state);

    _x += deltaX;
    _y += deltaY;

    _frameBuilder.setFigure(_x, _y, _figureId, _state);
}

void Gameplay::rotate(int8_t deltaState)
{
    if(deltaState > 1 || deltaState < -1){
        return;
    }

    if (_x + _figureHeight >= _frameBuilder.getFrame()[0].size()) {
        return;
    }
    if (_y + _figureWidth >= _frameBuilder.getFrame().size()) {
        return;
    }

    _frameBuilder.removeFigure(_x, _y, _figureId, _state);

    uint8_t temp = _figureHeight;
    _figureHeight = _figureWidth;
    _figureWidth = temp;

    if (_state + deltaState > 3) {
        _state = 0;
    }
    else if (_state + deltaState < 0) {
        _state = 3;
    }
    else {
        _state += deltaState;
    }

    _frameBuilder.setFigure(_x, _y, _figureId, _state);
    *_figure = _frameBuilder.getFigure();
}

void Gameplay::setNewFigure(uint8_t x, uint8_t y, uint8_t figureId)
{
    _isNewFigure = true;
    _isSpace = false;
    _currentSpeed = _gameSpeed;

    if (!_figure) {
        _figure = new std::vector<std::string>(_shapeStorage.getFigure(_figureId));
    }
    else {
        *_figure = _shapeStorage.getFigure(_figureId);
    }

    _x = x;
    _y = y;
    _figureId = figureId;
    _state = 0;

    _figureHeight = (*_figure).size();
    _figureWidth = (*_figure)[0].size();

    if (_figureWidth > _frameBuilder.getFrame()[0].size() - 2) {
        throw 0;
    }
    if (_figureHeight > _frameBuilder.getFrame().size()-2) {
        throw 1;
    }
    if (_figureWidth-1+_x > _frameBuilder.getFrame()[0].size() - 2) {
        _x += ((_frameBuilder.getFrame()[0].size() - 2) - (_figureWidth - 1 + _x));
    }
    if (_figureHeight-1+_y > _frameBuilder.getFrame().size() - 2) {
        _y += ((_frameBuilder.getFrame().size() - 2) - (_figureHeight - 1 + _y));
    }

    _frameBuilder.setFigure(_x, _y, _figureId, _state);
}

uint8_t Gameplay::getFigureHeight()
{
    return _figureHeight;
}

uint8_t Gameplay::getFigureWidth()
{
    return _figureWidth;
}

std::vector<char> Gameplay::getSymbols()
{
    std::vector<char> symbols = { _shapeStorage.getBorderSymbol(), _shapeStorage.getFieldSymbol(), _shapeStorage.getFigureSymbol() };
    return symbols;
}

void Gameplay::checkLine()
{
    uint8_t upscale = 1;
    bool isFit = true;
    std::vector<std::string> frame = _frameBuilder.getFrame();
    for (int i = 0; i < frame.size() - 4; ++i) {
        isFit = true;
        while (isFit) {
            for (int j = 1; j < frame[0].size() - 1; ++j) {
                if (frame[frame.size() - 2 - i][j] != _shapeStorage.getFigureSymbol()) {
                    isFit = false;
                    break;
                }
            }
            if (isFit) {
                for (int k = i; k < frame.size()-2; k++) {
                    if (frame[frame.size() - 3 - k][1] == _borderSymbol) {
                        frame[frame.size() - 2 - k] = std::string(1, _borderSymbol) + std::string(_frameBuilder.getFrame()[0].size()-2, _fieldSymbol) + std::string(1, _borderSymbol);
                    }
                    else {
                        frame[frame.size() - 2 - k] = frame[frame.size() - 3 - k];
                    }
                }
                ++upscale;
                ++_line;
                _frameBuilder.setFrame(frame);
            }
        }
    }
    scoreUpdate(upscale);
}

void Gameplay::cheatLine() {
    _y++;
    std::vector<std::string> frame = _frameBuilder.getFrame();
    for (int k = 0; k < frame.size() - 3; k++) {
        frame[frame.size() - 2 - k] = frame[frame.size() - 3 - k];
    }
    ++_line;
    _frameBuilder.setFrame(frame);
    scoreUpdate(1);
}

bool Gameplay::checkWall(int8_t deltaX, int8_t deltaY, int8_t deltaState)
{
    std::vector<std::string> _frame = _frameBuilder.getFrame();

    if (_x <= 1 && deltaX < 0) {
        return false;
    }
    if (_x + _figureWidth >= _frame[0].size() - 1 && deltaX > 0) {
        return false;
    }
    if (_y <= 1 && deltaY < 0) {
        return false;
    }
    if (_y + _figureHeight >= _frame.size() - 1 && deltaY > 0) {
        _isNewFigure = false;
        return false;
    }
    if (deltaY != 0) {
        int offsetY = 0;
        for (int i = 0; i < _figureWidth; ++i) {
            while ((*_figure)[_figureHeight - 1 - offsetY][i] == _fieldSymbol && deltaY > 0) {
                ++offsetY;
            }
            if (_frame[_y + _figureHeight - offsetY][_x + i] == _shapeStorage.getFigureSymbol() && deltaY > 0) {
                _isNewFigure = false;
                return false;
            }
            offsetY = 0;
        }
    }
    if (deltaX != 0) {
        int offsetX = 0;
        for (int i = 0; i < _figureHeight; ++i) {
            while ((*_figure)[i][offsetX] == _fieldSymbol && deltaX < 0) {
                ++offsetX;
            }
            if (_frame[_y + i][_x + offsetX - 1] == _shapeStorage.getFigureSymbol() && deltaX < 0) {
                return false;
            }
            offsetX = 0;
        }
        for (int i = 0; i < _figureHeight; ++i) {
            while ((*_figure)[i][_figureWidth - 1 - offsetX] == _fieldSymbol && deltaX > 0) {
                ++offsetX;
            }
            if (_frame[_y + i][_x + _figureWidth - offsetX] == _shapeStorage.getFigureSymbol() && deltaX > 0) {
                return false;
            }
            offsetX = 0;
        }
    }
    if (deltaState != 0) {
        if (_figureHeight + _x > _frame[0].size()) {
            return false;
        }
        std::vector<std::string> temp = _frameBuilder.getFigure(_figureId, map(_state + deltaState, 3, 0));
        for (int i = 0; i < temp.size(); ++i) {
            for (int j = 0; j < temp[0].size(); ++j) {
                if (temp[i][j] == _fieldSymbol)
                    continue;
                else if (_frame[_y + i][_x + j] == _fieldSymbol)
                    continue;

                if (_frame[_y + i][_x + j] != _fieldSymbol) {
                    if (clip(i, (*_figure).size() - 1, 0) != i || clip(j, (*_figure)[0].size() - 1, 0) != j) {
                        return false;
                    }
                    else {
                        if ((*_figure)[clip(i, (*_figure).size() - 1, 0)][clip(j, (*_figure)[0].size() - 1, 0)] == _fieldSymbol) {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

uint8_t Gameplay::map(int8_t value, uint8_t max, uint8_t min)
{
    while (value > max) {
        value -= (max - min + 1);
    }
    while (value < min) {
        value += (max - min + 1);
    }
    return value;
}

uint8_t Gameplay::clip(int8_t value, uint8_t max, uint8_t min)
{
    if (value > max)
        return max;
    if (value < min)
        return min;
    return value;
}

void Gameplay::scoreUpdate(uint8_t upscale)
{
    _score += _scoreAdd * upscale;
    switch(_line)
    {
    case 10:
        _gameSpeed -= 50;
        _scoreAdd += 50;
        break;
    case 30:
        _gameSpeed -= 50;
        _scoreAdd += 200;
        break;
    case 50:
        _gameSpeed -= 50;
        _scoreAdd += 500;
        break;
    case 60:
        _gameSpeed -= 50;
        _scoreAdd += 2000;
        break;
    case 70:
        _gameSpeed -= 50;
        _scoreAdd += 10000;
        break;
    case 75:
        _gameSpeed -= 50;
        _scoreAdd += 10000;
        break;
    case 80:
        _gameSpeed -= 100;
        _scoreAdd += 10000;
        break;
    case 85:
        _gameSpeed -= 100;
        _scoreAdd += 10000;
        break;
    case 90:
        _gameSpeed -= 100;
        _scoreAdd += 10000;
        break;
    case 100:
        _gameSpeed -= 110;
        _scoreAdd += 10000;
        break;
    default:
        break;
    }
    _currentSpeed = _gameSpeed;
}
