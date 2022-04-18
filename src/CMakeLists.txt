cmake_minimum_required(VERSION 3.5)
project(ubus-server)

message(STATUS "start cmake ubus-lt")

# execute_process(COMMAND make WORKING_DIRECTORY include/dbus-adap/)


set(CMAKE_CXX_STANDARD 11)

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


add_executable(ubus-server ubus-server.c)

# target_link_libraries(ubus-lt paho-mqttpp3 paho-mqtt3a)
# target_link_libraries(ubus-lt mosquittopp jsoncpp uuid pthread avahi-client avahi-common)
target_link_libraries(ubus-server ubus ubox)


# For install targets
install(TARGETS ubus-server RUNTIME DESTINATION /usr/bin)
# install(PROGRAMS S99ubus-lt DESTINATION /etc/init.d)
# install(PROGRAMS ubus-lt.default.etc DESTINATION /etc/default/)

# install(PROGRAMS ./sources/repair.wav DESTINATION /usr/voice-lib/)
# install(PROGRAMS ./sources/overlaySaveList DESTINATION /usr/lib/)
