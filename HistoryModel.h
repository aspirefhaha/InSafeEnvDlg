#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractItemModel>
#include <qvector.h>

struct NodeInfo
{
    NodeInfo* parent;              // 父节点index
    QString sData;                   // 自身数据
    QVector<NodeInfo*> childNodes;   // 子节点
    NodeInfo(NodeInfo* parentIdx, QString s,int row,int col):parent(parentIdx), sData(s),nRow(row),nCol(col){}
	int nRow;
	int nCol;
};


class HistoryModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit HistoryModel(QObject *parent = 0);

    // 构造父节点下子节点的索引
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    // 通过子节点索引获取父节点索引
    virtual QModelIndex parent(const QModelIndex &child) const override;
    // 获取父节点下子节点的行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    // 获取父节点下子节点列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    // 获取节点数据：包括DisplayRole|TextAlignmentRole等
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
	NodeInfo *m_pRootNode ;
    NodeInfo *m_receiveInfo ;
    NodeInfo *m_replyInfo;
	
};

#endif // HISTORYMODEL_H
