#ifndef __TRIE_H__
#define __TRIE_H__
#include<vector>
#include<list>
#include<string>
#include<iostream>

struct node;

struct subnode{
  subnode(){
	value = -1;
	ptr = NULL;
  }
  ~subnode(){
	ptr = NULL;
  }
  subnode& operator = ( subnode& );
  unsigned int value;
  struct node* ptr;
};

struct dnode{
  dnode(){
  }
  void add( const std::string& str ){
	m_d.push_back( str );
#ifdef DEBUG2
	std::cout << "dnode::add data size : " << m_d.size() << "\n";
#endif
  }
  std::vector<std::string> m_d;
};

struct node{
public:
  node();
  ~node();
  node* query( const unsigned int key );
  node* add_child( unsigned int key );
  void add_data( const std::string& data );
  void copy_data( dnode& );
  bool data();
private:
  bool m_bdata;
  struct subnode* m_child;
  struct dnode* m_data;
  int m_cnt;
  int m_size;
  void init_child();
  void enlarge();
};

class trie {
public:
  trie();
  ~trie();
  void add(const std::vector<unsigned int>& key,const std::string& value );
  bool query(const std::vector<unsigned int>& key, dnode& data );
private:
  node m_root;
};

#endif // __TRIE_H__
