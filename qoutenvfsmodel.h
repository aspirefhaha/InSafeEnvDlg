#ifndef QOUTENVFSMODEL_H
#define QOUTENVFSMODEL_H

#include <QAbstractItemModel>
#include <Windows.h>
#include "FhahaLib.h"
enum OUTFSTYPE {
	OUTFTDRIVE=1,
	OUTFTSPEC,
	OUTFTDIR,
	OUTFTFILE,
	OUTFTUNKNOWN
};

struct OutEnvFSPrivate 
{
	QString absPath;
	OUTFSTYPE fstype;
	int m_col;
	int m_row;
	OutEnvFSPrivate * m_pParent;
	
	//QList<OutEnvFSPrivate *> m_lsChildren;

	bool match(QString name,OUTFSTYPE fstype){
		return name==absPath && fstype==this->fstype;
	}

	explicit OutEnvFSPrivate(QString devname,OUTFSTYPE tt,int row,int col,OutEnvFSPrivate * parent)
		:absPath(devname),fstype(tt),m_row(row),m_col(col),m_pParent(parent){}
};

class QOutEnvFSModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	QOutEnvFSModel(QObject *parent);
	~QOutEnvFSModel();
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

	void addRootDevice(QString,OUTFSTYPE);

	OutEnvFSPrivate * findOutFSChild (QString abspath,OUTFSTYPE type)const;

	void refreshRootDevice();

private:
	QList<OutEnvFSPrivate *> m_rootDrives;
	QList<OutEnvFSPrivate *> m_allItems;
	FhahaLib_Func * pFunc;
	HINSTANCE LibHandle;
};

#endif // QOUTENVFSMODEL_H
