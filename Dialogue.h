//#pragma once
#include <iostream>
class Dialogue : public std::ostream
{
    public :
        Dialogue(bool value) : std::ostream(0) 
		{
			if(value)
				this->rdbuf(std::cout.rdbuf());
		}

		~Dialogue(void){}
};

