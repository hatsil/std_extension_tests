include(FetchContent)

# Fetch and declare glog
FetchContent_Declare(
    glog
    GIT_REPOSITORY https://github.com/google/glog.git
    GIT_TAG        v0.7.0
)

# Enable/disable building glog tests and examples
set(GLOG_BUILD_TESTS OFF CACHE BOOL "Build glog tests")
set(GLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build glog examples")

# Make glog available to your project
FetchContent_MakeAvailable(glog)
