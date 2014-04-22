#pragma once

#include <functional>
#include "Common.h"
#include "Textbox.h"
#include "StringConverter.h"

class TextItem
{
public:
	TextItem(Textbox* owner, std::function<void(TextItem* source)> action_callback = nullptr, string text = 0, unsigned char index = 0, unsigned int value = 0)
	{
		this->owner_textbox = owner;
		//for this stupid block of code, see Textbox.cpp
		if(action_callback)
			this->callback = action_callback;
		else
			this->callback = nullptr;
		this->text = text;
		this->index = index;
		this->value = value;
		//pokestring(this->text);
	}

	~TextItem()
	{
	}

	void Action()
	{
		if (callback != nullptr)
			callback(this);
	}

	void SetText(const std::string& to)
	{
		this->text = to;
		//pokestring(this->text);
	}

	string& GetText() { return text; }
	Textbox* GetParent() { return owner_textbox; }

	string text; //the text that the item displays
	unsigned char index; //the slot of the item
	unsigned int value; //the value on the item (used for items)

private:
	Textbox* owner_textbox;
	std::function<void(TextItem* source)> callback; //the function that gets called when the item is selected
};
