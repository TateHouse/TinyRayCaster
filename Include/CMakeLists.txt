CMAKE_MINIMUM_REQUIRED(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

SET(HEADER_FILES
		Application.hpp
		Color.hpp
		FrameBuffer.hpp
		Image.hpp
		Map.hpp
		STBImage.hpp
		TextureAtlas.hpp)

TARGET_SOURCES(${PROJECT_NAME} PRIVATE ${HEADER_FILES})
