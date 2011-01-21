#ifndef OPENED_DB_LIST_H
#define OPENED_DB_LIST_H

#include <QtGui/QTabWidget>
#include "../pal/stdtype.h"
#include <vector>

class SessionWindow;
class dbmeta_t;
class DatabaseInterface;

/*
 * Although mix the domain layer and ui layer is not such good.
 * But there is no choice as seperating them may lead to more problems.
 */

class OpenedDBList
	:public QTabWidget
{
	Q_OBJECT;
public:
	OpenedDBList(QWidget *parent = NULL);

	void opendb(shared_ptr<dbmeta_t> );
public slots:
	void closedb(int );
private:
	std::vector< shared_ptr<DatabaseInterface> > odlist_; //Opened Database LIST
	std::vector< shared_ptr<dbmeta_t> > odmlist_;  // Opened Database Meta LIST
	std::vector< SessionWindow* > pages_;

	enum info_t
	{
		ALREADY_OPENED,
		CONNECTION_FAILD
	};

	void inform(info_t ) const;
};

#endif
