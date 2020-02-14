#ifndef CFRIENDVIEWMODEL_H
#define CFRIENDVIEWMODEL_H
 
#include <QAbstractItemModel>
#include <QListView>
 
//* ��ʾ�����ã���״�ڵ�ṹ��
struct FriendViewItem
{
    bool isGroup; //* �Ƿ��Ƿ���(��ʵ����ʹ��children.isEmpty�����)
    QString name; //* ����
    QList<FriendViewItem*> children; //* �ӽڵ�
};

typedef QList<FriendViewItem*> FriendViewItemList;

//*(ʾ������)�Զ�����״model
class CFriendViewModel: public QAbstractItemModel
{
    Q_OBJECT
public:
    CFriendViewModel(QObject* parent = NULL);
    ~CFriendViewModel();
    //* �����ṩView header���ֵ�����
	virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    //* ��������
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    //* ��ȡ�ڵ������
    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    //* ��ȡĳһ���ڵ�
    virtual QModelIndex	index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    //* ��ȡĳһ���ڵ��parent
    virtual QModelIndex	parent(const QModelIndex & index) const;
    //* ��������
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    //* ����ԭʼ����
    void setFriendViewItemList(const FriendViewItemList& li);
    //* ����ԭʼ����
    FriendViewItemList friendViewItemList();
private:
    FriendViewItemList mItemList; //* ��״�ṹ��ԭʼ����(���ڵ�list)
};

#endif // CFRIENDVIEWMODEL_H
