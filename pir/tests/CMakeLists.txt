add_definitions(${CHECK_DEFINITIONS})
include_directories(${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})

add_definitions("-DSOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\"")
foreach(target IN ITEMS shamir paillier pir bigint task3_custom)
  add_executable("test_${target}" "${target}.cpp")

  target_link_libraries("test_${target}" pir_primitives ${CHECK_LIBRARIES})
  
  if(PROTOCOLS)
    target_link_libraries("test_${target}" ${CMAKE_CURRENT_SOURCE_DIR}/../lib/libbigint.so)
  endif()

  add_test(NAME ${target}
    COMMAND $<TARGET_FILE:test_${target}> "${CMAKE_CURRENT_BINARY_DIR}/${target}.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

  add_custom_target("valgrind_test_${target}_xml"
    env CK_FORK=no valgrind ${VALGRIND_ARGUMENTS} --xml=yes --xml-file=${CMAKE_CURRENT_BINARY_DIR}/valgrind-${target}.xml
    $<TARGET_FILE:test_${target}>
    DEPENDS test_${target}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

  add_custom_target("valgrind_test_${target}"
    env CK_FORK=no valgrind ${VALGRIND_ARGUMENTS}
    $<TARGET_FILE:test_${target}>
    DEPENDS test_${target}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endforeach(target)
