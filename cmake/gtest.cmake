include(FetchContent)

# Fetch Google Test from its GitHub repository
FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1  # Specify the version you want
)

FetchContent_MakeAvailable(gtest)
