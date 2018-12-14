#include "block.hpp"

void Block::loadParamPartsBrick()
{
	if (this->type == Brick)
	{
		std::shared_ptr<PartsBrickArr> partBrick(new PartsBrickArr);
		this->partBrick = std::move(partBrick);

		int valX(0), valY(0);
		for (int index = 0; index < 16; ++index)
		{
			(index == 0 || index == 8) ? (valX = 0, valY = 0) : NULL;

			this->partBrick->pbArr[index].x = this->takeObj().getPosition().x + valX;
			this->partBrick->pbArr[index].y = this->takeObj().getPosition().y + valY;

			if (index < 8) {
				this->partBrick->pbArr[index].type = (int)'H';
				!valX ? valX = 8 : (valX = 0, valY += 4);
			}
			else {
				this->partBrick->pbArr[index].type = (int)'V';
				valY += 8;
				if (index % 2 != 0) { valX += 4; valY = 0; }
			}
		}
	}
	return;
}

void Block::onBrickDamage(std::vector<sf::RectangleShape> &partsBrickVec, const int index)
{
	sf::RectangleShape damageElement;
	damageElement.setFillColor(sf::Color::Black);

	sf::Vector2f size;
	if (this->partBrick->pbArr[index].type == 'H') {
		size.x = 8.f; size.y = 4.f;
	}
	if (this->partBrick->pbArr[index].type == 'V') {
		size.x = 4.f; size.y = 8.f;
	}
	damageElement.setSize(size);

	const float posX(this->partBrick->pbArr[index].x);
	const float posY(this->partBrick->pbArr[index].y);
	damageElement.setPosition(posX, posY);

	partsBrickVec.push_back(damageElement);

	return;
}
