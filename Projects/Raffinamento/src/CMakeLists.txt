list(APPEND raffinamento_headers ${CMAKE_CURRENT_SOURCE_DIR}/ordinamento.hpp)

list(APPEND raffinamento_headers ${CMAKE_CURRENT_SOURCE_DIR}/classe_raffinamento.hpp)

list(APPEND raffinamento_sources ${CMAKE_CURRENT_SOURCE_DIR}/classe_raffinamento.cpp)

list(APPEND raffinamento_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(raffinamento_sources ${raffinamento_sources} PARENT_SCOPE)
set(raffinamento_headers ${raffinamento_headers} PARENT_SCOPE)
set(raffinamento_includes ${raffinamento_includes} PARENT_SCOPE)
