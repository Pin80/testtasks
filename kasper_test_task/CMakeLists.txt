# cmake не ниже 3.5
cmake_minimum_required(VERSION 3.5)
# С++
project(kasper_test_task LANGUAGES CXX)

# Требуется поддержка 17го стандарта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =============== Настройка режимов сборки приложения ===============

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# Санитайзер вкл/выкл для Debug mode
set(USESANITIZER ON)

# Режим сборки приложения

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILDVAL 0)
else()
    set(BUILDVAL 1)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

# =============== Подключение библиотек и путей хидеров ===============
# Пока поддержка только для Linux
# Подключаем различные библиотеки и хидеры
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
if(UNIX)
    message("Configuring for Linux")
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
else(UNIX)
    perror("Platform have not supported yet")
endif(UNIX)
if(UNIX)

set(LIBLISTAPP
    -lstdc++fs -lrt ${GTEST_LIBRARIES})

else(UNIX)
    message(FATAL_ERROR "Only linux is supported")
endif(UNIX)

# =============== Установка флагов компиляции ===============
# Компактные структуры, position independent code
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-padded -fsplit-stack -fPIC")

# =============== Добавление исходников и хидеров ===============
# Файлы исходников и хидеров для всех целей
set(SRC_LIST
    ./base_types.h
    ./locale_ru.h
    ./locale_en.h
    ./converter.h
    )

# Файлы исходников и хидеров для тестирования
set(SRC_TEST_LIST
    ./unittest/main_test.cpp
    )


# Файлы исходников и хидеров для основного приложения
set(SRC_MAIN_LIST
    main.cpp
    )

# Различные файлы
set(PROJECT_OTHER_LIST
    ./README.txt
)

# =============== Определение целей ===============
# Фейковая цель для документации
add_custom_target(useful_files SOURCES ${PROJECT_OTHER_LIST} ${doc_files})

# Модульные тесты
add_executable(${PROJECT_NAME}_test ${SRC_LIST} ${SRC_TEST_LIST})
target_link_libraries(${PROJECT_NAME}_test ${LIBLISTAPP}  pthread)

# Главная цель.
add_executable(${PROJECT_NAME} ${SRC_LIST} ${SRC_MAIN_LIST})
