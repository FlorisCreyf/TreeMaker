TEMPLATE = app
# To generate a VS project file: Extensions -> Qt VS Tools -> Open .pro file
# Visual studio might not generate object files in sub-directories.
# Project settings -> C/C++ -> Output files -> Object file name: $(IntDir)%(RelativeDir)
CONFIG += qt object_parallel_to_source c++11 strict_c++ no_batch warn_on
unix::CONFIG += precompile_header
# win32::CONFIG += console
win32::DEFINES += PG_SERIALIZE GL_GLEXT_PROTOTYPES
unix::QMAKE_LFLAGS += -no-pie
TARGET = plant
QT = core gui opengl xml openglextensions
unix::LIBS += -lboost_serialization
# Change the path name to reflect the Boost version.
# To install Boost: .\bootstrap.bat; .\b2.exe;
win32::LIBS += "C:\Program Files\boost\boost_1_75_0\stage\lib\libboost_serialization-vc142-mt-x64-1_75.lib"
win32::LIBS += opengl32.lib
win32::INCLUDEPATH += "C:\Program Files\boost\boost_1_75_0"
win32::DEPENDPATH += "C:\Program Files\boost\boost_1_75_0"
FORMS += editor/qt/window.ui
CONFIG(release, debug|release) {
	OBJECTS_DIR = build/release
}
CONFIG(debug, debug|release) {
	OBJECTS_DIR = build/debug
}
MOC_DIR = editor/qt
RCC_DIR = editor/qt
UI_DIR = editor/qt

unix::PRECOMPILED_HEADER = pch.h

SOURCES += \
plant_generator/file/collada.cpp \
plant_generator/file/wavefront.cpp \
plant_generator/file/xml_writer.cpp \
plant_generator/math/curve.cpp \
plant_generator/math/intersection.cpp \
plant_generator/math/mat4.cpp \
plant_generator/math/quat.cpp \
plant_generator/math/vec2.cpp \
plant_generator/math/vec3.cpp \
plant_generator/math/vec4.cpp \
plant_generator/animation.cpp \
plant_generator/cross_section.cpp \
plant_generator/curve.cpp \
plant_generator/generator.cpp \
plant_generator/geometry.cpp \
plant_generator/joint.cpp \
plant_generator/leaf.cpp \
plant_generator/material.cpp \
plant_generator/mesh.cpp \
plant_generator/parameter_tree.cpp \
plant_generator/path.cpp \
plant_generator/plant.cpp \
plant_generator/pattern_generator.cpp \
plant_generator/scene.cpp \
plant_generator/spline.cpp \
plant_generator/stem.cpp \
plant_generator/stem_pool.cpp \
plant_generator/volume.cpp \
plant_generator/wind.cpp \
editor/commands/add_stem.cpp \
editor/commands/add_leaf.cpp \
editor/commands/command.cpp \
editor/commands/extrude_spline.cpp \
editor/commands/extrude_stem.cpp \
editor/commands/generate.cpp \
editor/commands/move_spline.cpp \
editor/commands/move_stem.cpp \
editor/commands/move_path.cpp \
editor/commands/remove_spline.cpp \
editor/commands/remove_stem.cpp \
editor/commands/rotate_stem.cpp \
editor/commands/save_point_selection.cpp \
editor/commands/save_curve.cpp \
editor/commands/save_selection.cpp \
editor/commands/save_stem.cpp \
editor/geometry/axes.cpp \
editor/geometry/geometry.cpp \
editor/geometry/path.cpp \
editor/geometry/translation_axes.cpp \
editor/geometry/rotation_axes.cpp \
editor/graphics/storage_buffer.cpp \
editor/graphics/vertex_buffer.cpp \
editor/graphics/shader_params.cpp \
editor/graphics/shared_resources.cpp \
editor/widgets/curve_editor.cpp \
editor/widgets/curve_viewer.cpp \
editor/widgets/editor.cpp \
editor/widgets/form.cpp \
editor/widgets/generator_editor.cpp \
editor/widgets/key_editor.cpp \
editor/widgets/material_viewer.cpp \
editor/widgets/mesh_viewer.cpp \
editor/widgets/pattern_editor.cpp \
editor/widgets/property_editor.cpp \
editor/widgets/widgets.cpp \
editor/widgets/window.cpp \
editor/camera.cpp \
editor/history.cpp \
editor/keymap.cpp \
editor/main.cpp \
editor/point_selection.cpp \
editor/selection.cpp \
editor/selector.cpp

unix::HEADERS += pch.h
HEADERS += \
plant_generator/file/collada.h \
plant_generator/file/wavefront.h \
plant_generator/file/xml_writer.h \
plant_generator/math/curve.h \
plant_generator/math/intersection.h \
plant_generator/math/mat4.h \
plant_generator/math/quat.h \
plant_generator/math/vec2.h \
plant_generator/math/vec3.h \
plant_generator/math/vec4.h \
plant_generator/animation.h \
plant_generator/cross_section.h \
plant_generator/curve.h \
plant_generator/generator.h \
plant_generator/geometry.h \
plant_generator/joint.h \
plant_generator/leaf.h \
plant_generator/material.h \
plant_generator/mesh.h \
plant_generator/parameter_tree.h \
plant_generator/path.h \
plant_generator/plant.h \
plant_generator/pattern_generator.h \
plant_generator/scene.h \
plant_generator/spline.h \
plant_generator/stem.h \
plant_generator/stem_pool.h \
plant_generator/volume.h \
plant_generator/wind.h \
editor/commands/add_stem.h \
editor/commands/add_leaf.h \
editor/commands/command.h \
editor/commands/extrude_spline.h \
editor/commands/extrude_stem.h \
editor/commands/generate.h \
editor/commands/move_spline.h \
editor/commands/move_stem.h \
editor/commands/move_path.h \
editor/commands/remove_spline.h \
editor/commands/remove_stem.h \
editor/commands/rotate_stem.h \
editor/commands/save_curve.h \
editor/commands/save_point_selection.h \
editor/commands/save_selection.h \
editor/commands/save_stem.h \
editor/geometry/axes.h \
editor/geometry/geometry.h \
editor/geometry/path.h \
editor/geometry/translation_axes.h \
editor/geometry/rotation_axes.h \
editor/graphics/storage_buffer.h \
editor/graphics/vertex_buffer.h \
editor/graphics/shader_params.h \
editor/graphics/shared_resources.h \
editor/widgets/curve_editor.h \
editor/widgets/curve_viewer.h \
editor/widgets/editor.h \
editor/widgets/form.h \
editor/widgets/generator_editor.h \
editor/widgets/widgets.h \
editor/widgets/key_editor.h \
editor/widgets/material_viewer.h \
editor/widgets/mesh_viewer.h \
editor/widgets/pattern_editor.h \
editor/widgets/property_editor.h \
editor/widgets/window.h \
editor/camera.h \
editor/history.h \
editor/keymap.h \
editor/point_selection.h \
editor/selection.h \
editor/selector.h
