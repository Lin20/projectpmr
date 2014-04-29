#include "Script.h"
#include "MapScene.h"

Script::Script(MapScene* on_scene)
{
	this->on_scene = on_scene;
	this->buffer = 0;
	this->built_menu = 0;
	this->entity_wait = false;
}

Script::~Script()
{
	if (buffer)
		delete buffer;
	if (built_menu)
		delete built_menu;
}

Script* Script::TryLoad(MapScene* on_scene, unsigned char map, unsigned char script_index)
{
	if (!CheckExists(map, script_index))
		return 0;
	Script* s = new Script(on_scene);
	s->Load(map, script_index);
	return s;
}

bool Script::CheckExists(unsigned char map, unsigned char script_index)
{
	string filename = ResourceCache::GetResourceLocation(string("scripts/bin/").append(itos(map)).append("_").append(itos(script_index)).append(".dat"));
	ifstream i(filename.c_str());
	//this is stupid, but ifstream can't implicitly be converted to a boolean
	//nor does it have a != operator, so... yeah...
	bool b = !i;
	return !b;
}

bool Script::Load(string& filename)
{
	ResetVariables();
	buffer = ReadFile(filename);
	return buffer != 0;
}

bool Script::Load(unsigned char map, unsigned char script_index)
{
	string filename = ResourceCache::GetResourceLocation(string("scripts/bin/").append(itos(map)).append("_").append(itos(script_index)).append(".dat"));
	return Load(filename);
}

bool Script::Done()
{
	return(!buffer || buffer->data == buffer->data_start + buffer->size);
}

void Script::Reset()
{
	ResetVariables();
	if (buffer)
		buffer->data = buffer->data_start;
	watch_entities.clear();
}

void Script::Update()
{
	//we have a while loop here to execute as many commands as possible
	//it's necessary if, for example, we have a lot of logic that goes on before showing
	//a textbox. the textbox would be severely delayed
	while (true)
	{
		if (!buffer)
			return;
		if (on_scene && on_scene->GetTextboxes().size() > 0)
			return;
		if (buffer->data - buffer->data_start >= buffer->size) //if the script is done
			return;

		if (on_scene && watch_entities.size() > 0)
		{
			for (unsigned int i = 0; i < watch_entities.size(); i++)
			{
				if (on_scene->GetEntities()[watch_entities[i]]->Snapped() && on_scene->GetEntities()[watch_entities[i]]->GetStepsRemaining() == 0)
				{
					on_scene->GetEntities()[watch_entities[i]]->SetEntityGhosting(false);
					watch_entities.erase(watch_entities.begin() + i--);
				}
			}
			if (entity_wait && watch_entities.size() > 0)
				return;
			else if (entity_wait)
				entity_wait = false;
		}

		unsigned char opcode = *buffer->data++;
		unsigned int index = 0;
		unsigned int value = 0;
		unsigned int a = 0, b = 0, c = 0, d = 0;
		string s = "";
		Variable v;

		switch (opcode)
		{
		case OPCODE_END: //end
			buffer->data = buffer->data_start + buffer->size;
			break;

		case OPCODE_VAR: //var
			index = buffer->getc() + (buffer->getc() << 8);
			if (index < MAX_VARS)
			{
				variables[index].int_value = 0;
				variables[index].string_value = "";
			}
			break;

		case OPCODE_SET: //set
			index = buffer->getc() + (buffer->getc() << 8);
			SetVariable(index);
			break;

		case OPCODE_JUMP: //jump
			index = buffer->getc() + (buffer->getc() << 8);
			if (index < buffer->size)
				buffer->data = buffer->data_start + index;
			break;

		case OPCODE_ADD: //add
			index = buffer->getc() + (buffer->getc() << 8);
			value = GetVariable().int_value;
			if (index < MAX_VARS)
				variables[index].int_value += value;
			break;

		case OPCODE_APP: //append
			index = buffer->getc() + (buffer->getc() << 8);
			s = GetVariable().string_value;
			if (index < MAX_VARS)
				variables[index].string_value.append(s);
			break;

		case OPCODE_APPI: //append integer
			index = buffer->getc() + (buffer->getc() << 8);
			value = GetVariable().int_value;
			if (index < MAX_VARS)
				variables[index].string_value.append(itos(value));
			break;

		case OPCODE_TEXT: //text
			if (on_scene)
			{
				Textbox* t = new Textbox();
				t->SetText(new TextItem(t, nullptr, pokestring(GetVariable().string_value)));
				on_scene->ShowTextbox(t);
			}
			break;

		case OPCODE_MOVE: //move
			if (on_scene)
			{
				index = GetVariable().int_value;
				if (index < on_scene->GetEntities().size())
				{
					unsigned char dir = GetVariable().int_value;
					unsigned char steps = GetVariable().int_value;
					on_scene->GetEntities()[index]->Move(dir, steps);
					on_scene->GetEntities()[index]->SetEntityGhosting(true);
					watch_entities.push_back(index);
				}
			}
			break;

		case OPCODE_JEQ: //jump equal to
			index = buffer->getc() + (buffer->getc() << 8);
			value = buffer->getc() + (buffer->getc() << 8);
			v = GetVariable();
			if (index < buffer->size && value < MAX_VARS)
			{
				if (variables[value].int_value == v.int_value && variables[value].string_value == v.string_value)
					buffer->data = buffer->data_start + index;
			}
			break;

		case OPCODE_JNE: //jump not equal to
			index = buffer->getc() + (buffer->getc() << 8);
			value = buffer->getc() + (buffer->getc() << 8);
			v = GetVariable();
			if (index < buffer->size && value < MAX_VARS)
			{
				if (variables[value].int_value != v.int_value || variables[value].string_value != v.string_value)
					buffer->data = buffer->data_start + index;
			}
			break;

		case OPCODE_JGT: //jump great than
			index = buffer->getc() + (buffer->getc() << 8);
			value = buffer->getc() + (buffer->getc() << 8);
			v = GetVariable();
			if (index < buffer->size && value < MAX_VARS)
			{
				if (variables[value].int_value > v.int_value)
					buffer->data = buffer->data_start + index;
			}
			break;

		case OPCODE_JLT: //jump less than
			index = buffer->getc() + (buffer->getc() << 8);
			value = buffer->getc() + (buffer->getc() << 8);
			v = GetVariable();
			if (index < buffer->size && value < MAX_VARS)
			{
				if (variables[value].int_value < v.int_value)
					buffer->data = buffer->data_start + index;
			}
			break;

		case OPCODE_CHECKFLAG: //check flag
			a = GetVariable().int_value; //map
			b = GetVariable().int_value; //flag index
			index = buffer->getc() + (buffer->getc() << 8);
			if (index < MAX_VARS)
				variables[index].int_value = on_scene->GetFlag(a * 16 + b);
			break;

		case OPCODE_SETFLAG: //set flag
			a = GetVariable().int_value; //map
			b = GetVariable().int_value; //flag index
			index = GetVariable().int_value; //flag value
			on_scene->SetFlag(a * 16 + b, index > 0 ? true : false);
			break;

		case OPCODE_CLEARMENU: //clears the built menu
			if (built_menu)
				built_menu->ClearItems();
			break;

		case OPCODE_ADDMENU: //adds an item to the built menu
			s = pokestring(GetVariable().string_value);
			if (s.length() != 0)
			{
				built_menu->GetItems().push_back(new TextItem(built_menu, [this](TextItem* i) { this->SetMenuResult(i->index + 1); this->GetBuiltMenu()->Close(); this->ClearBuiltMenu(); }, s, built_menu->GetItems().size()));
			}
			break;

		case OPCODE_SHOWMENU: //show built menu
			menu_variable = buffer->getc() + (buffer->getc() << 8); //variable to store result in
			if (built_menu)
			{
				built_menu->UpdateMenu();
				on_scene->ShowTextbox(built_menu);
			}
			break;

		case OPCODE_INITMENU: //init menu
			a = GetVariable().int_value;
			b = GetVariable().int_value;
			c = GetVariable().int_value;
			d = GetVariable().int_value;
			if (!built_menu)
			{
				built_menu = new Textbox(a, b, c, d);
			}
			built_menu->SetMenu(true, 16, sf::Vector2i(1, 1), sf::Vector2u(0, 2), [this](TextItem* i) { if (i)this->SetMenuResult(i->index + 1); this->ClearBuiltMenu(); }, MenuFlags::FOCUSABLE | MenuFlags::WRAPS);
			built_menu->SetArrowState(ArrowStates::ACTIVE);
			break;

		case OPCODE_GETX: //get entity x
			index = GetVariable().int_value;
			a = buffer->getc() + (buffer->getc() << 8);
			if (index < on_scene->GetEntities().size() && a < MAX_VARS)
				variables[a].int_value = on_scene->GetEntities()[index]->x / 16;
			break;

		case OPCODE_GETY: //get entity y
			index = GetVariable().int_value;
			a = buffer->getc() + (buffer->getc() << 8);
			if (index < on_scene->GetEntities().size() && a < MAX_VARS)
				variables[a].int_value = on_scene->GetEntities()[index]->y / 16;
			break;

		case OPCODE_TURN: //turn entity
			index = GetVariable().int_value;
			value = GetVariable().int_value;
			if (index < on_scene->GetEntities().size())
				on_scene->GetEntities()[index]->Face(value);
			break;

		case OPCODE_TEXTMENU: //text menu
			if (on_scene)
			{
				auto showmenu = [this](TextItem* src)
				{
					if (built_menu)
					{
						built_menu->UpdateMenu();
						src->GetParent()->ShowTextbox(built_menu);
						src->GetParent()->CancelClose();
					}
				};
				s = pokestring(GetVariable().string_value);
				menu_variable = buffer->getc() + (buffer->getc() << 8);
				Textbox* t = new Textbox();
				t->SetText(new TextItem(t, showmenu, s));
				on_scene->ShowTextbox(t);
			}
			break;

		case OPCODE_WAIT: //wait
			entity_wait = true;
			break;
		}
	}
}

void Script::ResetVariables()
{
	for (unsigned int i = 0; i < 256; i++)
	{
		variables[i].int_value = 0;
		variables[i].string_value = "";
	}
}

bool Script::CheckSpace(unsigned char amount)
{
	return buffer->data + amount < buffer->data_start + buffer->size;
}

void Script::SetVariable(unsigned int index)
{
	unsigned char type = buffer->getc();
	unsigned int v;
	switch (type)
	{
	case 0: //raw int
		v = buffer->getc() + (buffer->getc() << 8);
		variables[index].int_value = v;
		break;
	case 1: //int var
		v = buffer->getc() + (buffer->getc() << 8);
		if (v < MAX_VARS)
			variables[index].int_value = variables[v].int_value;
		break;
	case 2: //string var
		v = buffer->getc() + (buffer->getc() << 8);
		if (v < MAX_VARS)
			variables[index].string_value = variables[v].string_value;
		break;
	case 3: //raw string
		variables[index].string_value = ReadString();
		break;
	}
}

Variable Script::GetVariable()
{
	unsigned char type = buffer->getc();
	unsigned int v;
	Variable vari;
	vari.int_value = 0;
	vari.string_value = "";
	switch (type)
	{
	case 0: //raw int
		v = buffer->getc() + (buffer->getc() << 8);
		vari.int_value = v;
		return vari;
	case 1: //int var
	case 2: //string var
		v = buffer->getc() + (buffer->getc() << 8);
		if (v < MAX_VARS)
			return variables[v];
		return vari;
	case 3: //raw string
		vari.string_value = ReadString();
		return vari;
	}

	return vari;
}

string Script::ReadString()
{
	unsigned int len = buffer->getc() + (buffer->getc() << 8);
	char* at = (char*)buffer->data;
	buffer->data += len;
	return string(at, len);
}