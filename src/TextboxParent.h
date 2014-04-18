#pragma once

#include <vector>
#include "Common.h"

class TextboxParent
{
public:
	TextboxParent();
	virtual ~TextboxParent();

	virtual void ShowTextbox(Textbox* t);

	virtual bool UpdateTextboxes();

protected:
	std::vector<Textbox*> textboxes;
};