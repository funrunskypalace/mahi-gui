#pragma once
#include "pch/compat.h"

#include "framework/common/common.h"
#include "oos/OpenOrderApi.h"

#include <memory>
#include <mutex>

using namespace cycnoches::ort;

namespace framework
{
namespace trade
{

typedef int32_t or_execstatus_i;
///δ֪
const int OR_EST_Unkfalsewn = 0x01;
///���ύ
const int OR_EST_PendingNew = 0x04;
///�ѽ���
const int OR_EST_New = 0x08;
///���ֳɽ�
const int OR_EST_PartilyFilled = 0x10;
///ȫ���ɽ�
const int OR_EST_Filled = 0x20;
///���ֳɽ����ֳ���
const int OR_EST_PartilyCanceled = 0x40;
///�ѳ���
const int OR_EST_Canceled = 0x80;
///�Ѿܾ�
const int OR_EST_Rejected = 0x100;
///�ѷ��ͳ���
const int OR_EST_Cancelling = 0x200;
///�ѷ���ί��
const int OR_EST_Sending = 0x400;
const int OR_EST_ALL = 0xFFFFFFFF;

struct OrderPackInfoField;
using OrderPackInfoFieldPtr = std::shared_ptr<OrderPackInfoField>;

class OrderPackInfo
{
public:
    OrderPackInfo(or_channelid_i channelId, OROrderReqField& orderReq,
                  or_execstatus_i targetExecStatus, bool suppressing);

    OrderPackInfo();

    /// @param TargetExecStatus������������ִ��״̬��
    /// - ע�⣺��������������մﵽ���״̬����������Ż��������
    ///   TargetExecStatus���������ж��Ƿ�Terminated��
    void initialize(or_channelid_i channelId, or_investorid_i invesotrId,
                    or_productclass_e pc, const std::string& exId,
                    const std::string& insId, or_direction_e dir,
                    or_offsetflag_e offset, or_hedgeflag_e hedge,
                    or_volume_i volume, or_price_f price,
                    or_orderpricetype_e pt, or_timecondition_e tc,
                    or_contingentcondition_e cc, or_volumecondition_e vc,
                    or_execstatus_i targetExecStatus, bool suppressing,
                    const std::string& orderRef = "");

    void setRequestID(or_orrequestid_i requestId);
    std::string getOrderRef() const;
    void setOrderRef(const std::string& orderRef);
    std::string getInternalOrderRef() const;
    void setInternalOrderRef(const std::string& orderLocalId);
    or_channelid_i getChannelID() const;
    or_investorid_i getInvestorID() const;
    or_orrequestid_i getRequestID();
    OROrderReqField getOrderReq(bool getOriginal) const;
    bool isSuppressing() const;
    or_execstatus_i targetExecStatus() const;
    void setReSend(bool needReSend /*, int MaxSendTimes*/);
    bool needReSend() const;
    std::string description() const;
    or_volume_i originalVolume() const;
    or_price_f originalPrice() const;
    or_volume_i volumeTradedSum() const;
    or_money_f amountTradedSum() const;
    void addVolumeTraded(or_volume_i volume, or_money_f amount);
    or_price_f limitPrice() const;
    void updateInsert(or_volume_i volume, or_price_f limitPrice);
    bool meetTargetExecStatus(or_execstatus_i execStatus) const;

private:
    OrderPackInfoFieldPtr field_;
};
using OrderPackInfoPtr = std::shared_ptr<OrderPackInfo>;
using OrderPackList = std::list<OrderPackInfoPtr>;

} // namespace trade
} // namespace framework