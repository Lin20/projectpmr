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
	for (unsigned int i = 0; i < textboxes.size(); i++)
	{
		if (!textboxes[i])
			continue;
		if (textboxes[i]->SetToClose())
		{
			if (textboxes[i]->DeleteOnClose())
			{
				delete textboxes[i];
				textboxes[i] = 0;
			}
			else
				textboxes[i]->CancelClose();
			textboxes.erase(textboxes.begin() + i--);
		}
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