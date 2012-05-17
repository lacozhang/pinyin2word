#include "options.h"
#include<boost/regex.hpp>

options::options( std::string raw ){
  split( raw );
}

options::~options(){
}

options::iterator options::begin(){
  return m_body.begin();
}

options::iterator options::end(){
  return m_body.end();
}

std::string options::head(){
  return m_head;
}

void options::split( std::string raw ){

}
