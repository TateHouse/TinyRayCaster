CMAKE_MINIMUM_REQUIRED(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

SET(SOURCE_FILES
		Application.cpp
		Color.cpp
		FrameBuffer.cpp
		Image.cpp
		Map.cpp
		Player.cpp
		STBImage.cpp
		TextureAtlas.cpp)

TARGET_SOURCES(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})