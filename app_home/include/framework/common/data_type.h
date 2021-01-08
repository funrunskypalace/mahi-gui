#pragma once

namespace framework
{

typedef int32_t market_handler_type_i;
const int MHT_ORMDAPI = 1320;
const int MHT_MKTPLUGINMDAPI = 1010;

///////////////////////////////////////////////////////////////////////////////
const int TID_TE_Login = 10101; //��¼
/// Send Order TID's
const int TID_TE_ReqOrderInsert = 10201; // ���Ͷ�������
const int TID_TE_ReqOrderCancel = 10202; // ���ͳ�������
const int TID_TE_RtnOrder = 10203;       // ���Ͷ�����Ӧ��Ϣ
const int TID_TE_RtnTrade = 10204;       // ���ͳɽ���Ӧ��Ϣ
const int TID_TE_RspOrderInsert =
    10205; // ����Ӧ��(UFT:�ύ����̨�ӿ�ʱ��������,
           // RspInfo.ErrrorID ���ڱ����ɹ���ʧ��)
// const int TID_TE_RspOrderCancel = 10206;        // ��̨����ȷ��Ӧ��
// const int TID_TE_RspOrderCancelFailure = 10207; // ��̨����ʧ��ȷ��Ӧ��
const int TID_TE_ErrRtnOrder = 10208;       // ���Ͷ���ʧ����Ϣ
const int TID_TE_ErrRtnOrderCancel = 10209; // ���ͳ���ʧ����Ϣ
const int TID_TE_SubMarketData = 10210;     // ����������
const int TID_TE_RspSubMarketData = 10211;  // ����������ظ�

const int TID_TE_PushMarket = 15001; // ��������

enum MarketDataTagType : int
{
    /*------��Ʊ------*/
    /// �������۸����������
    Stock_Tick_Tag = 100,
    /// ���⼯�Ͼ��۸����������
    Virtual_Stock_Tick_Tag = 101,
    /// ���������������
    Repository_Stock_Tick_Tag = 109,
    /// ��ʳɽ�
    Stock_Transaction_Tag = 110,
    /// ���ί��
    Stock_Order_Tag = 120,
    /// ί�ж���
    Stock_Order_Queue_Tag = 121,

    /*------ָ��------*/
    /// ָ���������
    Index_Tick_Tag = 200,
    /// ����ָ���������
    Repository_Index_Tick_Tag = 209,

    /*------��Ȩ------*/
    /// ��Ȩ�������
    Option_Tick_Tag = 300,
    /// ������Ȩ�������
    Repository_Option_Tick_Tag = 309,
    /// ȫ��ģ����Ȩ�������
    Simoption_Tick_Tag = 400,
    /// ����ȫ��ģ����Ȩ�������
    Repository_Simoption_Tick_Tag = 409,

    /*------�ڻ�------*/
    /// �ڻ��������
    Futures_Tick_Tag = 500,
    /// �����ڻ��������
    Repository_Futures_Tick_Tag = 509,
    /// OTC��������
    OTCQuote_Tag = 600,
    /// OTC������������
    Repository_OTCQuote_Tag = 609,
    /// OTC�ɽ�����
    OTCTransaction_Msg = 610,

    /*--------ָ����ǿ--------*/
    EH_Custom_Tag_Min = 10000,
    EH_IC500_Tick_Tag = 10500,
    EH_IC500_Idx500_Tag = 10510,
    EH_01Stock_Tick_Tag,
    EH_01Futures_Tick_Tag,
    EH_IC_ETF_Diff_Tag,
    Multi_Road_Input_Tag,
    Multi_Road_Output_Tag,
    EH_Custom_Tag_Max = 19999
};

} // namespace framework

