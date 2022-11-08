cmake_minimum_required(VERSION 3.0.0)
project(CPP-Environment-Setup-REST-Example VERSION 0.1.0)

add_executable(CPP-Environment-Setup-REST-Example main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
# ============== START CUSTOM ================

# =============================
# CPR 
# (Run `vcpkg install cpr`)
# =============================
find_package(cpr CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr)

# ===============================================
# Nlohmann Json 
# (Run `vcpkg install nlohmann-json`)
# ===============================================
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
