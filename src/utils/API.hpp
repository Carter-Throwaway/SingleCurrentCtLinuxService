#ifndef API_HPP_
#define API_HPP_

#include <string>

namespace linuxservice {

class API {
public:
	API() = default;
	~API() = default;
	API(const API&) = default;
	API& operator=(const API&) = default;

    virtual std::string getApiType() = 0;
};

}

#endif /* API_HPP_ */