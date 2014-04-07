#include "TextboxParent.h"
#include "Textbox.h"

TextboxParent::TextboxParent()
{
}
TextboxParent::~TextboxParent() {};

void TextboxParent::ShowTextbox(Textbox* t)
{
	textboxes.push_back(t);
}

bool TextboxParent::UpdateTextboxes()
{
	if (textboxes.size() == 0)
		return false;
	Textbox* active = textboxes[textboxes.size() - 1];
	active->Update();
	if (active->SetToClose())
	{
		if (active->DeleteOnClose())
			delete active;
		else
			active->CancelClose();
		textboxes.pop_back();
	}
	return true;
}