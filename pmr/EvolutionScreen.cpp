#include "EvolutionScreen.h"


EvolutionScreen::EvolutionScreen(Pokemon* p, unsigned char evolution)
{
	pokemon = p;
	pokemon_to = new Pokemon(evolution);
	from_black = new PaletteTexture();
	to_black = new PaletteTexture();
	from_black->Copy(ResourceCache::GetPokemonFront(p->pokedex_index));
	to_black->Copy(ResourceCache::GetPokemonFront(ResourceCache::GetPokedexIndex(evolution - 1)));

	sf::Color black[4] = { ResourceCache::GetMenuTexture()->GetPalette()[0], sf::Color(7 * 8, 7 * 8, 7 * 8, 255), sf::Color(2 * 8, 3 * 8, 3 * 8, 255), sf::Color(2 * 8, 1 * 8, 1 * 8, 255) };

	from_black->SetPalette(black);
	to_black->SetPalette(black);

	main_frame = new Textbox(0, 0, 20, 12, true, true);
	main_frame->SetMenu(true, 0, sf::Vector2i(), sf::Vector2u(), [this](TextItem* s) {this->HitB(); }, MenuFlags::FOCUSABLE, 2147u, nullptr, true, sf::Vector2i(-100, 0));
	main_frame->SetArrowState(ArrowStates::ACTIVE);
	main_frame->SetRenderCallback([this](sf::RenderWindow* w) {this->Render(w); });

	begin_timer = 60;
}

EvolutionScreen::~EvolutionScreen()
{
	if (from_black)
		delete from_black;
	if (to_black)
		delete to_black;
	if (pokemon_to)
		delete pokemon_to;
}

void EvolutionScreen::Show(Textbox* src)
{
	parent = src;
	src->ShowTextbox(main_frame);
	frames = 1;
	delay = 16;
	color_timer = 80;
	frames_left = 0;
	delay_left = 4;
}

void EvolutionScreen::Render(sf::RenderWindow* window)
{
	if (begin_timer > 0)
	{
		begin_timer--;
		return;
	}


	unsigned char x = pokemon->size_x;
	unsigned char y = pokemon->size_y;

	sf::Sprite pokesprite;
	if (color_timer > 0)
	{
		if (color_timer < 255)
			color_timer--;
		if (frames == 1)
			pokesprite.setTexture(*ResourceCache::GetPokemonFront(pokemon->pokedex_index));
		else
		{
			pokesprite.setTexture(*to_black);
			x = pokemon_to->size_x;
			y = pokemon_to->size_y;
			if (!color_timer)
				delay = 255;
		}
	}
	else
	{
		if (delay_left > 0)
		{
			delay_left--;
			pokesprite.setTexture(*from_black);
			x = pokemon->size_x;
			y = pokemon->size_y;
		}
		else if (frames_left > 0)
		{
			pokesprite.setTexture((frames_left % 6 > 2 ? *from_black : *to_black));
			x = (frames_left % 6 > 2 ? pokemon->size_x : pokemon_to->size_x);
			y = (frames_left % 6 > 2 ? pokemon->size_y : pokemon_to->size_y);
			frames_left--;
		}
		else
		{
			if (delay == 255)
			{
				pokesprite.setTexture(*ResourceCache::GetPokemonFront(pokemon_to->pokedex_index));
				if (main_frame->GetTextboxes().size() == 0)
					Finalize();
			}
			else
			{
				pokesprite.setTexture(*to_black);
				color_timer = 50;
			}
			x = pokemon_to->size_x;
			y = pokemon_to->size_y;
		}
		if (frames_left == 0 && delay_left == 0 && delay > 1 && delay != 255 && !color_timer)
		{
			frames_left = frames++ * 6 - 3;
			delay_left = delay * 2;
			delay -= 2;
		}
	}
	sf::IntRect ir;
	ir.left = x * 8;
	ir.top = 0;
	ir.width = -x * 8;
	ir.height = y * 8;
	pokesprite.setTextureRect(ir);
	pokesprite.setPosition((float)(56), (float)(64 - ir.height + 16));
	window->draw(pokesprite);
}

void EvolutionScreen::Finalize()
{
	Textbox* main_frame = this->main_frame;
	Textbox* evolved = new Textbox();

	bool learned_move = false;
	std::function<void(TextItem* s)> m_f = nullptr;
	for (int i = 0; i < 16; i++)
	{
		if (pokemon_to->learnset[i].level == 0)
			break;
		if (pokemon_to->learnset[i].level == pokemon->level)
		{
			m_f = PokemonUtils::LearnMove(evolved, pokemon, pokemon_to->learnset[i].move);
			learned_move = true;
		}
	}

	std::function<void(TextItem* s)> a = [m_f, evolved](TextItem* src) {
		evolved->CancelClose(); 
		m_f(src); 
	};
	evolved->SetText(new TextItem(evolved, (!m_f ? [main_frame, evolved](TextItem* src) {evolved->Close(); main_frame->Close(); } : a), string(pokemon->nickname).append(pokestring(" evolved\ninto ")).append(pokemon_to->original_name).append(pokestring("!\t\t\t\t\t\t\a"))));
	main_frame->ShowTextbox(evolved, false);
	if (learned_move)
	{
		auto b = [evolved, main_frame, this](TextItem* s)
		{
			evolved->Close(true);
			main_frame->Close();
			this->parent->Close();
		};
		evolved->SetCloseCallback(b);
	}

	pokemon->id = pokemon_to->id;
	pokemon->pokedex_index = pokemon_to->pokedex_index;
	pokemon->LoadStats();
	unsigned int hp = pokemon->max_hp;
	pokemon->RecalculateStats();
	pokemon->hp += pokemon->max_hp - hp;
}

void EvolutionScreen::HitB()
{
	if (main_frame->GetTextboxes().size() == 0)
		main_frame->CancelClose();
	if (color_timer)
		return;
	if (frames == 1 || delay == 255)
		return;

	color_timer = 255;
	frames = 1;
	Textbox* frame = main_frame;
	Textbox* canceled = new Textbox();
	canceled->SetText(new TextItem(canceled, [frame](TextItem* src) {frame->Close(true); }, pokestring("Huh? ").append(pokemon->nickname).append(pokestring("\nstopped evolving!\f"))));
	main_frame->ShowTextbox(canceled, false);
}
