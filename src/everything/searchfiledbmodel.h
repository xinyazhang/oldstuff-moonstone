#ifndef SEARCHFILEDBMODEL_H
#define SEARCHFILEDBMODEL_H

#include <QAbstractItemModel>

class SearchFileDBModel : public QAbstractItemModel
{
public:
    SearchFileDBModel(class Preferences* pref);

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section,
                    Qt::Orientation orientation,
                    int role = Qt::DisplayRole) const;
    QModelIndex index(int row,
                    int column,
                    const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    int change_searching_text(const QString& str);

    static int EventCallback(void* /*SearchFileDBModel*, ENGINE_EVENT*/, ENGINE_EVENT, void*);
    bool event ( QEvent * e );
    void update_result();
private:
    class search_service *se_cookie;
    class Preferences* pref;
};

#endif // SEARCHFILEDBMODEL_H
