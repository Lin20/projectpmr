#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "ResourceCache.h"
#include "Textbox.h"

class Scene
{
public:
	Scene();
	virtual ~Scene() = 0;

	virtual void Update() = 0;
	virtual void Render(sf::RenderWindow* window) = 0;
	virtual void NotifySwitchedTo();

protected:
	std::vector<Textbox*> textboxes;
};