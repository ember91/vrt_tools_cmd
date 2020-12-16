cmake_minimum_required(VERSION 3.9)

project(
  vrt_tools_cmd
  LANGUAGES C
  DESCRIPTION
    "Command line tools for packets following the VITA Radio Transport (VRT) standard, i.e. VITA-49.0."
)

# Search for modules in root directory as well
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")

# Default to 'Release'
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE
        STRING
        "Choose the type of build. Options are: Debug, Release, RelWithDebInfo, MinSizeRel."
        FORCE)
endif()

# Add options
option(CPPLINT "Use google style guide linter" OFF)
option(CPPCHECK "Enable cppcheck" OFF)
option(CLANG_TIDY "Enable clang-tidy" OFF)
option(DOCUMENTATION "Build doxygen documentation" OFF)
option(IWYU "Include what you use" OFF)
option(TEST "Compile test suite" OFF)

find_package(Doxygen)
# Note that Breathe may located with find_package but may not work with sudo
# make install. Install with 'sudo pip3 install breathe'.
find_package(Breathe)
# Note that Sphinx may located with find_package but may not work with sudo make
# install. Install with 'sudo pip3 install sphinx_rtd_theme'.
find_package(Sphinx)
find_package(Exhale)

if(NOT DOXYGEN_FOUND)
  message(WARNING "Doxygen is required for documentation generation")
endif()
if(NOT BREATHE_FOUND)
  message(WARNING "Breathe is required for documentation generation")
endif()
if(NOT SPHINX_FOUND)
  message(WARNING "Sphinx is required for documentation generation")
endif()
if(NOT EXHALE_FOUND)
  message(WARNING "Exhale is required for documentation generation")
endif()

# Check if options are set
if(${CLANG_TIDY})
  message(STATUS "Enabling clang-tidy for C++")
  set(CMAKE_CXX_CLANG_TIDY
      "clang-tidy;-checks=*,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers,-hicpp-signed-bitwise,-fuchsia-default-arguments-calls,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-modernize-use-trailing-return-type,-cppcoreguidelines-pro-type-reinterpret-cast,-cert-err58-cpp,-cppcoreguidelines-non-private-member-variables-in-classes,-misc-non-private-member-variables-in-classes,-cppcoreguidelines-non-private-member-variables-in-classes,-cppcoreguidelines-special-member-functions,-fuchsia-statically-constructed-objects,-hicpp-special-member-functions,-cppcoreguidelines-owning-memory,-fuchsia-default-arguments-declarations"
  )
endif()
if(${CPPCHECK})
  message(STATUS "Enabling Cppcheck for C++")
  set(CMAKE_CXX_CPPCHECK "cppcheck;--std=c99")
endif()
if(${CPPLINT})
  message(STATUS "Enabling cpplint for C++")
  set(CMAKE_CXX_CPPLINT
      "cpplint;--linelength=120;--filter=-legal/copyright,-build/include_subdir"
  )
endif()
if(DOCUMENTATION)
  if(DOXYGEN_FOUND
     AND BREATHE_FOUND
     AND SPHINX_FOUND
     AND EXHALE_FOUND)
    # Set input and output files
    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in")
    set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile")

    # Copy all configuration files with file extension ".in"
    file(
      GLOB TEMPLATE_FILES CONFIGURE_DEPENDS
      RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/"
      "${CMAKE_CURRENT_SOURCE_DIR}/doc/*.in")
    foreach(FILE ${TEMPLATE_FILES})
      string(LENGTH ${FILE} LEN)
      math(EXPR LEN "${LEN} - 3")
      string(SUBSTRING ${FILE} 0 ${LEN} FILE_NO_EXT)
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
                     ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NO_EXT})
    endforeach()

    # Option ALL allows building documentation together with the application
    add_custom_target(
      doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      VERBATIM)

    if(NOT DEFINED SPHINX_THEME)
      set(SPHINX_THEME default)
    endif()

    if(NOT DEFINED SPHINX_THEME_DIR)
      set(SPHINX_THEME_DIR)
    endif()

    add_custom_target(
      sphinx ALL
      ${SPHINX_EXECUTABLE}
      -q
      -b
      html
      -c
      "${CMAKE_CURRENT_BINARY_DIR}/doc"
      -d
      "${CMAKE_CURRENT_BINARY_DIR}/doc/.doctrees"
      "${CMAKE_CURRENT_BINARY_DIR}/doc"
      "${CMAKE_CURRENT_SOURCE_DIR}/doc/html"
      COMMENT "Building HTML documentation with Sphinx"
      VERBATIM)
  else()
    message(
      WARNING
        "Doxygen with dot component is required for documentation generation")
  endif()
endif()
if(${IWYU})
  message(STATUS "Enabling Include-what-you-use for C++")
  set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "/usr/local/bin/include-what-you-use")
endif()
if(${TEST})
  message(STATUS "Compiling test suite")
endif()

find_package(Libvrt REQUIRED)

add_subdirectory(gen)
add_subdirectory(length)
add_subdirectory(lib)
add_subdirectory(merge)
add_subdirectory(packet_loss)
add_subdirectory(print)
add_subdirectory(socket)
add_subdirectory(split)
