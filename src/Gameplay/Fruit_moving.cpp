#include <Gameplay/Fruit_moving.h>

void Fruit_moving::update(float deltaMilliseconds)
{
	if (m_moveFruit) 
	{
		m_position += (m_moveDirection * m_speed * deltaMilliseconds);
		m_sprite.setPosition(m_position);
	}
}