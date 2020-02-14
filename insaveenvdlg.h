#ifndef INSAVEENVDLG_H
#define INSAVEENVDLG_H

#include <QtGui>
#include "qoutenvfsmodel.h"
#include "HistoryModel.h"
#include "ui_insaveenvdlg.h"
#include "CFriendViewModel.h"

class InSaveEnvDlg : public QDialog
{
	Q_OBJECT

public:
	InSaveEnvDlg(QWidget *parent = 0, Qt::WFlags flags = 0);
	~InSaveEnvDlg();

private slots:
	void OutEnvSelected(QModelIndex selIndex);
	void InEnvSelected(QModelIndex selIndex);
	void CopyToInEnv();
	void CopyToOutEnv();

private:
	Ui::InSaveEnvDlgClass ui;
#if 1
	QOutEnvFSModel OutModel;
#else
	//HistoryModel OutModel;
	CFriendViewModel OutModel;
#endif
	QFileSystemModel InModel;
};

#endif // INSAVEENVDLG_H
