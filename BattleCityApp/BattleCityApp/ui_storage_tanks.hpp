#pragma once

#include <SFML/Graphics.hpp>
#include "general.hpp"
#include "ui.hpp"

class StorageTanks : public sf::Drawable, public sf::Transformable
{
	sf::Texture *texture;

	UITank *elUITank;
	UITank arrUITank[20]
	{ *elUITank,*elUITank,*elUITank,*elUITank,*elUITank,*elUITank,*elUITank,*elUITank,*elUITank,*elUITank,
		*elUITank,*elUITank,*elUITank,*elUITank,*elUITank,*elUITank,*elUITank,*elUITank,*elUITank,*elUITank };
	
public:
	StorageTanks(sf::Texture &texture)
		: elUITank(new UITank(texture)), texture(&texture)
	{
		//init positions
		float posX = (256.f); float posY = (96.f);

		for (int index(0); index < 20; index += 2)
		{
			arrUITank[index + 0].setPosObj(posX, posY);
			arrUITank[index + 1].setPosObj(posX-8, posY);
			posY -= 8.f;
		}
	}
	~StorageTanks() { delete elUITank; }

	UITank* TakeArrUITank() { return arrUITank; }
	size_t TakeArrUITankSize() { return (sizeof arrUITank / sizeof *arrUITank); }

	void EnableStorageTanks()
	{
		int counter(0);
		for (int index(0); index < 20; ++index)
			if (arrUITank[index].isClearObj())
				counter++;

		if (counter == 20)
			for (int index(0); index < 20; ++index)
				arrUITank[index].onUITank();

		return;
	}

private:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
};

inline void StorageTanks::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &*texture;

	//draw the array
	for (int index(0); index < 20; ++index)
		target.draw(arrUITank[index].takeObj(), states);

	return;
}
