#pragma once

#include <string>

namespace nbeaterlib
{
	namespace logger
	{
		enum levels
		{
			Info,
			Error,
			Success
		};

		void log(levels format, std::string msg);
	}
}
