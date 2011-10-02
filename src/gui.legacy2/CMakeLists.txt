SET(QT_REQUIRED TRUE)
FIND_PACKAGE(Qt4 REQUIRED)

AUX_SOURCE_DIRECTORY( . GUI_SRC )
QT4_WRAP_UI(UI_H lain.ui DatabaseSessionLayout.ui Explorer.ui
	dock_file_explorer.ui dock_tag_editor.ui general_explorer.ui tage.ui)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

QT4_WRAP_CPP(MOC_SRC LainMain.h KBModel.h DatabaseSession.h GeneralExplorer.h
	conformwidget.h tag_dock.h)

ADD_EXECUTABLE(lain WIN32 ${UI_H} ${GUI_SRC} ${MOC_SRC})
TARGET_LINK_LIBRARIES(lain dal kernel QtCore4 QtGui4)
