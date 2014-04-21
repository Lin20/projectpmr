#include "MenuCache.h"

Textbox* MenuCache::start_menu = 0;
Textbox* MenuCache::debug_menu = 0;
ItemStorage* MenuCache::item_menu = 0;

MenuCache::MenuCache()
{
}

MenuCache::~MenuCache()
{
}

void MenuCache::ReleaseResources()
{
	if (start_menu)
		delete start_menu;
	if (debug_menu)
		delete debug_menu;
	if (item_menu)
		delete item_menu;
}

//Return the start menu or create it if it doesn't exist
//Since the game starts without the pokedex, we shouldn't add it here but in MapScene
Textbox* MenuCache::StartMenu(std::vector<Textbox*>* owner)
{
	if (start_menu)
		return start_menu;

	start_menu = new Textbox(10, 0, 10, 16, false);
	start_menu->SetMenu(true, 7, sf::Vector2i(1, 1), sf::Vector2u(0, 2), nullptr, MenuFlags::FOCUSABLE | MenuFlags::WRAPS);

	auto doe = [](TextItem* source)->void
	{

		Textbox* message = new Textbox();
		message->SetText(new TextItem(start_menu, [](TextItem* source)->void { start_menu->SetArrowState(ArrowStates::ACTIVE); }, pokestring(string("Sorry! That\nfeature has not\vbeen implemented\vyet."))));
		start_menu->SetArrowState(ArrowStates::INACTIVE);
		start_menu->ShowTextbox(message);
	};
	start_menu->GetItems().push_back(new TextItem(start_menu, doe, pokestring("POKéDEX"), 0));
	start_menu->GetItems().push_back(new TextItem(start_menu, doe, pokestring("POKéMON"), 1));
	start_menu->GetItems().push_back(new TextItem(start_menu, [](TextItem* source)->void
	{
		start_menu->SetArrowState(ArrowStates::INACTIVE);
		start_menu->ShowTextbox(ItemMenu()->GetMenu());
	}
	, pokestring("ITEMS"), 2));
	start_menu->GetItems().push_back(new TextItem(start_menu, doe, pokestring("Lin"), 3));
	start_menu->GetItems().push_back(new TextItem(start_menu, doe, pokestring("SAVE"), 4));
	start_menu->GetItems().push_back(new TextItem(start_menu, doe, pokestring("OPTIONS"), 5));
	start_menu->GetItems().push_back(new TextItem(start_menu, [](TextItem* source)->void { start_menu->Close(); }, pokestring("EXIT"), 6));

	start_menu->UpdateMenu();
	return start_menu;
}

Textbox* MenuCache::DebugMenu(std::vector<Textbox*>* owner)
{
	if (debug_menu)
		return debug_menu;

	debug_menu = new Textbox(0, 8, 14, 7, false);
	debug_menu->SetMenu(true, 3, sf::Vector2i(1, 1), sf::Vector2u(0, 1), nullptr, MenuFlags::FOCUSABLE);
	debug_menu->GetItems().push_back(new TextItem(debug_menu, nullptr, "Testing"));
	debug_menu->GetItems().push_back(new TextItem(debug_menu, nullptr, "multiple"));
	debug_menu->GetItems().push_back(new TextItem(debug_menu, nullptr, "menus."));

	debug_menu->UpdateMenu();
	return debug_menu;
}

ItemStorage* MenuCache::ItemMenu(std::vector<Textbox*>* owner)
{
	if (item_menu)
		return item_menu;

	item_menu = new ItemStorage();
	return item_menu;
}