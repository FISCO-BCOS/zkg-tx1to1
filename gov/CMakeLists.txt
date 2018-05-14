cmake_minimum_required (VERSION 3.0.0)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -g -Wall")

set(ZKG_CLIENT_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${ZKG_CLIENT_CMAKE_DIR})


# 项目信息
project (zkg-tx1to1-gov)
set(VERSION 1.0.0)

include(ProjectJsonCpp)
include(ProjectJsonRpcCpp)
include(ProjectLibZkg)

#编译环境
find_package(Boost)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_definitions( "-DHAS_BOOST" )
endif()

find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_compile_options(-DMULTICORE)
add_compile_options(-std=c++11)
add_compile_options(-DCURVE_ALT_BN128)
add_compile_options(-DNO_PROCPS)
add_compile_options(-DBINARY_OUTPUT=OFF)
#add_subdirectory(libzkg)

# 并将名称保存到 DIR_SRCS 变量
#aux_source_directory(. DIR_SRCS)

include_directories(${JSONCPP_INCLUDE_DIR})
include_directories(${JSONRPCCPP_INCLUDE_DIR})
include_directories(${CURL_INCLUDE_DIR})
include_directories(${MHD_INCLUDE_DIR})

# 指定生成目标
set(GOV_DATA_EXE_NAME "govdata")
add_executable(${GOV_DATA_EXE_NAME} gov.cpp)
# 添加链接库
target_link_libraries(${GOV_DATA_EXE_NAME} ${Boost_LIBRARIES})
target_link_libraries(${GOV_DATA_EXE_NAME} boost_system)
target_link_libraries(${GOV_DATA_EXE_NAME} JsonRpcCpp::Common)
target_link_libraries(${GOV_DATA_EXE_NAME} JsonRpcCpp::Client)
target_link_libraries(${GOV_DATA_EXE_NAME} JsonRpcCpp::Server)
target_link_libraries(${GOV_DATA_EXE_NAME} Zkg)
target_link_libraries(${GOV_DATA_EXE_NAME} Zkg::circuit)
target_link_libraries(${GOV_DATA_EXE_NAME} Zkg::snark)
target_link_libraries(${GOV_DATA_EXE_NAME} Zkg::ff)
target_link_libraries(${GOV_DATA_EXE_NAME} Zkg::zm)
target_link_libraries(${GOV_DATA_EXE_NAME} gmp)
target_link_libraries(${GOV_DATA_EXE_NAME} stdc++)
target_link_libraries(${GOV_DATA_EXE_NAME} procps)
target_link_libraries(${GOV_DATA_EXE_NAME} gmpxx)
target_link_libraries(${GOV_DATA_EXE_NAME} gomp)

# 指定生成目标
set(PKVK_GEN_EXE_NAME "pkvkgen")
add_executable(${PKVK_GEN_EXE_NAME} pkvkgen.cpp)
# 添加链接库
target_link_libraries(${PKVK_GEN_EXE_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PKVK_GEN_EXE_NAME} boost_system)
target_link_libraries(${PKVK_GEN_EXE_NAME} JsonRpcCpp::Common)
target_link_libraries(${PKVK_GEN_EXE_NAME} JsonRpcCpp::Client)
target_link_libraries(${PKVK_GEN_EXE_NAME} JsonRpcCpp::Server)
target_link_libraries(${PKVK_GEN_EXE_NAME} Zkg)
target_link_libraries(${PKVK_GEN_EXE_NAME} Zkg::circuit)
target_link_libraries(${PKVK_GEN_EXE_NAME} Zkg::snark)
target_link_libraries(${PKVK_GEN_EXE_NAME} Zkg::ff)
target_link_libraries(${PKVK_GEN_EXE_NAME} Zkg::zm)
target_link_libraries(${PKVK_GEN_EXE_NAME} gmp)
target_link_libraries(${PKVK_GEN_EXE_NAME} stdc++)
target_link_libraries(${PKVK_GEN_EXE_NAME} procps)
target_link_libraries(${PKVK_GEN_EXE_NAME} gmpxx)
target_link_libraries(${PKVK_GEN_EXE_NAME} gomp)

# 指定生成目标
#gpkgen
set(GPKGEN_EXE_NAME "gpkgen")
add_executable(${GPKGEN_EXE_NAME} gpkgen.cpp)
# 添加链接库
target_link_libraries(${GPKGEN_EXE_NAME} ${Boost_LIBRARIES})
target_link_libraries(${GPKGEN_EXE_NAME} boost_system)
target_link_libraries(${GPKGEN_EXE_NAME} JsonRpcCpp::Common)
target_link_libraries(${GPKGEN_EXE_NAME} JsonRpcCpp::Client)
target_link_libraries(${GPKGEN_EXE_NAME} JsonRpcCpp::Server)
target_link_libraries(${GPKGEN_EXE_NAME} Zkg)
target_link_libraries(${GPKGEN_EXE_NAME} Zkg::circuit)
target_link_libraries(${GPKGEN_EXE_NAME} Zkg::snark)
target_link_libraries(${GPKGEN_EXE_NAME} Zkg::ff)
target_link_libraries(${GPKGEN_EXE_NAME} Zkg::zm)
target_link_libraries(${GPKGEN_EXE_NAME} gmp)
target_link_libraries(${GPKGEN_EXE_NAME} stdc++)
target_link_libraries(${GPKGEN_EXE_NAME} procps)
target_link_libraries(${GPKGEN_EXE_NAME} gmpxx)
target_link_libraries(${GPKGEN_EXE_NAME} gomp)






