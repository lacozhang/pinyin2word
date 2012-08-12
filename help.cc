#include "help.h"
#include<boost/regex.hpp>

bool helper::operator()( std::string& raw, std::string& first, std::string& second ){
		boost::regex sp(":");
		boost::sregex_token_iterator tok( raw.begin(), raw.end(),
										  sp, -1);
		boost::sregex_token_iterator end;

		first = tok->str();
		++tok;
		second = tok->str();
  
}

