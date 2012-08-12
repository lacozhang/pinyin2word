#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include "input.h"
#include "options.h"
#include "trie.h"
#include "help.h"
#include "split.h"
#include "scorer.h"
#include "decoder.h"
#include "hypothesis.h"

using namespace std;

options g_Opt;
trie    g_Tree;


void LoadPinYin(const char* py, options& o){

		ifstream src;
		string tmp;
		src.open( py );
		if( !src.is_open() ){
				cerr<< "error" << endl;
				exit(1);
		}

		getline( src, tmp);
		while( src.good() ){
				o.add_new( tmp );
				getline( src, tmp);
		}
		src.close();

}

void LoadWords(const char* phrase, trie& t, converter& p){

		ifstream src;
		string tmp;
		src.open( phrase );

		if( !src.is_open() ){
				cout<< "error" << endl;
				exit(1);
		}

		getline( src, tmp);
		helper h;
		string head, body;
		while( src.good() ){

				h( tmp, head, body);
				input p_input( body, p);
				std::vector<unsigned int>& p_v = p_input.result();
				t.add( p_v, head );
				getline( src, tmp);
		}

		src.close();

}

template<class Model>
void work( const char* name, options& o, trie& t ){

		scorer<Model>* sco = new scorer<Model>(*(xQuery<Model>(name) ) );
		decoder<Model> *d = new decoder<Model>( *sco, t, o);
		std::vector<std::string> out;
		while( 1 ){

				std::string tmp;
				std::cout << "input pin yin\n";
				getline( cin, tmp );
				d->decode( tmp, out );
#ifdef DEBUG3
				std::cerr << "decoding voer. results ...\n";
#endif
				for( int i=0; i< out.size(); ++i ){
						cout << out[i] << "\n";
				}
				getchar();
		}

}

void out(dnode& d){
		for(int i=0; i<d.m_d.size(); ++i){
				cout << d.m_d[i] << "\n";
		}
}


void test(trie& t, converter& c){
		string tmp;

		input i(c);
		dnode ret;
		while(1){
				getline(cin, tmp);
				i.setsrc(tmp);
				vector<unsigned int>& r = i.result();
				t.query(r, ret);
				out(ret);
		}
}

int main( int argc,const char* argv[]){

		cout << argv[1] << "\n" << argv[2] << "\n" << argv[3] << "\n";
		LoadPinYin( argv[1], g_Opt);
		LoadWords( argv[2], g_Tree, g_Opt.pingying_c() );
//		test(g_Tree, g_Opt.pingying_c());
		work<lm::ngram::ProbingModel>( argv[3], g_Opt, g_Tree );
		return 0;
}
