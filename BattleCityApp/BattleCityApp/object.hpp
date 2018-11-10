#include <SFML/Graphics.hpp>
#include <pugixml.hpp>

class Object
{
	sf::Sprite* sprite;
	sf::Texture* texture;

	void loadTexture(const pugi::char_t* ui_name)
	{
		int posX, posY;
		pugi::xml_document doc;
		doc.load_file("data/ui/ui_texture.xml");
		posX = doc.child("sprite").child(ui_name).attribute("x").as_int();
		posY = doc.child("sprite").child(ui_name).attribute("y").as_int();
		this->texture->loadFromFile("data/graphics/texture/texture.png", sf::IntRect(posX, posY, 16, 16));
		return;
	}

public:
	Object(const pugi::char_t* ui_name = "", bool zoom = false) : texture(new sf::Texture())
	{
		loadTexture(ui_name); //load texture

		//init sprite and set texture
		this->sprite = new sf::Sprite(*this->texture);
		/*
		   z00m false : 1x --> 16x16
		   z00m true  : 2x --> 32x32
		*/
		if (zoom)
			sprite->scale(sf::Vector2f(2.f, 2.f));
	}

	void reloadObj(const pugi::char_t* ui_name = "")
	{
		loadTexture(ui_name); //load texture

		//set texture
		this->sprite->setTexture(*this->texture);
		return;
	}

	void setCoordObj(const float& x, const float& y)
	{
		//set coordinates
		this->sprite->setPosition(x, y);
		return;
	}

	void setCoordObj(const sf::Vector2f&  position)
	{
		//set coordinates
		this->sprite->setPosition(position);
		return;
	}

	const sf::Vector2f& getCoordObj()
	{
		//get coordinates
		return this->sprite->getPosition();
	}

	~Object()
	{
		delete sprite;
		delete texture;
	}

	sf::Sprite takeObj() const { return *sprite; }
};
