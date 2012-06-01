#ifndef __CONVERTER_H__

#define __CONVERTER_H__
#include<string>
#include<map>

class converter {
 public:
  converter();
  ~converter();
  unsigned int str2idx( std::string pinyin );
  unsigned int str2idx_c( std::string pinyin );
  std::string idx2str( unsigned int );
  unsigned int count() const;
  typedef std::map<unsigned int, std::string>::const_iterator iterator;
  iterator begin() const;
  iterator end() const;
  
 private:
  void add_new( std::string );
  std::map<unsigned int, std::string>  m_idx2str;
  std::map<std::string, unsigned int>  m_str2idx;
  unsigned int m_curidx;
};

#endif // __CONVERTER_H__
