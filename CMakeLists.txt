cmake_minimum_required(VERSION 3.16)
project(cpp_oop_lecture LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(add_lecture_target target source)
    add_executable(${target} ${source})
    target_compile_features(${target} PRIVATE cxx_std_20)

    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${source})
endfunction()

# Каждая демонстрация собирается только со своим файлом main.
set(DEMO_TARGETS
    demo0_review
    demo1_money
    demo2_account
    demo3_lifetime
    demo3_raii
    demo4_payments
    demo4b_virtual_dtor_bug
    demo5_order
)
foreach(target IN LISTS DEMO_TARGETS)
    add_lecture_target(${target} "demo/${target}.cpp")
endforeach()

# Задания отображаются в CLion отдельными целями.
# Некоторые заготовки намеренно не собираются до выполнения студентом.
set(TASK_TARGETS
    task0_account
    task1_product
    task2_lifetime
    task2_transaction
    task3_payments
    task4_bonus_matrix
)
foreach(target IN LISTS TASK_TARGETS)
    add_executable(${target} EXCLUDE_FROM_ALL "tasks/${target}.cpp")
    target_compile_features(${target} PRIVATE cxx_std_20)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()

# Решения видны в CLion и не входят в обычную сборку всего проекта.
set(SOLUTION_TARGETS
    sol0_account
    sol1_product
    sol2_lifetime
    sol2_transaction
    sol3_payments
    sol4_bonus_matrix
)
foreach(target IN LISTS SOLUTION_TARGETS)
    add_executable(${target} EXCLUDE_FROM_ALL "solutions/${target}.cpp")
    target_compile_features(${target} PRIVATE cxx_std_20)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()
