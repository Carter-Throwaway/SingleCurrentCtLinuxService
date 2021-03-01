#ifndef COUNTAPITEST_HPP_
#define COUNTAPITEST_HPP_

#include "utils/ExecutableTestUtil.hpp"

namespace linuxservice {

class CountAPITest : public ExecutableTestUtil {
public:
	CountAPITest() = default;
	~CountAPITest() = default;
	CountAPITest(const CountAPITest&) = delete;
	CountAPITest& operator=(const CountAPITest&) = delete;

	void runTests();

private:
	static ExecutableTestUtil::TestStatus Test1_HandleCommand_INCR_ExpectedInput();
    static ExecutableTestUtil::TestStatus Test2_HandleCommand_INCR_UnexpectedInput();
	static ExecutableTestUtil::TestStatus Test3_HandleCommand_DECR_ExpectedInput();
    static ExecutableTestUtil::TestStatus Test4_HandleCommand_DECR_UnexpectedInput();
    static ExecutableTestUtil::TestStatus Test5_HandleCommand_OUTPUT();
    static ExecutableTestUtil::TestStatus Test6_HandleCommand_INVALID();
};

}

#endif /* COUNTAPITEST_HPP_ */