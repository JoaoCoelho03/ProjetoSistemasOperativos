cmake_minimum_required(VERSION 3.31)
project(projeto_so C)

set(CMAKE_C_STANDARD 11)
#pipes
add_executable(projeto_so
        main.c
        utils.c
        process_sensor.c
)

#sockets
add_executable(projeto_so_sockets
        main_sockets.c
        process_sensor_sockets.c
        utils.c)

#threads
add_executable(projeto_so_threads
        main_threads.c
        process_sensor_threads.c)

#sem
add_executable(projeto_so_prodcons
        main_prodcons.c
        process_sensor_prodcons.c
        utils.c)

# Link com a biblioteca pthread para semáforos
target_link_libraries(projeto_so_prodcons pthread)