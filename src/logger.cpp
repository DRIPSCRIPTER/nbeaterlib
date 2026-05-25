#include "../include/nbeaterlib/logger.h"
#include <cstdio>

#define RESET "\033[0m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define GREEN "\033[32m"
    
static const char* prefix(nbeaterlib::logger::levels Format)
{
    switch (Format)
    {
    case nbeaterlib::logger::levels::Info: return YELLOW "[INFO] " RESET;
    case nbeaterlib::logger::levels::Error: return RED "[ERROR] " RESET;
    case nbeaterlib::logger::levels::Success: return GREEN  "[SUCCESS] " RESET;
    default: return "[LOG] ";
    }
}
namespace nbeaterlib
{
    namespace logger
    {
        void log(levels format, std::string msg)
        {
            printf("%s %s\n", prefix(format), msg.c_str());
        }
    }
}
