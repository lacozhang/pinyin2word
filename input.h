#ifndef __INPUT_H__
#define __INPUT_H__

#include<string>
#include<vector>
#include "converter.h"

class input {
public:
  input( converter& );
  input( std::string raw, converter& );
  ~input();
  std::vector<unsigned int>& result();
  void setsrc( std::string raw );
  
private:
  void split( std::string raw );
  void convert();
  std::vector<std::string>  m_tokens;
  std::vector<unsigned int> m_intoks;
  converter& m_conv;

};


#endif // __INPUT_H__
