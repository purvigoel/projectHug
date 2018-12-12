# -------------------------------------------------
# Project created by QtCreator 2010-08-22T14:12:19
# -------------------------------------------------
QT += opengl xml
TARGET = CS123
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14
CONFIG += c++14

unix:!macx {
    LIBS += -lGLU
}
macx {
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
    CONFIG += c++11
}
win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglu32
}

SOURCES += \
    brush/Brush.cpp \
    brush/ConstantBrush.cpp \
    brush/LinearBrush.cpp \
    brush/QuadraticBrush.cpp \
    brush/SmudgeBrush.cpp \
    camera/OrbitingCamera.cpp \
    camera/CamtransCamera.cpp \
    camera/QuaternionCamera.cpp \
    scenegraph/Scene.cpp \
    scenegraph/OpenGLScene.cpp \
    scenegraph/ShapesScene.cpp \
    scenegraph/SceneviewScene.cpp \
    scenegraph/RayScene.cpp \
    ui/Canvas2D.cpp \
    ui/SupportCanvas2D.cpp \
    ui/SupportCanvas3D.cpp \
    ui/Settings.cpp \
    ui/mainwindow.cpp \
    ui/Databinding.cpp \
    lib/BGRA.cpp \
    lib/CS123XmlSceneParser.cpp \
    lib/ResourceLoader.cpp \
    gl/shaders/Shader.cpp \
    gl/GLDebug.cpp \
    gl/datatype/VBOAttribMarker.cpp \
    gl/datatype/VBO.cpp \
    gl/datatype/IBO.cpp \
    gl/datatype/VAO.cpp \
    gl/datatype/FBO.cpp \
    gl/textures/Texture.cpp \
    gl/textures/Texture2D.cpp \
    gl/textures/TextureParameters.cpp \
    gl/textures/TextureParametersBuilder.cpp \
    gl/textures/RenderBuffer.cpp \
    gl/textures/DepthBuffer.cpp \
    gl/shaders/CS123Shader.cpp \
    gl/util/FullScreenQuad.cpp \
    main.cpp \
    glew-1.10.0/src/glew.c \
    brush/SpecialBrush.cpp \
    shape/openglshape.cpp \
    shape/glwidget.cpp \
    shape/cube.cpp \
    shape/sphere.cpp \
    shape/cone.cpp \
    shape/cylinder.cpp \
    shape/barrel.cpp \
    shape/cylinderbarrel.cpp \
    shape/spherebarrel.cpp \
    shape/cap.cpp \
    shape/cubebarrel.cpp \
    shape/cubecap.cpp \
    shape/torusbarrel.cpp \
    shape/torus.cpp \
    shape/spherecap.cpp \
    shape/bottlebarrel.cpp \
    shape/bottle.cpp \
    filter/filter.cpp \
    filter/filteraverage.cpp \
    filter/filteredge.cpp \
    filter/filtermedian.cpp \
    filter/filterscale.cpp \
    scenegraph/primitiveinfo.cpp \
    shape/implicitcone.cpp \
    shape/implicitcube.cpp \
    shape/implicitcylinder.cpp \
    shape/implicitshape.cpp \
    shape/implicitsphere.cpp \
    shape/quad.cpp

HEADERS += \
    brush/Brush.h \
    brush/ConstantBrush.h \
    brush/LinearBrush.h \
    brush/QuadraticBrush.h \
    brush/SmudgeBrush.h \
    camera/Camera.h \
    camera/OrbitingCamera.h \
    camera/CamtransCamera.h \
    camera/QuaternionCamera.h \
    scenegraph/Scene.h \
    scenegraph/OpenGLScene.h \
    scenegraph/ShapesScene.h \
    scenegraph/SceneviewScene.h \
    scenegraph/RayScene.h \
    ui/Canvas2D.h \
    ui/SupportCanvas2D.h \
    ui/SupportCanvas3D.h \
    ui/Settings.h \
    ui/mainwindow.h \
    ui/Databinding.h \
    ui/mainwindow.h \
    gl/shaders/Shader.h \
    gl/GLDebug.h \
    gl/shaders/ShaderAttribLocations.h \
    gl/datatype/VBOAttribMarker.h \
    gl/datatype/VBO.h \
    gl/datatype/IBO.h \
    gl/datatype/VAO.h \
    gl/datatype/FBO.h \
    gl/textures/Texture.h \
    gl/textures/Texture2D.h \
    gl/textures/TextureParameters.h \
    gl/textures/TextureParametersBuilder.h \
    gl/textures/RenderBuffer.h \
    gl/textures/DepthBuffer.h \
    gl/shaders/CS123Shader.h \
    gl/util/FullScreenQuad.h \
    lib/BGRA.h \
    lib/CS123XmlSceneParser.h \
    lib/CS123SceneData.h \
    lib/CS123ISceneParser.h \
    lib/ResourceLoader.h \
    glew-1.10.0/include/GL/glew.h \
    brush/SpecialBrush.h \
    shape/openglshape.h \
    shape/glwidget.h \
    shape/cube.h \
    scenegraph/cone.h \
    scenegraph/cylinder.h \
    shape/sphere.h \
    shape/cylinder.h \
    shape/cone.h \
    shape/barrel.h \
    shape/cylinderbarrel.h \
    shape/spherebarrel.h \
    shape/cap.h \
    shape/cubebarrel.h \
    shape/cubecap.h \
    shape/torusbarrel.h \
    shape/torus.h \
    shape/spherecap.h \
    shape/bottlebarrel.h \
    shape/bottle.h \
    filter/filter.h \
    filter/filteraverage.h \
    filter/filteredge.h \
    filter/filtermedian.h \
    filter/filterscale.h \
    scenegraph/primitiveinfo.h \
    shape/implicitcone.h \
    shape/implicitcube.h \
    shape/implicitcylinder.h \
    shape/implicitshape.h \
    shape/implicitsphere.h \
    shape/quad.h

FORMS += ui/mainwindow.ui
INCLUDEPATH += glm brush camera lib scenegraph ui glew-1.10.0/include
DEPENDPATH += glm brush camera lib scenegraph ui glew-1.10.0/include
DEFINES += _USE_MATH_DEFINES
DEFINES += TIXML_USE_STL
DEFINES += GLM_SWIZZLE GLM_FORCE_RADIANS
OTHER_FILES += shaders/shader.frag \
    shaders/shader.vert \
    shaders/wireframe/wireframe.vert \
    shaders/wireframe/wireframe.frag \
    shaders/normals/normals.vert \
    shaders/normals/normals.gsh \
    shaders/normals/normals.frag \
    shaders/normals/normalsArrow.vert \
    shaders/normals/normalsArrow.gsh \
    shaders/normals/normalsArrow.frag \
    shaders/deferredlighting/gbuffer/gbuffer.frag \
    shaders/deferredlighting/gbuffer/gbuffer.vert \
    shaders/deferredlighting/lighting/lighting.frag \
    shaders/deferredlighting/lighting/lighting.vert \
    shaders/deferredlighting/compositing/compositing.frag \
    shaders/deferredlighting/compositing/compositing.vert \
    shaders/fullscreenquad/fullscreenquad.frag \
    shaders/fullscreenquad/fullscreenquad.vert \
    shaders/horizontalBlur/horizontalBlur.frag \
    shaders/horizontalBlur/quad.vert

# Don't add the -pg flag unless you know what you are doing. It makes QThreadPool freeze on Mac OS X
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON += -Waddress -Warray-bounds -Wc++0x-compat -Wchar-subscripts -Wformat\
                          -Wmain -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type \
                          -Wsequence-point -Wsign-compare -Wstrict-overflow=1 -Wswitch \
                          -Wtrigraphs -Wuninitialized -Wunused-label -Wunused-variable \
                          -Wvolatile-register-var -Wno-extra

QMAKE_CXXFLAGS += -g

# QMAKE_CXX_FLAGS_WARN_ON += -Wunknown-pragmas -Wunused-function -Wmain

macx {
    QMAKE_CXXFLAGS_WARN_ON -= -Warray-bounds -Wc++0x-compat
}

RESOURCES += \
    resources.qrc

DISTFILES += \
    shaders/normals/normals.vert \
    shaders/normals/normals.frag \
    shaders/normals/normals.gsh \
    shaders/normals/normalsArrow.gsh \
    shaders/normals/normalsArrow.frag \
    shaders/normals/normalsArrow.vert