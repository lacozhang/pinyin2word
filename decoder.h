//-*C++*-
#ifndef __DECODER_H__
#define __DECODER_H__
#include "hypothesis.h"
#include "scorer.h"
#include "trie.h"
#include "options.h"
#include "split.h"
#include "input.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <set>
#include <vector>



class heapitem{
public:
  heapitem(){
	score = 0.0;
	ptr = NULL;
  }
  heapitem( heapitem& i){
	score = i.score;
	ptr = i.ptr;
  }
  float score;
  hypothesis* ptr;
  heapitem& operator = ( heapitem& i ){
	score = i.score;
	ptr = i.ptr;
  }
};


class heap{
public:
  heap( int );
  heap();
  ~heap();

  bool push( hypothesis* );
  void del( hypothesis* );
  heapitem* begin() const;
  heapitem* end() const;
  bool top( hypothesis*& ) const;
  void push_bak( hypothesis*);
  void setsize( int );
  

private:
  void init_heap();
  void reformulate();
  void heapify( int pos );
  heapitem* m_array ;
  heapitem* m_backup;
  int m_size ;
  int m_count ;
  int m_bcnt;
};

template<class Model>
class decoder{
public:
  decoder( scorer<Model>& lan, trie& t, options& opt )
	: m_scorer(lan), m_trie(t), m_opt(opt) {
	m_beams.clear();
  }

  ~decoder(){

	cleanbeam();

  }

  void cleanbeam(){

	for( std::vector<heap*>::iterator iter = m_beams.begin();
		 iter != m_beams.end(); ++iter ){
	  delete *iter;
	}
	m_beams.clear();
	
  }

  void processraw( std::string raw, ptable& out, int& size ){
	converter& p = m_opt.pingying_c();
	input str( raw, p );
	std::vector<unsigned int>& r = str.result();
	size = r.size();
	spliter sp;
	sp( r, m_trie, m_opt.ping2word(), out );
  }

  void init_decode( hypothesis& h ){

	cleanbeam();

	if( m_beams.size() < 1 ){
	  m_beams.push_back( new heap(1) );
	}

	(m_beams[0])->push( &h );

  }

  void decode( std::string str, std::vector<std::string>& out ){
	ptable phrases;
	int size;

	processraw( str, phrases, size );
	hypothesis root;
	init_decode( root );


	for( int i=0; i < size; ++i ){
	  try{
		hypothesis* cur = NULL;
		heap& hcur = *( m_beams.at(i) );
		while( hcur.top( cur ) ){
		  hcur.del(cur);
		  hcur.push_bak( cur );
		  if( ! cur->expand() ){
			cur->setexpand();
			std::vector<phrase>& possi = phrases.start_at( cur->nextphrase() );
			phrasexpansion( cur, possi );
		  }
		}
	  } catch ( ... ) {
		std::cerr << "decoder::decode at fault1\n";
		abort();
	  }

	}

	try{
	  heap& result = *(m_beams.at(size));
	  heapitem* iter = result.begin();
	  while( iter != result.end() ){
		out.push_back( (iter->ptr)->getstr() );
		++iter;
	  }
	} catch( ... ){
	  std::cerr << "decoder::decode at fault2\n";
	  abort();
	}
  }

  void phrasexpansion( hypothesis* h, std::vector<phrase>& pl ){

	for( int i=0; i<pl.size(); ++i ){
	  phrase& p = pl[i];
	  optexpansion( h, p );
	}

  }

  void optexpansion( hypothesis* h, phrase& p ){
	int plen = p.end - p.start + 1;
	int newlen = h->len() + plen;
	beamcheck( newlen );
	// create new hypothesis
	if( 1 == plen ){
	  converter& c = m_opt.chinese_c();
	  newhypo( h, p, c.idx2str( p.m_str[0] ) );
	} else {
	  dnode db;
	  m_trie.query( p.m_str, db );
	  for( int i=0; i< db.m_d.size(); ++i ){
		newhypo( h, p, db.m_d[i] );
	  }
	}
  }

  void beamcheck( int len ){
	while( m_beams.size() <= len+1 ){
	  m_beams.push_back( new heap(500) );
#ifdef DEBUG3
	  std::cerr << "expanded to size " << len << "\n";
#endif

	}

  }

  void newhypo( hypothesis* parent, phrase& p, std::string opt ){

	hypothesis* son = new hypothesis;
	son->setparent( parent );
	son->setphrases( p );
	son->settrans( opt );
	son->setscore( m_scorer.Query( son->getstr(), false ) );
	try{
	  heap& h = *(m_beams.at(son->len()));
	  pushstack( son, h );
	} catch( ... ){
	  std::cerr << "decoder::newhypo at abort\n";
	  abort();
	}
  }

  void pushstack( hypothesis* dat, heap& h){
  
	double thresh = 0.3;
	bool comb = false;
	while( ! h.push( dat ) ){

	  combine( dat, h, comb );

	  if( !h.push( dat ) ){

		prune( h, thresh );
		thresh += 0.05;

	  } else {
		break;
	  }

	}

  }


  void combine( hypothesis* dat, heap& h, bool& comb ){

	if( comb )
	  return;
	std::set<hypothesis*> del;
	heapitem* i = h.begin();
	while( i != h.end() ){

	  heapitem* j = i+1;
	  while( j != h.end() ){
		if( (i->ptr)->cover() == (j->ptr)->cover() ){
		  heapitem* bad = (i->ptr)->getscore() > (j->ptr)->getscore() ? j : i;
		  del.insert( bad->ptr );
		}
		++j;
	  }

	  ++i;
	}

	comb = true;
	for( std::set<hypothesis*>::iterator iter = del.begin();
		 iter != del.end(); ++iter ){
#ifdef DEBUG3
	  std::cout << "hypo combined : " << (*iter)->getstr() << "\n";
#endif
	  h.del( *iter );
	  delete *iter;
	}

  }

  void prune( heap& h, double thresh ){

	hypothesis* b;
	if( !h.top( b ) ){
	  std::cerr << "decoder::prune error \n";
	  abort();
	}

	double k = (b->getscore() )*thresh;
	std::set<hypothesis*>  del;
	heapitem* iter = h.begin();
	while( iter != h.end() ){
	  if( (iter->ptr)->getscore() < k ){
		del.insert( iter->ptr );
	  }
	}

	for( std::set<hypothesis*>::iterator iter = del.begin();
		 iter != del.end(); ++iter ){
#ifdef DEBUG3
	  std::cout << "hypo pruned : " << (*iter)->getstr() << "\n";
#endif
	  h.del( *iter );
	  delete *iter;
	}

  }


private:
  scorer<Model>& m_scorer;
  trie& m_trie;
  std::vector<heap*> m_beams;
  options& m_opt;
};
#endif
