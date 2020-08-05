#pragma once

#include "Common.h"
#include <SFML/Graphics.hpp>
#include "ResourceCache.h"
#include "TextboxParent.h"
#include "Fade.h"

class Scene : public TextboxParent
{
public:
	Scene();
	virtual ~Scene() = 0;

	virtual void Update() = 0;
	virtual void Render(sf::RenderWindow* window) = 0;
	virtual void NotifySwitchedTo();

protected:
	Fade current_fade;
	bool input_enabled;
};