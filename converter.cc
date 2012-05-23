#include "converter.h"
#include<utility>
#include<iostream>
#include<cstdlib>



converter::converter(){
  m_curidx = 0;
  m_idx2str.clear();
  m_str2idx.clear();
}

converter::~converter(){
#ifdef DEBUG
  std::cout << "max cout " << m_curidx << std::endl;
#endif  
}

void converter::add_new( std::string str ){
  //add to string to idx
  std::pair<std::map<std::string, unsigned int>::iterator, bool> ret;
  ret = m_str2idx.insert( std::pair<std::string, unsigned int>(str, m_curidx) );
  if( ! ret.second ){
	std::cerr << "converter insert Error"
			  << std::endl;
	exit(1);
  }

  std::pair<std::map<unsigned int, std::string>::iterator, bool> ret2;
  ret2 = m_idx2str.insert( std::pair<unsigned int, std::string>( m_curidx, str ) );
  if( ! ret2.second ){
	std::cerr << "converter 2 insert errror"
			  << std::endl;
	exit(1);
  }

  m_curidx++;

#ifdef DEBUG
  std::cout << "converter::add_new\tnew string : " << str << std::endl;
  std::cout << "converter::add_new\t" << "string count\t" << m_curidx << std::endl;
#endif

}


unsigned int converter::str2idx( std::string pingying ){
  if( !m_str2idx.count( pingying ) ){
	add_new( pingying );
  }
  return m_str2idx[pingying];
}

std::string converter::idx2str( unsigned int idx){
  return m_idx2str[idx];
}

unsigned int converter::count() const{
  return m_curidx;
}

converter::iterator converter::begin() const{
  return m_idx2str.begin();
}

converter::iterator converter::end() const{
  return m_idx2str.end();
}
