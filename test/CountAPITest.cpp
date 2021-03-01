#include "CountAPITest.hpp"
#include "../src/utils/CountAPI.hpp"
#include <iostream>
#include <string>

int main() {
    linuxservice::CountAPITest testSet;
    testSet.runTests();
    return 0;
}

namespace linuxservice {

void CountAPITest::runTests(){
    //Add tests here:
    m_testResults.push_back(CountAPITest::Test1_HandleCommand_INCR_ExpectedInput());
    m_testResults.push_back(CountAPITest::Test2_HandleCommand_INCR_UnexpectedInput());
    m_testResults.push_back(CountAPITest::Test3_HandleCommand_DECR_ExpectedInput());
    m_testResults.push_back(CountAPITest::Test4_HandleCommand_DECR_UnexpectedInput());
    m_testResults.push_back(CountAPITest::Test5_HandleCommand_OUTPUT());
    m_testResults.push_back(CountAPITest::Test6_HandleCommand_INVALID());
    //DO LAST:
    evaluateTests();
}

ExecutableTestUtil::TestStatus CountAPITest::Test1_HandleCommand_INCR_ExpectedInput(){
    std::cout << "Starting Test1_HandleCommand_INCR_ExpectedInput..." << std::endl;
    
    CountAPI api;
    std::string output = "";
    std::string input = "INCR 45"; //Input Command Under Test

    if(api.handleInCommand(input, output) != CountAPI::InputCommand::INCR){
        std::cerr << "Test1: FAIL - Did not determine correct InputCommand." << std::endl;
        return ExecutableTestUtil::TestStatus::FAILED;
    }

    if(api.getCount() != 45){
        std::cerr << "Test1: FAIL - Did not increment count correctly." << std::endl;
        return ExecutableTestUtil::TestStatus::FAILED;
    }

    std::cout << "Test1: PASS" << std::endl;
    return ExecutableTestUtil::TestStatus::PASSED;
}

ExecutableTestUtil::TestStatus CountAPITest::Test2_HandleCommand_INCR_UnexpectedInput(){
    std::cout << "Starting Test2_HandleCommand_INCR_UnexpectedInput..." << std::endl;
    
    CountAPI api;
    std::string output = "";
    std::string input = "INCR dfhjfhdk"; //Input Command Under Test

    if(api.handleInCommand(input, output) != CountAPI::InputCommand::INVALID){
        std::cerr << "Test2: FAIL - Did not determine correct InputCommand." << std::endl;
        return ExecutableTestUtil::TestStatus::FAILED;
    }

    std::cout << "Test2: PASS" << std::endl;
    return ExecutableTestUtil::TestStatus::PASSED;
}

ExecutableTestUtil::TestStatus CountAPITest::Test3_HandleCommand_DECR_ExpectedInput(){
    std::cout << "Starting Test3_HandleCommand_DECR_ExpectedInput..." << std::endl;
    
    CountAPI api;
    std::string output = "";
    std::string input = "DECR 45"; //Input Command Under Test

    if(api.handleInCommand(input, output) != CountAPI::InputCommand::DECR){
        std::cerr << "Test3: FAIL - Did not determine correct InputCommand." << std::endl;
        return ExecutableTestUtil::TestStatus::FAILED;
    }

    if(api.getCount() != -45){
        std::cerr << "Test3: FAIL - Did not decrement count correctly." << std::endl;
        return ExecutableTestUtil::TestStatus::FAILED;
    }

    std::cout << "Test3: PASS" << std::endl;
    return ExecutableTestUtil::TestStatus::PASSED;
}

ExecutableTestUtil::TestStatus CountAPITest::Test4_HandleCommand_DECR_UnexpectedInput(){
    std::cout << "Starting Test4_HandleCommand_DECR_UnexpectedInput..." << std::endl;
    
    CountAPI api;
    std::string output = "";
    std::string input = "DECR 77787878778978798978989789789799898989898"; //Input Command Under Test

    if(api.handleInCommand(input, output) != CountAPI::InputCommand::INVALID){
        std::cerr << "Test4: FAIL - Did not determine correct InputCommand." << std::endl;
        return ExecutableTestUtil::TestStatus::FAILED;
    }

    std::cout << "Test4: PASS" << std::endl;
    return ExecutableTestUtil::TestStatus::PASSED;
}

ExecutableTestUtil::TestStatus CountAPITest::Test5_HandleCommand_OUTPUT(){
    std::cout << "Starting Test5_HandleCommand_OUTPUT..." << std::endl;
    
    CountAPI api;
    std::string output = "";
    std::string input = "OUTPUT\r\n"; //Input Command Under Test

    if(api.handleInCommand(input, output) != CountAPI::InputCommand::OUTPUT){
        std::cerr << "Test5: FAIL - Did not determine correct InputCommand." << std::endl;
        return ExecutableTestUtil::TestStatus::FAILED;
    }

    std::cout << "Test5: PASS" << std::endl;
    return ExecutableTestUtil::TestStatus::PASSED;
}

ExecutableTestUtil::TestStatus CountAPITest::Test6_HandleCommand_INVALID(){
    std::cout << "Starting Test6_HandleCommand_INVALID..." << std::endl;
    
    CountAPI api;
    std::string output = "";
    std::string input = " "; //Input Command Under Test

    if(api.handleInCommand(input, output) != CountAPI::InputCommand::INVALID){
        std::cerr << "Test6: FAIL - Did not determine correct InputCommand." << std::endl;
        return ExecutableTestUtil::TestStatus::FAILED;
    }

    std::cout << "Test6: PASS" << std::endl;
    return ExecutableTestUtil::TestStatus::PASSED;
}

}