#include "TextboxParent.h"
#include "Textbox.h"

TextboxParent::TextboxParent()
{
}
TextboxParent::~TextboxParent() {};

void TextboxParent::ShowTextbox(Textbox* t)
{
	textboxes.push_back(t);
	t->SetJustOpened(); //sets the delay
}

bool TextboxParent::UpdateTextboxes()
{
	if (textboxes.size() == 0)
		return false;
	unsigned int index = textboxes.size() - 1;
	Textbox* active = textboxes[index];
	if (!active || active->SetToClose())
		return false;
	active->Update();
	if (index != textboxes.size() - 1)
		return true;
	if (active->SetToClose())
	{
		if (active->DeleteOnClose())
		{
			delete active;
			textboxes[textboxes.size() - 1] = 0;
		}
		else
			active->CancelClose();
		textboxes.pop_back();
	}
	return true;
}

void TextboxParent::CloseAll(bool include_this)
{
	for (int i = textboxes.size() - 1; i >= 0; i--)
	{
		textboxes[i]->CloseAll();
		textboxes[i]->Close(true);
		if (textboxes[i]->SetToClose())
		{
			if (textboxes[i]->DeleteOnClose())
			{
				delete textboxes[i];
			}
			else
				textboxes[i]->CancelClose();
			textboxes.pop_back();
		}
	}
}

std::vector<Textbox*>& TextboxParent::GetTextboxes()
{
	return textboxes;
}