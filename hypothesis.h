#ifndef __HYPOTHESIS_H__

#define __HYPOTHESIS_H__
#include<string>
#include<bitset>
#include "split.h"

class hypothesis{
 public:
  hypothesis();
  ~hypothesis();
  int len();
  void setparent( hypothesis* );
  void settrans( std::string );
  void setphrases( phrase& );
  std::string getstr();
  int nextphrase();
  void setexpand();
  void resetexpand();
  bool expand();

 private:
  bool stest();
  bool m_bexpand;
  std::string m_stropt;
  bitset<64> m_cover;
  int m_end;
  hypothesis* m_parent;

};


#endif __HYPOTHESIS_H__
