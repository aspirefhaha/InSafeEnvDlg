#include "HistoryModel.h"
#if 0
HistoryModel::HistoryModel(QObject *parent /*= 0*/)
    : QAbstractItemModel(parent)
{
}

QModelIndex HistoryModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
    // ������ͨ����
    return createIndex(row, column);
}
QModelIndex HistoryModel::parent(const QModelIndex &child) const
{
    // ���ڵ��Ϊ���ڵ�
    return QModelIndex();
}
int HistoryModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    // ���ڵ�����5�У���������û��
    if (parent.row() == -1)
    {
        return 5;
    }
    return 0;
}
int HistoryModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    // ÿ��������
    return 2;
}

QVariant HistoryModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    // �ڵ����ݣ�����룬��ʾ���к�
    if (role == Qt::TextAlignmentRole)
        return int(Qt::AlignLeft | Qt::AlignVCenter); 
    else if (role == Qt::DisplayRole) 
        return QString("row=%1,col=%2").arg(index.row()).arg(index.column());
    else
        return QVariant();
}
#else


HistoryModel::HistoryModel(QObject *parent /*= 0*/)
    : QAbstractItemModel(parent)
{
    // ����root�ڵ�
    m_pRootNode = new NodeInfo(nullptr, "rootNode", -1, -1);
    m_receiveInfo = new NodeInfo(m_pRootNode, "ReceiveMessage", 0, 0);
    m_replyInfo = new NodeInfo(m_pRootNode, "ReplyMessage", 1, 0);

    m_pRootNode->childNodes.append(m_receiveInfo);
    m_pRootNode->childNodes.append(m_replyInfo);
}

QModelIndex HistoryModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
    if (parent.row() == -1 && parent.column() == -1)
    {
        // �ײ�ڵ�󶨹�ϵ
        if (row < m_pRootNode->childNodes.count())
            return createIndex(row, column, m_pRootNode->childNodes[row]);
    }
    else
    {
        // ������ڵ�󶨹�ϵ
        if (parent.internalPointer() != nullptr)
        {
            NodeInfo* pNode = reinterpret_cast<NodeInfo*>(parent.internalPointer());
            if (pNode->childNodes.size() > row)
            {
                return createIndex(row, column, pNode->childNodes[row]);
            } 
        }
    }
    return QModelIndex();
}

QModelIndex HistoryModel::parent(const QModelIndex &child) const
{
    if (child.internalPointer() != nullptr)
    {
        NodeInfo* pNode = reinterpret_cast<NodeInfo*>(child.internalPointer());
        NodeInfo* pParent = pNode->parent;
        if (pParent != nullptr)
        {
            // ���ݸ��ڵ���Ϣ��row/col/node*��ȡIndex
            return createIndex(pParent->nRow, pParent->nCol, pParent);;
        }
    }
    return QModelIndex();
}
int HistoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.internalPointer() == nullptr)
    {
        // ���ڵ��µ���������
        return m_pRootNode->childNodes.count();
    }
    else
    {
        // �ڵ��µ���������
        NodeInfo* pNode = reinterpret_cast<NodeInfo*>(parent.internalPointer());
        return pNode->childNodes.size();
    }
}
int HistoryModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    // ÿ��������
    return 1;
}

QVariant HistoryModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    // �ڵ����ݣ�����룬��ʾ���к�
    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignLeft | Qt::AlignVCenter); 
    }
    else if (role == Qt::DisplayRole) 
    {
        if (index.internalPointer() == 0)
        {
            return QString("row=%1,col=%2").arg(index.row()).arg(index.column());
        }
        else
        {
            NodeInfo* pNode = reinterpret_cast<NodeInfo*>(index.internalPointer());
            return pNode->sData;
        }
    }
    else
    {
        return QVariant();
    }
}

#endif