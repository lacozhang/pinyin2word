#ifndef __OPTIONS_H__
#define __OPTIONS_H__
#include<string>
#include<vector>

class options{
 public:
  options( std::string raw );
  ~options();
  typedef std::vector<std::string>::const_iterator iterator;
  options::iterator begin() const;
  options::iterator end() const;
  std::string head() const;

 private:
  void split( std::string raw );
  std::string m_head;
  std::vector<std::string> m_body;
};

#endif // __OPTIONS_H__
