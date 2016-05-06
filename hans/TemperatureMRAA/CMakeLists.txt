cmake_minimum_required( VERSION 3.0 )
project( TemperatureMRAA CXX )

set( CMAKE_VERBOSE_MAKEFILE ON )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y " )

include_directories( /Users/hansklabbers/Dev/IoT/edison_media/iss-iot-mac/devkit-x86/sysroots/i586-poky-linux/usr/include/upm 
	/Users/hansklabbers/Dev/IoT/edison_media/iss-iot-mac/devkit-x86/sysroots/i586-poky-linux/usr/include/mraa
       	)

add_executable( TemperatureMRAA src/temperatureMRAA.cpp )
target_link_libraries( TemperatureMRAA -lmraa )

add_custom_command( TARGET TemperatureMRAA 
	POST_BUILD 
	COMMAND scp TemperatureMRAA root@192.168.2.18:/tmp/TemperatureMRAA )
