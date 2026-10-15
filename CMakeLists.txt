cmake_minimum_required(VERSION 3.10)

# 1. Project name and C++ standard configuration
project(TicTacToe-LV2)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Recursively find all source files in src/ and its subdirectories
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 3. Define the executable target
add_executable(game ${SOURCES})

if(EMSCRIPTEN)
    # ---------- Web build (Emscripten → WebAssembly) ----------
    # emcmake cmake -S . -B build-web && cmake --build build-web
    # Kết quả: index.html, index.js, index.wasm, index.data
    set(SDL_PORTS -sUSE_SDL=2 -sUSE_SDL_TTF=2 -sUSE_SDL_IMAGE=2)
    set_target_properties(game PROPERTIES OUTPUT_NAME "index" SUFFIX ".html")
    target_compile_options(game PRIVATE ${SDL_PORTS} -fexceptions -O2)
    target_link_options(game PRIVATE
        ${SDL_PORTS}
        "-sSDL2_IMAGE_FORMATS=[\"png\"]"
        -sASYNCIFY                 # cho phép vòng lặp chờ sự kiện (SDL_WaitEvent/SDL_Delay) chạy trên trình duyệt
        -sASYNCIFY_STACK_SIZE=65536
        -sALLOW_MEMORY_GROWTH=1
        -fexceptions
        -O2
        --preload-file ${CMAKE_SOURCE_DIR}/assets@/assets
        --shell-file ${CMAKE_SOURCE_DIR}/web/shell.html
    )
else()
    # ---------- Desktop build (Windows MSYS2 / Linux / macOS) ----------
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)

    target_include_directories(game PRIVATE ${SDL2_INCLUDE_DIRS})

    if(WIN32)
        target_link_libraries(game PRIVATE mingw32 SDL2::SDL2main)
    endif()
    target_link_libraries(game PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image)

    if(MSVC)
        target_compile_options(game PRIVATE /W4 /O2)
    else()
        target_compile_options(game PRIVATE -Wall -Wextra -O2)
    endif()

    # Copy assets/ next to the executable
    add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:game>/assets
    )
endif()
