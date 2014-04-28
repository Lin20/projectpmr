#include "Script.h"
#include "MapScene.h"

Script::Script(MapScene* on_scene)
{
	this->on_scene = on_scene;
	this->buffer = 0;
}

Script::~Script()
{
	if (buffer)
		delete buffer;
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
}

void Script::Update()
{
	if (on_scene && on_scene->GetTextboxes().size() > 0)
		return;
	if (!buffer)
		return;

	if (buffer->data - buffer->data_start >= buffer->size)
		return;

	unsigned char opcode = *buffer->data++;
	unsigned int index = 0;

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

	case OPCODE_TEXT: //text
		if (on_scene)
		{
			Textbox* t = new Textbox();
			t->SetText(new TextItem(t, nullptr, pokestring(GetVariable().string_value)));
			on_scene->ShowTextbox(t);
		}
		break;
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
	return string((char*)buffer->data, len);
}