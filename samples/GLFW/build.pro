TEMPLATE = app

INCLUDEPATH += \
    ../../tgl \
    /usr/include/FTGL \
    /usr/include/freetype2 \
    /usr/include/eigen3

QMAKE_CXX = ccache g++
#QMAKE_CXX = g++
QMAKE_CXXFLAGS += -std=c++0x -fopenmp -march=native -mtune=native -DEIGEN_NO_DEBUG

OBJECTS_DIR += tmp

# tgl	
TGL_LIB = ../../tgl

HEADERS += \
    $${TGL_LIB}/tglDriver/GLFWGraphicsDriver.h
    
SOURCES += \
    $${TGL_LIB}/tglDriver/GLFWGraphicsDriver.cpp \
    main.cpp
    
# tglCore
HEADERS += \
    $${TGL_LIB}/tglCore/GraphicsView.h \
    $${TGL_LIB}/tglCore/Camera.h \
    $${TGL_LIB}/tglCore/Common.h \
    $${TGL_LIB}/tglCore/GraphicsItem.h \
    $${TGL_LIB}/tglCore/GraphicsItemEvent.h \
    $${TGL_LIB}/tglCore/InputEvent.h \
    $${TGL_LIB}/tglCore/Light.h \
    $${TGL_LIB}/tglCore/Renderer2D.h \
    $${TGL_LIB}/tglCore/Renderer3D.h \
    $${TGL_LIB}/tglCore/TextRenderer.h \
    $${TGL_LIB}/tglCore/SphericalCamera.h \
    $${TGL_LIB}/tglCore/StandardCamera.h \
    $${TGL_LIB}/tglCore/GraphicsDriver.h
    
SOURCES += \
    $${TGL_LIB}/tglCore/GraphicsView.cpp \
    $${TGL_LIB}/tglCore/Camera.cpp \
    $${TGL_LIB}/tglCore/GraphicsItem.cpp \
    $${TGL_LIB}/tglCore/GraphicsItemEvent.cpp \
    $${TGL_LIB}/tglCore/InputEvent.cpp \
    $${TGL_LIB}/tglCore/Light.cpp \
    $${TGL_LIB}/tglCore/Renderer2D.cpp \
    $${TGL_LIB}/tglCore/Renderer3D.cpp \
    $${TGL_LIB}/tglCore/TextRenderer.cpp \
    $${TGL_LIB}/tglCore/SphericalCamera.cpp \
    $${TGL_LIB}/tglCore/StandardCamera.cpp \
    $${TGL_LIB}/tglCore/GraphicsDriver.cpp
    
# tglGUI
HEADERS += \
    $${TGL_LIB}/tglGUI/AbstractButton.h \
    $${TGL_LIB}/tglGUI/ButtonPanel.h \
    $${TGL_LIB}/tglGUI/DockWidget.h \
    $${TGL_LIB}/tglGUI/PushButton.h \
    $${TGL_LIB}/tglGUI/Widget.h
    
SOURCES += \
    $${TGL_LIB}/tglGUI/AbstractButton.cpp \
    $${TGL_LIB}/tglGUI/ButtonPanel.cpp \
    $${TGL_LIB}/tglGUI/DockWidget.cpp \
    $${TGL_LIB}/tglGUI/PushButton.cpp \
    $${TGL_LIB}/tglGUI/Widget.cpp
    
# tglUtil
HEADERS += \
    $${TGL_LIB}/tglUtil/SE3.h \
    $${TGL_LIB}/tglUtil/EigenUtil.h \
    $${TGL_LIB}/tglUtil/Intersection.h
    
SOURCES += \
    $${TGL_LIB}/tglUtil/EigenUtil.cpp \
    $${TGL_LIB}/tglUtil/Intersection.cpp
    
# tglHandle
HEADERS += \
    $${TGL_LIB}/tglHandle/RotateHandle.h \
    $${TGL_LIB}/tglHandle/TranslateHandle.h
    
SOURCES += \
    $${TGL_LIB}/tglHandle/RotateHandle.cpp \
    $${TGL_LIB}/tglHandle/TranslateHandle.cpp
    
LIBS += \
    -lboost_filesystem \
    -lboost_system \
    -lboost_signals \ 
    -lboost_thread \
    -lboost_program_options \
    -lboost_date_time \
    -lGL \
    -lGLU \
    -lglfw \
    -lftgl
    
TARGET = main