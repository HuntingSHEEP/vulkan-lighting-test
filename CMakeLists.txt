cmake_minimum_required(VERSION 3.5)
project(someApp)

# Pobierz wszystkie pliki źródłowe (cpp) z folderów src i src/GraphicsEngine
file(GLOB_RECURSE CPP_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/GraphicsEngine/*.cpp)

# Pobierz wszystkie pliki nagłówkowe (h) z folderów include i include/GraphicsEngine
file(GLOB_RECURSE H_SOURCES ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/include/GraphicsEngine/*.h)

# Ustaw pliki źródłowe dla twojego projektu
add_executable(someApp ${CPP_SOURCES} ${H_SOURCES})

# Dodaj główny katalog include do ścieżek dołączania
include_directories(${CMAKE_SOURCE_DIR}/include)

# Dodaj ścieżki dołączania dla podkatalogów
target_include_directories(someApp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    # Dodaj tutaj inne ścieżki, jeśli są
)


# Ścieżki do bibliotek zewnętrznych
set(EXTERNAL_LIBS_DIR "${CMAKE_SOURCE_DIR}/libraries")


target_include_directories(someApp PRIVATE ${EXTERNAL_LIBS_DIR}/stb)
target_include_directories(someApp PRIVATE ${EXTERNAL_LIBS_DIR}/glfw/include)
target_include_directories(someApp PRIVATE ${EXTERNAL_LIBS_DIR}/glm)
target_include_directories(someApp PRIVATE ${EXTERNAL_LIBS_DIR}/tinyObjLoader)
target_include_directories(someApp PRIVATE ${EXTERNAL_LIBS_DIR}/VulkanSDK/1.3.231.1/Include)

# Zostawiono tylko te biblioteki które są obecnie używane
target_link_libraries(someApp PRIVATE ${EXTERNAL_LIBS_DIR}/VulkanSDK/1.3.231.1/vulkan-1.lib)
target_link_libraries(someApp PRIVATE ${EXTERNAL_LIBS_DIR}/glfw/glfw3.lib)





# Reszta twojego kodu CMakeLists.txt
#set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT someApp)
target_compile_features(someApp PRIVATE cxx_std_17)
target_compile_options(someApp PRIVATE /EHsc)
target_link_options(someApp PRIVATE /NODEFAULTLIB:MSVCRT)

add_custom_command(TARGET someApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:someApp>
            ${CMAKE_SOURCE_DIR}/bin
    COMMENT "Kopiowanie pliku wykonywalnego do folderu bin"
)

# Ustawienie domyślnego folderu dla plików źródłowych w
