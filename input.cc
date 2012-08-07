
#include "input.h"
#include<cstring>
#include<boost/regex.hpp>
#include<iostream>

input::input( converter& conv)
		:m_conv( conv ){
}

input::input( std::string raw, converter& conv )
		:m_conv(conv){
		m_tokens.clear();
		split( raw );
		convert();
}

input::~input(){

}

void input::convert(){
		m_intoks.clear();
		for( int i=0; i < m_tokens.size(); ++i){
				m_intoks.push_back( m_conv.str2idx( m_tokens[i] ) );
		}
}

void input::split( std::string raw ){

#ifdef DEBUG
		std::cout << "input::split\t origianl string\n" << raw << std::endl;
#endif
		boost::regex sp("\\s+");
		boost::sregex_token_iterator  sent( raw.begin(),
											raw.end(),
											sp, -1);
		boost::sregex_token_iterator end;
		while( sent != end ){

				m_tokens.push_back( *sent );
#ifdef DEBUG
				std::cout << "input::split\t" << sent->str() << std::endl;
#endif
				++sent;

		}

#ifdef DEBUG
		std::cout << std::endl;
#endif
}

void input::setsrc( std::string raw ){
		m_tokens.clear();
		split( raw );
		convert();
}

std::vector<unsigned int>& input::result(){
		return m_intoks;
}
