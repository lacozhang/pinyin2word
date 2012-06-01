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
  bool expand();
  std::bitset<64> cover() const;
  void setscore( float s ) {
	m_score = s;
  }

  float getscore( ) const {
	return m_score;
  }

 private:
  bool stest();
  bool m_bexpand;
  std::string m_stropt;
  std::bitset<64> m_cover;
  int m_end;
  hypothesis* m_parent;
  float m_score;

};


#endif //__HYPOTHESIS_H__
