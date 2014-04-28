#include "Compiler.h"

Compiler::Compiler(ostream& o, const char* filename, string output_dir, bool silent, bool debug_info) : out(o)
{
	Commands::LoadCommands();
	lines.clear();

	this->filename = filename;
	this->output_directory = output_dir;
	this->silent = silent;
	this->print_debug = debug_info;
}

Compiler::~Compiler()
{
}

void Compiler::Compile()
{
	std::ifstream t(filename);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	std::string orig = string(str);

	if (!str.empty())
	{
		if (!silent)
			out << "Compiling " << filename << "...\n\n";
		Preprocessor::Process(str);
		Preprocessor::FindLines(lines, str);
		Preprocessor::ProcessDirectives(lines, str, string(filename));
		unsigned int offset = 0;
		unsigned char buffer[65536];
		unsigned char* _buffer = &buffer[0];
		for (unsigned int i = 0; i < lines.size(); i++)
		{
			unsigned int p_off = offset;
			CommandParser::ProcessCommandStage1(_buffer, lines[i], offset);
			if (print_debug)
			{
				out << lines[i].GetFormattedText() << " = ";
				for (unsigned int i = p_off; i < offset; i++)
					out << (buffer[i] < 16 ? "0" : "") << hex << (int)buffer[i] << " ";
				out << "\n";
			}
		}
		buffer[offset++] = 0;
		CommandParser::ProcessCommandStage2(_buffer);

		if (ErrorReporter::GetErrorCount())
		{
			if (!silent)
				ErrorReporter::Printout(out);
			out << "Build failed with " << ErrorReporter::GetErrorCount() << " errors and " << (ErrorReporter::GetMessageCount() - ErrorReporter::GetErrorCount()) << " warnings.\n\n";
		}
		else
		{
			string name = filename;
			ofstream file_out(output_directory, ofstream::trunc | ofstream::binary);
			if (!file_out)
			{
				ErrorReporter::AddError(string("Cannot write to file \'").append(output_directory).append("\'."), 0, name);
			}
			else
			{
				file_out.write((const char*)buffer, offset);
			}

			if (!silent)
				ErrorReporter::Printout(out);
			if (ErrorReporter::GetErrorCount())
				out << "Build failed with " << ErrorReporter::GetErrorCount() << " errors and " << (ErrorReporter::GetMessageCount() - ErrorReporter::GetErrorCount()) << " warnings.\n\n";
			else
				out << "Build succeeded with 0 errors and " << (ErrorReporter::GetMessageCount() - ErrorReporter::GetErrorCount()) << " warnings.\n\n";
		}

		if (!silent && print_debug)
		{
			out << "\nBinary Printout:\n\n";
			for (unsigned int i = 0; i < offset; i++)
				out << (buffer[i] < 16 ? "0" : "") << hex << (int)buffer[i] << " ";

			out << "\n\n\nToken Printout:\n\n";
			for (unsigned int i = 0; i < lines.size(); i++)
			{
				out << "Line " << lines[i].GetLineNumber() << " (" << lines[i].GetTokens().size() << "): ";
				for (unsigned int k = 0; k < lines[i].GetTokens().size(); k++)
				{
					out << "| " << lines[i].GetTokens()[k] << " | ";
				}
				out << "\n";
			}

			out << "\n\nOriginal:\n\n";
			out << orig;
		}
	}
	else
	{
		out << "File empty or nonexistant: " << filename << ".\n";
	}
}