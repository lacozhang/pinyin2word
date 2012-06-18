// -*- C++ -*-
#ifndef __HELP_H__
#define __HELP_H__
#include "converter.h"

#include<string>

class helper{
 public:
  bool operator()( std::string& raw, std::string& first, std::string& second);
};



#endif //__HELP_H__
