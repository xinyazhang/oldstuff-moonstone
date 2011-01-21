#include <QApplication>
#include "mainwindow.h"

#include "../dal/sqlitemeta.h"
#include "../kernel/metadb.h"

void register_database_types()
{
	metadb_t::registermeta(sqlitemeta::instance());
}

int main(int argc, char* argv[])
{
	QApplication qapp(argc, argv);
	register_database_types();
	MainWindow win;
	win.show();
	qapp.exec();
	return 0;
}
