#include "CountAPI.hpp"
#include <iostream>

namespace linuxservice {

/**
 * CountAPI encapsulates 'counting' server functionality laid out by a 3rd party spec.
 */
CountAPI::CountAPI() {
    m_count = 0;
}

/**
 * Required override of pure virtual.
 * 
 * @return string that uniquely identifies the API type (allows for static_casting).
 */
std::string CountAPI::getApiType(){
    return "CountAPI";
}

/**
 * Getter for count member variable.
 * 
 * @return int value of m_count.
 */
int CountAPI::getCount(){
    return m_count;
}

/**
 * Parses the raw input from a server, completes any internal functionality/computation,
 * and returns an (optional) output and required InputCommand type as defined in header.
 * 
 * @param[in] rawInput address for the string of raw input from the server.
 * @param[out] output (optional) address for the string of output that may be used elsewhere.
 * @return CountAPI::InputCommand enum type of input command that was just parsed.
 */
CountAPI::InputCommand CountAPI::handleInCommand(std::string& rawInput, std::string& output){
    std::string crlf = "\r\n";

    //no need to look for the subsequent value if it's just 'OUTPUT'
    if(rawInput == "OUTPUT\r\n"){
        std::string ctStr = std::to_string(m_count);
        output = "Current Count: " + ctStr + crlf;
        return CountAPI::InputCommand::OUTPUT;
    }

    //now split at the space:
    std::string delimiter = " ";
    size_t pos = 0;
    std::string command;
    while ((pos = rawInput.find(delimiter)) != std::string::npos) {
        command = rawInput.substr(0, pos);
        rawInput.erase(0, pos + delimiter.length());
    }

    //adjust the count and send back response
    if(command == "INCR") {
        int val = 0;
        try { val = stoi(rawInput); } 
		catch (const std::exception& e) {
            output = "INCR command takes an integer. " + crlf;
			std::cerr << "INCR command takes an integer. " << e.what() << std::endl;
            return CountAPI::InputCommand::INVALID;
		}
        m_count+=val;
        std::string ctStr = std::to_string(m_count);
        output = "Increased by " + rawInput + " (Current Count: " + ctStr + ")" + crlf;
        return CountAPI::InputCommand::INCR;
    } else if(command == "DECR"){
        int val = 0;
        try { val = stoi(rawInput); } 
		catch (const std::exception& e) {
            output = "DECR command takes an integer. " + crlf;
			std::cerr << "DECR command takes an integer. " << e.what() << std::endl;
            return CountAPI::InputCommand::INVALID;
		}
        m_count-=val;
        std::string ctStr = std::to_string(m_count);
        output = "Decreased by " + rawInput + " (Current Count: " + ctStr + ")" + crlf;
        return CountAPI::InputCommand::DECR;
    } else {
        return CountAPI::InputCommand::INVALID;
    }
}

}