set(SCC_SOURCES "utils/TCPServer.cpp" "utils/ConnectionManager.cpp" "utils/API.hpp" "utils/CountAPI.cpp")
add_library(utils STATIC ${SCC_SOURCES})

set(SOURCE SingleCurrentCtLinuxService.cpp)
add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} utils)