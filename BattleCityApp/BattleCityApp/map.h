#ifndef MAP_H
#define MAP_H

#include <SFML\System.hpp>

#define sizeMap 13

class Map
{
	int map[sizeMap][sizeMap];
public:
	explicit Map() { *this = 0; }
	~Map() = default;

	Map& operator= (const int value)
	{
		for (size_t i(0); i < sizeMap; ++i)
			for (size_t j(0); j < sizeMap; ++j)
				map[i][j] = value;
		return *this;
	}

	operator int*() { return (int*)map; } //map[index], index from 0 to sizeMap*sizeMap-1 (168)

	void ClearValueMap() { *this = 0; return; }

	void SetValueMap(const int value) { *this = value; return; }

	void SetValueMap(const int value, const int i, const int j) { this->map[i][j] = value; return; }

	void SetValueMap(const int value, sf::Vector2f coord)
	{
		int j = (int)(coord.x - 16) / 16;
		int i = (int)(coord.y - 8) / 16;
		this->map[i][j] = value;
		return;
	}

	int GetValueMap(const int i, const int j) const
	{
		if (i < sizeMap && j < sizeMap)
			return this->map[i][j];
		return -1;
	}

	int GetNextUpValueMap(const int i, const int j) const
	{
		if (i == 0)
			return -1;
		else {
			return this->map[i - 1][j];
		}
	}

	int GetNextDownValueMap(const int i, const int j) const
	{
		if (i == sizeMap - 1)
			return -1;
		else {
			return this->map[i + 1][j];
		}
	}

	int GetNextLeftValueMap(const int i, const int j) const
	{
		if (j == 0)
			return -1;
		else {
			return this->map[i][j - 1];
		}
	}

	int GetNextRightValueMap(const int i, const int j) const
	{
		if (j == sizeMap - 1)
			return -1;
		else {
			return this->map[i][j + 1];
		}
	}

	sf::Vector2f TakeCoord(const int i, const int j) const
	{
		sf::Vector2f coord;
		coord.x = float(j * 16 + 16);
		coord.y = float(i * 16 + 8);
		return coord;
	}

	int TakeIndex(sf::Vector2f coord, char ch) const
	{
		int index(0);

		if (ch == 'j')
			index = (int)(coord.x - 16) / 16;
		else if (ch == 'i')
			index = (int)(coord.y - 8) / 16;

		return index;
	}
};

#endif
