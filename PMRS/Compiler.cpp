#include "Compiler.h"

Compiler::Compiler(ostream& out, const char* filename)
{
	Commands::LoadCommands();
	lines.clear();

	std::ifstream t(filename);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	std::string orig = string(str);

	if (!str.empty())
	{
		Preprocessor::Process(str);
		Preprocessor::FindLines(lines, str);
		Preprocessor::ProcessDirectives(lines, str, string(filename));
		if (ErrorReporter::GetErrorCount())
		{
			ErrorReporter::Printout(out);
			out << "Build failed with " << ErrorReporter::GetErrorCount() << " errors and " << (ErrorReporter::GetMessageCount() - ErrorReporter::GetErrorCount()) << " warnings.\n\n";
		}
		else
		{
			out << "Build succeeded with 0 errors and " << (ErrorReporter::GetMessageCount() - ErrorReporter::GetErrorCount()) << " warnings.\n\n";
		}

		out << "\nToken Printout:\n\n";
		for (unsigned int i = 0; i < lines.size(); i++)
		{
			out << "Line " << i << " (" << lines[i].GetTokens().size() << "): ";
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

Compiler::~Compiler()
{
}
