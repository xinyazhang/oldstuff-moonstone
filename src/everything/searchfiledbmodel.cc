#include "searchfiledbmodel.h"
#include "Preferences.h"
#include <QtCore/QCoreApplication>
#include <kernel/search_engine.h>
#include "mainwindow.h"
#include <kernel/feedback.h>

SearchFileDBModel::SearchFileDBModel(Preferences* preference)
    :pref(preference)
{
    se_cookie = pref->search_engine->setup_service(this/*Cookie*/, SearchFileDBModel::EventCallback);
}

SearchFileDBModel::~SearchFileDBModel()
{
    pref->search_engine->finalize_service(se_cookie);
}

QVariant SearchFileDBModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
            return QVariant();

    if (role != Qt::DisplayRole)
            return QVariant();

    return pref->search_engine->data(se_cookie, index.row(), index.column());
}

Qt::ItemFlags SearchFileDBModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return Qt::ItemIsDropEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

const char* headers[] = {"Name", "Path", "Size", "Date Modified"};

QVariant SearchFileDBModel::headerData(int section,
                Qt::Orientation orientation,
                int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
            return headers[section];

    return QVariant();
}

QModelIndex SearchFileDBModel::index(int row,
                int column,
                const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
            return QModelIndex();

    if ( row < 0 || row >= pref->search_engine->line_count(se_cookie) )
            return QModelIndex();

    return createIndex(row, column);
}

QModelIndex SearchFileDBModel::parent(const QModelIndex &index) const
{
    return QModelIndex(); /* No tree structure ... */
}

int SearchFileDBModel::rowCount(const QModelIndex &) const
{
    return pref->search_engine->line_count(se_cookie);
}

int SearchFileDBModel::columnCount(const QModelIndex&) const
{
    return 4;
}

int SearchFileDBModel::change_searching_text(const QString& str)
{
    pref->search_engine->post_search(se_cookie, nativelize_QString(str));
	return 0;
}

int SearchFileDBModel::EventCallback(void* cookie, int ev, void*)
{
	SearchFileDBModel* model = (SearchFileDBModel*)cookie;
	log().printf(LOG_DEBUG, UT("Post a search done event\n"));
	QCoreApplication::postEvent(model, new QEvent((QEvent::Type)EVENT_SEARCHDONE));
	return 0;
}

bool SearchFileDBModel::event(QEvent* e)
{
    if (e->type()==EVENT_SEARCHDONE) {
		log().printf(LOG_DEBUG, UT("Search done acknowledged\n"));
        update_result();
		log().printf(LOG_DEBUG, UT("Search results updated\n"));
        return true;
    } else {
        return false;
    }
}

void SearchFileDBModel::update_result()
{
    beginResetModel();

    pref->search_engine->update_service_state(se_cookie);

    endResetModel();
}
