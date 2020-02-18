#ifndef FPROGWAITDLG_H
#define FPROGWAITDLG_H

#include <QDialog>
#include <qlabel.h>
#include <qmovie.h>

enum FDTYPE{
	FDT_WAIT,
	FDT_PROG
};

class FProgWaitDlg : public QDialog
{
	Q_OBJECT

public:
	explicit FProgWaitDlg(QWidget *parent,FDTYPE fdtype);
	~FProgWaitDlg();
	void updateText(QString);
	void setCountIndex(int idx,int totalcount);

private:
	QLabel * label;
	QLabel * icolabel;
	QMovie * movie;
	QPushButton * button;
	FDTYPE m_fdtype;
};

#endif // FPROGWAITDLG_H
