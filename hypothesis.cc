#include "hypothesis.h"
#include<iostream>
#include<cstdlib>


hypothesis::hypothesis(){
  
		m_bexpand = false;
		m_stropt.clear();
		m_cover.reset();
		m_end = -1;
		m_parent.clear();
		m_score = 0;
		m_history = NULL;
}

hypothesis::~hypothesis(){

}

int hypothesis::len(){
		return m_end + 1;
}

void hypothesis::setparent( hypothesis* p ){
		
		list<hypothesis*>::iterator iter = m_parent.begin();
		while( iter != m_parent.end() ){
				if( *iter == p )
						break;
				iter++;
		}
		if( iter == m_parent.end() ){
				m_parent.push_back( p );
		}

		if( !m_history ){
				m_history = p;
		}
}

void hypothesis::settrans( std::string& opt ){
		m_stropt = opt;
}

void hypothesis::setphrases( phrase& p ){
		m_end = p.end;
		for( int i=p.start; i<=p.end; ++i ){
				m_cover.set(i);
		}
}

void GetOpt(std::string& ret){
		ret = m_stropt;
}

std::string hypothesis::GetHistory(){
		
		std::string h="";
		if( m_history ){
				std::string tmp;
				m_history->GetOpt(tmp);
				h += tmp;
				if( m_history->m_history ){
						m_history->m_history->GetOpt(tmp);
						h += tmp;
				}
		}
		return h;
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
				abort();
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
