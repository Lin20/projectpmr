#pragma once

#include <vector>
#include "Common.h"

class TextboxParent
{
public:
	TextboxParent();
	virtual ~TextboxParent();

	virtual void ShowTextbox(Textbox* t, bool show_delay = true);

	virtual bool UpdateTextboxes();
	virtual void CloseAll(bool include_this = false);

	std::vector<Textbox*>& GetTextboxes();

protected:
	std::vector<Textbox*> textboxes;
};