#include "insaveenvdlg.h"
#include <QtGui/QApplication>
#include <string>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
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