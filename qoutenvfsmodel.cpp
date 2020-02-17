#ifdef UNICODE
#undef UNICODE
#endif

#include "qoutenvfsmodel.h"
#include <qimage.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qdatetime.h>
#include <qdebug.h>
#include <qfileiconprovider.h>

#include <Windows.h>
typedef FhahaLib_Func * (*PFhahaLibGetProcStruct)();

HINSTANCE QOutEnvFSModel::LibHandle = NULL;

FhahaLib_Func * QOutEnvFSModel::pFunc = NULL;

QOutEnvFSModel::QOutEnvFSModel(QObject *parent)
	: QAbstractItemModel(parent)
{
	LibHandle = NULL;
	PFhahaLibGetProcStruct ProcAdd;
	if(LibHandle == NULL){
		LibHandle = LoadLibraryA("FhahaLib.dll");
		qDebug() << "user32 LibHandle = " << LibHandle;
		if(LibHandle){
			ProcAdd=(PFhahaLibGetProcStruct)GetProcAddress(LibHandle,"FhahaLibGetProcStruct");    
			qDebug() << "Get Func Address" <<  ProcAdd;
			if(ProcAdd!=NULL){
			
				pFunc = ProcAdd();
				qDebug() << "Get Pointer Address "<< pFunc;
			}
			else{
			
			}
		
		}
	}
	
}

void QOutEnvFSModel::calcSize_slot(QList<QString> selPaths)
{

}

QOutEnvFSModel::~QOutEnvFSModel()
{
	if(LibHandle!=NULL){
		FreeLibrary(LibHandle);
		LibHandle = NULL;
		pFunc = NULL;
	}
	qDeleteAll(m_allItems);
}

OutEnvFSPrivate * QOutEnvFSModel::findOutFSChild(QString abspath,OUTFSTYPE type) const{
	QList<OutEnvFSPrivate *>::const_iterator pitem= m_allItems.begin();
	while(pitem != m_allItems.end()){
		if((*pitem)->match(abspath,type)){
			return *pitem;
		}
		pitem++;
	}
	return NULL;
}

void BgWorkThread::run()
{
	if(QOutEnvFSModel::pFunc == NULL){
		return;
	}
	switch(m_bgworkmode){
	case WMCALCSIZE:
		{
			DWORD totalsize = 0;
			QList<QString>::iterator iter = m_selPaths.begin();
			for(;iter!=m_selPaths.end() && !m_bIsQuit;iter++){
				totalsize += getFileDirSize((*iter).toStdString().c_str());
		
			}
			emit calcSizeRes(totalsize);
		}
		break;
	
	case WMCOPYTOINNER:
		{
			int totalcount = 0;
			QList<QString>::Iterator iter = m_selPaths.begin();
			for(;iter!=m_selPaths.end() && !m_bIsQuit;iter++){
				totalcount += getSubItemCount((*iter).toStdString().c_str());
			}
			emit calcItemCount(totalcount);
			
			CopyFilesToInner(m_selPaths,m_innertarget);
		}
		break;
	case WMCOPYTOOUTER:
		{

		}
		break;
	}
}

void  BgWorkThread::CopyFileToInner(const char * outdir,const char * filename,const char * indir)
{
	char tmpdata[QOutEnvFSModel::ONCEBLOCK];
	char sourcefilename[MAX_PATH]={0};
	DWORD readCount = 0;
	sprintf(sourcefilename,"%s\\%s",outdir,filename);
	QFile file;
	QString targetfilename = QString("%1\\%2").arg(QString::fromLocal8Bit(indir)).arg(QString::fromLocal8Bit(filename));
    //关联文件名字
    file.setFileName(targetfilename);

    //打开文件，只写方式
    bool isOk = file.open(QIODevice::WriteOnly);
	if(!isOk)
		return;
	HANDLE inh = (HANDLE)QOutEnvFSModel::pFunc->CreateFile((char *)sourcefilename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_RANDOM_ACCESS,NULL);
	if(inh != (HANDLE)(-1) && inh != (HANDLE)0){
		BOOL tRet = TRUE;
		do{
			tRet = QOutEnvFSModel::pFunc->ReadFile((long long)inh,tmpdata,QOutEnvFSModel::ONCEBLOCK,&readCount,NULL);
			if(tRet && readCount!=0){
				if(file.write(tmpdata,readCount)!=readCount){
					tRet = FALSE;
				}
			}
		}while(tRet && readCount!=0);
	}
	file.close();
	QOutEnvFSModel::pFunc->CloseHandle((long long)inh);
}

int BgWorkThread::CopyFilesToInner(QList<QString> selOutPaths,QString intargetdir)
{
	QList<QString>::Iterator iter = m_selPaths.begin();
	for(;iter!=m_selPaths.end() && !m_bIsQuit;iter++){
		char outdirname[MAX_PATH];
		char outRootDir[MAX_PATH]={0};
		strcpy(outdirname,(*iter).toStdString().c_str());
		strcpy(outRootDir,outdirname);
		char * pUpSlash = strrchr(outRootDir,'\\');
		char * filename = pUpSlash + 1;
		*pUpSlash=0;
		WIN32_FILE_ATTRIBUTE_DATA wfad;
		if(strlen(outdirname)>=MAX_PATH)
			return 0;
		BOOL tRet = QOutEnvFSModel::pFunc->GetFileAttributesEx(outdirname,GetFileExInfoStandard,&wfad);
		if(!tRet){
			return 0;
		}
		if(wfad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){ // is a dir

		}
		else{ // is a file 
			CopyFileToInner(outRootDir,filename,intargetdir.toStdString().c_str());

		}

		//totalcount += getSubItemCount();
	}
	return 0;
}

BgWorkThread::BgWorkThread(QObject * parent):QThread(parent),m_bIsQuit(false),m_bgworkmode(WMCALCSIZE)
{

}

BgWorkThread::~BgWorkThread()
{

}


DWORD BgWorkThread::getSubItemCount(const char * abspath)
{
	DWORD totalcount = 0;
	WIN32_FILE_ATTRIBUTE_DATA wfad;
	if(strlen(abspath)>=MAX_PATH)
		return 0;
	BOOL tRet = QOutEnvFSModel::pFunc->GetFileAttributesEx(abspath,GetFileExInfoStandard,&wfad);
	if(!tRet){
		return 0;
	}
	if(wfad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
		char tmppath[MAX_PATH];
		strcpy_s(tmppath,MAX_PATH,abspath);
		int len = strlen(tmppath);
		if(tmppath[len-1]!='\\')
		{
			tmppath[len]='\\';
			tmppath[len+1]='*';
			tmppath[len+2] = 0;
		}
		else{
			tmppath[len]='*';
			tmppath[len+1]=0;
		}
		WIN32_FIND_DATA fd;
		long long hFindFile = QOutEnvFSModel::pFunc->FindFirstFile(tmppath,&fd);
		if(hFindFile == -1 || hFindFile == 0){
			QOutEnvFSModel::pFunc->CloseHandle(hFindFile);
			return totalcount;
		}
		else{
			bool isFinished = false;
			BOOL bIsDirectory = FALSE;
			int idx = 0;
			while(!isFinished){  
				bIsDirectory = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);  
              
				//如果是.或..  
				if( bIsDirectory  
					&& (strcmp(fd.cFileName, ".")==0 || strcmp(fd.cFileName, "..")==0))   
				{         
					isFinished = (QOutEnvFSModel::pFunc->FindNextFile(hFindFile, &fd) == FALSE);  
					continue;  
				} 
				char subAbsPath[MAX_PATH];
				sprintf(subAbsPath,"%s\\%s",abspath,fd.cFileName);
				totalcount += getSubItemCount(subAbsPath);
				totalcount ++;
				emit updateSize(totalcount);
				isFinished = (QOutEnvFSModel::pFunc->FindNextFile(hFindFile, &fd) == FALSE);  
			}  
          
			QOutEnvFSModel::pFunc->FindClose(hFindFile);  
		}
	}
	else {
		totalcount = 1 ;
	}

	return totalcount;
}


DWORD BgWorkThread::getFileDirSize(const char * abspath)
{
	DWORD totalsize = 0;
	WIN32_FILE_ATTRIBUTE_DATA wfad;
	if(strlen(abspath)>=MAX_PATH)
		return 0;
	BOOL tRet = QOutEnvFSModel::pFunc->GetFileAttributesEx(abspath,GetFileExInfoStandard,&wfad);
	if(!tRet){
		return 0;
	}
	if(wfad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
		char tmppath[MAX_PATH];
		strcpy_s(tmppath,MAX_PATH,abspath);
		int len = strlen(tmppath);
		if(tmppath[len-1]!='\\')
		{
			tmppath[len]='\\';
			tmppath[len+1]='*';
			tmppath[len+2] = 0;
		}
		else{
			tmppath[len]='*';
			tmppath[len+1]=0;
		}
		WIN32_FIND_DATA fd;
		long long hFindFile = QOutEnvFSModel::pFunc->FindFirstFile(tmppath,&fd);
		if(hFindFile == -1 || hFindFile == 0){
			QOutEnvFSModel::pFunc->CloseHandle(hFindFile);
			return totalsize;
		}
		else{
			bool isFinished = false;
			BOOL bIsDirectory = FALSE;
			int idx = 0;
			while(!isFinished){  
				bIsDirectory = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);  
              
				//如果是.或..  
				if( bIsDirectory  
					&& (strcmp(fd.cFileName, ".")==0 || strcmp(fd.cFileName, "..")==0))   
				{         
					isFinished = (QOutEnvFSModel::pFunc->FindNextFile(hFindFile, &fd) == FALSE);  
					continue;  
				} 
				char subAbsPath[MAX_PATH];
				sprintf(subAbsPath,"%s\\%s",abspath,fd.cFileName);
				totalsize += getFileDirSize(subAbsPath);
				emit updateSize(totalsize);
				isFinished = (QOutEnvFSModel::pFunc->FindNextFile(hFindFile, &fd) == FALSE);  
			}  
          
			QOutEnvFSModel::pFunc->FindClose(hFindFile);  
		}
	}
	else {
		long long thisSize = wfad.nFileSizeHigh ;
		thisSize <<= 32;
		thisSize += wfad.nFileSizeLow;
		thisSize /= QOutEnvFSModel::ONCEBLOCK;
		totalsize = thisSize ;
	}

	return totalsize;
}

QModelIndex QOutEnvFSModel::index(int row, int column,
                  const QModelIndex &parent) const
{
	//qDebug() << "call index with row" << row << " col " << column  << " parent row " << parent.row() << " column " << parent.column() ;

	if (!parent.isValid())
    {
        // 首层节点绑定关系
        if (row < m_rootDrives.size() && row != -1)
            return createIndex(row, column, m_rootDrives.at(row));
		else
			return QModelIndex();
    }
    else
    {
        // 其它层节点绑定关系
        if (parent.internalPointer() != nullptr)
        {
            OutEnvFSPrivate * parentItemPtr = static_cast<OutEnvFSPrivate *>(parent.internalPointer());
			/*if(selItemPtr == NULL){
				selItemPtr = m_rootDrives.at(row);
			}*/
			QString childPath ;
			OUTFSTYPE childType = OUTFTUNKNOWN;
			switch(parentItemPtr->fstype){
			case OUTFTDRIVE:
			case OUTFTDIR:
				{
					//根据路径，row行数,返回绝对路径和类型
#if 0
					QDir rootDir = QDir(parentItemPtr->absPath);
					rootDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::AllDirs | QDir::NoDotAndDotDot);
					//rootDir.setSorting(QDir::Size | QDir::Reversed);
					QFileInfoList list = rootDir.entryInfoList();//返回这个目录中所有目录和文件的QFileInfo对象的列表
					if(!list.isEmpty()){
						QFileInfo tem = list.at(row);
						childPath =  tem.absoluteFilePath() ;
						if(!tem.isDir()){
							childType = OUTFTFILE;
						}else {
							childType = OUTFTDIR;
						}
						OutEnvFSPrivate * pFind = NULL;
						pFind = findOutFSChild(childPath,childType);
						if(pFind == NULL){
							pFind = new OutEnvFSPrivate(childPath,childType,row,column,parentItemPtr);
							(const_cast<QOutEnvFSModel*>(this))->m_allItems.append(pFind);
						}
						return createIndex(row,column,pFind);
						
					}
#else
					//childType = InEnvDirPlugin::GetFileInfoInDir(parentItemPtr->absPath,row,childPath);
					char lpPath[MAX_PATH] = {0};
					int len = 0;
					strcpy(lpPath,parentItemPtr->absPath.toStdString().c_str());
					len = strlen(lpPath);
					if(lpPath[len-1]!='\\'){
						lpPath[len]='\\';
						lpPath[len+1]=0;
					}
					BOOL tmpRet = pFunc->SetCurrentDirectory((LPCTSTR)lpPath);
					if(!tmpRet){
						return QModelIndex();
					}
					WIN32_FIND_DATA fd;
					long long hFindFile = pFunc->FindFirstFile("*",&fd);
					if(hFindFile == -1 || hFindFile == 0){
						pFunc->CloseHandle(hFindFile);
						return QModelIndex();
					}
					else{
						bool isFinished = false;
						BOOL bIsDirectory = FALSE;
						int idx = 0;
						while(!isFinished){
							//strcpy(tempPath, lpPath);  
							//if(lpPath[len-1] != '/') strcat(tempPath, "/");  
							//strcat(tempPath, fd.cFileName);  
							bIsDirectory = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);  
              
							//如果是.或..  
							if( bIsDirectory  
								&& (strcmp(fd.cFileName, ".")==0 || strcmp(fd.cFileName, "..")==0))   
							{         
								isFinished = (pFunc->FindNextFile(hFindFile, &fd) == FALSE);  
								continue;  
							} 
							if(idx == row){ //find one
								if(bIsDirectory){
									childType = OUTFTDIR;
								}
								else
									childType = OUTFTFILE;
								childPath = parentItemPtr->absPath + "\\" + fd.cFileName;
								isFinished = TRUE;
								continue;
							}
							idx++;
							isFinished = (pFunc->FindNextFile(hFindFile, &fd) == FALSE);  
						}  
          
						pFunc->FindClose(hFindFile);  
					}
					if(childType != OUTFTUNKNOWN){
						OutEnvFSPrivate * pFind = NULL;
						pFind = findOutFSChild(childPath,childType);
						if(pFind == NULL){
							pFind = new OutEnvFSPrivate(childPath,childType,row,column,parentItemPtr);
							(const_cast<QOutEnvFSModel*>(this))->m_allItems.append(pFind);
						}
						return createIndex(row,column,pFind);
					}
#endif

				}
				break;
			case OUTFTFILE:
				{
					//QFileInfo info(parentItemPtr->first);
					//return createIndex(row,column,new QPair<QString,OUTFSTYPE>(info.absoluteFilePath(),OUTFTFILE));
					return QModelIndex();
				}
				break;
			default:
				return QModelIndex();
			}
        }
    }
    return QModelIndex();

}

char* GBKToUTF8( const char* chGBK ){	
	DWORD dWideBufSize=MultiByteToWideChar(CP_ACP, 0,(LPCSTR)chGBK,-1, NULL, 0);  	
	wchar_t * pWideBuf=new wchar_t[dWideBufSize];  	
	wmemset(pWideBuf, 0, dWideBufSize);  	
	MultiByteToWideChar(CP_ACP,0,(LPCSTR)chGBK,-1,pWideBuf,dWideBufSize); 	
	DWORD dUTF8BufSize=WideCharToMultiByte(CP_UTF8,0,(LPCWSTR)pWideBuf,-1,NULL,0,NULL,NULL);  	
	char * pUTF8Buf=new char[dUTF8BufSize];  	
	memset(pUTF8Buf, 0, dUTF8BufSize);  	
	DWORD chgSize = WideCharToMultiByte( CP_UTF8,0,(LPCWSTR)pWideBuf,-1,pUTF8Buf,dUTF8BufSize,NULL,NULL); 	
	char tmpstr[256];
	sprintf(tmpstr,"widechartomb ret %d\n",chgSize);
	OutputDebugString(tmpstr);
	delete[]pWideBuf;	
	return pUTF8Buf;
}


QVariant QOutEnvFSModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
	switch (role) {
    case Qt::DecorationRole:
        if (section == 0) {
            // ### TODO oh man this is ugly and doesn't even work all the way!
            // it is still 2 pixels off
            QImage pixmap(16, 1, QImage::Format_Mono);
            pixmap.fill(0);
            pixmap.setAlphaChannel(pixmap.createAlphaMask());
            return pixmap;
        }
        break;
    case Qt::TextAlignmentRole:
        return Qt::AlignLeft;
    }

    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QAbstractItemModel::headerData(section, orientation, role);
	QString returnValue;
    switch (section) {
    case 0: returnValue = tr("Name");
            break;
    case 1: returnValue = tr("Size");
            break;
    case 2: returnValue =
#ifdef Q_OS_MAC
               tr("Kind", "Match OS X Finder");
#else
               tr("Type", "All other platforms");
#endif
           break;
    // Windows   - Type
    // OS X      - Kind
    // Konqueror - File Type
    // Nautilus  - Type
    case 3: returnValue = tr("Date Modified");
            break;
    default: return QVariant();
    }
    return returnValue;
}
QVariant QOutEnvFSModel::data(const QModelIndex & index,
                                    int role ) const
{
	//qDebug() << "call data with row " << index.row() << " column " << index.column() << " role " << role ;
    if(!index.isValid())
    {
        return QVariant();
    }
    if( (role == Qt::TextAlignmentRole) || (role == Qt::ForegroundRole) )
    {
		if(index.column() != 0)
			return int(Qt::AlignHCenter | Qt::AlignVCenter);
		else
			return int(Qt::AlignLeft| Qt::AlignVCenter);
    }
	
	//qDebug() << "data role " << role << " row " << index.row() << " column " << index.column() ;
	if(role == Qt::DecorationRole && index.column() == 0){
		OutEnvFSPrivate * selPtr = static_cast<OutEnvFSPrivate *>(index.internalPointer()); 
		OUTFSTYPE fstype = selPtr->fstype;
		QFileIconProvider icon_provider;
		switch(fstype){
		case OUTFTFILE:
			{
				QIcon icon = icon_provider.icon(QFileIconProvider::File);
				return QVariant(icon);
			}
			break;
		case OUTFTDRIVE:
			{
				QIcon icon = icon_provider.icon(QFileIconProvider::Drive);
				return QVariant(icon);
			}
			break;
		case OUTFTDIR:
			{
				QIcon icon = icon_provider.icon(QFileIconProvider::Folder);
				return QVariant(icon);
			}
			break;
		}
	}
    if(role == Qt::DisplayRole)
    {
		//int rowcount = index.row();
		OutEnvFSPrivate * selPtr = static_cast<OutEnvFSPrivate *>(index.internalPointer()); 
		OUTFSTYPE fstype = selPtr->fstype;
		QString fspath = selPtr->absPath;
		//qDebug() << "want display " << fspath << " type" << fstype << " row " << index.row() << " column " << index.column() << " parent" << index.parent();
		switch(fstype){
		case OUTFTDIR:
		case OUTFTFILE:
			{
				//获取fspath文件的文件名、大小、类型、最后修改时间
#if 1
				QString filename ;
				QString filesize ;
				QString filetype ;
				QString filelastmodifytime ;
				//InEnvGetOutFileInfo(fspath,filename,filesize,filetype,filelastmodifytime);
				char lpPath[MAX_PATH] = {0};
				int len = 0;
				strcpy(lpPath,fspath.toStdString().c_str());
				WIN32_FILE_ATTRIBUTE_DATA wfad;
				char * pos = strrchr(lpPath,'\\');
#define TRANSUTF8 0
				char * pUtf8 = lpPath;
				if(pos != NULL){

#if TRANSUTF8
					pUtf8 = GBKToUTF8(pos+1);
#else
					pUtf8=pos+1;
#endif
				}
				else{
#if TRANSUTF8
					pUtf8 = GBKToUTF8(lpPath);
#endif
				}
				filename =  QString::fromLocal8Bit(pUtf8);
#if TRANSUTF8
				delete[] pUtf8;
#endif
				BOOL tmpRet = pFunc->GetFileAttributesEx((char*)lpPath,GetFileExInfoStandard,&wfad);
				if(tmpRet){
					
					if(wfad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
						filetype = "File Folder";
						
						//filename = "中文";
					}
					else{
						//filename = GBKToUTF8("中文");
						filetype = "File";
						//
						//pUtf8 = GBKToUTF8(pos+1);
						//delete[] pUtf8 ;

						//long long tH = pFunc->CreateFile((char *)lpPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_RANDOM_ACCESS,NULL);
						//if(tH != -1 && tH != 0){
						//	BY_HANDLE_FILE_INFORMATION bhfi;
						//	pFunc->GetFileInformationByHandle(tH,&bhfi);
						//
						//	filesize = QString::number(bhfi.nFileSizeHigh * 2ll^32  + bhfi.nFileSizeLow);	
						//}

					}
					filesize = QString::number(wfad.nFileSizeHigh * 2ll^32  + wfad.nFileSizeLow);
					SYSTEMTIME sysTime;
					FileTimeToSystemTime(&wfad.ftLastWriteTime,&sysTime);
					filelastmodifytime = QString("%1-%2-%3 %4::%5::%6").arg(sysTime.wYear,4,10,QLatin1Char('0'))
						.arg(sysTime.wMonth,2,10,QLatin1Char('0'))
						.arg(sysTime.wDay,2,10,QLatin1Char('0'))
						.arg(sysTime.wHour,2,10,QLatin1Char('0'))
						.arg(sysTime.wMinute,2,10,QLatin1Char('0'))
						.arg(sysTime.wSecond,2,10,QLatin1Char('0'));
				}
				else{
					//char tmpretstr[256]={0};
					//sprintf(tmpretstr,"GetFileAttributes ret %d %s\n",tmpRet,lpPath);
					//OutputDebugString(tmpretstr);
					
				}
				

#else
				QFileInfo info(fspath);
				QString filename = info.fileName();
				QString filesize = info.isDir()?"":QString::number(info.size());
				QString filetype = info.isFile()?tr("File"):tr("File Folder");
				QString filelastmodifytime = info.lastModified().toString("yyyy-MM-dd hh::mm::ss");
#endif
				switch(index.column()){
				case 0:
					return QVariant(filename);
					break;
				case 1:
					return QVariant(filesize);
					break;
				case 2:
					return QVariant(filetype);
					break;
				case 3:
					return QVariant(filelastmodifytime);
					break;
				default:
					qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!here!!!!!!!!!!!!!!!!!!!!!!!!!!";
					break;
				}
			}
			break;
		case OUTFTDRIVE:
			{
#if 1
				QString filename ;
				QString filesize ;
				QString filetype = "Drive";
				QString filelastmodifytime ="";
				//InEnvGetOutDriveInfo(fspath,filename,filesize,filetype,filelastmodifytime);
				char lpPath[MAX_PATH] = {0};
				//char tempPath[MAX_PATH] ={0};
				int len = 0;
				strcpy(lpPath,fspath.toStdString().c_str());
				len = strlen(lpPath);
				if(lpPath[len-1]!='\\'){
					lpPath[len]='\\';
					lpPath[len+1]=0;
				}
				filename = lpPath;
				DWORD64 qwFreeBytesToCaller, qwTotalBytes, qwFreeBytes;
				DWORD dwSectPerClust, dwBytesPerSect, dwFreeClusters, dwTotalClusters;
				BOOL bResult;
				//使用GetDiskFreeSpaceEx获取磁盘信息并打印结果
				bResult = pFunc->GetDiskFreeSpaceEx (lpPath,
					(PULARGE_INTEGER)&qwFreeBytesToCaller,
					(PULARGE_INTEGER)&qwTotalBytes,
					(PULARGE_INTEGER)&qwFreeBytes);
				if(bResult)
					filesize = QString::number(qwFreeBytesToCaller);
					//printf("空闲空间（字节）: \t\t%I64d\n", qwFreeBytes);
					//printf("磁盘总容量（字节）: \t\t%I64d\n", qwTotalBytes);
				else
					filesize = "0";
#else
				QFileInfo info(fspath);
				QString filename = info.absoluteFilePath();
				QString filesize = "";
				QString filetype = "Drive";
				QString filelastmodifytime = info.lastModified().toString("yyyy-MM-dd hh::mm::ss");
#endif
				switch(index.column()){
				case 0:
					return QVariant(filename);
					break;
				case 1:
					return QVariant(filesize);
					break;
				case 2:
					return QVariant(filetype);
					break;
				case 3:
					return QVariant(filelastmodifytime);
					break;
				default:
					qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!here!!!!!!!!!!!!!!!!!!!!!!!!!!";
					break;
				}
			}
			break;
		}
			
        return QVariant("unknown");
    }
    //return QAbstractItemModel::data();
	return QVariant();
}
QModelIndex QOutEnvFSModel::parent(const QModelIndex &child) const
{
	//qDebug() << "call parent with cild  row " << child.row() << " column " << child.column() ;
    //* 如果是无效的节点则返回无效节点
	if(!child.isValid())
	{
		//qDebug() << "no parent ";
		return QModelIndex();
	}

	OutEnvFSPrivate* childData = static_cast<OutEnvFSPrivate*>(child.internalPointer());
	for(int i = 0; i < m_rootDrives.count(); i++)
	{
		if(m_rootDrives[i] == childData) //* 如果是父节点（分组）则返回无效父节点（分组没有父节点）
		{
			//qDebug() << "no parent ";
			return QModelIndex();
		}
	}
	for(int i = 0; i<m_allItems.count();i++){
		OutEnvFSPrivate* item = m_allItems[i];
		if(item == childData){
			if(item->m_pParent != NULL){
				//qDebug() << "find parent " << item->m_pParent;
				return createIndex(item->m_pParent->m_row,item->m_pParent->m_col,item->m_pParent);
			}
		}
		//return QModelIndex();
	}
	//* 未匹配到有效的父节点数据，返回无效节点
	//qDebug() << "no parent ";
	return QModelIndex();
}

void QOutEnvFSModel::refreshRootDevice()
{
	m_rootDrives.clear();
	if(pFunc){
		int icount = 0;
		DWORD outDVs = pFunc->GetLogicalDrives();
		TCHAR tmpDv[3] = {'A',':',0};
		while(outDVs != 0l){
			if(outDVs & 1 != 0){
				tmpDv[0] = 'A' + icount;
				UINT DvType = pFunc->GetDriveType(tmpDv);
				switch(DvType){
				case DRIVE_FIXED:
				case DRIVE_NO_ROOT_DIR:
					{
						QString tmpDvName = (char *)tmpDv;
						addRootDevice(tmpDvName,OUTFTDRIVE);
					}
					break;
				default:
					break;
				}
			}
			icount++;
			outDVs>>=1;
		}
	}
}

void QOutEnvFSModel::addRootDevice(QString devname,OUTFSTYPE fstype)
{
	bool alreadyHas = false;
	QList<OutEnvFSPrivate*>::iterator litem = m_rootDrives.begin();
	while(litem != m_rootDrives.end()){
		if((*litem)->match(devname, fstype)){
			alreadyHas = true;
			break;
		}
		litem++;
	}
	if(!alreadyHas){
		OutEnvFSPrivate * newItem = new OutEnvFSPrivate(devname,fstype,m_rootDrives.size(),0,NULL);
		this->m_rootDrives.append(newItem);
		this->m_allItems.append(newItem);
	}
}

int QOutEnvFSModel::rowCount(const QModelIndex &parent ) const
{
    if(!parent.isValid()){
		return m_rootDrives.size();
	}
	else{
		OutEnvFSPrivate * parentData = static_cast<OutEnvFSPrivate*>(parent.internalPointer());
		switch(parentData->fstype ){
		case OUTFTDRIVE:
		case OUTFTDIR:
			{
				//根据输入的目录绝对地址，获取下级文件和文件夹的数量
				int listsize = 0;
#if 1
				//listsize = InEnvDirPlugin::GetSubItemCount(parentData->absPath);
				char lpPath[MAX_PATH] = {0};
				//char tempPath[MAX_PATH] ={0};
				int len = 0;
				strcpy(lpPath,parentData->absPath.toStdString().c_str());
				len = strlen(lpPath);
				if(lpPath[len-1]!='\\'){
					lpPath[len]='\\';
					lpPath[len+1]=0;
				}
				BOOL tmpRet  = pFunc->SetCurrentDirectory((LPCTSTR)lpPath);
				if(!tmpRet){
					return 0;
				}
				WIN32_FIND_DATA fd;
				long long hFindFile = pFunc->FindFirstFile("*",&fd);
				if(hFindFile == -1 || hFindFile == 0){
					pFunc->CloseHandle(hFindFile);
					listsize = 0;
				}
				else{
					bool isFinished = false;
					BOOL bIsDirectory = FALSE;
					while(!isFinished){
						//strcpy(tempPath, lpPath);  
						//if(lpPath[len-1] != '/') strcat(tempPath, "/");  
						//strcat(tempPath, fd.cFileName);  
						bIsDirectory = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);  
              
						//如果是.或..  
						if( bIsDirectory  
							&& (strcmp(fd.cFileName, ".")==0 || strcmp(fd.cFileName, "..")==0))   
						{         
							isFinished = (pFunc->FindNextFile(hFindFile, &fd) == FALSE);  
							continue;  
						}  
						listsize++;
						isFinished = (pFunc->FindNextFile(hFindFile, &fd) == FALSE);  
					}  
          
					pFunc->FindClose(hFindFile);  
				}
#else
				QDir curDir(parentData->absPath);
				curDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::AllDirs|QDir::NoDotAndDotDot);
				//curDir.setSorting(QDir::Size | QDir::Reversed);
				QFileInfoList list = curDir.entryInfoList();//返回这个目录中所有目录和文件的QFileInfo对象的列表
				listsize = list.size();
				//qDebug() << "get " << parentData->absPath << " size " << listsize;
#endif
				return listsize;
				//return 0;
			}
			break;
		case OUTFTFILE:
			return 0;
		default:
			return 0;
		} 
			
	}
	return 0;
}
int QOutEnvFSModel::columnCount(const QModelIndex &parent ) const
{
    return 4;
}

