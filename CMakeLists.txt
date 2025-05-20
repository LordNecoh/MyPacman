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


add_executable(main progetto/pacman.cpp) #Edit her to add your source files

#Older versions, remove if not interested !
add_executable(01 progetto/VersioniIntermedie/postRiordine_01/progetto/pacman.cpp)
add_executable(02 progetto/VersioniIntermedie/disegnoFantasmi_02/pacman.cpp)
add_executable(03 progetto/VersioniIntermedie/movimentoPrimoFantasma_03/pacman.cpp)
add_executable(04 progetto/VersioniIntermedie/movimentoRedSbagliato_04/pacman.cpp)
add_executable(05 progetto/VersioniIntermedie/movimentoGiustoRed_05/pacman.cpp)
add_executable(06 progetto/VersioniIntermedie/movimentoPinky_06/pacman.cpp)
add_executable(07 progetto/VersioniIntermedie/movimentoOrange_07/pacman.cpp)
add_executable(08 progetto/VersioniIntermedie/raffinamentoFantasmi_08/pacman.cpp)
add_executable(09 progetto/VersioniIntermedie/superDots_09/pacman.cpp)
add_executable(10 progetto/VersioniIntermedie/siMuovonoTutti_10/pacman.cpp)
add_executable(11 progetto/VersioniIntermedie/animazioniPacman_11/pacman.cpp)
add_executable(12 progetto/VersioniIntermedie/suoni_12/pacman.cpp)
add_executable(13 progetto/VersioniIntermedie/punteggio_13/pacman.cpp)
add_executable(14 progetto/VersioniIntermedie/frutta_14/pacman.cpp)
add_executable(15 progetto/VersioniIntermedie/highScore_vite_15/pacman.cpp)
add_executable(16 progetto/VersioniIntermedie/fixTunnel_16/pacman.cpp)
add_executable(17 progetto/VersioniIntermedie/riordinateFunzioni_17/pacman.cpp)
add_executable(18 progetto/VersioniIntermedie/livelli_18/pacman.cpp)
add_executable(19 progetto/VersioniIntermedie/diversaFrutta_19/pacman.cpp)

target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(01 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(02 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(03 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(04 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(05 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(06 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(07 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(08 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(09 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(10 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(11 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(12 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(13 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(14 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(15 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(16 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(17 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(18 PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(19 PRIVATE SFML::Graphics SFML::Audio)