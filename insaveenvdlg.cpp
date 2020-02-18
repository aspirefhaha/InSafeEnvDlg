
#include "insaveenvdlg.h"

InSaveEnvDlg::InSaveEnvDlg(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags),OutModel(this)
{
	ui.setupUi(this);
	m_pBgWorkThread = new BgWorkThread(this);
	connect(m_pBgWorkThread,SIGNAL(calcSizeRes(unsigned int)),this,SLOT(showCalcSize(unsigned int)));
	connect(m_pBgWorkThread,SIGNAL(updateSize(unsigned int)),this,SLOT(updateMsg(unsigned int)));
	connect(m_pBgWorkThread,SIGNAL(calcItemCount(int,int)),this,SLOT(updateItemCount(int,int)));
	connect(m_pBgWorkThread,SIGNAL(copyDone()),this,SLOT(copyFinished()));
	//OutModel.setRootPath("");

	//ui.tv_OutEnv->setRootIndex(OutModel.index("C:\\hadoop-2.7.7"));
	m_tpDlg = new FProgWaitDlg(NULL,FDT_WAIT);
	m_tpDlg->setModal(true);
	
#if 1
	//OutModel.addRootDevice("C:\\hadoop-2.7.7",OUTFTDIR);
	//OutModel.addRootDevice("D:\\ddj\\xt1pilot_ms\\GenFunc.c",OUTFTFILE);
	//OutModel.addRootDevice("D:",OUTFTDRIVE);
	//OutModel.addRootDevice("D:",OUTFTDRIVE);
	OutModel.refreshRootDevice();
#else
	//ui.tv_OutEnv->setSelectionMode(QAbstractItemView::ExtendedSelection); 
	FriendViewItemList fli;
    for(int i = 0; i <5 ;i++)
    {
        FriendViewItem* gi = new FriendViewItem;
        gi->isGroup = true;
        gi->name = QString("group %1").arg(i);
        for(int ii = 0; ii < 10; ii++)
        {
            FriendViewItem* ci = new FriendViewItem;
            ci->isGroup = false;
            ci->name = QString("children %1").arg(ii);
            gi->children << ci;
        }
        fli << gi;

    }
    OutModel.setFriendViewItemList(fli);
#endif
	ui.tv_OutEnv->setSelectionMode(QAbstractItemView::ExtendedSelection); 
	ui.tv_OutEnv->setModel(&OutModel);
	//ui.tv_OutEnv->setUniformRowHeights(true);


    //InModel.setRootPath(QDir::currentPath());
	ui.tv_InEnv->setSelectionMode(QAbstractItemView::ExtendedSelection); 
	InModel.setRootPath("D:/Temp/");
    ui.tv_InEnv->setModel(&InModel);
	//ui.tv_InEnv->setRootIndex(InModel.index(QDir::currentPath()));
	ui.tv_InEnv->setRootIndex(InModel.index("D:/Temp/"));
	
    // Demonstrating look and feel features
    ui.tv_InEnv->setAnimated(false);
    ui.tv_InEnv->setIndentation(20);
    ui.tv_InEnv->setSortingEnabled(true);
	//ui.tv_InEnv->setUniformRowHeights(true);
}

InSaveEnvDlg::~InSaveEnvDlg()
{

}

void InSaveEnvDlg::copyFinished()
{
	m_tpDlg->close();
}

void InSaveEnvDlg::updateItemCount(int idx,int totalcount)
{
	m_tpDlg->setCountIndex(idx,totalcount);
	m_selTotalCount = totalcount;
	//ui.lb_OutSelected->setText(calcShowSize(totalsize));
}

void InSaveEnvDlg::showCalcSize(unsigned int totalsize)
{
	m_tpDlg->close();
	m_selOutTotalSize = totalsize;
	ui.lb_OutSelected->setText(calcShowSize(totalsize));
}

void InSaveEnvDlg::updateMsg(unsigned int size)
{
	m_tpDlg->updateText(calcShowSize(size));
}

QString InSaveEnvDlg::calcShowSize(DWORD showsize)
{
	long long actSize = showsize;
	actSize *= QOutEnvFSModel::ONCEBLOCK;
	if(actSize >= 1024*1024 * 1024 *0.7)
		return QString::number(actSize/1024.0/1024/1024) + "GB";
	else if(actSize >= 1024*1024*0.7)
		return QString::number(actSize/1024.0/1024 ) + "MB";
	else if(actSize >= 1024*0.7)
		return QString::number(actSize/1024.0 ) + "kB";
	return QString::number(actSize ) + "B";
}

void InSaveEnvDlg::OutEnvSelected(QModelIndex selIndex)
{
	//ui.lb_OutSelected->setWindowTitle("haha");
	//QString fhahaStr= QString("column %1 row %2 of parent %3").arg(selIndex.column()).arg(selIndex.row()).arg(selIndex.parent().row());
	//QString dirStr = QString("%1").arg(OutModel.filePath(selIndex));
#if 0	
	QModelIndexList selected = ui.tv_OutEnv->selectionModel()->selectedRows(0);

	QList<QModelIndex>::const_iterator cit;
	QModelIndex temp;
	QString strSelect;
	DWORD selectSize = 0; 
	QList<QString> selpath;
	for (cit = selected.begin(); cit != selected.end(); ++cit)
	{
		temp = *cit;
		//strSelect += ui.tv_InEnv->model()->itemData(temp).values()[0].toString();
		OutEnvFSPrivate * pSelItemPrivate = static_cast<OutEnvFSPrivate *>(temp.internalPointer());
		QString subItemPath = pSelItemPrivate->absPath;
		if(pSelItemPrivate->fstype == OUTFTDRIVE)
			continue;
		//strSelect += QString::fromLocal8Bit(subItemPath.toStdString().c_str());
		//selectSize += OutModel.getFileDirSize(subItemPath.toStdString().c_str());
		//strSelect += " " ;
		selpath.append(subItemPath);
	}
	//ui.lb_OutSelected->setText(calcShowSize(selectSize) + " " + strSelect);
	m_pBgWorkThread->setWorkMode(WMCALCSIZE);
	m_pBgWorkThread->setSelPath(selpath);
	m_pBgWorkThread->start();
	m_tpDlg = new FProgWaitDlg(NULL,FDT_WAIT);
	//m_tpDlg->setModal(true);
	m_tpDlg->exec();
#endif
}

void InSaveEnvDlg::InEnvSelected(QModelIndex selIndex)
{
	QModelIndexList selected = ui.tv_InEnv->selectionModel()->selectedRows(0);
	QList<QModelIndex>::const_iterator cit;
	QModelIndex temp;
	QString strSelect;
	for (cit = selected.begin(); cit != selected.end(); ++cit)
	{
		temp = *cit;
		//strSelect += ui.tv_InEnv->model()->itemData(temp).values()[0].toString();
		strSelect += InModel.filePath(temp);
		strSelect += " " ;
	}
	ui.lb_InSelected->setText(strSelect);

}

void InSaveEnvDlg::CopyToInEnv()
{
	QModelIndexList selected = ui.tv_InEnv->selectionModel()->selectedRows(0);
	if(selected.size() >1 || selected.size() <=0){
		QMessageBox::warning(this,tr("Target Count Err"), tr("multi target dir or nothing is choosed,pls select a singe dir as target"), 
			QMessageBox::Ok,QMessageBox::Ok);
		return;
	}
	QModelIndex temp = selected.first();
	QString tarDir = InModel.filePath(temp);
	if(!InModel.isDir(temp)){
		QMessageBox::warning(this, tr("Target Type Err"), tr("Target is Not Dir"),QMessageBox::Ok,QMessageBox::Ok);
		return;
	}
	
	//处理环境外的选择
	QModelIndexList outselected = ui.tv_OutEnv->selectionModel()->selectedRows(0);
	if(outselected.size() <=0){
		QMessageBox::warning(this,tr("Source Count Err"), tr("no source dir or file is choosed,pls select source"), 
			QMessageBox::Ok,QMessageBox::Ok);
		return;
	}
	QList<QModelIndex>::iterator cit = outselected.begin();
	QList<QString> selOutPaths ;
	for (; cit != outselected.end(); ++cit)
	{
		temp = *cit;
		OutEnvFSPrivate * pPriv = static_cast<OutEnvFSPrivate *>(temp.internalPointer());
		selOutPaths.append(pPriv->absPath);
		//strSelect += ui.tv_InEnv->model()->itemData(temp).values()[0].toString();
		//TODO
		//InEnvCopyFileFromOutEnv(pPriv->absPath,tarDir);
	}
	m_pBgWorkThread->setSelPath(selOutPaths);
	m_pBgWorkThread->setInnerTargetDir(tarDir);
	m_pBgWorkThread->setWorkMode(WMCOPYTOINNER);
	m_pBgWorkThread->start();
	m_tpDlg->exec();
}

void InSaveEnvDlg::CopyToOutEnv()
{
	//确保环境外选择的是一个目录
	QModelIndexList selected = ui.tv_OutEnv->selectionModel()->selectedRows(0);
	if(selected.size() >1 || selected.size() <=0){
		QMessageBox::warning(this,tr("Target Count Err"), tr("multi target dir or nothing is choosed,pls select a singe dir as target"), 
			QMessageBox::Ok,QMessageBox::Ok);
		return;
	}
	QModelIndex temp = selected.first();
	OutEnvFSPrivate * pPriv = static_cast<OutEnvFSPrivate *>(temp.internalPointer());
	QString tarDir = pPriv->absPath;
	if(!OutModel.isDirectory(tarDir.toStdString().c_str())){
		QMessageBox::warning(this, tr("Target Type Err"), tr("Target is Not Dir"),QMessageBox::Ok,QMessageBox::Ok);
		return;
	}
	
	//处理环境内的选择
	QModelIndexList inselected = ui.tv_InEnv->selectionModel()->selectedRows(0);
	if(inselected.size() <=0){
		QMessageBox::warning(this,tr("Source Count Err"), tr("no source dir or file is choosed,pls select source"), 
			QMessageBox::Ok,QMessageBox::Ok);
		return;
	}
	
	QList<QModelIndex>::iterator cit = inselected.begin();
	QList<QString> selInPaths ;
	for (; cit != inselected.end(); ++cit)
	{
		temp = *cit;
		selInPaths.append(InModel.filePath(temp));
		
	}
	m_pBgWorkThread->setSelPath(selInPaths);
	m_pBgWorkThread->setOutTargetDir(tarDir);
	m_pBgWorkThread->setWorkMode(WMCOPYTOOUTER);
	m_pBgWorkThread->start();
	m_tpDlg->exec();
}


