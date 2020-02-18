#ifndef INSAVEENVDLG_H
#define INSAVEENVDLG_H

#include <QtGui>
#include "qoutenvfsmodel.h"
#include "ui_insaveenvdlg.h"
#include "FProgWaitDlg.h"

class InSaveEnvDlg : public QDialog
{
	Q_OBJECT

public:
	InSaveEnvDlg(QWidget *parent = 0, Qt::WFlags flags = 0);
	~InSaveEnvDlg();
signals:
	//void calcSize(QList<QString>);
	//void copyToInEnvs(QList<QString>,QString target);
	//void copyToOutEnvs(QList<QString>,QString target);

private slots:
	void OutEnvSelected(QModelIndex selIndex);
	void InEnvSelected(QModelIndex selIndex);
	void CopyToInEnv();
	void CopyToOutEnv();
	QString calcShowSize(DWORD showsize);
	void showCalcSize(unsigned int calcSize);
	void updateMsg(unsigned int);
	void updateItemCount(int,int);
	void copyFinished();

private:
	Ui::InSaveEnvDlgClass ui;
	QOutEnvFSModel OutModel;
	FProgWaitDlg *m_tpDlg;

	QFileSystemModel InModel;
	int m_selOutTotalSize;
	int m_selTotalCount ;
	BgWorkThread * m_pBgWorkThread;
};

#endif // INSAVEENVDLG_H
