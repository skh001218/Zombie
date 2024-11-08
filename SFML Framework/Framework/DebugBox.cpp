#include "stdafx.h"
#include "DebugBox.h"

DebugBox::DebugBox()
{
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);
}

void DebugBox::SetBounds(const sf::FloatRect& bounds)
{
	shape.setSize({ bounds.width, bounds.height });
	shape.setPosition(bounds.left, bounds.top);
}

void DebugBox::Draw(sf::RenderWindow& window)
{
	window.draw(shape);
}
