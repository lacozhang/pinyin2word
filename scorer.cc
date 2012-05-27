#include "scorer.h"





/*scorer* getscorer( lm::ngram::ModelType type, const char* name ){
  switch(type) {
  case lm::ngram::HASH_PROBING:
	return new scorer<lm::ngram::ProbingModel>( *(xQuery<lm::ngram::ProbingModel>( name )) );
  case lm::ngram::TRIE_SORTED:
	return new scorer<lm::ngram::TrieModel>( *(xQuery<lm::ngram::TrieModel>( name ) ) );
  case lm::ngram::QUANT_TRIE_SORTED:
	return new scorer<lm::ngram::QuantTrieModel>( *(xQuery<lm::ngram::QuantTrieModel>( name ) ) );
  case lm::ngram::ARRAY_TRIE_SORTED:
	return new scorer<lm::ngram::ArrayTrieModel>( *(xQuery<lm::ngram::ArrayTrieModel>( name ) ) );
  case lm::ngram::QUANT_ARRAY_TRIE_SORTED:
	return new scorer<lm::ngram::QuantArrayTrieModel>( *(xQuery<lm::ngram::QuantArrayTrieModel>( name ) ) );
  case lm::ngram::HASH_SORTED:
  default:
	std::cerr << "Unrecognized kenlm model type " << type << std::endl;
	abort();
  }
  
  }*/

