cmake_minimum_required(VERSION 3.10.2)
#------------------------------------------------------------------------------
# Version information
#------------------------------------------------------------------------------


set(HPC_DEBUGGER_VERSION_MAJOR "0")
set(HPC_DEBUGGER_VERSION_MINOR "0")
set(HPC_DEBUGGER_VERSION_PATCH "1")
set(HPC_DEBUGGER_PACKAGE ${PROJECT_NAME})
set(HPC_DEBUGGER_PACKAGE_NAME ${PROJECT_NAME})
set(HPC_DEBUGGER_PACKAGE_VERSION "${HPC_DEBUGGER_VERSION_MAJOR}.${HPC_DEBUGGER_VERSION_MINOR}.${HPC_DEBUGGER_VERSION_PATCH}")
set(HPC_DEBUGGER_PACKAGE_VERSION_MAJOR "${HPC_DEBUGGER_VERSION_MAJOR}.${HPC_DEBUGGER_VERSION_MINOR}")
set(HPC_DEBUGGER_PACKAGE_VERSION_MINOR "${HPC_DEBUGGER_VERSION_PATCH}")
set(HPC_DEBUGGER_PACKAGE_STRING "${HPC_DEBUGGER_PACKAGE_NAME} ${HPC_DEBUGGER_PACKAGE_VERSION}")
set(HPC_DEBUGGER_PACKAGE_TARNAME "${HPC_DEBUGGER_PACKAGE}")
set(HPC_DEBUGGER_LIBTOOL_VERSION "${HPC_DEBUGGER_PACKAGE_VERSION}")

set(HPC_DEBUGGER_VERSION "(0, 0, 1)")

project(hpc-debugger LANGUAGES C CXX)

# Convenience defines
string(TOUPPER "${PROJECT_NAME}" UPPER_PROJECT_NAME)
string(TOLOWER "${PROJECT_NAME}" LOWER_PROJECT_NAME)

#------------------------------------------------------------------------------
# Internal Includes for header and libraries
#------------------------------------------------------------------------------
# Get installation directories -- these get used in various places;
# best to just make them available
option (HPC_DEBUGGER_LIBDIR_AS_LIB OFF)
if (NOT HPC_DEBUGGER_LIBDIR_AS_LIB)
    include(GNUInstallDirs)
endif ()
#
if (CMAKE_INSTALL_LIBDIR)
   message(STATUS "CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR}")
   set(HPC_DEBUGGER_LIBDIR ${CMAKE_INSTALL_LIBDIR})
   set(HPC_DEBUGGER_INSTALL_INCLUDE_DIR
       ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})
   set(HPC_DEBUGGER_INSTALL_DOCDIR
       ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DOCDIR})
   set(HPC_DEBUGGER_INSTALL_SYSCONFDIR
       ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/modulefiles)
else ()
   set(HPC_DEBUGGER_LIBDIR "lib")
   set(HPC_DEBUGGER_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
   set(HPC_DEBUGGER_INSTALL_DOCDIR "${CMAKE_INSTALL_PREFIX}/doc")
   set(HPC_DEBUGGER_INSTALL_SYSCONFDIR "${CMAKE_INSTALL_PREFIX}/etc/modulefiles")
   message(STATUS "HPC_DEBUGGER_LIBDIR set to ${HPC_DEBUGGER_LIBDIR}")
endif ()

#------------------------------------------------------------------------------
# Internal Paths for cmake libraries and Setup install and output Directories
#------------------------------------------------------------------------------
# This sets where to look for dependent libraries
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_BINARY_DIR} ${CMAKE_INSTALL_PREFIX})
# This sets where to look for dependent library's cmake files
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/${HPC_DEBUGGER_LIBDIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/share/cmake)

#------------------------------------------------------------------------------
if (NOT HPC_DEBUGGER_EXTERNALLY_CONFIGURED)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables.")
    set(CMAKE_INCLUDE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/include CACHE PATH "Store the headers.")
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${HPC_DEBUGGER_LIBDIR} CACHE PATH "Single Directory for all Libraries")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${HPC_DEBUGGER_LIBDIR} CACHE PATH "Single Directory for all static libraries.")
endif ()

#-----------------------------------------------------------------------------
# Targets built within this project are exported at Install time for use
# by other projects.
#-----------------------------------------------------------------------------
if (NOT HPC_DEBUGGER_EXPORTED_TARGETS)
    set(HPC_DEBUGGER_EXPORTED_TARGETS "hpc_debugger-targets")
endif ()
#------------------------------------------------------------------------------
# Build options
#------------------------------------------------------------------------------
# option(ENABLE_HPC_DEBUGGER_DEBUG "Include debugging prints and logging" OFF) # This is not needed as we have CMAKE_BUILD_TYPE
# This is verbose, maybe an alternate might help simplify
#option(BUILD_URPC "Build HPC_DEBUGGER's URPC code" OFF)
#option(ENABLE_PERFFLOW "Build with PerfFlow Aspect support" OFF)
#option(ENABLE_UCX_DTL "Build HPC_DEBUGGER's UCX data transport layer" OFF)

# This option is off by default because non-developers should not use
# this option under normal circumstances.
option(HPC_DEBUGGER_WARNINGS_AS_ERRORS "Build with warnings promoted to errors." OFF)
mark_as_advanced(HPC_DEBUGGER_WARNINGS_AS_ERRORS)

option (HPC_DEBUGGER_USE_CLANG_LIBCXX OFF)
if (HPC_DEBUGGER_USE_CLANG_LIBCXX)
    set (GCC_TOOLCHAIN "")
endif (HPC_DEBUGGER_USE_CLANG_LIBCXX)

# Public
option(HPC_DEBUGGER_BUILD_WITH_MPI "Build with mpi and mpiio interception libraries." OFF)
option(HPC_DEBUGGER_BUILD_DEPENDENCIES "Build Dependencies for HPC_DEBUGGER" OFF)
option(HPC_DEBUGGER_INSTALL_DEPENDENCIES "Install dependencies for HPC_DEBUGGER" OFF)

if(HPC_DEBUGGER_BUILD_WITH_MPI)
    set(HPC_DEBUGGER_ENABLE_MPI 1)
endif()

# Profiling and logging
set(HPC_DEBUGGER_LOGGER "NONE" CACHE STRING "Logger to use for HPC_DEBUGGER")
set_property(CACHE HPC_DEBUGGER_LOGGER PROPERTY STRINGS CPP_LOGGER NONE)
set(HPC_DEBUGGER_LOGGER_LEVEL "NONE" CACHE STRING "Logging level to use for HPC_DEBUGGER")
set_property(CACHE HPC_DEBUGGER_LOGGER_LEVEL PROPERTY STRINGS TRACE DEBUG INFO WARN ERROR NONE)

# Testing
option (HPC_DEBUGGER_ENABLE_TESTING "Enable tests for HPC_DEBUGGER." OFF)

#------------------------------------------------------------------------------
# Compiler setup
#------------------------------------------------------------------------------
include(ExternalProject)
# It requires include(GNUInstallDirs) before if that is needed
include(SetupCompiler)

#------------------------------------------------------------------------------
# Build Type based configurations
#------------------------------------------------------------------------------

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release") # Setting default as Release
endif()

if(HPC_DEBUGGER_LOGGER STREQUAL "CPP_LOGGER")
    set(HPC_DEBUGGER_LOGGER_CPP_LOGGER 1)
endif()

function(check_logger)
endfunction(check_logger)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (HPC_DEBUGGER_LOGGER STREQUAL "NONE")
        message(STATUS "[${PROJECT_NAME}] disabling logger as no logger is selected")
        set(HPC_DEBUGGER_LOGGER_NO_LOG 1)
    else()
        if (HPC_DEBUGGER_LOGGER_LEVEL STREQUAL "NONE")
          set(HPC_DEBUGGER_LOGGER_LEVEL "DEBUG")
        endif()
        
        if (HPC_DEBUGGER_LOGGER_LEVEL STREQUAL "TRACE")
          set(HPC_DEBUGGER_LOGGER_LEVEL_ERROR 1)
          set(HPC_DEBUGGER_LOGGER_LEVEL_WARN 1)
          set(HPC_DEBUGGER_LOGGER_LEVEL_INFO 1)
          set(HPC_DEBUGGER_LOGGER_LEVEL_DEBUG 1)
          set(HPC_DEBUGGER_LOGGER_LEVEL_TRACE 1)
        elseif (HPC_DEBUGGER_LOGGER_LEVEL STREQUAL "DEBUG")
          set(HPC_DEBUGGER_LOGGER_LEVEL_ERROR 1)
          set(HPC_DEBUGGER_LOGGER_LEVEL_WARN 1)
          set(HPC_DEBUGGER_LOGGER_LEVEL_INFO 1)
          set(HPC_DEBUGGER_LOGGER_LEVEL_DEBUG 1)
        elseif(HPC_DEBUGGER_LOGGER_LEVEL STREQUAL "ERROR")
          set(HPC_DEBUGGER_LOGGER_LEVEL_ERROR 1)
        elseif(HPC_DEBUGGER_LOGGER_LEVEL STREQUAL "WARN")
          set(HPC_DEBUGGER_LOGGER_LEVEL_ERROR 1)
          set(HPC_DEBUGGER_LOGGER_LEVEL_WARN 1)
        elseif(HPC_DEBUGGER_LOGGER_LEVEL STREQUAL "INFO")
          set(HPC_DEBUGGER_LOGGER_LEVEL_ERROR 1)
          set(HPC_DEBUGGER_LOGGER_LEVEL_WARN 1)
          set(HPC_DEBUGGER_LOGGER_LEVEL_INFO 1)
        else()
          set(HPC_DEBUGGER_LOGGER_NO_LOG 1)
        endif()
    endif()
else()
  if (HPC_DEBUGGER_LOGGER_LEVEL STREQUAL "NONE")
    set(HPC_DEBUGGER_LOGGER_LEVEL "WARN")
  endif()
        
  if (HPC_DEBUGGER_LOGGER_LEVEL STREQUAL "TRACE")
    set(HPC_DEBUGGER_LOGGER_LEVEL_ERROR 1)
    set(HPC_DEBUGGER_LOGGER_LEVEL_WARN 1)
    set(HPC_DEBUGGER_LOGGER_LEVEL_INFO 1)
    set(HPC_DEBUGGER_LOGGER_LEVEL_DEBUG 1)
    set(HPC_DEBUGGER_LOGGER_LEVEL_TRACE 1)
  elseif (HPC_DEBUGGER_LOGGER_LEVEL STREQUAL "DEBUG")
    set(HPC_DEBUGGER_LOGGER_LEVEL_ERROR 1)
    set(HPC_DEBUGGER_LOGGER_LEVEL_WARN 1)
    set(HPC_DEBUGGER_LOGGER_LEVEL_INFO 1)
    set(HPC_DEBUGGER_LOGGER_LEVEL_DEBUG 1)
  elseif(HPC_DEBUGGER_LOGGER_LEVEL STREQUAL "ERROR")
    set(HPC_DEBUGGER_LOGGER_LEVEL_ERROR 1)
  elseif(HPC_DEBUGGER_LOGGER_LEVEL STREQUAL "WARN")
    set(HPC_DEBUGGER_LOGGER_LEVEL_ERROR 1)
    set(HPC_DEBUGGER_LOGGER_LEVEL_WARN 1)
  elseif(HPC_DEBUGGER_LOGGER_LEVEL STREQUAL "INFO")
    set(HPC_DEBUGGER_LOGGER_LEVEL_ERROR 1)
    set(HPC_DEBUGGER_LOGGER_LEVEL_WARN 1)
    set(HPC_DEBUGGER_LOGGER_LEVEL_INFO 1)
  else()
    set(HPC_DEBUGGER_LOGGER_NO_LOG 1)
  endif()
endif()

if (NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
endif ()
#------------------------------------------------------------------------------
# Disallow in-source build
#------------------------------------------------------------------------------
if ("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR
            "${PROJECT_NAME} requires an out of source Build. "
            "Please create a separate binary directory and run CMake there.")
endif ()
#------------------------------------------------------------------------------
# Set a default build type if none was specified
#------------------------------------------------------------------------------
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
            "MinSizeRel" "RelWithDebInfo")
endif ()


function(hpc_debugger_install_headers public_headers current_dir)
    #message("-- [${PROJECT_NAME}] " "installing headers ${public_headers}")
    foreach (header ${public_headers})
        file(RELATIVE_PATH header_file_path "${PROJECT_SOURCE_DIR}/src" "${header}")
        #message("-- [${PROJECT_NAME}] " "installing header ${header_file_path}")
        get_filename_component(header_directory_path "${header_file_path}" DIRECTORY)
        install(
                FILES ${header}
                DESTINATION "include/${header_directory_path}"
        )
        file(COPY ${header}
                DESTINATION "${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/${header_directory_path}")
    endforeach ()
    set(INCLUDE_INSTALL_DIRS ${public_headers})
endfunction()


#------------------------------------------------------------------------------
# Dependencies
#------------------------------------------------------------------------------

# Install Dependencies
# =============================================================================
if (HPC_DEBUGGER_BUILD_DEPENDENCIES)
  add_subdirectory(dependency)

else()


#-----------------------------------------------------------------------------
# Source for HPC_DEBUGGER
#-----------------------------------------------------------------------------
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
include_directories(${CMAKE_SOURCE_DIR}/include)  # public header
include_directories(${CMAKE_SOURCE_DIR}/src)  # private header
include_directories(${CMAKE_BINARY_DIR}/include)  # build header

set(HPC_DEBUGGER_MPI_SRC )
set(HPC_DEBUGGER_MPI_INCLUDE )

set(HPC_DEBUGGER_SRC  ${CMAKE_CURRENT_SOURCE_DIR}/src/hpc-debugger/signal_handler.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/src/hpc-debugger/backtrace.cpp)
set(HPC_DEBUGGER_PRIVATE_HEADER  ${CMAKE_CURRENT_SOURCE_DIR}/src/hpc-debugger/backtrace.h)
set(HPC_DEBUGGER_PUBLIC_HEADER        ${CMAKE_CURRENT_SOURCE_DIR}/include/hpc-debugger/signal_handler.h)
set(HPC_DEBUGGER_SRC_PRIVATE )

if(HPC_DEBUGGER_BUILD_WITH_MPI)
  set(HPC_DEBUGGER_SRC ${HPC_DEBUGGER_SRC} ${HPC_DEBUGGER_MPI_SRC})
  set(HPC_DEBUGGER_PUBLIC_HEADER ${HPC_DEBUGGER_PUBLIC_HEADER} ${HPC_DEBUGGER_MPI_INCLUDE})
endif()

add_library(${PROJECT_NAME} SHARED ${HPC_DEBUGGER_SRC} ${HPC_DEBUGGER_PUBLIC_HEADER} ${HPC_DEBUGGER_PRIVATE_HEADER})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LINKER_LANGUAGE CXX
    VERSION ${HPC_DEBUGGER_LIBTOOL_VERSION}
    SOVERSION 1
    PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/hpc_debugger/hpc_debugger.h)
target_compile_definitions(${PROJECT_NAME} PUBLIC HPC_DEBUGGER_HAS_CONFIG)
hpc_debugger_install_headers("${HPC_DEBUGGER_PUBLIC_HEADER};${HPC_DEBUGGER_SRC_PRIVATE}" ${CMAKE_CURRENT_SOURCE_DIR})

# Required Dependencies
# =============================================================================
set(DEPENDENCY_LIB )

# Optional Dependencies
# =============================================================================
if(HPC_DEBUGGER_BUILD_WITH_MPI)
    # find MPI
    find_package(MPI COMPONENTS CXX REQUIRED)
    if (MPI_FOUND)
        message(STATUS "[HPC_DEBUGGER] found mpi.h at ${MPI_CXX_INCLUDE_DIRS}")
        include_directories(${MPI_CXX_INCLUDE_DIRS})
        set(DEPENDENCY_LIB ${DEPENDENCY_LIB} ${MPI_CXX_LIBRARIES})
    else ()
        message(FATAL_ERROR "-- [HPC_DEBUGGER] mpi is needed for ${PROJECT_NAME} build")
    endif ()
endif()

if (HPC_DEBUGGER_LOGGER STREQUAL "CPP_LOGGER")
    find_package(cpp-logger 1.0.0 REQUIRED
                 HINTS ${CPP_LOGGER_DIR} ${cpp-logger_DIR}
                       ${CPP_LOGGER_PATH} ${cpp-logger_PATH}
                       $ENV{CPP_LOGGER_DIR} $ENV{cpp-logger_DIR}
                       $ENV{CPP_LOGGER_PATH} $ENV{cpp-logger_PATH})
    if (${cpp-logger_FOUND})
        message(STATUS "[${PROJECT_NAME}] found cpp-logger at ${CPP_LOGGER_INCLUDE_DIRS}")
        include_directories(SYSTEM ${CPP_LOGGER_INCLUDE_DIRS})
        set(DEPENDENCY_LIB ${DEPENDENCY_LIB} ${CPP_LOGGER_LIBRARIES})
    else ()
        message(FATAL_ERROR "-- [${PROJECT_NAME}] cpp-logger is not found but selected in cmake options for ${PROJECT_NAME} build")
    endif ()
endif()

target_link_libraries(${PROJECT_NAME} ${DEPENDENCY_LIB})

install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${HPC_DEBUGGER_EXPORTED_TARGETS}
        LIBRARY DESTINATION ${HPC_DEBUGGER_INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${HPC_DEBUGGER_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${HPC_DEBUGGER_INSTALL_BIN_DIR}
)
#-----------------------------------------------------------------------------
# Testing
#-----------------------------------------------------------------------------

#cmake_policy(SET CMP0079 NEW) # In case that we need more control over the target building order

if(HPC_DEBUGGER_ENABLE_TESTING)
  enable_testing()
  set(TEST_LIBS )
  add_subdirectory(tests)
endif()

#-----------------------------------------------------------------------------
# Configure the config.cmake file for the build directory
#-----------------------------------------------------------------------------
include(CMakePackageConfigHelpers)
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/configure_files/${PROJECT_NAME}-config.cmake.build.in
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
        INSTALL_DESTINATION  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}/${PROJECT_NAME}-config.cmake 
        PATH_VARS CMAKE_BINARY_DIR
)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/configure_files/${PROJECT_NAME}-config.cmake.install.in
        "${CMAKE_BINARY_DIR}/install/${PROJECT_NAME}-config.cmake"
        INSTALL_DESTINATION  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}/install/${PROJECT_NAME}-config.cmake
        PATH_VARS CMAKE_BINARY_DIR
)

# configure_file(
#         ${CMAKE_CURRENT_SOURCE_DIR}/cmake/configure_files/${PROJECT_NAME}-config.cmake.install.in
#         ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}/install/${PROJECT_NAME}-config.cmake @ONLY
# )
install(
        FILES
        "${CMAKE_BINARY_DIR}/install/${PROJECT_NAME}-config.cmake"
        DESTINATION
        ${HPC_DEBUGGER_LIBDIR}/cmake/${PROJECT_NAME}
)
#-----------------------------------------------------------------------------
# Configure the ${PROJECT_NAME}-config-version .cmake file for the install directory
#-----------------------------------------------------------------------------
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/configure_files/${PROJECT_NAME}-config-version.cmake.in
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}/${PROJECT_NAME}-config-version.cmake @ONLY
)

install(
        FILES
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}/${PROJECT_NAME}-config-version.cmake
        DESTINATION
        ${HPC_DEBUGGER_LIBDIR}/cmake/${PROJECT_NAME}
)

export(EXPORT ${HPC_DEBUGGER_EXPORTED_TARGETS}
        FILE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}/${PROJECT_NAME}Targets.cmake")

# Write the configure file
configure_file("${CMAKE_SOURCE_DIR}/cmake/configure_files/hpc_debugger_config.hpp.in"
               "${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/hpc-debugger/hpc_debugger_config.hpp" @ONLY)


################################################################
# Install HPC_DEBUGGER
################################################################

include(CMakePackageConfigHelpers)

# Write the version file. This is independent of build/install tree.
write_basic_package_version_file(
  HPC_DEBUGGERConfigVersion.cmake
  VERSION "${HPC_DEBUGGER_PACKAGE_VERSION}"
  COMPATIBILITY SameMajorVersion)

install(FILES
        "${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/hpc_debugger/hpc_debugger_config.hpp"
        DESTINATION
        ${HPC_DEBUGGER_INSTALL_INCLUDE_DIR}/hpc_debugger)

install(EXPORT
        ${HPC_DEBUGGER_EXPORTED_TARGETS}
        DESTINATION
        ${HPC_DEBUGGER_LIBDIR}/cmake/${PROJECT_NAME}
        FILE
        ${HPC_DEBUGGER_EXPORTED_TARGETS}.cmake
        )

# Install export
install(EXPORT ${HPC_DEBUGGER_EXPORTED_TARGETS}
        NAMESPACE hpc_debugger::
        FILE HPC_DEBUGGERTargets.cmake
        DESTINATION "${HPC_DEBUGGER_LIBDIR}/cmake/hpc_debugger")

# Install license and readme
install(FILES
        "${PROJECT_SOURCE_DIR}/NOTICE"
        "${PROJECT_SOURCE_DIR}/LICENSE"
        "${PROJECT_SOURCE_DIR}/README.md"
        DESTINATION
        ${HPC_DEBUGGER_INSTALL_DOCDIR})

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/hpc-debugger-utils.cmake"
        DESTINATION "${HPC_DEBUGGER_LIBDIR}/cmake/hpc_debugger")


###############################################################################
#                      Print out configuration summary
###############################################################################
include(hpc-debugger-utils)
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
  set(HPC_DEBUGGER_GNU_LINUX TRUE)
endif ()

# NOTE: message() outputs to stderr by default. We now use a string to
# maintain this information and then have cmake echo it to stdout. The
# only side effects are that if you use the CMake GUI, you won't see
# this output anymore (they only report stderr) and that if you add
# something to the list, you must remember your newline!
set(_str "\n== HPC_DEBUGGER Configuration Summary ==\n\n")
string(APPEND _str "  PROJECT_SOURCE_DIR:   ${PROJECT_SOURCE_DIR}\n"
  "  PROJECT_BINARY_DIR:   ${PROJECT_BINARY_DIR}\n\n"
  "  CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}\n"
  "  CMAKE_BUILD_TYPE:     ${CMAKE_BUILD_TYPE}\n\n")
if (CMAKE_BUILD_TYPE MATCHES None)
  string(APPEND _str
    "  CXX FLAGS:            ${CMAKE_CXX_FLAGS}\n")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
  string(APPEND _str
    "  CXX FLAGS:            ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}\n")
  string(APPEND _str
    "  C FLAGS:              ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}\n")
elseif (CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  string(APPEND _str
    "  CXX FLAGS:            ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}\n")
  string(APPEND _str
    "  C FLAGS:              ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELWITHDEBINFO}\n")
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
  string(APPEND _str
    "  CXX FLAGS:            ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}\n")
  string(APPEND _str
    "  C FLAGS:              ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}\n")
endif ()
string(APPEND _str
  "  DEPENDENCY_LIB:              ${DEPENDENCY_LIB}\n")
string(APPEND _str "\n")

string(APPEND _str
  "  HPC_DEBUGGER_PACKAGE_VERSION:        ${HPC_DEBUGGER_PACKAGE_VERSION}\n")
string(APPEND _str
  "  HPC_DEBUGGER_GIT_VERSION:            ${HPC_DEBUGGER_GIT_VERSION}\n")
string(APPEND _str
  "  HPC_DEBUGGER_BUILD_WITH_MPI:          ${HPC_DEBUGGER_BUILD_WITH_MPI}\n")
if(HPC_DEBUGGER_BUILD_WITH_MPI)
  string(APPEND _str
  "  MPIEXEC_EXECUTABLE:                ${MPIEXEC_EXECUTABLE}\n")
endif()
string(APPEND _str
  "  HPC_DEBUGGER_PROFILER:               ${HPC_DEBUGGER_PROFILER}\n")
string(APPEND _str
  "  HPC_DEBUGGER_LOGGER:                 ${HPC_DEBUGGER_LOGGER}\n")
string(APPEND _str
  "  HPC_DEBUGGER_LOGGER_LEVEL:           ${HPC_DEBUGGER_LOGGER_LEVEL}\n")
string(APPEND _str
  "  GLIBC_VERSION:               ${GLIBC_VERSION}\n")
string(APPEND _str
  "  TEST_LIBS:               ${TEST_LIBS}\n")

#Print the true/false variables
append_str_tf(_str
  HPC_DEBUGGER_GNU_LINUX
  HPC_DEBUGGER_BUILD_WITH_MPI
  HPC_DEBUGGER_LIBDIR_AS_LIB
  HPC_DEBUGGER_USE_CLANG_LIBCXX
  HPC_DEBUGGER_WARNINGS_AS_ERRORS
  HPC_DEBUGGER_ENABLE_TESTING
  
)
string(APPEND _str
  "\n== End HPC_DEBUGGER Configuration Summary ==\n")

# Output to stdout
execute_process(COMMAND ${CMAKE_COMMAND} -E echo "${_str}")
set(_str)


###############################################################################
# Write a basic modulefile
###############################################################################

set(HPC_DEBUGGER_MODULEFILE_NAME "hpc_debugger-${HPC_DEBUGGER_PACKAGE_VERSION}.lua")
#  CACHE STRING
#  "The name of the HPC_DEBUGGER modulefile to install. Must end in .lua.")

if (NOT (HPC_DEBUGGER_MODULEFILE_NAME MATCHES ".+\.lua"))
  message(WARNING
    "HPC_DEBUGGER_MODULEFILE_NAME must have extension \".lua\". Appending.")
  set(HPC_DEBUGGER_MODULEFILE_NAME "${HPC_DEBUGGER_MODULEFILE_NAME}.lua"
    CACHE STRING "" FORCE)
endif ()

configure_file(
  "${CMAKE_SOURCE_DIR}/cmake/configure_files/hpc_debugger_module.lua.in"
  "${CMAKE_BINARY_DIR}/hpc_debugger_module.lua.install"
  @ONLY)

install(FILES "${CMAKE_BINARY_DIR}/hpc_debugger_module.lua.install"
  RENAME "${HPC_DEBUGGER_MODULEFILE_NAME}"
  DESTINATION
  "${HPC_DEBUGGER_INSTALL_SYSCONFDIR}")
endif()