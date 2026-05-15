cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER   gcc-14)
set(CMAKE_CXX_COMPILER g++-14)

project(tuto_cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


### Inclusion des tests
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG       v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# Librairie contenant notre code à tester
add_library(mon_code STATIC
    advanced_concepts/compilation/test_ex/src/Camera.cpp
    advanced_concepts/compilation/test_ex/src/Calculatrice.cpp
)

# Headers de la libraire
target_include_directories(mon_code PUBLIC advanced_concepts/compilation/test_ex/headers)



add_subdirectory(tests)


add_executable(tuto_cpp main.cpp
        src/personnage.cpp
        headers/personnage.h
        src/_inline_use.cpp
        headers/_inline_use.h
        src/Iterator.cpp
        headers/Iterator.h
        headers/template.h
        headers/file_handler.h
        headers/race_conditions.h
        headers/union_use.h
        headers/chrono_use.h
        headers/enumerations.h
        headers/namespace_use.h
        headers/static_key_word_use.h
        headers/interface_use.h
        headers/mutable_key_word_use.h
        headers/operator_redefinition.h
        headers/virtual_destructor.h
        headers/explicit_key_word_use.h
        headers/value_type_use.h
        headers/linked_list.h
        headers/no_execept_key_word_use.h
        headers/future_promesses_use.h
        headers/string_view_use.h
        headers/singleton_pattern_use.h
        headers/template_variadique_use.h
        advanced_concepts/advanced_fundamentals/memory_alignment.h
        advanced_concepts/advanced_fundamentals/value_category.h
        advanced_concepts/advanced_fundamentals/rule_of_zero_three_five.h
        advanced_concepts/advanced_fundamentals/advanced_consts.h
        advanced_concepts/advanced_template/templates.h
        advanced_concepts/advanced_template/template_specialisation.h
        advanced_concepts/advanced_template/sfinae.h
        advanced_concepts/advanced_template/cpp_20_concepts.h
        advanced_concepts/advanced_template/metaprogramming.h
        advanced_concepts/advanced_stl/containers.h
        advanced_concepts/advanced_stl/iterators.h
        advanced_concepts/advanced_stl/stl_algorithms.h
        advanced_concepts/advanced_stl/ranges.h
        advanced_concepts/advanced_stl/utilitaires_stl.h
        advanced_concepts/concurrency_parallelism/threads_concepts.h
        advanced_concepts/concurrency_parallelism/mutex_concepts.h
        advanced_concepts/concurrency_parallelism/atomic_op.h
        advanced_concepts/concurrency_parallelism/asynchrone.h
        advanced_concepts/concurrency_parallelism/coroutines.h
        advanced_concepts/concurrency_parallelism/condition_variables.h
        advanced_concepts/design_pattern_architectures/pattern_creationnels.h
        advanced_concepts/design_pattern_architectures/structural_patterns.h
        advanced_concepts/design_pattern_architectures/behavior_pattern.h
        advanced_concepts/design_pattern_architectures/cpp_pattern.h
        advanced_concepts/optimization/advanced_preprocessing.h
        advanced_concepts/optimization/optimization.h
        advanced_concepts/optimization/compilateur.h
        advanced_concepts/optimization/low_level.h
        advanced_concepts/modern_cpp/cpp_17.h
        advanced_concepts/modern_cpp/cpp_20.h
        advanced_concepts/modern_cpp/cpp_23.h
        advanced_concepts/compilation/profiling.h
)


target_link_libraries(tuto_cpp PUBLIC mon_code)


find_program(VALGRIND valgrind)

if(VALGRIND)
    add_custom_target(valgrind
    COMMAND ${VALGRIND}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:tuto_cpp>
            DEPENDS tuto_cpp
            COMMENT "Analyse Valgrind en cours ..."
    )
    message(STATUS "Analyse Valgrind en cours ...")
else()
    message(WARNING "Valgrind non trouvé !")
endif()



target_compile_options(tuto_cpp PRIVATE
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:RelWithDebInfo>:-O2 -g -fno-omit-frame-pointer>
)



# Activation des tests avec ctest
enable_testing()