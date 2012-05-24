#include "options.h"
#include<iostream>
#include<boost/regex.hpp>

options::options(){

}

options::~options(){
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

converter& options::pingying_c(){
  return m_pingying;
}

converter& options::chinese_c(){
  return m_word;
}

MyMap& options::word2ping(){
  return m_w2p;
}

MyMap& options::ping2word(){
  return m_p2w;
}

void options::add_new( std::string str ){
#ifdef DEBUG
  static int s_cnt=0;
#endif
  split( str );
  unsigned int hidx = m_pingying.str2idx(m_head);
  for( int i=0; i<m_body.size(); ++i){
	unsigned int wordidx = m_word.str2idx( m_body[i] );
	m_w2p.add( wordidx, hidx );
	m_p2w.add( hidx, wordidx );
#ifdef DEBUG
	s_cnt++;
	std::cout << "s_cnt\t" << s_cnt << std::endl;
#endif	
  }
  m_head.clear();
  m_body.clear();
}

void MyMap::add( unsigned int key, unsigned int value ){
  if( (m_element[key]).value == -1 ){
	(m_element[key]).value = value;
  } else {
	if( m_free <= m_count ){
	  std::cerr << "MyMap::add : error" << std::endl;
	  std::cerr << "count : " << m_count << std::endl;
	  std::cerr << "m_free : "<< m_free << std::endl;
	  exit(1);
	}
	(m_element[m_free]).next = (m_element[key]).next;
	(m_element[key]).next = m_free;
	(m_element[m_free]).value = value;
	m_free--;
  }
  if( key > m_count )
	m_count = key+1;
#ifdef DEBUG
  std::cout << "MyMap::add : m_count" << m_count << std::endl;
#endif
}
