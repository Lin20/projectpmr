#include "ItemStorage.h"

ItemStorage::ItemStorage()
{
	items.resize(MAX_ITEMS);
	for (int i = 0; i < MAX_ITEMS; i++)
		items.push_back(Item());
	menu = new Textbox(4, 2, 16, 11);
	auto a = [this](TextItem* i) { this->GenerateItems(); };
	TextItem* i = new TextItem(0, a);
	
}

ItemStorage::~ItemStorage()
{
}

void ItemStorage::GenerateItems()
{

}