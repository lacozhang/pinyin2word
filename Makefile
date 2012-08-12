
test : p2w.cc options.h options.cc input.h input.cc converter.h converter.cc trie.h trie.cc split.h split.cc scorer.cc decoder.h decoder.cc hypothesis.h hypothesis.cc
	g++ -g -I/cygdrive/f/zhangyu/include -I. -L/cygdrive/f/zhangyu/lib -L. -DDEBUG4 -o p2w p2w.cc options.h options.cc input.h input.cc converter.h converter.cc trie.h trie.cc help.h help.cc split.h split.cc scorer.cc scorer.h decoder.h decoder.cc hypothesis.h hypothesis.cc -lboost_regex -llm -lutil
