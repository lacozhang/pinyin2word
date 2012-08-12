// -*- c++ -*-

#ifndef __SCORER_H__
#define __SCORER_H__
#include "lm/enumerate_vocab.hh"
#include "lm/model.hh"
#include <boost/regex.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <ctype.h>



class Selector{
public:
		lm::ngram::ModelType operator()( const char* name ){
				lm::ngram::ModelType model_type;
				if( lm::ngram::RecognizeBinary( name, model_type) ){
						return model_type;
				} else {
						return lm::ngram::HASH_PROBING;
				}
		}
};

template<class Model >
class scorer{
		//everting here
public:
		scorer( Model& model): m_model( model ){
		}

		~scorer(){
		}
		double Query( std::string str, bool sent_context ){

				double total = 0;
				total += LM_score(str, sent_context);
				return total;
		}

private:
		Model& m_model;
		void split( std::string raw, std::vector<std::string>& ret ){
				boost::regex sp("\\s+");
				boost::sregex_token_iterator iter( raw.begin(), raw.end(),
												   sp, -1 );
				boost::sregex_token_iterator end;
				while( iter != end ){
						ret.push_back( iter->str() );
						iter++;
				}
		}
		
		double LM_score(std::string str, bool sent_context){

				double total = 0;
				std::vector<std::string> tokens;
				typename Model::State state, out;
				lm::FullScoreReturn ret;
				int oov;

				split( str, tokens );
				state = sent_context ? m_model.BeginSentenceState() : m_model.NullContextState();
				for( int i=0; i<tokens.size(); ++i ){
						lm::WordIndex vocab = m_model.GetVocabulary().Index( tokens[i] );
						if( 0 == vocab ) ++oov;
						ret = m_model.FullScore( state, vocab, out );
						total = ret.prob;
						state = out;
				}
/*				if( sent_context ){
						ret = m_model.FullScore( state, m_model.GetVocabulary().EndSentence(), out);
						total += ret.prob;
				}
*/
				return total;

		}
  
};

template <class Model> 
Model* xQuery(const char *name) {
		lm::ngram::Config config;
		Model *model = new Model(name, config);
		return model;
}

//scorer* getscorer( lm::ngram::ModelType type, const char* name );

#endif // __SCORER_H__
