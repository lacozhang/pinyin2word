#include "hypothesis.h"
#include<iostream>
#include<cstdlib>


hypothesis::hypothesis(){
  
  m_bexpand = false;
  m_stropt = "";
  for( int i=0; i<64; ++i ){
	m_cover[i] = false;
  }

  m_end = -1;

  m_parent = NULL;
}

hypothesis::~hypothesis(){
}

int hypothesis::len(){
  return m_end + 1;
}

void hypothesis::setparent( hypothesis* p ){
  if( m_parent ){
	std::cerr << "hypothesis::setparent reset parent\n";
  }
  m_parent = p;
  for( int i=0; i < p->len(); ++i ){
	m_cover[i] = true;
  }
}

void hypothesis::settrans( std::string opt ){
  m_stropt = opt;
}

void hypothesis::setphrases( phrase& p ){
  m_end = p.end;
  for( int i=p.start; i<=p.end; ++i ){
	m_cover[i] = true;
  }
}

std::string hypothesis::getstr(){
  std::string tmp="";
  if( m_parent != NULL )
	tmp = m_parent->getstr();
  tmp += " ";
  return tmp + m_stropt;
}

bool hypothesis::stest(){
  bool ret = true;
  for( int i=0; i<=m_end; ++i ){
	ret = m_cover[i];
  }
  return ret;
}

int hypothesis::nextphrase(){
  if( !stest() ){
	std::cerr << "hypothesis::nextphrase error \n";
	exit(1);
  }
  return m_end + 1;
}

void hypothesis::setexpand(){
  m_bexpand = true;
}


bool hypothesis::expand(){
  return m_bexpand;
}

std::bitset<64> hypothesis::cover() const {
  return m_cover;
}
