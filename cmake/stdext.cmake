include(FetchContent)

# Fetch stdext from its GitHub repository
FetchContent_Declare(
  stdext
  GIT_REPOSITORY https://github.com/hatsil/std_extention.git
  GIT_TAG        main  # Specify the version you want
)

FetchContent_MakeAvailable(stdext)
