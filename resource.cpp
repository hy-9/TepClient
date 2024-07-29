#include "resource.h"
#include "protocol.h"
#include "tcpclient.h"
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>

Resource::Resource(QWidget *parent)
    : QWidget{parent}
{

    m_pResListW = new QListWidget;
    m_pReturnPB = new QPushButton("返回");
    m_pCreateDirPB = new QPushButton("创建文件夹");
    m_pDelDirPB = new QPushButton("删除文件夹");
    m_pRenamePB = new QPushButton("重命名文件夹");
    m_pFlushFilePB = new QPushButton("刷新文件夹");

    QVBoxLayout *pDirVBL = new QVBoxLayout;
    pDirVBL->addWidget(m_pReturnPB);
    pDirVBL->addWidget(m_pCreateDirPB);
    pDirVBL->addWidget(m_pDelDirPB);
    pDirVBL->addWidget(m_pRenamePB);
    pDirVBL->addWidget(m_pFlushFilePB);

    m_pUploadPB = new QPushButton("上传文件");
    m_pDownLoadPB = new QPushButton("下载文件");
    m_pDelFilePB = new QPushButton("删除文件");
    m_pShareFilePB = new QPushButton("共享文件");

    QVBoxLayout *pFileVBL = new QVBoxLayout;
    pFileVBL->addWidget(m_pUploadPB);
    pFileVBL->addWidget(m_pDownLoadPB);
    pFileVBL->addWidget(m_pDelFilePB);
    pFileVBL->addWidget(m_pShareFilePB);

    QHBoxLayout *pMain = new QHBoxLayout;
    pMain->addWidget(m_pResListW);
    pMain->addLayout(pDirVBL);
    pMain->addLayout(pFileVBL);

    setLayout(pMain);
    connect(m_pCreateDirPB, SIGNAL(clicked())
            , this, SLOT(createDir()));
}

void Resource::createDir()
{
    QString strNewDir = QInputDialog::getText(this, "创建文件夹", "文件夹名");
    if (strNewDir.isEmpty()) {
        QMessageBox::warning(this, "创建文件夹", "文件夹名不能为空");
        return ;
    }
    if (strNewDir.size()>32) {
        QMessageBox::warning(this, "创建文件夹", "文件夹名过长");
        return ;
    }
    PDU *pdu = mkPDU(TcpClient::getInstance()->m_strCurPuath.size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_CREAT_DIR_REQUEST;
    memcpy(pdu->caData, TcpClient::getInstance()->m_strName.toStdString().c_str()
           , TcpClient::getInstance()->m_strName.size());
    memcpy(pdu->caData+32, strNewDir.toStdString().c_str()
           , strNewDir.size());
    memcpy(pdu->caMsg, TcpClient::getInstance()->m_strCurPuath.toStdString().c_str()
           , TcpClient::getInstance()->m_strCurPuath.size());
    TcpClient::getInstance()->getTcpSocket()->write((char *)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = NULL;

}
