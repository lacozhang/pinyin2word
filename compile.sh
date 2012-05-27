#!/bin/sh

for i in util/{bit_packing,ersatz_progress,exception,file_piece,murmur_hash,file,mmap} lm/{bhiksha,binary_format,config,lm_exception,model,quantize,read_arpa,search_hashed,search_trie,trie,trie_sort,virtual_interface,vocab}; do
  g++ -I. -O3 -DNDEBUG $CXXFLAGS -c $i.cc -o $i.o
done
g++ -I. -I/cygdrive/f/zhangyu/include -L/cygdrive/f/zhangyu/lib -O3 -DNDEBUG $CXXFLAGS scorer.cc {lm,util}/*.o -lz -o scorer.o -lboost_regex
