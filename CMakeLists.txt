cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)


add_executable(main src/progetto/pacman.cpp) #Edit her to add your source files

#Older versions, remove if not interested !
add_executable(01 src/progetto/VersioniIntermedie/postRiordine_01/pacman.cpp)
add_executable(02 src/progetto/VersioniIntermedie/disegnoFantasmi_02/pacman.cpp)
add_executable(03 src/progetto/VersioniIntermedie/movimentoPrimoFantasma_03/pacman.cpp)
add_executable(04 src/progetto/VersioniIntermedie/movimentoRedSbagliato_04/pacman.cpp)
add_executable(05 src/progetto/VersioniIntermedie/movimentoGiustoRed_05/pacman.cpp)
add_executable(06 src/progetto/VersioniIntermedie/movimentoPinky_06/pacman.cpp)
add_executable(07 src/progetto/VersioniIntermedie/movimentoOrange_07/pacman.cpp)
add_executable(08 src/progetto/VersioniIntermedie/raffinamentoFantasmi_08/pacman.cpp)
add_executable(09 src/progetto/VersioniIntermedie/superDots_09/pacman.cpp)
add_executable(10 src/progetto/VersioniIntermedie/siMuovonoTutti_10/pacman.cpp)
add_executable(11 src/progetto/VersioniIntermedie/animazioniPacman_11/pacman.cpp)
add_executable(12 src/progetto/VersioniIntermedie/suoni_12/pacman.cpp)
add_executable(13 src/progetto/VersioniIntermedie/punteggio_13/pacman.cpp)
add_executable(14 src/progetto/VersioniIntermedie/frutta_14/pacman.cpp)
add_executable(15 src/progetto/VersioniIntermedie/highScore_vite_15/pacman.cpp)
add_executable(16 src/progetto/VersioniIntermedie/fixTunnel_16/pacman.cpp)
add_executable(17 src/progetto/VersioniIntermedie/riordinateFunzioni_17/pacman.cpp)
add_executable(18 src/progetto/VersioniIntermedie/livelli_18/pacman.cpp)
add_executable(19 src/progetto/VersioniIntermedie/diversaFrutta_19/pacman.cpp)

target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)

