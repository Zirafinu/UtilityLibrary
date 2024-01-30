
include(FetchContent)

FetchContent_Declare(DocTest
        GIT_REPOSITORY "https://github.com/doctest/doctest.git"
        GIT_TAG "ae7a13539fb71f270b87eb2e874fbac80bc8dda2" # 2.4.11
)

FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL/"
        GIT_TAG "0f6dbc9e2915ef5c16830f3fa3565738de2a9230"
)

FetchContent_MakeAvailable(DocTest GSL)
