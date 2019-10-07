add_executable(cin_test cin_test.cpp)

target_link_libraries(cin_test lexc89)

include_directories(../lib/re2)
add_executable(re2_ml_test re2_ml_test.cpp)
target_link_libraries(re2_ml_test re2)
target_link_libraries(re2_ml_test pthread)
