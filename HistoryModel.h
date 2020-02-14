#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractItemModel>
#include <qvector.h>

struct NodeInfo
{
    NodeInfo* parent;              // ���ڵ�index
    QString sData;                   // ��������
    QVector<NodeInfo*> childNodes;   // �ӽڵ�
    NodeInfo(NodeInfo* parentIdx, QString s,int row,int col):parent(parentIdx), sData(s),nRow(row),nCol(col){}
	int nRow;
	int nCol;
};


class HistoryModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit HistoryModel(QObject *parent = 0);

    // ���츸�ڵ����ӽڵ������
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    // ͨ���ӽڵ�������ȡ���ڵ�����
    virtual QModelIndex parent(const QModelIndex &child) const override;
    // ��ȡ���ڵ����ӽڵ������
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    // ��ȡ���ڵ����ӽڵ�����
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    // ��ȡ�ڵ����ݣ�����DisplayRole|TextAlignmentRole��
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
	NodeInfo *m_pRootNode ;
    NodeInfo *m_receiveInfo ;
    NodeInfo *m_replyInfo;
	
};

#endif // HISTORYMODEL_H
