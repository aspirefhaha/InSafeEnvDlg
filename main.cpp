#include "insaveenvdlg.h"
#include <QtGui/QApplication>
#include <string>

int main(int argc, char *argv[])
{
	QSharedMemory mem("SystemName");//以系统exe名称为参数，定义共享内存mem
	if(!mem.create(1))//创建共享内存mem，如果该共享内存已存在，则弹出提示对话框，并退出
	{
		//QMessageBox::information(0,QObject::tr("Warning"),QObject::tr("An instance is running"));
		return 0;
	}
	QApplication a(argc, argv);
	//QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTranslator translator;  
	//QString Q_str("hello");
	//std::string S_str;
	//S_str = Q_str.toStdString();
    translator.load(QString(":/qm/zh_CN.qm")); 
	a.installTranslator(&translator);
	InSaveEnvDlg w;
	w.show();
	return a.exec();
}
