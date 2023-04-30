#include "FrameBuilder.h"

FrameBuilder::FrameBuilder()
{
	_frame = nullptr;

	_fieldSymbol = _shape.getFieldSymbol();
	_trajectorySymbol = _shape.getTrajectorySymbol();
}

FrameBuilder::FrameBuilder(std::vector<std::string>& frame)
{
	_frame = &frame;
}

FrameBuilder::~FrameBuilder()
{
	if (_frame) {
		_frame->clear();
		delete _frame;
	}
}

void FrameBuilder::setFigure(uint8_t x, uint8_t y, uint8_t figureId, uint8_t state)
{
	_figure = _shape.getFigure(figureId);
	_figure = rotate(_figure, state);

	if (x + _figure[0].size() > (*_frame)[0].size() || y + _figure.size() > _frame->size()) {
		return;
	}
	else if (x < 0 || y < 0 || x >= (*_frame)[0].size() || y >= _frame->size()) {
		return;
	}
	else if (figureId < 0) {
		return;
	}

	bool isCollision = false;

	for (int i = 0; i < _figure.size(); ++i) {
		for (int j = 0; j < _figure[0].size(); ++j) {
			if (_figure[i][j] == _fieldSymbol) {
				continue;
			}
			if ((*_frame)[y + i][x + j] != _fieldSymbol) {
				isCollision = true;
				break;
			}
		}
		if (isCollision)
			break;
	}

	if (!isCollision) {
		for (int i = 0; i < _figure.size(); ++i) {
			for (int j = 0; j < _figure[0].size(); ++j) {
				if (_figure[i][j] == _fieldSymbol) {
					continue;
				}
				(*_frame)[y + i][x + j] = _figure[i][j];
			}
		}
	}
}

std::vector<std::string>& FrameBuilder::getFigure()
{
	return _figure;
}

std::vector<std::string> FrameBuilder::getFigure(uint8_t figureId, uint8_t state)
{
	return rotate(_shape.getFigure(figureId), state);
}

void FrameBuilder::removeFigure(uint8_t x, uint8_t y, uint8_t figureId, uint8_t state)
{
	std::vector<std::string> _figureSymbol = _shape.getFigure(figureId);
	_figureSymbol = rotate(_figureSymbol, state);

	if (x + _figureSymbol[0].size() > (*_frame)[0].size() || y + _figureSymbol.size() > _frame->size()) {
		return;
	}
	else if (x < 0 || y < 0 || x >= (*_frame)[0].size() || y >= _frame->size()) {
		return;
	}
	else if (figureId < 0) {
		return;
	}


	uint8_t height = _figureSymbol.size();
	uint8_t width = _figureSymbol[0].size();

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (_figureSymbol[i][j] == _fieldSymbol)
				continue;
			else if ((*_frame)[y + i][x + j] == _fieldSymbol)
				continue;

			if (_figureSymbol[i][j] == (*_frame)[y + i][x + j]) {
				(*_frame)[y + i][x + j] = _fieldSymbol;
			}
		}
	}
}

void FrameBuilder::setTrajectory(uint8_t x, int8_t y)
{
	if (y + _figure.size() == (*_frame).size() - 1)
		return;

	int8_t _y = y;
	for (int i = 0; i < 2; ++i) {
		_y = y;
		if ((*_frame)[_figure.size() + _y][x] != _fieldSymbol)
			continue;
		while ((*_frame)[_figure.size() + _y--][x] == _fieldSymbol);
		++_y;
		while ((*_frame)[_figure.size() + ++_y][x] == _fieldSymbol) {
			(*_frame)[_figure.size() + _y][x] = _trajectorySymbol;
		}
		x += _figure[0].size() - 1;
	}
}

void FrameBuilder::removeTrajectory(uint8_t x, uint8_t y)
{
	if (y + _figure.size() == (*_frame).size() - 1)
		return;

	int8_t _y = y;
	for (int i = 0; i < 2; ++i) {
		_y = y;
		if ((*_frame)[_figure.size() + _y][x] != _trajectorySymbol)
			continue;
		while ((*_frame)[_figure.size() + _y--][x] == _trajectorySymbol);
		++_y;
		while ((*_frame)[_figure.size() + ++_y][x] == _trajectorySymbol) {
			(*_frame)[_figure.size() + _y][x] = _fieldSymbol;
		}
		x += _figure[0].size() - 1;
	}
}

void FrameBuilder::print()
{
	_output.print(*_frame);
}

std::vector<std::string>& FrameBuilder::getFrame()
{
	return *_frame;
}

void FrameBuilder::setFrame(std::vector<std::string>& frame)
{
	if (_frame)
		*_frame = frame;
	else
		_frame = new std::vector<std::string>(frame);
}

void FrameBuilder::setRow(std::string str)
{
	_output.setRow(str);
}

void FrameBuilder::showMessage(std::string message)
{
	_output.print(message);

}

void FrameBuilder::theEnd()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { 0, _frame->size()+1 };
	_output.end(console, position);
}

std::vector<std::string>& FrameBuilder::rotate(std::vector<std::string>& _figureSymbol, uint8_t state)
{
	uint8_t height = _figureSymbol.size();
	uint8_t width = _figureSymbol[0].size();
	std::vector<std::string> temp;

	switch (state)
	{
	case 0:
		return _figureSymbol;
		break;
	case 1:
		for (int i = 0; i < width; ++i)
			temp.emplace_back(std::string(height,_fieldSymbol));
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				temp[j][height - i - 1] = _figureSymbol[i][j];
			}
		}
		break;
	case 2:
		for (int i = 0; i < height; ++i)
			temp.emplace_back(std::string(width, _fieldSymbol));
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				temp[height - 1 - i][width - 1 - j] = _figureSymbol[i][j];
			}
		}
		break;
	case 3:
		for (int i = 0; i < width; ++i)
			temp.emplace_back(std::string(height, _fieldSymbol));
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				temp[width - j - 1][i] = _figureSymbol[i][j];
			}
		}
		break;
	}

	_figureSymbol = temp;
	return _figureSymbol;
}

std::vector<std::string>& FrameBuilder::rotate(std::vector<std::string>&& _figureSymbol, uint8_t state)
{
	uint8_t height = _figureSymbol.size();
	uint8_t width = _figureSymbol[0].size();
	std::vector<std::string> temp;

	switch (state)
	{
	case 0:
		return _figureSymbol;
		break;
	case 1:
		for (int i = 0; i < width; ++i)
			temp.emplace_back(std::string(height, _fieldSymbol));
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				temp[j][height - i - 1] = _figureSymbol[i][j];
			}
		}
		break;
	case 2:
		for (int i = 0; i < height; ++i)
			temp.emplace_back(std::string(width, _fieldSymbol));
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				temp[height - 1 - i][width - 1 - j] = _figureSymbol[i][j];
			}
		}
		break;
	case 3:
		for (int i = 0; i < width; ++i)
			temp.emplace_back(std::string(height, _fieldSymbol));
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				temp[width - j - 1][i] = _figureSymbol[i][j];
			}
		}
		break;
	}

	_figureSymbol = temp;
	return _figureSymbol;
}
