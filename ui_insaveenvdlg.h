/********************************************************************************
** Form generated from reading UI file 'insaveenvdlg.ui'
**
** Created: Thu Feb 13 15:33:42 2020
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INSAVEENVDLG_H
#define UI_INSAVEENVDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_InSaveEnvDlgClass
{
public:
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *lb_OutEnv;
    QTreeView *tv_OutEnv;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pb_CopyToLeft;
    QSpacerItem *verticalSpacer;
    QPushButton *pb_CopyToRight;
    QSpacerItem *verticalSpacer_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *lb_InEnv;
    QTreeView *tv_InEnv;
    QFormLayout *formLayout_2;
    QLabel *label;
    QLabel *lb_OutSelected;
    QLabel *label_3;
    QLabel *lb_InSelected;

    void setupUi(QDialog *InSaveEnvDlgClass)
    {
        if (InSaveEnvDlgClass->objectName().isEmpty())
            InSaveEnvDlgClass->setObjectName(QString::fromUtf8("InSaveEnvDlgClass"));
        InSaveEnvDlgClass->setWindowModality(Qt::WindowModal);
        InSaveEnvDlgClass->resize(1074, 682);
        verticalLayout_5 = new QVBoxLayout(InSaveEnvDlgClass);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lb_OutEnv = new QLabel(InSaveEnvDlgClass);
        lb_OutEnv->setObjectName(QString::fromUtf8("lb_OutEnv"));

        verticalLayout_2->addWidget(lb_OutEnv);

        tv_OutEnv = new QTreeView(InSaveEnvDlgClass);
        tv_OutEnv->setObjectName(QString::fromUtf8("tv_OutEnv"));

        verticalLayout_2->addWidget(tv_OutEnv);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        pb_CopyToLeft = new QPushButton(InSaveEnvDlgClass);
        pb_CopyToLeft->setObjectName(QString::fromUtf8("pb_CopyToLeft"));

        verticalLayout->addWidget(pb_CopyToLeft);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer);

        pb_CopyToRight = new QPushButton(InSaveEnvDlgClass);
        pb_CopyToRight->setObjectName(QString::fromUtf8("pb_CopyToRight"));

        verticalLayout->addWidget(pb_CopyToRight);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        lb_InEnv = new QLabel(InSaveEnvDlgClass);
        lb_InEnv->setObjectName(QString::fromUtf8("lb_InEnv"));

        verticalLayout_3->addWidget(lb_InEnv);

        tv_InEnv = new QTreeView(InSaveEnvDlgClass);
        tv_InEnv->setObjectName(QString::fromUtf8("tv_InEnv"));

        verticalLayout_3->addWidget(tv_InEnv);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout_4->addLayout(horizontalLayout);

        formLayout_2 = new QFormLayout();
        formLayout_2->setSpacing(6);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label = new QLabel(InSaveEnvDlgClass);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label);

        lb_OutSelected = new QLabel(InSaveEnvDlgClass);
        lb_OutSelected->setObjectName(QString::fromUtf8("lb_OutSelected"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, lb_OutSelected);

        label_3 = new QLabel(InSaveEnvDlgClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_3);

        lb_InSelected = new QLabel(InSaveEnvDlgClass);
        lb_InSelected->setObjectName(QString::fromUtf8("lb_InSelected"));
        lb_InSelected->setTextFormat(Qt::PlainText);
        lb_InSelected->setScaledContents(true);
        lb_InSelected->setWordWrap(true);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lb_InSelected);


        verticalLayout_4->addLayout(formLayout_2);


        verticalLayout_5->addLayout(verticalLayout_4);


        retranslateUi(InSaveEnvDlgClass);
        QObject::connect(tv_InEnv, SIGNAL(clicked(QModelIndex)), InSaveEnvDlgClass, SLOT(InEnvSelected(QModelIndex)));
        QObject::connect(tv_OutEnv, SIGNAL(clicked(QModelIndex)), InSaveEnvDlgClass, SLOT(OutEnvSelected(QModelIndex)));
        QObject::connect(pb_CopyToLeft, SIGNAL(clicked()), InSaveEnvDlgClass, SLOT(CopyToOutEnv()));
        QObject::connect(pb_CopyToRight, SIGNAL(clicked()), InSaveEnvDlgClass, SLOT(CopyToInEnv()));

        QMetaObject::connectSlotsByName(InSaveEnvDlgClass);
    } // setupUi

    void retranslateUi(QDialog *InSaveEnvDlgClass)
    {
        InSaveEnvDlgClass->setWindowTitle(QApplication::translate("InSaveEnvDlgClass", "InSaveEnvDlg", 0, QApplication::UnicodeUTF8));
        lb_OutEnv->setText(QApplication::translate("InSaveEnvDlgClass", "OutEnv", 0, QApplication::UnicodeUTF8));
        pb_CopyToLeft->setText(QApplication::translate("InSaveEnvDlgClass", "<", 0, QApplication::UnicodeUTF8));
        pb_CopyToRight->setText(QApplication::translate("InSaveEnvDlgClass", ">", 0, QApplication::UnicodeUTF8));
        lb_InEnv->setText(QApplication::translate("InSaveEnvDlgClass", "InEnv", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("InSaveEnvDlgClass", "OutSelected", 0, QApplication::UnicodeUTF8));
        lb_OutSelected->setText(QString());
        label_3->setText(QApplication::translate("InSaveEnvDlgClass", "InSelected", 0, QApplication::UnicodeUTF8));
        lb_InSelected->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class InSaveEnvDlgClass: public Ui_InSaveEnvDlgClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSAVEENVDLG_H
