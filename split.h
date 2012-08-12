// -*- C++ -*-
#ifndef __SPLIT_H__
#define __SPLIT_H__

#include<vector>
#include<map>
#include "trie.h"
#include "options.h"
#include "converter.h"

struct phrase{
		phrase(){
				start = end = -1;
		}
		int start, end;
		std::vector<unsigned int> m_str;
};

class ptable{
public:
		ptable();
		~ptable();
		std::vector<phrase>& start_at( int pos );
		void insert( phrase& p );

private:
		std::map<int, std::vector<phrase> > m_table;
};

class spliter{
public:
		bool operator()( const std::vector<unsigned int>& src, trie& tree, ptable& out );
};

#endif
