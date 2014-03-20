CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT( SOGLU )

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
endif(UNIX)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost REQUIRED COMPONENTS program_options filesystem system timer chrono)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} "include")

SET(${PROJECT_NAME}_HEADERS
	"include/soglu/Attributes.hpp"
	"include/soglu/Camera.hpp" 
	"include/soglu/CgFXShader.hpp"
	"include/soglu/BoundingBox.hpp" 
	"include/soglu/GLViewSetup.hpp" 
	"include/soglu/GLSLShader.hpp"
	"include/soglu/FrameBufferObject.hpp" 
	"include/soglu/ErrorHandling.hpp"
	"include/soglu/OGLTools.hpp" 
	"include/soglu/OGLDrawing.hpp"
	"include/soglu/OpenGLWrappers.hpp"
	"include/soglu/utils.hpp"
	)

SET(${PROJECT_NAME}_SOURCES 
	"src/CgFXShader.cpp" 
	"src/OGLTools.cpp" 
	"src/OGLDrawing.cpp" 
	"src/BoundingBox.cpp" 
	"src/GLViewSetup.cpp"
	"src/GLSLShader.cpp"  
	"src/FrameBufferObject.cpp"
	"src/OpenGLWrappers.cpp" 
	)

ADD_LIBRARY(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d ) 
IF(WIN32)
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES STATIC_LIBRARY_FLAGS /NODEFAULTLIB)
ENDIF(WIN32)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY lib)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY lib)
