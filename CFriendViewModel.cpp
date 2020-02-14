
#include "CFriendViewModel.h"
#include <qdebug.h>
CFriendViewModel::CFriendViewModel(QObject *parent)
	:QAbstractItemModel(parent)
{
}

CFriendViewModel::~CFriendViewModel()
{

}

int CFriendViewModel::columnCount(const QModelIndex &parent) const
{
	//* 本示例中只有一列数据
	return 1;
}

QVariant CFriendViewModel::data(const QModelIndex &index, int role) const
{
	qDebug() << "call data with row " << index.row() << " column " << index.column() << " role " << role ;
	//* 如果是无效节点则返回空数据
	if(!index.isValid())
	{
		return QVariant();
	}
	//* 尝试获取原始数据

	FriendViewItem* d = static_cast<FriendViewItem*>(index.internalPointer());

	if(d == NULL)//* 没有原始数据则返回空数据
	{
		return QVariant();
	}
	if(role == Qt::DisplayRole)//* 本例只返回了DisplayRole的数据
	{
		return d->name;
	}
	return QVariant();
}



QModelIndex CFriendViewModel::index(int row, int column, const QModelIndex &parent) const
{
	qDebug() << "call index with row" << row << " col " << column  << " parent row " << parent.row() << " column " << parent.column() ;
	//* 看父节点是否有效
	if(!parent.isValid()) //* 无效的父节点(那肯定是分组)
	{
		QModelIndex groupIndex;
		if(mItemList.count() > row && row != -1) //* 使用有效的数据创建一个节点
			groupIndex = createIndex(row, column, mItemList.at(row)); 
		return groupIndex;
	}
	else //* 有效的父节点
	{
		//* 获取父节点的数据
		FriendViewItem* parentData = static_cast<FriendViewItem*>(parent.internalPointer());
		if(parentData != NULL)
		{
			if(parentData->children.count() > row) //* 使用父节点下的对应row的子对象结构创建一个节点
			{
				QModelIndex childIndex = createIndex(row, column, parentData->children[row]);
				return childIndex;
			}
		}
	}
	//* 没有有效的数据项返回一个无效的节点
	return QModelIndex();

}

QModelIndex CFriendViewModel::parent(const QModelIndex &index) const
{
	qDebug() << "call parent with child  row " << index.row() << " column " << index.column() ;
	//* 如果是无效的节点则返回无效节点
	if(!index.isValid())
	{
		return QModelIndex();
	}

	FriendViewItem* indexData = static_cast<FriendViewItem*>(index.internalPointer());
	for(int i = 0; i < mItemList.count(); i++)
	{
		if(mItemList[i] == indexData) //* 如果是父节点（分组）则返回无效父节点（分组没有父节点）
		{
			return QModelIndex();
		}
		else
		{
			//* 试图判断该节点的父节点是不是mItemList[i]
			for(int ii = 0; ii < mItemList[i]->children.count(); ii++)
			{
				if(indexData == mItemList[i]->children[ii]) //* mItemList[i] 成功匹配为indexData的父节点
				{
					//* 使用mItemList[i]作为数据创建一个父节点
					QModelIndex parentIndex = createIndex(i,0,mItemList[i]);
					return parentIndex;
				}
			}
		}
	}
	//* 未匹配到有效的父节点数据，返回无效节点
	return QModelIndex();
}

int CFriendViewModel::rowCount(const QModelIndex &parent) const
{
	//* 父节点是否有效
	if(!parent.isValid()) //* 无效的父节点（说明是分组）返回父节点个数
	{
		return mItemList.count();
	}
	//*父节点是有效的，获取父节点原始数据
	FriendViewItem* d = static_cast<FriendViewItem*>(parent.internalPointer());
	if(d == NULL)
	{
		return 0;
	}
	//* 父节点的原始数据中的子节点个数即为rowCount
	return d->children.count();
}

void CFriendViewModel::setFriendViewItemList(const FriendViewItemList &li)
{
	mItemList = li;
	layoutChanged(); //* 通知View刷新数据
}

FriendViewItemList CFriendViewModel::friendViewItemList()
{
	return mItemList;
}

QVariant CFriendViewModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/ ) const
{
	//* 只返回header的DisplayRole数据
	if(role == Qt::DisplayRole)
		return QString("this is friend view model.");
	return __super::headerData(section, orientation, role);

}
