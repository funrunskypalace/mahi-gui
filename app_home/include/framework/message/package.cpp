#include "pch/compat.h"

#include "framework/message/package.h"

namespace framework
{
namespace message
{

CPackageBuffer::CPackageBuffer(int nSize)
{
    m_nBufferSize = nSize;
    m_pHead = new char[m_nBufferSize];
    memset(m_pHead, 0, m_nBufferSize);
    m_bReleaseData = true;
    m_nRefCount = 1;
}

CPackageBuffer::~CPackageBuffer()
{
    if (m_bReleaseData)
    {
        delete[] m_pHead;
        m_pHead = NULL;
    }
}

char* CPackageBuffer::data() { return m_pHead; }

int CPackageBuffer::length() { return m_nBufferSize; }

CPackage::CPackage()
{
    packageBuffer_ = NULL;

    pHead_ = pTail_ = NULL;

    reserve = 0;
}

CPackage::~CPackage()
{
    if (packageBuffer_ != NULL)
    {
        delete packageBuffer_;
        packageBuffer_ = NULL;
        pHead_ = pTail_ = NULL;
    }
}

void CPackage::constructAllocate(int nMaxData, int nReserve)
{
    if (packageBuffer_ != NULL)
        delete packageBuffer_;

    packageBuffer_ = new CPackageBuffer(nMaxData + nReserve);

    reserve = nReserve;

    clear();
}

char* CPackage::pushMax(void)
{
    if (packageBuffer_ == NULL)
        return NULL;

    char* tmp = pTail_;
    pTail_ = packageBuffer_->data() + packageBuffer_->length();

    return tmp;
}

char* CPackage::push(int nSize)
{
    if (pTail_ + nSize > packageBuffer_->data() + packageBuffer_->length())
        return NULL;

    char* tmp = pTail_;
    pTail_ += nSize;

    return tmp;
}

bool CPackage::pop(int nSize)
{
    if (length() - reserve < nSize)
        return false;

    pTail_ -= nSize;
    return true;
}

void CPackage::clear()
{
    if (packageBuffer_ == NULL)
    {
        pHead_ = pTail_ = NULL;
    }

    pHead_ = packageBuffer_->data();
    pTail_ = pHead_ + reserve;
}

void CPackage::truncate(int nNewSize)
{
    if (length() > nNewSize)
    {
        pTail_ = pHead_ + nNewSize;
    }
}

bool CPackage::makePackage() { return 0; }

int CPackage::validPackage() { return 0; }

int CPackage::getTID() { return 0; }

int CPackage::getTopicID() { return 0; }

char CPackage::getPackageType() { return '\0'; }

} // namespace message
} // namespace framework
