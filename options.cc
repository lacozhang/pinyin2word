#include "options.h"
#include<iostream>
#include<boost/regex.hpp>

options::options( std::string raw ){
  split( raw );
}

options::~options(){
}

options::iterator options::begin() const{
  return m_body.begin();
}

options::iterator options::end() const{
  return m_body.end();
}

std::string options::head() const{
  return m_head;
}

//file format : head:options1\toptions2\t...optionsn\n
void options::split( std::string raw ){

  boost::regex hs(":");//split head and body
  boost::regex bs("\t+");//split body

  boost::sregex_token_iterator hs_it(raw.begin(),
									  raw.end(),
									  hs, -1);
  boost::sregex_token_iterator end;

  std::string head, body;
  m_head = hs_it->str();
  hs_it++;
  body = hs_it->str();

  boost::sregex_token_iterator bs_it( body.begin(),
									  body.end(),
									  bs, -1);
  while( bs_it != end ){
	m_body.push_back( *bs_it);
	++bs_it;
  }
 
}
