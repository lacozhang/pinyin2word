
test : test.cc options.h options.cc input.h input.cc converter.h converter.cc trie.h trie.cc split.h split.cc
	g++ -g -I/cygdrive/f/zhangyu/include -L/cygdrive/f/zhangyu/lib -o test test.cc options.h options.cc input.h input.cc converter.h converter.cc trie.h trie.cc help.h help.cc split.h split.cc -lboost_regex
