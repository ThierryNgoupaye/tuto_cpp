cmake_minimum_required(VERSION 3.28)
project(tuto_cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
        advanced_concepts/advanced_stl/utilitaires_stl.h)
