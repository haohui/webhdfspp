add_executable(cat cat.cc)
target_link_libraries(cat webhdfspp ${CURL_LIBRARIES})