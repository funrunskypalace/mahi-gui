#pragma once

namespace framework
{

typedef int32_t market_handler_type_i;
const int MHT_ORMDAPI = 1320;
const int MHT_MKTPLUGINMDAPI = 1010;

///////////////////////////////////////////////////////////////////////////////
const int TID_TE_Login = 10101; //登录
/// Send Order TID's
const int TID_TE_ReqOrderInsert = 10201; // 发送订单请求
const int TID_TE_ReqOrderCancel = 10202; // 发送撤单请求
const int TID_TE_RtnOrder = 10203;       // 推送订单响应消息
const int TID_TE_RtnTrade = 10204;       // 推送成交响应消息
const int TID_TE_RspOrderInsert =
    10205; // 订单应答(UFT:提交到柜台接口时立即返回,
           // RspInfo.ErrrorID 用于表明成功或失败)
// const int TID_TE_RspOrderCancel = 10206;        // 柜台撤单确认应答
// const int TID_TE_RspOrderCancelFailure = 10207; // 柜台撤单失败确认应答
const int TID_TE_ErrRtnOrder = 10208;       // 推送订单失败消息
const int TID_TE_ErrRtnOrderCancel = 10209; // 推送撤单失败消息
const int TID_TE_SubMarketData = 10210;     // 请求订阅行情
const int TID_TE_RspSubMarketData = 10211;  // 请求订阅行情回复

const int TID_TE_PushMarket = 15001; // 行情推送

enum MarketDataTagType : int
{
    /*------股票------*/
    /// 连续竞价个股行情快照
    Stock_Tick_Tag = 100,
    /// 虚拟集合竞价个股行情快照
    Virtual_Stock_Tick_Tag = 101,
    /// 存量个股行情快照
    Repository_Stock_Tick_Tag = 109,
    /// 逐笔成交
    Stock_Transaction_Tag = 110,
    /// 逐笔委托
    Stock_Order_Tag = 120,
    /// 委托队列
    Stock_Order_Queue_Tag = 121,

    /*------指数------*/
    /// 指数行情快照
    Index_Tick_Tag = 200,
    /// 存量指数行情快照
    Repository_Index_Tick_Tag = 209,

    /*------期权------*/
    /// 期权行情快照
    Option_Tick_Tag = 300,
    /// 存量期权行情快照
    Repository_Option_Tick_Tag = 309,
    /// 全真模拟期权行情快照
    Simoption_Tick_Tag = 400,
    /// 存量全真模拟期权行情快照
    Repository_Simoption_Tick_Tag = 409,

    /*------期货------*/
    /// 期货行情快照
    Futures_Tick_Tag = 500,
    /// 存量期货行情快照
    Repository_Futures_Tick_Tag = 509,
    /// OTC报价行情
    OTCQuote_Tag = 600,
    /// OTC存量报价行情
    Repository_OTCQuote_Tag = 609,
    /// OTC成交行情
    OTCTransaction_Msg = 610,

    /*--------指数增强--------*/
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

