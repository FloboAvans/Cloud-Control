cmake_minimum_required(VERSION 2.6.2)
project(edison)

find_library(mraa_LIBRARIES mraa REQUIRED)
find_library(m_LIBRARIES m REQUIRED)

message(STATUS ${mraa_LIBRARIES})
message(STATUS ${m_LIBRARIES})

add_definitions(-Wall -Wextra -std=gnu11)

add_executable(blinkled "src/blinkLed.c")
target_link_libraries(blinkled ${mraa_LIBRARIES} ${m_LIBRARIES})

add_executable(readsensor "src/readSensor.c")
target_link_libraries(readsensor ${mraa_LIBRARIES})
target_link_libraries(readsensor ${m_LIBRARIES})

add_executable(readgassensor "src/readGasSensor.c")
target_link_libraries(readgassensor ${mraa_LIBRARIES})
target_link_libraries(readgassensor ${m_LIBRARIES})

