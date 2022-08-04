cmake_minimum_required(VERSION 3.20)
project(C_SWSB)

if (${CMAKE_C_COMPILER_ID} MATCHES "Clang")
    set(FLAGS "-std=c17 -Wall -Wextra -Wconversion -Wunreachable-code -Wnewline-eof -Wno-error=unused-variable -Wshadow -Wfloat-equal -Wcovered-switch-default -Wunreachable-code-break")

    if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
        set(FLAGS "${FLAGS} -g")
    endif()

    set(CMAKE_C_FLAGS ${FLAGS})
endif()

set(SWSBSrcDir "src")
set(SWSBSrcFiles
    "${SWSBSrcDir}/SWSB_assets.c"
    "${SWSBSrcDir}/SWSB_background.c"
    "${SWSBSrcDir}/SWSB_ent.c"
    "${SWSBSrcDir}/SWSB_handler.c"
    "${SWSBSrcDir}/SWSB_screen_text.c"
    "${SWSBSrcDir}/SWSB_timer.c"
    "${SWSBSrcDir}/SWSB.c"
)

add_executable(${PROJECT_NAME} src/main.c ${SWSBSrcFiles})
target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image SDL2_ttf)
