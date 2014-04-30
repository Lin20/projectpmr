set ( PMRS_SRCS
        CommandParser.cpp
        Commands.cpp
        Compiler.cpp
        ErrorReporter.cpp
        Line.cpp
        Preprocessor.cpp
        Startup.cpp
        TokenParser.cpp
        )

add_executable(pmrs ${PMRS_SRCS})

