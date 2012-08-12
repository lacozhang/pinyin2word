#include "split.h"
#include<utility>
#include<iostream>
#include<cstdlib>

ptable::ptable(){
}

ptable::~ptable(){
}

std::vector<phrase>& ptable::start_at( int pos ){
		if( ! m_table.count(pos) ){
				std::cerr << "ptable::start_at\t" << "error\n";
				exit(1);
		}
		return m_table[pos];
}

void ptable::insert( phrase& p ){
		std::map<int, std::vector<phrase> >::iterator iter = m_table.find( p.start );
		if( iter != m_table.end() ){
				(iter->second).push_back( p );
		} else {
				std::pair< std::map<int, std::vector<phrase> >::iterator, bool > ret;
				ret = m_table.insert( std::pair<int, std::vector<phrase> >( p.start, std::vector<phrase>() ) );
				if( ret.second ){
						((ret.first)->second).push_back( p );
				} else {
						std::cerr << "ptable::insert  failed\n";
						exit(1);
				}
		}
  
}

bool spliter::operator()( const std::vector<unsigned int>& src, trie& tree, ptable& out ){
		int start, end;
		for( end=1; end<src.size(); ++end ){
				for( start=0; start < end; ++start){
						int i=start;
						phrase tmp;
						tmp.start = start;
						tmp.end = end;
						while( i <= end ){
								tmp.m_str.push_back( src[i] );
								++i;
						}
						if( tree.query( tmp.m_str ) ){
								out.insert( tmp );
						}
				}
		}

		for( int i=0; i<src.size(); ++i ){
				phrase tmp;
				tmp.start = tmp.end = i;
				tmp.m_str.push_back(src[i]);
				out.insert(tmp);
		 }
		 return true;
 }
