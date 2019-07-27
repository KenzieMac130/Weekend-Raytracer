cmake_minimum_required(VERSION 3.9)
project (WeekendRaytracer)

#OpenMP
option(USE_OPENMP "OpenMP Multithreading" ON)
if(USE_OPENMP)
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
endif()

#Open Image Denoise
option(USE_INTEL_OPEN_DENOISE "OpenImageDenoise Denoising" OFF)
if(USE_INTEL_OPEN_DENOISE)
if(NOT INTEL_OPEN_DENOISE_INCLUDE)
set (INTEL_OPEN_DENOISE_INCLUDE "" CACHE PATH "Path to OpenImageDenoise include" FORCE)
endif()
if(NOT INTEL_OPEN_DENOISE_LIBS_DEBUG)
set (INTEL_OPEN_DENOISE_LIBS_DEBUG "" CACHE PATH "Path to OpenImageDenoise debug build")
endif()
if(NOT INTEL_OPEN_DENOISE_LIBS_RELEASE)
set (INTEL_OPEN_DENOISE_LIBS_RELEASE "" CACHE PATH "Path to OpenImageDenoise release build")
endif()
endif()

#Config File
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/config.h.in ${CMAKE_BINARY_DIR}/config/config.h @ONLY)
include_directories(${CMAKE_BINARY_DIR}/config)

#Copy Includes
if(INTEL_OPEN_DENOISE_INCLUDE)
	include_directories(${INTEL_OPEN_DENOISE_INCLUDE})
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-DUNICODE -D_UNICODE)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
add_executable(Raytracer ${SRC_FILES} ${HEADER_FILES})
set_property(TARGET Raytracer PROPERTY C_STANDARD 99)

find_library(OpenImageDenoise NAMES OpenImageDenoise
    HINTS "${INTEL_OPEN_DENOISE_LIBS_RELEASE}")
target_link_libraries (Raytracer
    debug ${OpenImageDenoise}
    optimized ${OpenImageDenoise})
