#ifndef CFRIENDVIEWMODEL_H
#define CFRIENDVIEWMODEL_H
 
#include <QAbstractItemModel>
#include <QListView>
 
//* （示例所用）树状节点结构体
struct FriendViewItem
{
    bool isGroup; //* 是否是分组(其实可以使用children.isEmpty来替代)
    QString name; //* 名字
    QList<FriendViewItem*> children; //* 子节点
};

typedef QList<FriendViewItem*> FriendViewItemList;

//*(示例所用)自定义树状model
class CFriendViewModel: public QAbstractItemModel
{
    Q_OBJECT
public:
    CFriendViewModel(QObject* parent = NULL);
    ~CFriendViewModel();
    //* 用来提供View header部分的数据
	virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    //* 返回列数
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    //* 获取节点的数据
    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    //* 获取某一个节点
    virtual QModelIndex	index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    //* 获取某一个节点的parent
    virtual QModelIndex	parent(const QModelIndex & index) const;
    //* 返回行数
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    //* 设置原始数据
    void setFriendViewItemList(const FriendViewItemList& li);
    //* 返回原始数据
    FriendViewItemList friendViewItemList();
private:
    FriendViewItemList mItemList; //* 树状结构的原始数据(父节点list)
};

#endif // CFRIENDVIEWMODEL_H
