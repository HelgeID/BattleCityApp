#include <iostream>
#include <string>
#include <pugixml.hpp>
using namespace std;

int main()
{
	pugi::xml_document doc;
	doc.load_file("ui_sprite.xml");

	int x = doc.child("sprite").child("element1").attribute("x").as_int();
	cerr << x << endl;

	system("pause");
	return 0;
}
