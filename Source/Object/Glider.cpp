
#include "Glider.h"

GliderTemplate GliderTemplate::_template;

Glider::Glider()
{
	_template = &GliderTemplate::_template;
	_ai = new AIInterface();

	_speedHeight = _template->_speedHeight;
	setHeight(_template->_minHeight);
}

Glider::~Glider()
{
	delete _ai;
}

void Glider::action()
{
	if (_ai) {
		_ai->action();
	}

	if (getHeight() > _template->_maxHeight)
	{
		_speedHeight = -_template->_speedHeight;
	}

	if (getHeight() < _template->_minHeight)
	{
		_speedHeight = _template->_speedHeight;
	}

	setHeight(getHeight() + _speedHeight);
}

void Glider::move(const glm::vec3 &vector)
{
	glm::vec3 offsetVector = vector * _template->_speed;
	_matrix[3][0] += offsetVector.x;
	_matrix[3][1] += offsetVector.y;
}
