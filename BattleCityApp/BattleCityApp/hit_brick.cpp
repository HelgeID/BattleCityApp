//FINAL
#include "block.hpp"

void Block::loadParamPartsBrick(std::vector<sf::RectangleShape> &partsBrickVec, Part_Bricks_Map &pbmap)
{
	if (this->type == Brick || this->type == Steel)
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

			this->partBrick->pbArr[index].presence = false;
		}

		auto self_hit = [&](const int indxArr[], const int size, const Direction dir)
		{
			for (int indx(0); indx < size; indx++) {
				brickDamage(partsBrickVec, indxArr[indx]);
				pbmap.fillMap(partsBrickVec[partsBrickVec.size() - 1]);
				brickDamageAdditional(partsBrickVec, pbmap);
				overloadFrame(dir);
			}
		};

		if (this->mod == ModBlock::modRight)
		{
			int indxArr[]{ 8, 9, 10, 11 };
			self_hit(indxArr, 4, Direction::RIGHT);
		}
		else if (this->mod == ModBlock::modDown)
		{
			int indxArr[]{ 0, 1, 2, 3 };
			self_hit(indxArr, 4, Direction::DOWN);
		}
		else if (this->mod == ModBlock::modLeft)
		{
			int indxArr[]{ 15, 14, 13, 12 };
			self_hit(indxArr, 4, Direction::LEFT);
		}
		else if (this->mod == ModBlock::modUp)
		{
			int indxArr[]{ 7, 6, 5, 4 };
			self_hit(indxArr, 4, Direction::UP);
		}

		else if (this->mod == ModBlock::modRightUpMini)
		{
			int indxArr1[]{ 8, 9, 10, 11 };
			self_hit(indxArr1, 4, Direction::RIGHT);
			int indxArr2[]{ 7, 5 };
			self_hit(indxArr2, 2, Direction::UP);
		}
		else if (this->mod == ModBlock::modRightDownMini)
		{
			int indxArr1[]{ 8, 9, 10, 11 };
			self_hit(indxArr1, 4, Direction::RIGHT);
			int indxArr2[]{ 1, 3 };
			self_hit(indxArr2, 2, Direction::DOWN);
		}
		else if (this->mod == ModBlock::modLeftUpMini)
		{
			int indxArr1[]{ 15, 14, 13, 12 };
			self_hit(indxArr1, 4, Direction::LEFT);
			int indxArr2[]{ 6, 4 };
			self_hit(indxArr2, 2, Direction::UP);
		}
		else if (this->mod == ModBlock::modLeftDownMini)
		{
			int indxArr1[]{ 15, 14, 13, 12 };
			self_hit(indxArr1, 4, Direction::LEFT);
			int indxArr2[]{ 0, 2 };
			self_hit(indxArr2, 2, Direction::DOWN);
		}
	}
	return;
}

void Block::brickDamage(std::vector<sf::RectangleShape> &partsBrickVec, const int index)
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

	this->partBrick->pbArr[index].presence = true;

	const float posX(this->partBrick->pbArr[index].x);
	const float posY(this->partBrick->pbArr[index].y);
	damageElement.setPosition(posX, posY);

	partsBrickVec.push_back(damageElement);

	return;
}

struct INDEX { int in1, in2, out1, out2; };

void Block::brickDamageAdditional(std::vector<sf::RectangleShape> &partsBrickVec, Part_Bricks_Map &pbmap)
{
	const INDEX groupeA = { 0,  2,  8,  10 };
	const INDEX groupeB = { 1,  3, 12,  14 };
	const INDEX groupeC = { 4,  6,  9,  11 };
	const INDEX groupeD = { 5,  7, 13, 15 };
	INDEX groupe[] = { groupeA, groupeB, groupeC, groupeD };

	auto control = [&](const INDEX* groupe)
	{
		if (this->partBrick->pbArr[groupe->in1].presence && this->partBrick->pbArr[groupe->in2].presence)
		{
			if (!this->partBrick->pbArr[groupe->out1].presence)
			{
				this->brickDamage(partsBrickVec, groupe->out1);
				pbmap.fillMap(partsBrickVec[partsBrickVec.size() - 1]);
			}
			if (!this->partBrick->pbArr[groupe->out2].presence)
			{
				this->brickDamage(partsBrickVec, groupe->out2);
				pbmap.fillMap(partsBrickVec[partsBrickVec.size() - 1]);
			}
		}
	};

	auto reverse = [&](INDEX* groupe)
	{
		//in1 <-> out1
		groupe->in1 = groupe->in1 ^ groupe->out1;
		groupe->out1 = groupe->in1 ^ groupe->out1;
		groupe->in1 ^= groupe->out1;

		//in2 <-> out2
		groupe->in2 = groupe->in2 ^ groupe->out2;
		groupe->out2 = groupe->in2 ^ groupe->out2;
		groupe->in2 ^= groupe->out2;
	};

	for (int i = 0; i < 4; ++i) {
		control(const_cast<INDEX*>(&groupe[i]));
		reverse(const_cast<INDEX*>(&groupe[i]));
		control(const_cast<INDEX*>(&groupe[i]));
	}

	return;
}

void Block::overloadFrame(const Direction dir)
{
	//read data from frame
	int posX1 = static_cast<int>(frame.getPosition().x);
	int posY1 = static_cast<int>(frame.getPosition().y);
	int posX2 = posX1 + static_cast<int>(frame.getSize().x);
	int posY2 = posY1 + static_cast<int>(frame.getSize().y);

	struct Check
	{
		static bool CheckPbArrFunc(const Block& obj, const int first, const int second, const int third, const int fourth)
		{
			bool Check1 = obj.partBrick->pbArr[first].presence;
			bool Check2 = obj.partBrick->pbArr[second].presence;
			bool Check3 = obj.partBrick->pbArr[third].presence;
			bool Check4 = obj.partBrick->pbArr[fourth].presence;
			return Check1 && Check2 && Check3 && Check4;
		}

		static bool CheckPbStateArrFunc(const Block& obj, const Direction dir)
		{
			if (dir == UP || dir == DOWN)
				return (obj.partBrick->pbStateArr[0] && obj.partBrick->pbStateArr[1] && obj.partBrick->pbStateArr[2] && obj.partBrick->pbStateArr[3]);

			if (dir == LEFT || dir == RIGHT)
				return (obj.partBrick->pbStateArr[4] && obj.partBrick->pbStateArr[5] && obj.partBrick->pbStateArr[6] && obj.partBrick->pbStateArr[7]);
		}
	};

	auto paramset = [&](const int nothing = 0)
	{
		float xSize = (float)posX2 - posX1;
		float ySize = (float)posY2 - posY1;
		nothing ? frame.setSize(sf::Vector2f(xSize, ySize)) : 
			frame.setSize(sf::Vector2f((float)nothing, (float)nothing));
		nothing ? frame.setPosition(sf::Vector2f((float)posX1, (float)posY1)) : 
			frame.setPosition(sf::Vector2f((float)nothing, (float)nothing));
		return true;
	};

	int coeff(NULL); //coefficient passage of the array (pbArr)
	auto offset1 = [&](int& II, int& JJ, const int indx, const bool det_dir, const int coeff)
	{
		if (!partBrick->pbStateArr[indx]) {
			if (partBrick->pbArr[II].presence && partBrick->pbArr[JJ].presence) {
				partBrick->pbStateArr[indx] = true;

				if (dir == UP || dir == DOWN) {
					if (!det_dir)
						posY1 += 4;
					else
						posY2 -= 4;

					return paramset(1);
				}

				if (dir == LEFT || dir == RIGHT) {
					if (!det_dir)
						posX1 += 4;
					else
						posX2 -= 4;

					return paramset(1);
				}

			}
		}
		else {
			II = II + coeff;
			JJ = JJ + coeff;
		}
		return false;
	};

	auto offset2 = [&](const Direction dir, const bool Check1, const bool Check2)
	{
		if (Check1 == true || Check2 == true) {
			if (dir == UP || dir == DOWN) {
				*partBrick->pbStateL = Check1 ? posX1 += 8 : NULL;
				*partBrick->pbStateR = Check2 ? posX2 -= 8 : NULL;
			}

			if (dir == LEFT || dir == RIGHT) {
				*partBrick->pbStateU = Check1 ? posY1 += 8 : NULL;
				*partBrick->pbStateD = Check2 ? posY2 -= 8 : NULL;
			}

			return paramset(1);
		}
		return false;
	};

	//determination of the direction offsets
	//UP, LEFT => true; DOWN, RIGHT => false; (0123 - value dir)
	auto DetDir = [&](const Direction dir) { return (dir < 2); };

	bool CheckL = (Check::CheckPbArrFunc(*this, 0, 2, 4, 6));
	bool CheckR = (Check::CheckPbArrFunc(*this, 1, 3, 5, 7));
	bool CheckU = (Check::CheckPbArrFunc(*this, 8, 10, 12, 14));
	bool CheckD = (Check::CheckPbArrFunc(*this, 9, 11, 13, 15));

	if (dir == DOWN) {
		if (Check::CheckPbStateArrFunc(*this, dir))
			;
		else {
			if (*partBrick->pbStateL || *partBrick->pbStateR)
				;
			else
				offset2(dir, CheckL, CheckR);
		}

		(Check::CheckPbArrFunc(*this, 0, 1, 2, 3)) ?
			*partBrick->pbStateU = true : NULL;

		int II(0), JJ(1);
		for (int indx = 0, coeff = 2; indx <= 3; indx = indx + 1) {
			if (offset1(II, JJ, indx, DetDir(dir), coeff))
				break;
		}

		goto add_checking;
	}

	if (dir == UP) {
		if (Check::CheckPbStateArrFunc(*this, dir))
			;
		else {
			if (*partBrick->pbStateL || *partBrick->pbStateR)
				;
			else
				offset2(dir, CheckL, CheckR);
		}

		(Check::CheckPbArrFunc(*this, 4, 5, 6, 7)) ?
			*partBrick->pbStateD = true : NULL;

		int II(7), JJ(6);
		for (int indx = 3, coeff = -2; indx >= 0; indx = indx - 1) {
			if (offset1(II, JJ, indx, DetDir(dir), coeff))
				break;
		}

		goto add_checking;
	}

	if (dir == RIGHT) {
		if (Check::CheckPbStateArrFunc(*this, dir))
			;
		else {
			if (*partBrick->pbStateU || *partBrick->pbStateD)
				;
			else
				offset2(dir, CheckU, CheckD);
		}

		(Check::CheckPbArrFunc(*this, 8, 9, 10, 11)) ?
			*partBrick->pbStateL = true : NULL;

		int II(8), JJ(9);
		for (int indx = 4, coeff = 2; indx <= 7; indx = indx + 1) {
			if (offset1(II, JJ, indx, DetDir(dir), coeff))
				break;
		}

		goto add_checking;
	}

	if (dir == LEFT) {
		if (Check::CheckPbStateArrFunc(*this, dir))
			;
		else {
			if (*partBrick->pbStateU || *partBrick->pbStateD)
				;
			else
				offset2(dir, CheckU, CheckD);
		}

		(Check::CheckPbArrFunc(*this, 12, 13, 14, 15)) ?
			*partBrick->pbStateR = true : NULL;

		int II(15), JJ(14);
		for (int indx = 7, coeff = -2; indx >= 4; indx = indx - 1) {
			if (offset1(II, JJ, indx, DetDir(dir), coeff))
				break;
		}

		goto add_checking;
	}

	//Add Checking
add_checking:
	{
		if (CheckL && CheckR && CheckU && CheckD)
			paramset();
	}

	return;
}
