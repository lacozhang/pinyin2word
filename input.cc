
#include "input.h"
#include<cstring>

input::input(){
}

input::input( std::string raw ){
  split( raw );
}

input::~input(){
  
}

void input::split( std::string raw ){
  
  std::string tmp;
  tmp.clear();
  for( int i=0; i < raw.length(); ++i){

	if( isspace( raw[i]) ){
	  while( isspace( raw[i] ) ) ++i;
	  m_tokens.push_back( tmp );
	  tmp.clear();
	}
	tmp += raw[i];
  }
  m_tokens.push_back( tmp );

}

void input::setsrc( std::string raw ){
  m_tokens.clear();
  split( raw );
}

input::iterator input::begin(){
  return m_tokens.begin();
}

input::iterator input::end(){
  return m_tokens.end();
}
