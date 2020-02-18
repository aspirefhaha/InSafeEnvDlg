#include "FProgWaitDlg.h"
#include <qlayout.h>
#include <qpushbutton.h>

FProgWaitDlg::FProgWaitDlg(QWidget *parent,FDTYPE fdtype)
	: QDialog(parent),m_fdtype(fdtype),label(NULL),movie(NULL)
{
    this->setFixedSize(200,200);

    //setWindowOpacity(0.5);  //设置透明用；
    this->setWindowFlags(Qt::Dialog);
	QVBoxLayout*  layout = new QVBoxLayout(this);
	this->setLayout(layout);
    //取消对话框标题

    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);

    //取消对话框标题和边框

    this->setAutoFillBackground(true);
	
    this->setContentsMargins(0,0,0,0);
	switch(m_fdtype){
	case FDT_WAIT:
		label = new QLabel(this);
		icolabel = new QLabel(this);
		button  = new QPushButton("Cancel",this);
		//label->setContentsMargins(0,0,0,0);
		/*QPalettepalette;palette.setBrush(QPalette::Background,QBrush(QPixmap("E:/qml/imgdialog/loading.gif")));
		this->setPalette(palette)*/;
		movie = new QMovie(":/gif/waiting.gif");
		//movie = new QMovie("D:/waiting.gif");
		label->setText("Please Waiting... ...");
		label->setAutoFillBackground(true);
		icolabel->setAutoFillBackground(true);
		/*
		icolabel->setPixmap(QPixmap(":/jpg/time.png").scaled(
                        icolabel->size(),
                        Qt::IgnoreAspectRatio,
                        Qt::SmoothTransformation)); 
						*/

		icolabel->setMovie(movie);
		movie->start();
		icolabel->show();
		label->show();
		layout->addWidget(icolabel);
		layout->addWidget(label);
		layout->addWidget(button);
		break;
	case FDT_PROG:
		break;
	default:
		break;
	}
	connect(button,SIGNAL(clicked()),this,SLOT(close()));
	
}

void FProgWaitDlg::setCountIndex(int idx,int totalcount)
{
	if(label){
		label->setText(QString("%1/%2").arg(idx).arg(totalcount));
		this->update();
	}
}

void FProgWaitDlg::updateText(QString text)
{
	if(label)
		label->setText(text);
}


FProgWaitDlg::~FProgWaitDlg()

{
	switch(m_fdtype){
	case FDT_WAIT:
		break;
	case FDT_PROG:
		break;
	}
	if(label){
		delete label;
		label = NULL;
	}
	if(icolabel){
		delete icolabel;
		icolabel = NULL;
	}
	if(movie){
		delete movie;
		movie = NULL;
	}
	if(button){
		delete button;
		button = NULL;
	}

}

