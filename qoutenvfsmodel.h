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
	
	static QString calcShowSize(qlonglong showsize);
	void refreshRootDevice();
	const static int ONCEBLOCK = 512;
	
	static FhahaLib_Func * pFunc;
	BOOL isDirectory(const char * dirpath);

public slots:

	
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
	void setOutTargetDir(QString tardir){
		m_outtertarget = tardir;
	}
	void setInnerTargetDir(QString &targetdir){
		m_innertarget = targetdir;
	}
	
	qlonglong getFileDirSize(const char * abspath);
	DWORD getOutSubItemCount(const char * abspath);
	qlonglong getInSubItemCount(QString rootdir,QString curdir,QList<QPair<QString,QString>> &subitems);
	int CopyFilesToInner(QList<QString> selOutPaths,QString intargetdir);
	int CopyFilesToOuter(QList<QPair<QString,QString>> & copyitems);
	void setWorkMode(BGWORKMODE mode){
		m_bgworkmode = mode;
	}
	void setBgWorkNextMode(BGWORKMODE mode){
		m_bgworknextmode = mode;
	}
signals:
	//void calcSizeRes(unsigned int);
	//void calcItemCount(int,int);
	//void updateSize(unsigned int);
	void UpdateTotalCountSize(int,qlonglong);
	void ProcCountSize(int,qlonglong);
	void copyDone();
protected:
	bool m_bIsQuit;
	QList<QString> m_selPaths;
	void run();

private:
	BGWORKMODE m_bgworkmode ;
	BGWORKMODE m_bgworknextmode;
	QString m_innertarget;
	QString m_outtertarget;
	int m_alreadyCopyedCount;
	int m_currentTotalCount;
	qlonglong m_currentTotalSize;
	qlonglong m_alreadyCopyedSize;
	QList<QPair<QString,QString>> selSources;
	void CopyFileToInner(const char * outdir,const char * filename,const char * indir,const char * outrootdir);
	void CopyDirToInner(const char * outdir,const char * outdirname,const char * indir,const char * outrootdir);
};

#endif // QOUTENVFSMODEL_H
