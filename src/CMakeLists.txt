
set(CPLEXDIR /opt/ibm/ILOG/CPLEX_Studio128/cplex)
set(CONCERTDIR /opt/ibm/ILOG/CPLEX_Studio128/concert)

set(CPLEXLIBDIR ${CPLEXDIR}/lib/x86-64_linux/static_pic)
set(CONCERTLIBDIR ${CONCERTDIR}/lib/x86-64_linux/static_pic)

add_library(cplex STATIC IMPORTED)
set_property(TARGET cplex PROPERTY IMPORTED_LOCATION ${CPLEXLIBDIR}/libcplex.a)
target_include_directories(cplex INTERFACE ${CPLEXDIR}/include)

add_library(ilocplex STATIC IMPORTED)
set_property(TARGET ilocplex PROPERTY IMPORTED_LOCATION ${CPLEXLIBDIR}/libilocplex.a)

add_library(concert STATIC IMPORTED)
set_property(TARGET concert PROPERTY IMPORTED_LOCATION ${CONCERTLIBDIR}/libconcert.a)
target_include_directories(concert INTERFACE ${CONCERTDIR}/include)

add_subdirectory(agp)

add_subdirectory(client)

add_subdirectory(experiments)
