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

    //��biz����serialize��data��������
    virtual bool packBody(char* data, int len) PURE;
    // biz�������л���ĳ���
    virtual int getBodyLen() PURE;
    //�ӻ������з����л�biz����
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
    ///����һ���ռ�,����ĳ���Ϊ body + nReserve
    ///@param nSize : body�ĳ���
    ///@param nReserve : Ԥ���ĳ���
    void constructAllocate(int nSize, int nReserve);

    ///��ȡ�ռ䣬ֻ����nNewSize�Ĵ�С������nReserve��
    void truncate(int nNewSize);

    inline int length();
    inline char* address();

public:
    ///��buffer������һ�����Ŀռ�
    ///@return : ����ռ�ĵ�ַ
    char* pushMax(void);

    ///��buffer������nSize�Ŀռ�
    ///@return : NULL ����ռ䲻�㣻 ����Ϊ����ռ�ĵ�ַ
    char* push(int nSize);

    ///�ӿռ����ͷ�nSize�Ŀռ䡣 Ϊջ���������Ƚ��ȳ���
    ///������pop��ϵͳʣ��nReserve�Ŀռ�
    bool pop(int nSize);
    ///������ݣ�ֻ����reserve�ռ�
    void clear();

    inline char* bodyAddress();
    inline int bodyLength();

public:
    /// Biz method
    /**ҵ��ϵͳ��������������������Զ���
     */
    virtual bool makePackage();

    /**ҵ��ϵͳ�Ľ�����������������Զ���
     */
    virtual int validPackage();

    /**��ȡ���׵���Ϣ����
     */
    virtual int getTID();

    /**��ȡ���׵���Ϣ���⡣ֻ�Է�����Ϣ�����塣
     */
    virtual int getTopicID();

    /**��ȡ�������͡�
     */
    virtual char getPackageType();

    /**
     * ���ñ��Ĵ�����š���ʱ�����(һ��ȡflow�ĵ�ǰcount)��
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

/**�������ݣ�����reserver���ĳ���
 */
int CPackage::length() { return (int)(pTail_ - pHead_); }

/**�������ݣ�����reserver������ʼ����
 */
char* CPackage::address() { return pHead_; }

/**�޳�reserve����ʼ����
 */
char* CPackage::bodyAddress() { return pHead_ + reserve; }

int CPackage::bodyLength() { return (int)(pTail_ - pHead_ - reserve); }

} // namespace message
} // namespace framework
