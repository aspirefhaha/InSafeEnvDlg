#ifndef QOUTENVFSMODEL_H
#define QOUTENVFSMODEL_H

#include <QAbstractItemModel>
#include <qthread.h>
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
	const static int ONCEBLOCK = 512;
	
	static FhahaLib_Func * pFunc;

public slots:
	void calcSize_slot(QList<QString> selPaths);
	//void copyToInEnv_slot(QList<QString> selpaths,QString target);
	
private:
	QList<OutEnvFSPrivate *> m_rootDrives;
	QList<OutEnvFSPrivate *> m_allItems;
	static HINSTANCE LibHandle;
};

enum BGWORKMODE
{
	WMCALCSIZE=1,
	WMCOPYTOINNER,
	WMCOPYTOOUTER
};

class BgWorkThread:public QThread
{
	Q_OBJECT

public:
	explicit BgWorkThread(QObject * parent=0);
	~BgWorkThread();
	void setSelPath(QList<QString> &selpaths){
		m_selPaths=selpaths;
	}
	void setInnerTargetDir(QString &targetdir){
		m_innertarget = targetdir;
	}
	
	DWORD getFileDirSize(const char * abspath);
	DWORD getSubItemCount(const char * abspath);
	int CopyFilesToInner(QList<QString> selOutPaths,QString intargetdir);
	void setWorkMode(BGWORKMODE mode){
		m_bgworkmode = mode;
	}
signals:
	void calcSizeRes(unsigned int);
	void calcItemCount(int);
	void updateSize(unsigned int);
protected:
	bool m_bIsQuit;
	QList<QString> m_selPaths;
	void run();

private:
	BGWORKMODE m_bgworkmode ;
	QString m_innertarget;
	void CopyFileToInner(const char * outdir,const char * filename,const char * indir);

};

#endif // QOUTENVFSMODEL_H
