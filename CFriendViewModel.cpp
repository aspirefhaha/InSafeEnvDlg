
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
	//* ��ʾ����ֻ��һ������
	return 1;
}

QVariant CFriendViewModel::data(const QModelIndex &index, int role) const
{
	qDebug() << "call data with row " << index.row() << " column " << index.column() << " role " << role ;
	//* �������Ч�ڵ��򷵻ؿ�����
	if(!index.isValid())
	{
		return QVariant();
	}
	//* ���Ի�ȡԭʼ����

	FriendViewItem* d = static_cast<FriendViewItem*>(index.internalPointer());

	if(d == NULL)//* û��ԭʼ�����򷵻ؿ�����
	{
		return QVariant();
	}
	if(role == Qt::DisplayRole)//* ����ֻ������DisplayRole������
	{
		return d->name;
	}
	return QVariant();
}



QModelIndex CFriendViewModel::index(int row, int column, const QModelIndex &parent) const
{
	qDebug() << "call index with row" << row << " col " << column  << " parent row " << parent.row() << " column " << parent.column() ;
	//* �����ڵ��Ƿ���Ч
	if(!parent.isValid()) //* ��Ч�ĸ��ڵ�(�ǿ϶��Ƿ���)
	{
		QModelIndex groupIndex;
		if(mItemList.count() > row && row != -1) //* ʹ����Ч�����ݴ���һ���ڵ�
			groupIndex = createIndex(row, column, mItemList.at(row)); 
		return groupIndex;
	}
	else //* ��Ч�ĸ��ڵ�
	{
		//* ��ȡ���ڵ������
		FriendViewItem* parentData = static_cast<FriendViewItem*>(parent.internalPointer());
		if(parentData != NULL)
		{
			if(parentData->children.count() > row) //* ʹ�ø��ڵ��µĶ�Ӧrow���Ӷ���ṹ����һ���ڵ�
			{
				QModelIndex childIndex = createIndex(row, column, parentData->children[row]);
				return childIndex;
			}
		}
	}
	//* û����Ч���������һ����Ч�Ľڵ�
	return QModelIndex();

}

QModelIndex CFriendViewModel::parent(const QModelIndex &index) const
{
	qDebug() << "call parent with child  row " << index.row() << " column " << index.column() ;
	//* �������Ч�Ľڵ��򷵻���Ч�ڵ�
	if(!index.isValid())
	{
		return QModelIndex();
	}

	FriendViewItem* indexData = static_cast<FriendViewItem*>(index.internalPointer());
	for(int i = 0; i < mItemList.count(); i++)
	{
		if(mItemList[i] == indexData) //* ����Ǹ��ڵ㣨���飩�򷵻���Ч���ڵ㣨����û�и��ڵ㣩
		{
			return QModelIndex();
		}
		else
		{
			//* ��ͼ�жϸýڵ�ĸ��ڵ��ǲ���mItemList[i]
			for(int ii = 0; ii < mItemList[i]->children.count(); ii++)
			{
				if(indexData == mItemList[i]->children[ii]) //* mItemList[i] �ɹ�ƥ��ΪindexData�ĸ��ڵ�
				{
					//* ʹ��mItemList[i]��Ϊ���ݴ���һ�����ڵ�
					QModelIndex parentIndex = createIndex(i,0,mItemList[i]);
					return parentIndex;
				}
			}
		}
	}
	//* δƥ�䵽��Ч�ĸ��ڵ����ݣ�������Ч�ڵ�
	return QModelIndex();
}

int CFriendViewModel::rowCount(const QModelIndex &parent) const
{
	//* ���ڵ��Ƿ���Ч
	if(!parent.isValid()) //* ��Ч�ĸ��ڵ㣨˵���Ƿ��飩���ظ��ڵ����
	{
		return mItemList.count();
	}
	//*���ڵ�����Ч�ģ���ȡ���ڵ�ԭʼ����
	FriendViewItem* d = static_cast<FriendViewItem*>(parent.internalPointer());
	if(d == NULL)
	{
		return 0;
	}
	//* ���ڵ��ԭʼ�����е��ӽڵ������ΪrowCount
	return d->children.count();
}

void CFriendViewModel::setFriendViewItemList(const FriendViewItemList &li)
{
	mItemList = li;
	layoutChanged(); //* ֪ͨViewˢ������
}

FriendViewItemList CFriendViewModel::friendViewItemList()
{
	return mItemList;
}

QVariant CFriendViewModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/ ) const
{
	//* ֻ����header��DisplayRole����
	if(role == Qt::DisplayRole)
		return QString("this is friend view model.");
	return __super::headerData(section, orientation, role);

}
