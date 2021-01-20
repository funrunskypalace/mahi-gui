# include(FetchContent) 
# FetchContent_Declare(mahi-gui GIT_REPOSITORY https://github.com/mahilab/mahi-gui.git) 
# FetchContent_MakeAvailable(mahi-gui)

set(ENV{OOSTYPE}                    OpenOrder)
set(ENV{OSVER}                      win64)
set(MahiSrcDir                      ${PROJECT_SOURCE_DIR})
set(OrderRouterSrcDir               ${PROJECT_SOURCE_DIR}/../QTSF_OrderRouter)
set(OpenOrderDistDir                ${PROJECT_SOURCE_DIR})
set(StratRunnerSrcDir               ${PROJECT_SOURCE_DIR}/../SkyPalaceRunner)
set(OrchidCommonLibraryRef          orchid_common_$ENV{OSVER}$<$<CONFIG:Debug>:d>)
set(OpenOrderLibraryRef             $ENV{OOSTYPE}_$ENV{OSVER}$<$<CONFIG:Debug>:d>)
set(StratFrameworkCommonLibraryRef  strat_framework_common_$ENV{OSVER}$<$<CONFIG:Debug>:d>)
set(StratFrameworkLibraryRef        skypalace_framework_$ENV{OSVER}$<$<CONFIG:Debug>:d>)
set(ImPluginSrcDir                  ${PROJECT_SOURCE_DIR}/../implugin)
set(ImPluginLibraryRef              implugin_$ENV{OSVER}$<$<CONFIG:Debug>:d>)

add_definitions(-DPROTOBUF_USE_DLLS)
add_definitions(-DSPDLOG_FMT_EXTERNAL)
add_definitions(-DFMT_HEADER_ONLY=1)

file(GLOB_RECURSE dear_trader_app_SRC
    "${MahiSrcDir}/src/pch/compat.h"
    "${MahiSrcDir}/src/pch/compat.cc"
    "${MahiSrcDir}/src/pch/compat_*.*"
    "${MahiSrcDir}/config/dear_trader_config.json"
    "${MahiSrcDir}/application/dear_trader/*.h"
    "${MahiSrcDir}/application/dear_trader/*.cpp"
    "${MahiSrcDir}/application/dear_trader/*.cc"
)

add_executable(dear_trader_app ${dear_trader_app_SRC})
target_include_directories(dear_trader_app PRIVATE ${OrderRouterSrcDir}/include)
target_include_directories(dear_trader_app PRIVATE ${OrderRouterSrcDir}/api)
target_include_directories(dear_trader_app PRIVATE ${StratRunnerSrcDir}/include)
target_include_directories(dear_trader_app PRIVATE ${StratRunnerSrcDir}/third)
target_include_directories(dear_trader_app PRIVATE ${MahiSrcDir}/src)
target_include_directories(dear_trader_app PRIVATE ${ImPluginSrcDir}/include)
target_link_directories(dear_trader_app PRIVATE ${StratRunnerSrcDir}/target/lib)
target_link_directories(dear_trader_app PRIVATE ${OrderRouterSrcDir}/target/lib)
target_link_directories(dear_trader_app PRIVATE ${ImPluginSrcDir}/target/lib$<$<CONFIG:Debug>:/Debug>)
target_link_libraries(dear_trader_app mahi::gui  ${OrchidCommonLibraryRef} ${StratFrameworkCommonLibraryRef} ${OpenOrderLibraryRef} ${ImPluginLibraryRef})
if(UNIX)
else()
    #set_target_properties(dear_trader_app PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "--log-level debug")
    set_target_properties(dear_trader_app PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE" VS_DEBUGGER_WORKING_DIRECTORY ${OpenOrderDistDir}/app_home/bin)
    set_target_properties(dear_trader_app PROPERTIES LINK_FLAGS "/STACK:16777216" )
endif()
add_custom_command(TARGET dear_trader_app POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:dear_trader_app> ${OpenOrderDistDir}/app_home/bin/)
INSTALL(TARGETS dear_trader_app RUNTIME)
