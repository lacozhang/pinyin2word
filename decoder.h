//-*- C++ -*-
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
		int GetSize(){
				return m_count;
		}
  

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
//convert from a string to phrase collection
		void processraw( std::string raw, ptable& out, int& size ){

				converter& p = m_opt.pingying_c();
				input str( raw, p );
				std::vector<unsigned int>& r = str.result();
				size = r.size();
				spliter sp;
				sp( r, m_trie, out );

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
				int cnt =0;
				
				for( int i=0; i < size; ++i ){
#ifdef DEBUG4
						std::cout << "expand to size " << i << "\n";
#endif
						try{
								hypothesis* cur = NULL;
								heap& hcur = *( m_beams.at(i) );
								cnt = 0;
								while( hcur.top( cur ) ){
										hcur.del(cur);
										cnt++;
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
#ifdef DEBUG4
						std::cout << "beam "<<i<<"produce "<<cnt << "hypothesis\n";
#endif

				}

				try{
						heap& result = *(m_beams.at(size));
						heapitem* iter = result.begin();
						while( iter != result.end() ){
								out.push_back( (iter->ptr)->FullPath() );
								++iter;
						}
				} catch( ... ){
						std::cerr << "decoder::decode at fault2\n";
						abort();
				}
		}
//phrase start at the same position
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
						converter& cw = m_opt.chinese_c();
						MyMap& p2w = m_opt.ping2word();
						for(int key = p.m_str[0]; key != -1;
							key = (p2w.m_element[key]).next){
								int val = (p2w.m_element[key]).value;
								newhypo( h, p, cw.idx2str( val ) );
						}
				} else {
						dnode db;
						m_trie.query( p.m_str, db );
						for( int i=0; i< db.m_d.size(); ++i ){
								newhypo( h, p, db.m_d[i] );
						}
				}
		}

		void beamcheck( int len ){
				while( m_beams.size() < len+1 ){
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
				bool sentcontext = false;
				if( son->GetDepth() <= 3 ){
						sentcontext = true;
				}
				son->setscore( m_scorer.Query( son->GetHistory() + " " + opt, sentcontext ) );
				try{
						heap& h = *(m_beams.at(son->len()));
						pushstack( son, h );
				} catch( ... ){
						std::cerr << "decoder::newhypo at abort\n";
						abort();
				}
		}

		void pushstack( hypothesis* dat, heap& h){
  
				double thresh = 1.5;
				bool comb = false, succ;

				succ = h.push( dat );
				if( !succ ){
						combine(h, comb);
						succ = h.push(dat);
				}

				while( !succ ){
						prune(h, thresh);
						succ = h.push(dat);
						thresh -= 0.05;
				}

		}


		void combine( heap& h, bool& comb ){
#ifdef DEBUG4
				std::cout << "Before Combine "<< h.GetSize() << "\n";
#endif
				if( comb )
						return;
				std::set<hypothesis*> del;
				heapitem* i = h.begin();
				while( i != h.end() ){

						heapitem* j = i+1;
						while( j != h.end() ){
								if( (i->ptr)->cover() == (j->ptr)->cover() 
									&& (i->ptr)->GetHistory() == (j->ptr)->GetHistory() ){
										heapitem* bad = (i->ptr)->getscore() > (j->ptr)->getscore() ? j : i;
										del.insert( bad->ptr );
								}
								++j;
						}

						++i;
				}

				comb = true;
#ifdef DEBUG4
				std::cout << "Total "<<del.size() << "combinded" << "\n";
#endif
				for( std::set<hypothesis*>::iterator iter = del.begin();
					 iter != del.end(); ++iter ){
#ifdef DEBUG3
						std::cout << "hypo combined : " << (*iter)->FullPath() << "\n";
#endif
						h.del( *iter );
						delete *iter;
				}
#ifdef DEBUG4
				std::cout << "After Combine: " << h.GetSize() << "\n";
#endif

		}

		void prune( heap& h, double thresh ){

#ifdef DEBUG4
				std::cout << "Before Prune : "<< h.GetSize() << "\n";
#endif
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
						iter++;
				}
#ifdef DEBUG4
				std::cout << "Total "<<del.size() << "Pruned\n";
#endif
				for( std::set<hypothesis*>::iterator iter = del.begin();
					 iter != del.end(); ++iter ){
#ifdef DEBUG3
						std::cout << "hypo pruned : " << (*iter)->FullPath() << "\n";
#endif
						h.del( *iter );
						delete *iter;
				}
#ifdef DEBUG4
				std::cout << "After Prune : " << h.GetSize() << "\n";
#endif
		}


private:
		scorer<Model>& m_scorer;
		trie& m_trie;
		std::vector<heap*> m_beams;
		options& m_opt;
};
#endif
