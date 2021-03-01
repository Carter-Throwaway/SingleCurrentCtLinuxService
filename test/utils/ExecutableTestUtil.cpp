#include "ExecutableTestUtil.hpp"
#include <iostream>

namespace linuxservice {

void ExecutableTestUtil::evaluateTests(){
    for(std::vector<TestStatus>::iterator it = m_testResults.begin(); it != m_testResults.end(); it++ ) {
        TestStatus current = *it;
        if((current != FAILED) && (current != EXCEPTION)){
            std::cout << "Test passed" << std::endl;
        } else if (current != PASSED) {
            std::cout << "Exception" << std::endl;
        } else {
            std::cout << "Test failed" << std::endl;
        }
    }
}

}