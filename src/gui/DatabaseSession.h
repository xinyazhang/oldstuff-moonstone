#ifndef LAIN_GUI_DB_SESSION_H
#define LAIN_GUI_DB_SESSION_H

#include <QtGui/QWidget>
#include <kernel/common_declare.h>
#include <dal/db_sqlite_impl.h>

namespace Ui
{
	class DatabaseSession;
};

class LainMain;

class DatabaseSession
	:public QWidget
{
	Q_OBJECT;
public:
	DatabaseSession(LainMain*);
	~DatabaseSession();

public slots:
	void s2tag();

private:
	Ui::DatabaseSession* ui_;
	db_sqlite_impl* sqlite_;
	Database* db_;
};

#endif
