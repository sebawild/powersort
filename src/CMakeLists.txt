

file(GLOB ALGOS ./sorts/*)
file(GLOB HEADERS ./*.h)
set(SOURCES ${HEADERS} ${ALGOS})

add_executable(mergesorts main.cpp ${SOURCES})
target_compile_definitions(mergesorts PRIVATE ELEM_T=int)

add_executable(mergesorts-short main.cpp ${SOURCES})
target_compile_definitions(mergesorts-short PRIVATE ELEM_T=Short)
target_compile_definitions(mergesorts-short PRIVATE EXCLUDE_POWERSORTS_WITH_SENTINEL=true)

add_executable(mergesorts-long main.cpp ${SOURCES})
target_compile_definitions(mergesorts-long PRIVATE ELEM_T=long)

add_executable(mergesorts-blob main.cpp ${SOURCES})
target_compile_definitions(mergesorts-blob PRIVATE ELEM_T=data::blob<16,long>)

add_executable(mergesorts-long+pointer main.cpp ${SOURCES})
target_compile_definitions(mergesorts-long+pointer PRIVATE ELEM_T=blob_long_and_pointer)

add_executable(mergesorts-int+3pointer main.cpp ${SOURCES})
target_compile_definitions(mergesorts-int+3pointer PRIVATE ELEM_T=data::blob<4,int,data::FIRST_ENTRY>)

add_executable(mergesorts-count-cmps main.cpp ${SOURCES})
target_compile_definitions(mergesorts-count-cmps PRIVATE ELEM_T=comp_counter)
target_compile_definitions(mergesorts-count-cmps PRIVATE COUNT_MERGECOST=true)



add_subdirectory(test)
