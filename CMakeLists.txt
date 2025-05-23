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


add_executable(main progetto/Tappa20_Ultima/pacman.cpp)

#Older versions, remove if not interested !
add_executable(01 progetto/Tappa01_PostRiordine/progetto/pacman.cpp)
add_executable(02 progetto/Tappa02_DisegnoFantasmi/pacman.cpp)
add_executable(03 progetto/Tappa03_movimentoPrimoFantasma/pacman.cpp)
add_executable(04 progetto/Tappa04_movimentoRedSbagliato/pacman.cpp)
add_executable(05 progetto/Tappa05_movimentoGiustoRed/pacman.cpp)
add_executable(06 progetto/Tappa06_movimentoPinky/pacman.cpp)
add_executable(07 progetto/Tappa07_movimentoOrange/pacman.cpp)
add_executable(08 progetto/Tappa08_raffinamentoFantasmi/pacman.cpp)
add_executable(09 progetto/Tappa09_superDots/pacman.cpp)
add_executable(10 progetto/Tappa10_siMuovonoTutti/pacman.cpp)
add_executable(11 progetto/Tappa11_animazioniPacman/pacman.cpp)
add_executable(12 progetto/Tappa12_suoni/pacman.cpp)
add_executable(13 progetto/Tappa13_punteggio/pacman.cpp)
add_executable(14 progetto/Tappa14_frutta/pacman.cpp)
add_executable(15 progetto/Tappa15_highScore_vite/pacman.cpp)
add_executable(16 progetto/Tappa16_fixTunnel/pacman.cpp)
add_executable(17 progetto/Tappa17_riordinateFunzioni/pacman.cpp)
add_executable(18 progetto/Tappa18_livelli/pacman.cpp)
add_executable(19 progetto/Tappa19_diversaFrutta/pacman.cpp)

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