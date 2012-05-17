#ifndef __OPTIONS_H__
#define __OPTIONS_H__

class options{
 public:
  options( std::string raw );
  ~options();
  typedef std::vector<std::string>::iterator iterator;
  options::iterator begin();
  options::iterator end();
  std::string head() const;

 private:
  void split( std::string raw );
  std::string m_head;
  std::vector<std::string> m_body;
};

#endif // __OPTIONS_H__
