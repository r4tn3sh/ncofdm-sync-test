########################################################################
# Src CMakeLists.txt
#
#
########################################################################



########################################################################
# List Header .h & Source .cpp Files
########################################################################

# Not necessary to list headers, but helps QtCreator load them properly in sources window
# Also doesn't hurt anything so why not
list(APPEND headers
    ul_tx.h
)

list(APPEND sources 

    ul_tx.cpp
)


########################################################################
# Build Library wl_sync_test from sources
########################################################################
 add_library(wl_sync_test SHARED ${headers} ${sources})

########################################################################
# Link external dependencies:
#	uhd
#	fftw3
#	boost
#	pthread (this one is a bit strange)
########################################################################
# target_link_libraries(wl_sync_test uhd fftw3 pthread)  # Equivalent to below
 target_link_libraries(wl_sync_test ${UHD_LIBRARIES} ${FFTW3_LIBRARIES} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}) #equivalent to line above but better style?

########################################################################
# Install Rules
########################################################################

#Install Library (defaults to /usr/local/lib on linux)
install(TARGETS wl_sync_test
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION wl_sync_test)

#Install Header files to system
install(FILES ${headers}
        DESTINATION include/wl_sync_test)
