#include "trie.h"
#include<iostream>
#include<cstdlib>

subnode& subnode::operator = ( subnode& src ){
  value = src.value;
  ptr = src.ptr;
}

node::node(){
  m_bdata = false;
  m_child = NULL;
  m_data = NULL;
  m_cnt = 0;
  m_size = 0;
  init_child();
}

node::~node(){

  if( m_bdata && (m_data != NULL) ){
	delete m_data;
  }
  if( m_child )
	delete [] m_child;

}

node* node::query(  unsigned int key ){

  int high = m_cnt -1, low=0, mid=0;

  while( low <= high ){

	mid = ( high + low )/2;
	if( (m_child[mid]).value == key ){
	  return (m_child[mid]).ptr;
	}

	if ( key > (m_child[mid]).value ) {
	  low = mid + 1;
	} else {
	  high = mid -1;
	}

  }
  return NULL;
}

void node::init_child(){
  m_child = new subnode[4];
  if( ! m_child ){
	std::cerr << "node::init_child : new failed"
			  << std::endl;
	exit(1);
  }
  m_size = 4;
}

void node::enlarge(){

  subnode* tmp = new subnode[m_size*2];
  if( !tmp ){
	std::cerr << "node::enlarge new failed"
			  << std::endl;
  }
  for( int i=0; i < m_cnt; ++i ){
	tmp[i] = m_child[i];
  }

  delete [] m_child;
  m_child = tmp;
  m_size = m_size*2;

}


node* node::add_child( unsigned int key ){
  node* target = query( key );
  if( target )
	return target;
  
  if( m_cnt+1 > m_size )
	enlarge();

  int j=m_cnt-1;
  while( (j >= 0) && ((m_child[j]).value > key) )
	--j;
  ++j;
  int i=m_cnt-1;
  while( i >= j ){
	m_child[i+1] = m_child[i];
	--i;
  }
  (m_child[j]).value = key;
  (m_child[j]).ptr = new node;
  if( !(m_child[j]).ptr ){
	std::cerr << "node::add_child new failed"
			  << std::endl;
  }
  m_cnt = m_cnt + 1;
  return (m_child[j]).ptr;
}

void node::add_data(const std::string& str ){
  if( ! m_bdata ){
	m_data = new dnode;
#ifdef DEBUG2
	std::cout << "node::add_data Create New dnode\n";
#endif
	if( ! m_data ){
	  std::cerr << "node::add_data new failed"
				<<"\n";
	}
	m_bdata = true;
  }
#ifdef DEBUG2
  std::cout << "node::add_data\t" << str << "\n";
#endif
  m_data->add( str );
}

bool node::data(){
  return m_bdata;
}

void node::copy_data( dnode& d ){
  d.m_d = m_data->m_d;
}

void trie::add(const std::vector<unsigned int>& key,const std::string& value ){
  node* ptr = &m_root;
  int i;
#ifdef DEBUG
  std::cout << "trie::add " << value << "\n";
#endif
  for( i=0; i<key.size() && ptr; ++i ){
	ptr = ptr->add_child( key.at(i) );
  }
  if( i < key.size() || !ptr ){
	std::cerr << "trie::add add failed\n";
	exit(1);
  }
  ptr->add_data( value );
}

bool trie::query( const std::vector<unsigned int>& key, dnode& data){
  node* ptr = &m_root;
  int i;
  for(i=0; i<key.size() && ptr; ++i){
	ptr = ptr->query( key[i] );
  }

  if( i < key.size() || !ptr ){
#ifdef DEBUG2
	std::cout << "trie::query No such words\n";
#endif
	return false;
  }
  if( ! ptr->data() ){
#ifdef DEBUG2
	std::cout << "trie::query No Data\n";
#endif
	return false;
  }
  ptr->copy_data( data );
  return true;
}

trie::trie(){
}

trie::~trie(){
}


bool trie::query( const std::vector<unsigned int>& key ){
  node* ptr = &m_root;
  int i;
  for(i=0; i<key.size() && ptr; ++i){
	ptr = ptr->query( key[i] );
  }

  if( i < key.size() || !ptr ){
	return false;
  }
  if( ! ptr->data() ){
	return false;
  }
  return true;
}
