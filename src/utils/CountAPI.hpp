#ifndef COUNTAPI_HPP_
#define COUNTAPI_HPP_

#include "API.hpp"

namespace linuxservice {

class CountAPI : public API {
public:
	CountAPI();
	~CountAPI() = default;
	CountAPI(const CountAPI&) = default;
	CountAPI& operator=(const CountAPI&) = default;

    enum InputCommand {
        INCR,
        DECR,
        OUTPUT,
        INVALID
    };

    enum OutputCommand {
        //NO OUTPUT COMMANDS IN SPEC
    };

    std::string getApiType();
    int getCount();
    
    InputCommand handleInCommand(std::string& rawInput, std::string& output);
    //void handleOutCommand(OutputCommand output); //server never sends OUT command without first having an IN in spec

private:
    int m_count;

};

}

#endif /* COUNTAPI_HPP_ */