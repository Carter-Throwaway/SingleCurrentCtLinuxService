#ifndef EXECUTABLETESTUTIL_HPP_
#define EXECUTABLETESTUTIL_HPP_

#include <vector>

namespace linuxservice {

class ExecutableTestUtil {
public:
	ExecutableTestUtil() = default;
	~ExecutableTestUtil() = default;
	ExecutableTestUtil(const ExecutableTestUtil&);
	ExecutableTestUtil& operator=(const ExecutableTestUtil&);

    enum TestStatus {
        FAILED = 0,
        PASSED,
        EXCEPTION
    };

    //members
    std::vector<TestStatus> m_testResults;

    void runTests();
    void evaluateTests();
};

}

#endif /* EXECUTABLETESTUTIL_HPP_ */