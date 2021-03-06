######################################################################
# Automatically generated by qmake (3.0) Sat Sep 8 14:15:18 2018
######################################################################

TEMPLATE = app
TARGET = projects
INCLUDEPATH += . \
               ui \
               brush \
               lib \
               camera \
               glm \
               glm/detail \
               glm/gtc \
               glm/gtx \
               gl \
               scenegraph \
               gl/shaders \
               gl/datatype \
               gl/textures \
               gl/util \
               glew-1.10.0/auto/src

# Input
HEADERS += brush/Brush.h \
           brush/ConstantBrush.h \
           brush/LinearBrush.h \
           brush/QuadraticBrush.h \
           brush/SmudgeBrush.h \
           camera/Camera.h \
           camera/CamtransCamera.h \
           camera/OrbitingCamera.h \
           camera/QuaternionCamera.h \
           gl/GLDebug.h \
           glm/common.hpp \
           glm/exponential.hpp \
           glm/ext.hpp \
           glm/fwd.hpp \
           glm/geometric.hpp \
           glm/glm.hpp \
           glm/integer.hpp \
           glm/mat2x2.hpp \
           glm/mat2x3.hpp \
           glm/mat2x4.hpp \
           glm/mat3x2.hpp \
           glm/mat3x3.hpp \
           glm/mat3x4.hpp \
           glm/mat4x2.hpp \
           glm/mat4x3.hpp \
           glm/mat4x4.hpp \
           glm/matrix.hpp \
           glm/packing.hpp \
           glm/trigonometric.hpp \
           glm/vec2.hpp \
           glm/vec3.hpp \
           glm/vec4.hpp \
           glm/vector_relational.hpp \
           lib/BGRA.h \
           lib/CS123ISceneParser.h \
           lib/CS123SceneData.h \
           lib/CS123XmlSceneParser.h \
           lib/ResourceLoader.h \
           scenegraph/OpenGLScene.h \
           scenegraph/RayScene.h \
           scenegraph/Scene.h \
           scenegraph/SceneviewScene.h \
           scenegraph/ShapesScene.h \
           ui/Canvas2D.h \
           ui/Databinding.h \
           ui/mainwindow.h \
           ui/Settings.h \
           ui/SupportCanvas2D.h \
           ui/SupportCanvas3D.h \
           gl/datatype/FBO.h \
           gl/datatype/IBO.h \
           gl/datatype/VAO.h \
           gl/datatype/VBO.h \
           gl/datatype/VBOAttribMarker.h \
           gl/shaders/CS123Shader.h \
           gl/shaders/Shader.h \
           gl/shaders/ShaderAttribLocations.h \
           gl/textures/DepthBuffer.h \
           gl/textures/RenderBuffer.h \
           gl/textures/Texture.h \
           gl/textures/Texture2D.h \
           gl/textures/TextureParameters.h \
           gl/textures/TextureParametersBuilder.h \
           gl/util/FullScreenQuad.h \
           glm/detail/_features.hpp \
           glm/detail/_fixes.hpp \
           glm/detail/_literals.hpp \
           glm/detail/_noise.hpp \
           glm/detail/_swizzle.hpp \
           glm/detail/_swizzle_func.hpp \
           glm/detail/_vectorize.hpp \
           glm/detail/func_common.hpp \
           glm/detail/func_exponential.hpp \
           glm/detail/func_geometric.hpp \
           glm/detail/func_integer.hpp \
           glm/detail/func_matrix.hpp \
           glm/detail/func_noise.hpp \
           glm/detail/func_packing.hpp \
           glm/detail/func_trigonometric.hpp \
           glm/detail/func_vector_relational.hpp \
           glm/detail/hint.hpp \
           glm/detail/intrinsic_common.hpp \
           glm/detail/intrinsic_exponential.hpp \
           glm/detail/intrinsic_geometric.hpp \
           glm/detail/intrinsic_integer.hpp \
           glm/detail/intrinsic_matrix.hpp \
           glm/detail/intrinsic_trigonometric.hpp \
           glm/detail/intrinsic_vector_relational.hpp \
           glm/detail/precision.hpp \
           glm/detail/setup.hpp \
           glm/detail/type_float.hpp \
           glm/detail/type_gentype.hpp \
           glm/detail/type_half.hpp \
           glm/detail/type_int.hpp \
           glm/detail/type_mat.hpp \
           glm/detail/type_mat2x2.hpp \
           glm/detail/type_mat2x3.hpp \
           glm/detail/type_mat2x4.hpp \
           glm/detail/type_mat3x2.hpp \
           glm/detail/type_mat3x3.hpp \
           glm/detail/type_mat3x4.hpp \
           glm/detail/type_mat4x2.hpp \
           glm/detail/type_mat4x3.hpp \
           glm/detail/type_mat4x4.hpp \
           glm/detail/type_vec.hpp \
           glm/detail/type_vec1.hpp \
           glm/detail/type_vec2.hpp \
           glm/detail/type_vec3.hpp \
           glm/detail/type_vec4.hpp \
           glm/gtc/constants.hpp \
           glm/gtc/epsilon.hpp \
           glm/gtc/matrix_access.hpp \
           glm/gtc/matrix_integer.hpp \
           glm/gtc/matrix_inverse.hpp \
           glm/gtc/matrix_transform.hpp \
           glm/gtc/noise.hpp \
           glm/gtc/packing.hpp \
           glm/gtc/quaternion.hpp \
           glm/gtc/random.hpp \
           glm/gtc/reciprocal.hpp \
           glm/gtc/type_precision.hpp \
           glm/gtc/type_ptr.hpp \
           glm/gtc/ulp.hpp \
           glm/gtx/associated_min_max.hpp \
           glm/gtx/bit.hpp \
           glm/gtx/closest_point.hpp \
           glm/gtx/color_space.hpp \
           glm/gtx/color_space_YCoCg.hpp \
           glm/gtx/compatibility.hpp \
           glm/gtx/component_wise.hpp \
           glm/gtx/constants.hpp \
           glm/gtx/dual_quaternion.hpp \
           glm/gtx/epsilon.hpp \
           glm/gtx/euler_angles.hpp \
           glm/gtx/extend.hpp \
           glm/gtx/extented_min_max.hpp \
           glm/gtx/fast_exponential.hpp \
           glm/gtx/fast_square_root.hpp \
           glm/gtx/fast_trigonometry.hpp \
           glm/gtx/gradient_paint.hpp \
           glm/gtx/handed_coordinate_space.hpp \
           glm/gtx/inertia.hpp \
           glm/gtx/int_10_10_10_2.hpp \
           glm/gtx/integer.hpp \
           glm/gtx/intersect.hpp \
           glm/gtx/io.hpp \
           glm/gtx/log_base.hpp \
           glm/gtx/matrix_cross_product.hpp \
           glm/gtx/matrix_interpolation.hpp \
           glm/gtx/matrix_major_storage.hpp \
           glm/gtx/matrix_operation.hpp \
           glm/gtx/matrix_query.hpp \
           glm/gtx/mixed_product.hpp \
           glm/gtx/multiple.hpp \
           glm/gtx/noise.hpp \
           glm/gtx/norm.hpp \
           glm/gtx/normal.hpp \
           glm/gtx/normalize_dot.hpp \
           glm/gtx/number_precision.hpp \
           glm/gtx/optimum_pow.hpp \
           glm/gtx/orthonormalize.hpp \
           glm/gtx/perpendicular.hpp \
           glm/gtx/polar_coordinates.hpp \
           glm/gtx/projection.hpp \
           glm/gtx/quaternion.hpp \
           glm/gtx/random.hpp \
           glm/gtx/raw_data.hpp \
           glm/gtx/reciprocal.hpp \
           glm/gtx/rotate_normalized_axis.hpp \
           glm/gtx/rotate_vector.hpp \
           glm/gtx/scalar_relational.hpp \
           glm/gtx/simd_mat4.hpp \
           glm/gtx/simd_quat.hpp \
           glm/gtx/simd_vec4.hpp \
           glm/gtx/spline.hpp \
           glm/gtx/std_based_type.hpp \
           glm/gtx/string_cast.hpp \
           glm/gtx/transform.hpp \
           glm/gtx/transform2.hpp \
           glm/gtx/ulp.hpp \
           glm/gtx/unsigned_int.hpp \
           glm/gtx/vec1.hpp \
           glm/gtx/vector_angle.hpp \
           glm/gtx/vector_query.hpp \
           glm/gtx/wrap.hpp \
           glm/virtrev/xstream.hpp \
           glew-1.10.0/auto/src/glew_head.h \
           glew-1.10.0/auto/src/glew_license.h \
           glew-1.10.0/auto/src/glew_tail.h \
           glew-1.10.0/auto/src/glew_utils.h \
           glew-1.10.0/auto/src/glxew_head.h \
           glew-1.10.0/auto/src/glxew_mid.h \
           glew-1.10.0/auto/src/glxew_tail.h \
           glew-1.10.0/auto/src/khronos_license.h \
           glew-1.10.0/auto/src/mesa_license.h \
           glew-1.10.0/auto/src/wglew_head.h \
           glew-1.10.0/auto/src/wglew_mid.h \
           glew-1.10.0/auto/src/wglew_tail.h \
           glew-1.10.0/include/GL/glew.h \
           glew-1.10.0/include/GL/glxew.h \
           glew-1.10.0/include/GL/wglew.h \
           glm/detail/type_vec2.inl \
           glm/detail/type_vec3.inl \
           glm/detail/type_vec4.inl \
           glm/detail/type_mat2x2.inl \
           glm/detail/type_mat2x3.inl \
           glm/detail/type_mat2x4.inl \
           glm/detail/type_mat3x2.inl \
           glm/detail/type_mat3x3.inl \
           glm/detail/type_mat3x4.inl \
           glm/detail/type_mat4x2.inl \
           glm/detail/type_mat4x3.inl \
           glm/detail/type_mat4x4.inl \
           glm/detail/func_trigonometric.inl \
           glm/detail/type_vec1.inl \
           glm/detail/func_exponential.inl \
           glm/detail/func_vector_relational.inl \
           glm/detail/func_common.inl \
           glm/detail/func_packing.inl \
           glm/detail/type_half.inl \
           glm/detail/func_geometric.inl \
           glm/detail/func_matrix.inl \
           glm/detail/func_integer.inl \
           glm/gtc/matrix_transform.inl \
           glm/gtx/transform.inl \
           glm/gtc/constants.inl \
           glm/gtc/quaternion.inl \
           glm/gtx/norm.inl \
           glm/gtc/type_ptr.inl \
           glm/gtx/dual_quaternion.inl
FORMS += ui/mainwindow.ui
SOURCES += main.cpp \
           brush/Brush.cpp \
           brush/ConstantBrush.cpp \
           brush/LinearBrush.cpp \
           brush/QuadraticBrush.cpp \
           brush/SmudgeBrush.cpp \
           camera/CamtransCamera.cpp \
           camera/OrbitingCamera.cpp \
           camera/QuaternionCamera.cpp \
           gl/GLDebug.cpp \
           lib/BGRA.cpp \
           lib/CS123XmlSceneParser.cpp \
           lib/ResourceLoader.cpp \
           scenegraph/OpenGLScene.cpp \
           scenegraph/RayScene.cpp \
           scenegraph/Scene.cpp \
           scenegraph/SceneviewScene.cpp \
           scenegraph/ShapesScene.cpp \
           ui/Canvas2D.cpp \
           ui/Databinding.cpp \
           ui/mainwindow.cpp \
           ui/Settings.cpp \
           ui/SupportCanvas2D.cpp \
           ui/SupportCanvas3D.cpp \
           gl/datatype/FBO.cpp \
           gl/datatype/IBO.cpp \
           gl/datatype/VAO.cpp \
           gl/datatype/VBO.cpp \
           gl/datatype/VBOAttribMarker.cpp \
           gl/shaders/CS123Shader.cpp \
           gl/shaders/Shader.cpp \
           gl/textures/DepthBuffer.cpp \
           gl/textures/RenderBuffer.cpp \
           gl/textures/Texture.cpp \
           gl/textures/Texture2D.cpp \
           gl/textures/TextureParameters.cpp \
           gl/textures/TextureParametersBuilder.cpp \
           gl/util/FullScreenQuad.cpp \
           glew-1.10.0/src/glew.c \
           glew-1.10.0/src/glewinfo.c \
           glew-1.10.0/src/visualinfo.c \
           glm/detail/dummy.cpp \
           glm/detail/glm.cpp \
           glew-1.10.0/auto/src/glew_head.c \
           glew-1.10.0/auto/src/glew_init_gl.c \
           glew-1.10.0/auto/src/glew_init_glx.c \
           glew-1.10.0/auto/src/glew_init_tail.c \
           glew-1.10.0/auto/src/glew_init_wgl.c \
           glew-1.10.0/auto/src/glew_str_glx.c \
           glew-1.10.0/auto/src/glew_str_head.c \
           glew-1.10.0/auto/src/glew_str_tail.c \
           glew-1.10.0/auto/src/glew_str_wgl.c \
           glew-1.10.0/auto/src/glew_utils.c \
           glew-1.10.0/auto/src/glewinfo_gl.c \
           glew-1.10.0/auto/src/glewinfo_glx.c \
           glew-1.10.0/auto/src/glewinfo_head.c \
           glew-1.10.0/auto/src/glewinfo_tail.c \
           glew-1.10.0/auto/src/glewinfo_wgl.c
RESOURCES += resources.qrc
