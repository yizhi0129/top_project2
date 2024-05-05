cmake_minimum_required(VERSION 3.10)
project(TOP-project)

# Recherche d'OpenMP
find_package(OpenMP REQUIRED)

# Recherche de MPI
find_package(MPI REQUIRED)

# Ajout de la bibliothèque stencil
add_library(stencil SHARED stencil/config.c stencil/comm_handler.c stencil/mesh.c stencil/init.c stencil/solve.c)
# Ajout des répertoires d'inclusion pour stencil
target_include_directories(stencil PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${MPI_C_INCLUDE_PATH}  # Inclusion des fichiers d'en-tête MPI
)
# Liaison des bibliothèques à stencil
target_link_libraries(stencil PUBLIC 
    m
    ${MPI_C_LIBRARIES}  # Liaison avec la bibliothèque MPI
    OpenMP::OpenMP_C  # Liaison avec OpenMP
)

# Ajout de la bibliothèque utils
add_library(utils SHARED chrono.c)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Alias des bibliothèques
add_library(stencil::stencil ALIAS stencil)
add_library(stencil::utils ALIAS utils)

# Ajout d'un exécutable
add_executable(my_executable main.c)

# Liaison des bibliothèques à l'exécutable, en supposant que main.c dépend de utils et stencil
target_link_libraries(my_executable stencil::stencil stencil::utils)

# Si OpenMP et MPI ont été trouvés, assurez-vous d'ajouter les options de compilation à toutes les cibles
if(OPENMP_FOUND AND MPI_FOUND)
    # Add AVX compiler flags
    if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        # For GCC/G++
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
    elseif(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # For Clang/Clang++
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
    elseif(MSVC)
        # MSVC doesn't use the same flag, but AVX support is enabled by default
    endif()
    target_compile_options(stencil PRIVATE ${OpenMP_C_FLAGS} -mavx -fopenmp)
    target_compile_options(utils PRIVATE ${OpenMP_C_FLAGS})
    target_compile_options(my_executable PRIVATE ${OpenMP_C_FLAGS})
endif()
