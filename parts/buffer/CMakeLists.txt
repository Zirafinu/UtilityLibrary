
CreateLibrary(
	NAME RingBuffer
	IS_INTERFACE
	DEPENDS
	PUBLIC_HEADER
		ring_buffer.hpp
	)

CreateTest(
	NAME RingBuffer_Test
	SOURCES test_ring_buffer.cpp
	DEPENDS doctest::doctest ${PROJECT_NAME}::RingBuffer
	)
