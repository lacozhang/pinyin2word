#ifndef __OPTIONS_H__
#define __OPTIONS_H__
#include<string>
#include<vector>
#include "converter.h"

//用于存储汉字和拼音的对应关系
struct MapIdx{
		MapIdx(){
				value = -1;
				next  = -1;
		}
		long value;
		long next;
};
struct MyMap{
		MyMap(){
				m_free =299999;
				m_count =0;
		};
		void add( unsigned int key, unsigned int value);
		MapIdx m_element[300000];
		unsigned int m_free;
		unsigned int m_count;
};

class options{
public:
		options();
		~options();
		converter& pingying_c();
		converter& chinese_c();
		MyMap& word2ping();
		MyMap& ping2word();
		void add_new( std::string str );

private:
		converter m_pingying;
		converter m_word;
		MyMap m_w2p;
		MyMap m_p2w;
		void split( std::string raw );
		std::string m_head;
		std::vector<std::string> m_body;
};

#endif // __OPTIONS_H__
