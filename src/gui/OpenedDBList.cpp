#include "OpenedDBList.h"
#include "sessionwindow.h"
#include "../dal/dbmeta.h"
#include "../dal/DatabaseInterface.h"

OpenedDBList::OpenedDBList(QWidget* parent)
	:QTabWidget(parent)
{
}

void OpenedDBList::opendb(shared_ptr<dbmeta_t> dbm)
{
	for(std::vector< shared_ptr<dbmeta_t> >::const_iterator iter = odmlist_.begin();
			iter != odmlist_.end();
			iter++)
	{
		if ( *(*iter) == *dbm )
		{
			inform(ALREADY_OPENED);
			return ;
		}
	}
	DatabaseInterface* dbi_raw = dbm->opendb();
	shared_ptr<DatabaseInterface> dbi(dbi_raw);
	if (!dbi->connect())
	{
		inform(CONNECTION_FAILD);
		return ;
	}

	odlist_.push_back(dbi);
	odmlist_.push_back(dbm);

	/*
	 * Manipulate the UI
	 */
	SessionWindow* ses = new SessionWindow(dbi, this);
	addTab(ses, dbm->seg_data(0));
	pages_.push_back(ses);
}

void OpenedDBList::inform(OpenedDBList::info_t ) const
{
}
