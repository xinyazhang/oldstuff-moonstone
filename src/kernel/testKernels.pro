######################################################################
# Automatically generated by qmake (2.01a) ?? ?? 26 21:14:36 2010
######################################################################

TEMPLATE = app
TARGET = testKernels
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += D:/Res/boost_1_42_0/

# Input
HEADERS += common.h \
           common_declare.h \
           Database.h \
           error_handle.h \
           Master.h \
           metadb.h \
           sql_stmt.h \
           tag.h \
           taglist.h \
           TagMan.h \
           tnode.h \
           TnodeMan.h \
           ../pal/stdtype.h \
           ../pal/int_type.h \
           ../pal/unistr.h \
           ../pal/unicode.h \
           ../dal/DatabaseInterface.h \
           ../dal/dbmeta.h \
           ../dal/sql_stmt_interface.h \
		   testKernels.h
SOURCES += testKernels.cpp

QT += testlib
LIB += debug/kernel.lib
DEPENDPATH += D:/Res/boost_1_42_0/stage/lib/
