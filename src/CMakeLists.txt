cmake_minimum_required(VERSION 3.0)

project(TCPIPSocketsInC VERSION 1.0.0)

# find_package(library)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

configure_file(Practical.h.in Practical.h)

# add_subdirectory(src)

add_library(DieWithMessage DieWithMessage.c)
add_library(TCPServerUtility TCPServerUtility.c AddressUtility.c)
add_library(UDPServerUtility UDPServerUtility.c AddressUtility.c)
add_library(TCPClientUtility TCPClientUtility.c)
add_library(AddressUtility AddressUtility.c)

add_library(DelimFramer DelimFramer.c)
add_library(LengthFramer LengthFramer.c)

add_library(VoteEncoding VoteEncoding.c)
add_library(VoteEncodingBin VoteEncodingBin.c)


add_executable(voteServer VoteServerTCP.c)
add_executable(voteServer_LF_VE VoteServerTCP.c)

add_executable(voteClient VoteClientTcp.c)
add_executable(voteClient_LF_VE VoteClientTcp.c)

add_executable(TestSizes TestSizes.c)

add_executable(TCPEchoClient4 TCPEchoClient4.c)
add_executable(TCPEchoClient TCPEchoClient.c)
add_executable(UDPEchoClient UDPEchoClient.c)

add_executable(TCPEchoServer4 TCPEchoServer4.c)
add_executable(TCPEchoServer6 TCPEchoServer6.c)
add_executable(TCPEchoServer TCPEchoServer.c)
add_executable(UDPEchoServer UDPEchoServer.c)

add_executable(GetAddrInfo GetAddrInfo.c)

target_link_libraries(voteClient PUBLIC
    DieWithMessage
    TCPClientUtility
    VoteEncodingBin
    DelimFramer
    )



target_link_libraries(voteClient_LF_VE PUBLIC
    DieWithMessage
    TCPClientUtility
    VoteEncoding
    LengthFramer
    )



target_link_libraries(voteServer PUBLIC
    DieWithMessage
    TCPServerUtility
    VoteEncodingBin
    DelimFramer
    )



target_link_libraries(voteServer_LF_VE PUBLIC
    DieWithMessage
    TCPServerUtility
    VoteEncoding
    LengthFramer
    )


target_link_libraries(TCPEchoClient4 PUBLIC
    DieWithMessage
    TCPClientUtility)


target_link_libraries(TCPEchoClient PUBLIC
    DieWithMessage
    TCPClientUtility)

target_link_libraries(UDPEchoClient PUBLIC
    DieWithMessage)

target_link_libraries(TCPEchoServer4 PUBLIC
    DieWithMessage
    TCPServerUtility)


target_link_libraries(TCPEchoServer6 PUBLIC
    DieWithMessage
    TCPServerUtility)


target_link_libraries(TCPEchoServer PUBLIC
    DieWithMessage
    TCPServerUtility)


target_link_libraries(UDPEchoServer PUBLIC
    DieWithMessage
    UDPServerUtility)

target_link_libraries(GetAddrInfo PUBLIC
    DieWithMessage
    AddressUtility)

# target_include_directories(TCPEchoClient4 PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

install(TARGETS DieWithMessage
    AddressUtility
    TCPServerUtility
    UDPServerUtility
    LengthFramer
    DelimFramer
    VoteEncoding
    VoteEncodingBin
    TCPClientUtility DESTINATION lib)

install(TARGETS TCPEchoClient4
    TCPEchoClient
    UDPEchoClient
    TCPEchoServer4
    TCPEchoServer6
    TCPEchoServer
    UDPEchoServer
    TestSizes
    voteServer
    voteServer_LF_VE
    voteClient
    voteClient_LF_VE
    GetAddrInfo DESTINATION bin)


install(FILES
    Practical.h
    VoteEncoding.h
    VoteProtocol.h
    Framer.h
    DESTINATION include)
