########################################################################
# Examples CMakeLists.txt
#
#
########################################################################

########################################################################
# Include directories so cmake knows where .h files live
########################################################################
include_directories (${PROJECT_SOURCE_DIR}/src)

########################################################################
# Example Sources
########################################################################
list(APPEND test_tx_srcs
	test_tx.cpp
)
list(APPEND test_rx_srcs
	test_rx.cpp
)
list(APPEND test_rxtx_srcs
	test_rxtx.cpp
)


########################################################################
# Create executables
########################################################################
add_executable(test_tx ${test_tx_srcs})
add_executable(test_rx ${test_rx_srcs})
add_executable(test_rxtx ${test_rxtx_srcs})


########################################################################
# Link executables to wl_sync_test library
########################################################################
target_link_libraries(test_tx wl_sync_test)
target_link_libraries(test_rx wl_sync_test)
target_link_libraries(test_rxtx wl_sync_test)
