#pragma once

#include "framework/common/common.h"

namespace framework
{
namespace message
{

const int PACK_MAX_LEN = 16 * 1024;

class IBiz
{
public:
    IBiz(){};
    virtual ~IBiz(){};

    //将biz对象serialize到data缓冲区中
    virtual bool packBody(char* data, int len) PURE;
    // biz对象序列化后的长度
    virtual int getBodyLen() PURE;
    //从缓冲区中反序列化biz对象
    virtual bool unpackBody(const char* data, int len) PURE;
};

class GWSDK_API CPackageBuffer
{
public:
    CPackageBuffer(int nSize);

    ~CPackageBuffer();

public:
    char* data();

    int length();

private:
    char* m_pHead;

    int m_nBufferSize;

    int m_nRefCount;

    bool m_bReleaseData;
};

class GWSDK_API CPackage
{
public:
    CPackage();
    virtual ~CPackage();

public:
    ///构造一个空间,整体的长度为 body + nReserve
    ///@param nSize : body的长度
    ///@param nReserve : 预留的长度
    void constructAllocate(int nSize, int nReserve);

    ///截取空间，只留下nNewSize的大小（包括nReserve）
    void truncate(int nNewSize);

    inline int length();
    inline char* address();

public:
    ///从buffer内申请一个最大的空间
    ///@return : 申请空间的地址
    char* pushMax(void);

    ///从buffer内申请nSize的空间
    ///@return : NULL 如果空间不足； 否则为申请空间的地址
    char* push(int nSize);

    ///从空间中释放nSize的空间。 为栈操作，即先进先出。
    ///至多能pop到系统剩余nReserve的空间
    bool pop(int nSize);
    ///清空内容，只留下reserve空间
    void clear();

    inline char* bodyAddress();
    inline int bodyLength();

public:
    /// Biz method
    /**业务系统的组包操作。由派生类自定义
     */
    virtual bool makePackage();

    /**业务系统的解包操作。由派生类自定义
     */
    virtual int validPackage();

    /**获取交易的消息类型
     */
    virtual int getTID();

    /**获取交易的消息主题。只对发布消息有意义。
     */
    virtual int getTopicID();

    /**获取包的类型。
     */
    virtual char getPackageType();

    /**
     * 设置报文处理序号。按时间递增(一般取flow的当前count)。
     */
    virtual void setLastSn(int64_t lastSn) PURE;

    virtual const char* getTradeInterfaceName() const = 0;

protected:
    char* pHead_;
    char* pTail_;
    int reserve;
    CPackageBuffer* packageBuffer_;
};

using CPackagePtr = std::shared_ptr<CPackage>;

/**整个内容（包含reserver）的长度
 */
int CPackage::length() { return (int)(pTail_ - pHead_); }

/**整个内容（包含reserver）的起始长度
 */
char* CPackage::address() { return pHead_; }

/**剔除reserve的起始长度
 */
char* CPackage::bodyAddress() { return pHead_ + reserve; }

int CPackage::bodyLength() { return (int)(pTail_ - pHead_ - reserve); }

} // namespace message
} // namespace framework
