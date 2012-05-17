#ifndef __INPUT_H__
#define __INPUT_H__

#include<string>
#include<vector>


class input {
    public:
        input();
        input( std::string raw );
        ~input();
		typedef std::vector<std::string>::iterator iterator;
		input::iterator begin();
		input::iterator end();
		void setsrc( std::string raw );

    private:
		void split( std::string raw );
        std::vector<std::string>  m_tokens;

};


#endif // __INPUT_H__
