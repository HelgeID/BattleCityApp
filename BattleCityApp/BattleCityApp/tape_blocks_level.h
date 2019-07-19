#ifndef TAPE_BLOCKS_LEVEL_H
#define TAPE_BLOCKS_LEVEL_H

#include <SFML\Graphics.hpp>
#include <iostream>
#include <sstream>

struct SUMM_RGB
{
	int rSUMM{ 0 }, gSUMM{ 0 }, bSUMM{ 0 };

	bool operator==(const SUMM_RGB& obj) const
	{
		return (obj.rSUMM == rSUMM && obj.gSUMM == gSUMM && obj.bSUMM == bSUMM);
	}

	SUMM_RGB& operator=(const sf::Color color)
	{
		rSUMM = (int)color.r;
		gSUMM = (int)color.g;
		bSUMM = (int)color.b;

		return *this;
	}

	SUMM_RGB& operator+=(const sf::Color color)
	{
		rSUMM += (int)color.r;
		gSUMM += (int)color.g;
		bSUMM += (int)color.b;

		return *this;
	}

	int hash_summ() const { return (rSUMM + gSUMM + bSUMM); }
};

struct LoadLevel
{
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	LoadLevel(const std::string level) {
		image.loadFromFile(level);
	}
};

void TakeTapeBlocksSpeciallyForLevel(LoadLevel* ptrll, int& value, const int i, const int j)
{
	//----------------------------------------------------------------
	ptrll->texture.loadFromImage(ptrll->image, sf::IntRect(16 * j, 16 * i, 16, 16));
	ptrll->sprite.setTexture(ptrll->texture);

	//----------------------------------------------------------------
	auto cond_black_square = [&](const sf::Vector2i& pos) {
		sf::RectangleShape rs(sf::Vector2f(8.f, 8.f));
		rs.setTexture(&ptrll->texture);
		rs.setTextureRect(sf::IntRect(pos.x, pos.y, 8, 8));
		const int width(pos.x + rs.getTextureRect().width);
		const int height(pos.y + rs.getTextureRect().height);
		int indX(rs.getTextureRect().left);
		int indY(rs.getTextureRect().top);

		SUMM_RGB summ;
		int allsumm(0);
		while (indX < width) {
			while (indY < height) {

				{
					//calculating the sum of pixels
					summ = rs.getTexture()->copyToImage().getPixel(indX, indY);
					allsumm += summ.hash_summ();
				}

				indY = indY + 1;
			}
			indX = indX + 1;
			indY = rs.getTextureRect().top;
		}

		if (allsumm == 0)
			return true;
		return false;
	};

	const bool f_UP_LEFT = cond_black_square(sf::Vector2i(0, 0));
	const bool f_UP_RIGHT = cond_black_square(sf::Vector2i(8, 0));
	const bool f_DOWN_LEFT = cond_black_square(sf::Vector2i(0, 8));
	const bool f_DOWN_RIGHT = cond_black_square(sf::Vector2i(8, 8));

	//std::cerr << f_UP_LEFT << std::endl;
	//std::cerr << f_UP_RIGHT << std::endl;
	//std::cerr << f_DOWN_LEFT << std::endl;
	//std::cerr << f_DOWN_RIGHT << std::endl;

	//----------------------------------------------------------------
	auto toString = [&](int& arg) {
		std::stringstream ss;
		ss << arg;
		return ss.str();
	};

	int allsumm(0);
	SUMM_RGB summ;
	const int width(ptrll->sprite.getTextureRect().width);
	const int height(ptrll->sprite.getTextureRect().height);
	int indX(ptrll->sprite.getTextureRect().left);
	int indY(ptrll->sprite.getTextureRect().top);

	while (indX < width) {
		while (indY < height) {

			{
				//calculating the sum of pixels
				summ = ptrll->sprite.getTexture()->copyToImage().getPixel(indX, indY);
				allsumm += summ.hash_summ();
			}

			indY = indY + 1;
		}
		indX = indX + 1;
		indY = ptrll->sprite.getTextureRect().top;
	}

	int valBlackSquare(0); //_FULL(00)
	if (f_UP_LEFT && f_UP_RIGHT && f_DOWN_LEFT && f_DOWN_RIGHT)
		;
	else {
		f_DOWN_LEFT && f_UP_LEFT ? valBlackSquare = 1 : 0; //_Right(..1)
		f_UP_RIGHT && f_UP_LEFT ? valBlackSquare = 2 : 0; //_Down(..2)
		f_UP_RIGHT && f_DOWN_RIGHT ? valBlackSquare = 3 : 0; //_Left(..3)
		f_DOWN_LEFT && f_DOWN_RIGHT ? valBlackSquare = 4 : 0; //_Up(..4)
	}

	int valBlock(0);
	if (allsumm == 77688 || allsumm == 38844)
		valBlock = 1;
	else if (allsumm == 141144 || allsumm == 70572)
		valBlock = 2;
	else if (allsumm == 70016)
		valBlock = 3;
	else if (allsumm == 108928)
		valBlock = 4;
	else if (allsumm == 146124)
		valBlock = 5;
	else
		valBlock = 0;

	std::string str(toString(valBlock) + toString(valBlackSquare));
	int crc(atoi(str.c_str()));

	value = crc;
	return;
}

#endif
