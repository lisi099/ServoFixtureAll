#include "csysparam.h"
#include<QDateTime>

CParamItem::CParamItem( QString  strName , int  iID, short  fValue)
{
    memcpy(m_Name,strName.toLatin1().data(),strName.toLatin1().size());
    m_ID = iID;
    m_Value = fValue;
}
CParamItem::CParamItem( const CParamItem &old )
{
    memcpy(m_Name,old.m_Name,20);
    m_ID = old.m_ID;
    m_Value = old.m_Value;
}
const CParamItem &CParamItem::operator=(const CParamItem & old)
{
    memcpy(m_Name,old.m_Name,20);
    m_ID = old.m_ID;
    m_Value = old.m_Value;
    return  *this;
}
CParamItem::~CParamItem()
{
    m_ID = -1;
    m_Value = 0.0;
}
void     CParamItem::SetName(QString  strName)
{
    memcpy(m_Name,strName.toLatin1().data(),strName.toLatin1().size());
}
QString  CParamItem::GetName()
{
    return  m_Name;
}
void     CParamItem::SetID(int  iID)
{
    m_ID = iID;
}
int      CParamItem::GetID()
{
    return  m_ID;
}
void     CParamItem::SetValue(short fValue)
{
    m_Value = fValue;
}
short    CParamItem::GetValue()
{
    return  m_Value;
}


//设置值
void   CNormSteeringEngineParam::SetValue(short  sValue)
{
    //脉宽上限
    m_UpperPulseWidthlimit = CParamItem(QStringLiteral("脉宽上限"),0,sValue) ;
    //脉宽中点
    m_MiddlePulseWidthlimit = CParamItem(QStringLiteral("脉宽中点"),0,sValue) ;
    //脉宽下限
    m_DownPulseWidthlimit = CParamItem(QStringLiteral("脉宽下限"),0,sValue) ;
    //启动电压
    m_StartingVoltage= CParamItem(QStringLiteral("启动电压"),0,sValue) ;
    //驱动频率
    m_DrivingFrequency= CParamItem(QStringLiteral("驱动频率"),0,sValue) ;
    //死区
    m_DeadZone= CParamItem(QStringLiteral("死区"),0,sValue) ;
    //自锁设置
    m_Self_lockingSetting= CParamItem(QStringLiteral("自锁设置"),0,sValue) ;
    //堵转保护
    m_Locked_rotorProtection= CParamItem(QStringLiteral("堵转保护"),0,sValue) ;
    //信号复位
    m_SignalReset= CParamItem(QStringLiteral("信号复位"),0,sValue) ;
    //保护时间
    m_ProtectionTime= CParamItem(QStringLiteral("保护时间"),0,sValue) ;
    //舵机方向
    m_SteeringGearDirection= CParamItem(QStringLiteral("舵机方向"),0,sValue) ;
    //保护输出
    m_ProtectionOutput= CParamItem(QStringLiteral("保护输出"),0,sValue) ;
    //角度上限
    m_UpperLimitOfAngle= CParamItem(QStringLiteral("角度上限"),0,sValue) ;
    //角度中点
    m_AngularMidpoint= CParamItem(QStringLiteral("角度中点"),0,sValue) ;
    //角度下限
    m_LowerAngleLimit= CParamItem(QStringLiteral("角度下限"),0,sValue) ;
    //最大输出
    m_MaximumOutput= CParamItem(QStringLiteral("最大输出"),0,sValue) ;
    //频率设置
    m_FrequencySetting= CParamItem(QStringLiteral("频率设置"),0,sValue) ;
}
//判断-1
bool   CNormSteeringEngineParam::Jump()
{
    //脉宽上限
    if( m_UpperPulseWidthlimit.GetValue() == -1 )
        return  false;

    //脉宽中点
    if( m_MiddlePulseWidthlimit.GetValue() == -1 )
        return  false;
    //脉宽下限
    if( m_DownPulseWidthlimit.GetValue() == -1 )
        return  false;
    //启动电压
    if( m_StartingVoltage.GetValue() == -1 )
        return  false;
    //驱动频率
    if( m_DrivingFrequency.GetValue() == -1 )
        return  false;
    //死区
    if( m_DeadZone.GetValue() == -1 )
        return  false;
    //自锁设置
    if( m_Self_lockingSetting.GetValue() == -1 )
        return  false;
    //堵转保护
    if( m_Locked_rotorProtection.GetValue() == -1 )
        return  false;
    //信号复位
    if( m_SignalReset.GetValue() == -1 )
        return  false;
    //保护时间
    if( m_ProtectionTime.GetValue() == -1 )
        return  false;
    //舵机方向
    if( m_SteeringGearDirection.GetValue() == -1 )
        return  false;
    //保护输出
    if( m_ProtectionOutput.GetValue() == -1 )
        return  false;
    //角度上限
    if( m_UpperLimitOfAngle.GetValue() == -1 )
        return  false;
    //角度中点
    if( m_AngularMidpoint.GetValue() == -1 )
        return  false;
    //角度下限
    if( m_LowerAngleLimit.GetValue() == -1 )
        return  false;
    //最大输出
    if( m_MaximumOutput.GetValue() == -1 )
        return  false;
    //频率设置
    if( m_FrequencySetting.GetValue() == -1 )
        return  false;

    return  true;
}

CNormSteeringEngineParam::CNormSteeringEngineParam()
{
    //脉宽上限
    m_UpperPulseWidthlimit = CParamItem(QStringLiteral("脉宽上限"),0,2200) ;
    //脉宽中点
    m_MiddlePulseWidthlimit = CParamItem(QStringLiteral("脉宽中点"),0,1500) ;
    //脉宽下限
    m_DownPulseWidthlimit = CParamItem(QStringLiteral("脉宽下限"),0,800) ;
    //启动电压
    m_StartingVoltage= CParamItem(QStringLiteral("启动电压"),0,9) ;
    //驱动频率
    m_DrivingFrequency= CParamItem(QStringLiteral("驱动频率"),0,3) ;
    //死区
    m_DeadZone= CParamItem(QStringLiteral("死区"),0,20) ;
    //自锁设置
    m_Self_lockingSetting= CParamItem(QStringLiteral("自锁设置"),0,1) ;
    //堵转保护
    m_Locked_rotorProtection= CParamItem(QStringLiteral("堵转保护"),0,5) ;
    //信号复位
    m_SignalReset= CParamItem(QStringLiteral("信号复位"),0,1) ;
    //保护时间
    m_ProtectionTime= CParamItem(QStringLiteral("保护时间"),0,101) ;
    //舵机方向
    m_SteeringGearDirection= CParamItem(QStringLiteral("舵机方向"),0,0) ;
    //保护输出
    m_ProtectionOutput= CParamItem(QStringLiteral("保护输出"),0,1020) ;
    //角度上限
    m_UpperLimitOfAngle= CParamItem(QStringLiteral("角度上限"),0,3548) ;
    //角度中点
    m_AngularMidpoint= CParamItem(QStringLiteral("角度中点"),0,2048) ;
    //角度下限
    m_LowerAngleLimit= CParamItem(QStringLiteral("角度下限"),0,548) ;
    //最大输出
    m_MaximumOutput= CParamItem(QStringLiteral("最大输出"),0,1023) ;
    //频率设置
    m_FrequencySetting= CParamItem(QStringLiteral("频率设置"),0,0) ;
}
CNormSteeringEngineParam::CNormSteeringEngineParam( const CNormSteeringEngineParam & old )
{
    //脉宽上限
    m_UpperPulseWidthlimit = old.m_UpperPulseWidthlimit ;
    //脉宽中点
    m_MiddlePulseWidthlimit = old.m_MiddlePulseWidthlimit ;
    //脉宽下限
    m_DownPulseWidthlimit = old.m_DownPulseWidthlimit ;
    //启动电压
    m_StartingVoltage= old.m_StartingVoltage ;
    //驱动频率
    m_DrivingFrequency= old.m_DrivingFrequency ;
    //死区
    m_DeadZone= old.m_DeadZone ;
    //自锁设置
    m_Self_lockingSetting= old.m_Self_lockingSetting ;
    //堵转保护
    m_Locked_rotorProtection= old.m_Locked_rotorProtection ;
    //信号复位
    m_SignalReset= old.m_SignalReset ;
    //保护时间
    m_ProtectionTime= old.m_ProtectionTime ;
    //舵机方向
    m_SteeringGearDirection= old.m_SteeringGearDirection ;
    //保护输出
    m_ProtectionOutput= old.m_ProtectionOutput ;
    //角度上限
    m_UpperLimitOfAngle= old.m_UpperLimitOfAngle ;
    //角度中点
    m_AngularMidpoint= old.m_AngularMidpoint ;
    //角度下限
    m_LowerAngleLimit= old.m_LowerAngleLimit ;
    //最大输出
    m_MaximumOutput= old.m_MaximumOutput ;
    //频率设置
    m_FrequencySetting= old.m_FrequencySetting ;
}
const  CNormSteeringEngineParam &CNormSteeringEngineParam::operator=( const CNormSteeringEngineParam & old )
{
    //脉宽上限
    m_UpperPulseWidthlimit = old.m_UpperPulseWidthlimit ;
    //脉宽中点
    m_MiddlePulseWidthlimit = old.m_MiddlePulseWidthlimit ;
    //脉宽下限
    m_DownPulseWidthlimit = old.m_DownPulseWidthlimit ;
    //启动电压
    m_StartingVoltage= old.m_StartingVoltage ;
    //驱动频率
    m_DrivingFrequency= old.m_DrivingFrequency ;
    //死区
    m_DeadZone= old.m_DeadZone ;
    //自锁设置
    m_Self_lockingSetting= old.m_Self_lockingSetting ;
    //堵转保护
    m_Locked_rotorProtection= old.m_Locked_rotorProtection ;
    //信号复位
    m_SignalReset= old.m_SignalReset ;
    //保护时间
    m_ProtectionTime= old.m_ProtectionTime ;
    //舵机方向
    m_SteeringGearDirection= old.m_SteeringGearDirection ;
    //保护输出
    m_ProtectionOutput= old.m_ProtectionOutput ;
    //角度上限
    m_UpperLimitOfAngle= old.m_UpperLimitOfAngle ;
    //角度中点
    m_AngularMidpoint= old.m_AngularMidpoint ;
    //角度下限
    m_LowerAngleLimit= old.m_LowerAngleLimit ;
    //最大输出
    m_MaximumOutput= old.m_MaximumOutput ;
    //频率设置
    m_FrequencySetting= old.m_FrequencySetting ;
    return  *this;
}
CNormSteeringEngineParam::~CNormSteeringEngineParam()
{

}


CCOMMSteeringEngineParam::CCOMMSteeringEngineParam()
{
    //目前ID
    m_CurID = CParamItem(QStringLiteral("目前ID"),0,10) ;
    //新设ID
    m_NewID= CParamItem(QStringLiteral("新设ID"),0,0) ;

    //脉宽上限
    m_UpperPulseWidthlimit = CParamItem(QStringLiteral("脉宽上限"),0,2200) ;
    //脉宽中点
    m_MiddlePulseWidthlimit = CParamItem(QStringLiteral("脉宽中点"),0,1500) ;
    //脉宽下限
    m_DownPulseWidthlimit = CParamItem(QStringLiteral("脉宽下限"),0,800) ;
    //启动电压
    m_StartingVoltage= CParamItem(QStringLiteral("启动电压"),0,9) ;
    //驱动频率
    m_DrivingFrequency= CParamItem(QStringLiteral("驱动频率"),0,3) ;
    //死区
    m_DeadZone= CParamItem(QStringLiteral("死区"),0,20) ;
    //自锁设置
    m_Self_lockingSetting= CParamItem(QStringLiteral("自锁设置"),0,1) ;
    //堵转保护
    m_Locked_rotorProtection= CParamItem(QStringLiteral("堵转保护"),0,5) ;
    //信号复位
    m_SignalReset= CParamItem(QStringLiteral("信号复位"),0,1) ;
    //保护时间
    m_ProtectionTime= CParamItem(QStringLiteral("保护时间"),0,101) ;
    //舵机方向
    m_SteeringGearDirection= CParamItem(QStringLiteral("舵机方向"),0,0) ;
    //保护输出
    m_ProtectionOutput= CParamItem(QStringLiteral("保护输出"),0,1020) ;
    //角度上限
    m_UpperLimitOfAngle= CParamItem(QStringLiteral("角度上限"),0,3548) ;
    //角度中点
    m_AngularMidpoint= CParamItem(QStringLiteral("角度中点"),0,2048) ;
    //角度下限
    m_LowerAngleLimit= CParamItem(QStringLiteral("角度下限"),0,548) ;
    //最大输出
    m_MaximumOutput= CParamItem(QStringLiteral("最大输出"),0,1023) ;
    //频率设置
    m_FrequencySetting= CParamItem(QStringLiteral("频率设置"),0,0) ;
}
CCOMMSteeringEngineParam::CCOMMSteeringEngineParam( const CCOMMSteeringEngineParam & old )
{
    //目前ID
    m_CurID = old.m_CurID ;
    //脉宽上限
    m_UpperPulseWidthlimit = old.m_UpperPulseWidthlimit ;
    //脉宽中点
    m_MiddlePulseWidthlimit = old.m_MiddlePulseWidthlimit ;
    //脉宽下限
    m_DownPulseWidthlimit = old.m_DownPulseWidthlimit ;
    //启动电压
    m_StartingVoltage= old.m_StartingVoltage ;
    //驱动频率
    m_DrivingFrequency= old.m_DrivingFrequency ;
    //死区
    m_DeadZone= old.m_DeadZone ;
    //自锁设置
    m_Self_lockingSetting= old.m_Self_lockingSetting ;
    //堵转保护
    m_Locked_rotorProtection= old.m_Locked_rotorProtection ;
    //信号复位
    m_SignalReset= old.m_SignalReset ;
    //保护时间
    m_ProtectionTime= old.m_ProtectionTime ;
    //舵机方向
    m_SteeringGearDirection= old.m_SteeringGearDirection ;
    //保护输出
    m_ProtectionOutput= old.m_ProtectionOutput ;
    //目前ID
    m_NewID = old.m_NewID ;
    //角度上限
    m_UpperLimitOfAngle= old.m_UpperLimitOfAngle ;
    //角度中点
    m_AngularMidpoint= old.m_AngularMidpoint ;
    //角度下限
    m_LowerAngleLimit= old.m_LowerAngleLimit ;
    //最大输出
    m_MaximumOutput= old.m_MaximumOutput ;
    //频率设置
    m_FrequencySetting= old.m_FrequencySetting ;
}
const  CCOMMSteeringEngineParam &CCOMMSteeringEngineParam::operator=( const CCOMMSteeringEngineParam & old )
{
    //目前ID
    m_CurID = old.m_CurID ;
    //脉宽上限
    m_UpperPulseWidthlimit = old.m_UpperPulseWidthlimit ;
    //脉宽中点
    m_MiddlePulseWidthlimit = old.m_MiddlePulseWidthlimit ;
    //脉宽下限
    m_DownPulseWidthlimit = old.m_DownPulseWidthlimit ;
    //启动电压
    m_StartingVoltage= old.m_StartingVoltage ;
    //驱动频率
    m_DrivingFrequency= old.m_DrivingFrequency ;
    //死区
    m_DeadZone= old.m_DeadZone ;
    //自锁设置
    m_Self_lockingSetting= old.m_Self_lockingSetting ;
    //堵转保护
    m_Locked_rotorProtection= old.m_Locked_rotorProtection ;
    //信号复位
    m_SignalReset= old.m_SignalReset ;
    //保护时间
    m_ProtectionTime= old.m_ProtectionTime ;
    //舵机方向
    m_SteeringGearDirection= old.m_SteeringGearDirection ;
    //保护输出
    m_ProtectionOutput= old.m_ProtectionOutput ;
    //目前ID
    m_NewID = old.m_NewID ;
    //角度上限
    m_UpperLimitOfAngle= old.m_UpperLimitOfAngle ;
    //角度中点
    m_AngularMidpoint= old.m_AngularMidpoint ;
    //角度下限
    m_LowerAngleLimit= old.m_LowerAngleLimit ;
    //最大输出
    m_MaximumOutput= old.m_MaximumOutput ;
    //频率设置
    m_FrequencySetting= old.m_FrequencySetting ;
    return  *this;
}
CCOMMSteeringEngineParam::~CCOMMSteeringEngineParam()
{

}
//判断-1
bool   CCOMMSteeringEngineParam::Jump()
{
    //脉宽上限
    if(m_UpperPulseWidthlimit.GetValue() == -1)
        return false;

    //脉宽中点
    if(m_MiddlePulseWidthlimit.GetValue() == -1)
        return false;

    //脉宽下限
    if(m_DownPulseWidthlimit.GetValue() == -1)
        return false;

    //启动电压
    if(m_StartingVoltage.GetValue() == -1)
        return false;

    //驱动频率
    if(m_DrivingFrequency.GetValue() == -1)
        return false;

    //死区
    if(m_DeadZone.GetValue() == -1)
        return false;

    //自锁设置
    if(m_Self_lockingSetting.GetValue() == -1)
        return false;

    //堵转保护
    if(m_Locked_rotorProtection.GetValue() == -1)
        return false;

    //信号复位
    if(m_SignalReset.GetValue() == -1)
        return false;

    //保护时间
    if(m_ProtectionTime.GetValue() == -1)
        return false;

    //舵机方向
    if(m_SteeringGearDirection.GetValue() == -1)
        return false;

    //保护输出
    if(m_ProtectionOutput.GetValue() == -1)
        return false;

    //角度上限
    if(m_UpperLimitOfAngle.GetValue() == -1)
        return false;

    //角度中点
    if(m_AngularMidpoint.GetValue() == -1)
        return false;

    //角度下限
    if(m_LowerAngleLimit.GetValue() == -1)
        return false;

    //最大输出
    if(m_MaximumOutput.GetValue() == -1)
        return false;

    //频率设置
    if(m_FrequencySetting.GetValue() == -1)
        return false;

    return true;
 }
//设置值
void   CCOMMSteeringEngineParam::SetValue(short  sValue)
{
    //脉宽上限
    m_UpperPulseWidthlimit = CParamItem(QStringLiteral("脉宽上限"),0,sValue) ;
    //脉宽中点
    m_MiddlePulseWidthlimit = CParamItem(QStringLiteral("脉宽中点"),0,sValue) ;
    //脉宽下限
    m_DownPulseWidthlimit = CParamItem(QStringLiteral("脉宽下限"),0,sValue) ;
    //启动电压
    m_StartingVoltage= CParamItem(QStringLiteral("启动电压"),0,sValue) ;
    //驱动频率
    m_DrivingFrequency= CParamItem(QStringLiteral("驱动频率"),0,sValue) ;
    //死区
    m_DeadZone= CParamItem(QStringLiteral("死区"),0,sValue) ;
    //自锁设置
    m_Self_lockingSetting= CParamItem(QStringLiteral("自锁设置"),0,sValue) ;
    //堵转保护
    m_Locked_rotorProtection= CParamItem(QStringLiteral("堵转保护"),0,sValue) ;
    //信号复位
    m_SignalReset= CParamItem(QStringLiteral("信号复位"),0,sValue) ;
    //保护时间
    m_ProtectionTime= CParamItem(QStringLiteral("保护时间"),0,sValue) ;
    //舵机方向
    m_SteeringGearDirection= CParamItem(QStringLiteral("舵机方向"),0,sValue) ;
    //保护输出
    m_ProtectionOutput= CParamItem(QStringLiteral("保护输出"),0,sValue) ;
    //角度上限
    m_UpperLimitOfAngle= CParamItem(QStringLiteral("角度上限"),0,sValue) ;
    //角度中点
    m_AngularMidpoint= CParamItem(QStringLiteral("角度中点"),0,sValue) ;
    //角度下限
    m_LowerAngleLimit= CParamItem(QStringLiteral("角度下限"),0,sValue) ;
    //最大输出
    m_MaximumOutput= CParamItem(QStringLiteral("最大输出"),0,sValue) ;
    //频率设置
    m_FrequencySetting= CParamItem(QStringLiteral("频率设置"),0,sValue) ;
}
//

CNormSteeringEngineTest::CNormSteeringEngineTest()
{
    //值
    m_Value= CParamItem(QStringLiteral("值"),0,500) ;
    //最大值
    m_MaxValue= CParamItem(QStringLiteral("最大值"),0,2500) ;
    //中间值
    m_MiddleValue= CParamItem(QStringLiteral("中间值"),0,1500) ;
    //最小值
    m_MinValue= CParamItem(QStringLiteral("最小值"),0,500) ;
    //位置
    m_Position= CParamItem(QStringLiteral("位置"),0,0) ;
    //电压
    m_Voltage= CParamItem(QStringLiteral("电压"),0,0) ;
    //温度
    m_Temperature= CParamItem(QStringLiteral("温度"),0,0) ;
    //电波
    m_ElectricWave= CParamItem(QStringLiteral("电波"),0,0) ;
}
CNormSteeringEngineTest::CNormSteeringEngineTest( const CNormSteeringEngineTest & old )
{
    //值
    m_Value=old.m_Value;
    //最大值
    m_MaxValue= old.m_MaxValue;
    //中间值
    m_MiddleValue= old.m_MiddleValue;
    //最小值
    m_MinValue= old.m_MinValue;
    //位置
    m_Position= old.m_Position;
    //电压
    m_Voltage= old.m_Voltage;
    //温度
    m_Temperature= old.m_Temperature;
    //电波
    m_ElectricWave= old.m_ElectricWave;
}
const  CNormSteeringEngineTest &CNormSteeringEngineTest::operator=( const CNormSteeringEngineTest & old )
{
    //值
    m_Value=old.m_Value;
    //最大值
    m_MaxValue= old.m_MaxValue;
    //中间值
    m_MiddleValue= old.m_MiddleValue;
    //最小值
    m_MinValue= old.m_MinValue;
    //位置
    m_Position= old.m_Position;
    //电压
    m_Voltage= old.m_Voltage;
    //温度
    m_Temperature= old.m_Temperature;
    //电波
    m_ElectricWave= old.m_ElectricWave;
    return  *this;
}
CNormSteeringEngineTest::~CNormSteeringEngineTest()
{

}


CCOMMSteeringEngineTest::CCOMMSteeringEngineTest()
{
    for( int i = 0 ; i < 8 ;i++ )
    {
        //舵机ID - 8
        m_ID[i]= CParamItem(QStringLiteral("舵机ID"),0,i) ;
        //位置1 - 8
        m_Position1[i]= CParamItem(QStringLiteral("位置1"),0,500) ;
        //位置2 - 8
        m_Position2[i]= CParamItem(QStringLiteral("位置2"),0,1500) ;
        //速度  - 8
        m_Speed[i]= CParamItem(QStringLiteral("速度"),0,10) ;
    }
    //停止时间
    m_StopTime= CParamItem(QStringLiteral("停止时间"),0,100) ;
    //运行次数
    m_NumberOfRuns= CParamItem(QStringLiteral("运行次数"),0,10) ;
}
CCOMMSteeringEngineTest::CCOMMSteeringEngineTest( const CCOMMSteeringEngineTest & old )
{
    for( int i = 0 ; i < 8 ;i++ )
    {
        //舵机ID - 8
        m_ID[i]= old.m_ID[i];
        //位置1 - 8
        m_Position1[i]= old.m_Position1[i];
        //位置2 - 8
        m_Position2[i]= old.m_Position2[i];
        //速度  - 8
        m_Speed[i]= old.m_Speed[i];
    }
    //停止时间
    m_StopTime= old.m_StopTime;
    //运行次数
    m_NumberOfRuns= old.m_NumberOfRuns;
}
const  CCOMMSteeringEngineTest &CCOMMSteeringEngineTest::operator=( const CCOMMSteeringEngineTest & old )
{
    for( int i = 0 ; i < 8 ;i++ )
    {
        //舵机ID - 8
        m_ID[i]= old.m_ID[i];
        //位置1 - 8
        m_Position1[i]= old.m_Position1[i];
        //位置2 - 8
        m_Position2[i]= old.m_Position2[i];
        //速度  - 8
        m_Speed[i]= old.m_Speed[i];
    }
    //停止时间
    m_StopTime= old.m_StopTime;
    //运行次数
    m_NumberOfRuns= old.m_NumberOfRuns;
    return  *this;
}
CCOMMSteeringEngineTest::~CCOMMSteeringEngineTest()
{

}
//判断-1
bool   CAdditionalVariable::Jump()
{
    //电机方向
    if( m_MotorDirection.GetValue() == -1 )
        return  false;

    //信号容差
    if( m_SignalTolerance.GetValue() == -1 )
        return  false;

    //程序版本
    if( m_ProgramVersion.GetValue() == -1 )
        return  false;

    //掉电使能
    if( m_PowerDownEnable.GetValue() == -1 )
        return  false;

    //舵机ID
    if( m_SteeringGearID.GetValue() == -1 )
        return  false;

    //通用ID
    if( m_UniversalID.GetValue() == -1 )
        return  false;

    //复位位置
    if( m_ResetPosition.GetValue() == -1 )
        return  false;

    //复位时间
    if( m_ResetTime.GetValue() == -1 )
        return  false;

    //复位步长
    if( m_ResetStepSize.GetValue() == -1 )
        return  false;

    //启动时间
    if( m_StartTime.GetValue() == -1 )
        return  false;

    //启动步长
    if( m_StartingStepSize.GetValue() == -1 )
        return  false;

    //堵转时间
    if( m_BlockingTime.GetValue() == -1 )
        return  false;

    //上电复位
    if( m_Power_onReset.GetValue() == -1 )
        return  false;

    return  true;
}
    //设置值
void   CAdditionalVariable::SetValue(short  sValue)
{
    //电机方向
    m_MotorDirection= CParamItem(QStringLiteral("电机方向"),0,sValue) ;
    //信号容差
    m_SignalTolerance= CParamItem(QStringLiteral("信号容差"),0,sValue) ;
    //程序版本
    m_ProgramVersion= CParamItem(QStringLiteral("程序版本"),0,sValue) ;
    //掉电使能
    m_PowerDownEnable= CParamItem(QStringLiteral("掉电使能"),0,sValue) ;
    //舵机ID
    m_SteeringGearID= CParamItem(QStringLiteral("舵机ID"),0,sValue) ;
    //通用ID
    m_UniversalID= CParamItem(QStringLiteral("通用ID"),0,sValue) ;
    //复位位置
    m_ResetPosition= CParamItem(QStringLiteral("复位位置"),0,sValue) ;
    //复位时间
    m_ResetTime= CParamItem(QStringLiteral("复位时间"),0,sValue) ;
    //复位步长
    m_ResetStepSize= CParamItem(QStringLiteral("复位步长"),0,sValue) ;
    //启动时间
    m_StartTime= CParamItem(QStringLiteral("启动时间"),0,sValue) ;
    //启动步长
    m_StartingStepSize= CParamItem(QStringLiteral("启动步长"),0,sValue) ;
    //堵转时间
    m_BlockingTime= CParamItem(QStringLiteral("堵转时间"),0,sValue) ;
    //上电复位
    m_Power_onReset= CParamItem(QStringLiteral("上电复位"),0,sValue) ;
}

CAdditionalVariable::CAdditionalVariable()
{
    //电机方向
    m_MotorDirection= CParamItem(QStringLiteral("电机方向"),0,0) ;
    //信号容差
    m_SignalTolerance= CParamItem(QStringLiteral("信号容差"),0,2) ;
    //程序版本
    m_ProgramVersion= CParamItem(QStringLiteral("程序版本"),0,2) ;
    //掉电使能
    m_PowerDownEnable= CParamItem(QStringLiteral("掉电使能"),0,0) ;
    //舵机ID
    m_SteeringGearID= CParamItem(QStringLiteral("舵机ID"),0,2) ;
    //通用ID
    m_UniversalID= CParamItem(QStringLiteral("通用ID"),0,2) ;
    //复位位置
    m_ResetPosition= CParamItem(QStringLiteral("复位位置"),0,1500) ;
    //复位时间
    m_ResetTime= CParamItem(QStringLiteral("复位时间"),0,10) ;
    //复位步长
    m_ResetStepSize= CParamItem(QStringLiteral("复位步长"),0,1) ;
    //启动时间
    m_StartTime= CParamItem(QStringLiteral("启动时间"),0,1) ;
    //启动步长
    m_StartingStepSize= CParamItem(QStringLiteral("启动步长"),0,10) ;
    //堵转时间
    m_BlockingTime= CParamItem(QStringLiteral("堵转时间"),0,100) ;
    //上电复位
    m_Power_onReset= CParamItem(QStringLiteral("上电复位"),0,8) ;
}
CAdditionalVariable::CAdditionalVariable( const CAdditionalVariable & old )
{
    //电机方向
    m_MotorDirection= old.m_MotorDirection;
    //信号容差
    m_SignalTolerance= old.m_SignalTolerance;
    //程序版本
    m_ProgramVersion= old.m_ProgramVersion;
    //掉电使能
    m_PowerDownEnable= old.m_PowerDownEnable;
    //舵机ID
    m_SteeringGearID=old.m_SteeringGearID;
    //通用ID
    m_UniversalID=old.m_UniversalID;
    //复位位置
    m_ResetPosition= old.m_ResetPosition;
    //复位时间
    m_ResetTime= old.m_ResetTime;
    //复位步长
    m_ResetStepSize= old.m_ResetStepSize;
    //启动时间
    m_StartTime= old.m_StartTime;
    //启动步长
    m_StartingStepSize= old.m_StartingStepSize;
    //堵转时间
    m_BlockingTime= old.m_BlockingTime;
    //上电复位
    m_Power_onReset=old.m_Power_onReset;
}
const  CAdditionalVariable &CAdditionalVariable::operator=( const CAdditionalVariable & old )
{
    //电机方向
    m_MotorDirection= old.m_MotorDirection;
    //信号容差
    m_SignalTolerance= old.m_SignalTolerance;
    //程序版本
    m_ProgramVersion= old.m_ProgramVersion;
    //掉电使能
    m_PowerDownEnable= old.m_PowerDownEnable;
    //舵机ID
    m_SteeringGearID=old.m_SteeringGearID;
    //通用ID
    m_UniversalID=old.m_UniversalID;
    //复位位置
    m_ResetPosition= old.m_ResetPosition;
    //复位时间
    m_ResetTime= old.m_ResetTime;
    //复位步长
    m_ResetStepSize= old.m_ResetStepSize;
    //启动时间
    m_StartTime= old.m_StartTime;
    //启动步长
    m_StartingStepSize= old.m_StartingStepSize;
    //堵转时间
    m_BlockingTime= old.m_BlockingTime;
    //上电复位
    m_Power_onReset=old.m_Power_onReset;
    return  *this;
}
CAdditionalVariable::~CAdditionalVariable()
{

}
//判断-1
bool   CSysLoseParam::Jump()
{
    //模式锁定 servo_work_mode_lock_flag_set 0
    if( servo_work_mode_lock_flag_set.GetValue() == -1 )
        return  false;

    //模式设置 servo_work_mode_now 2
    if( servo_work_mode_now.GetValue() == -1 )
        return  false;

    //参数范围 servo_pram_config_set 4
    if( servo_pram_config_set.GetValue() == -1 )
        return  false;

    //反馈使能 servo_command_fb_flag 1
    if( servo_command_fb_flag.GetValue() == -1 )
        return  false;

    //死区时间 servo_zero_zone_time  1
    if( servo_zero_zone_time.GetValue() == -1 )
        return  false;

    //保护比较 servo_protect_pwm_cmpt 1000
    if( servo_protect_pwm_cmpt.GetValue() == -1 )
        return  false;

    //位置比例 servo_position_pid_parm_p_set 1
    if( servo_position_pid_parm_p_set[0].GetValue() == -1 )
        return  false;

    //位置比率 position_pid_speed_parm_radio 2
    if( position_pid_speed_parm_radio.GetValue() == -1 )
        return  false;

    //速度比率 servo_speed_run_sample_k_set 4
    if( servo_speed_run_sample_k_set[0].GetValue() == -1 )
        return  false;

    //位置时间 servo_position_sample_ov_time_set 16
    if( servo_position_sample_ov_time_set.GetValue() == -1 )
        return  false;

    //速度时间 servo_speed_sample_ov_time_set 32
    if( servo_speed_sample_ov_time_set.GetValue() == -1 )
        return  false;

    //速度比例 servo_speed_pid_parm_p_set 2
    if( servo_speed_pid_parm_p_set[0].GetValue() == -1 )
        return  false;

    //速度增量 servo_speed_pid_parm_p_radio 1
    if( servo_speed_pid_parm_p_radio.GetValue() == -1 )
        return  false;

    return  true;

}
//设置值
void   CSysLoseParam::SetValue(short  sValue)
{
    //模式锁定 servo_work_mode_lock_flag_set 0
    servo_work_mode_lock_flag_set= CParamItem(QStringLiteral("模式锁定"),0,sValue) ;
    //模式设置 servo_work_mode_now 2
    servo_work_mode_now= CParamItem(QStringLiteral("模式设置"),0,sValue) ;
    //参数范围 servo_pram_config_set 4
    servo_pram_config_set= CParamItem(QStringLiteral("参数范围"),0,sValue) ;
    //反馈使能 servo_command_fb_flag 1
    servo_command_fb_flag= CParamItem(QStringLiteral("反馈使能"),0,sValue) ;
    //死区时间 servo_zero_zone_time  1
    servo_zero_zone_time= CParamItem(QStringLiteral("死区时间"),0,sValue) ;
    //保护比较 servo_protect_pwm_cmpt 1000
    servo_protect_pwm_cmpt= CParamItem(QStringLiteral("保护比较"),0,sValue) ;
    //位置比例 servo_position_pid_parm_p_set 1
    servo_position_pid_parm_p_set[0] = CParamItem(QStringLiteral("位置比例"),0,sValue) ;
    servo_position_pid_parm_p_set[1] = CParamItem(QStringLiteral("位置比例"),0,sValue) ;
    //位置比率 position_pid_speed_parm_radio 2
    position_pid_speed_parm_radio= CParamItem(QStringLiteral("位置比率"),0,sValue) ;
    //速度比率 servo_speed_run_sample_k_set 4
    servo_speed_run_sample_k_set[0]= CParamItem(QStringLiteral("速度比率"),0,sValue) ;
    servo_speed_run_sample_k_set[1]= CParamItem(QStringLiteral("速度比率"),0,sValue) ;
    //位置时间 servo_position_sample_ov_time_set 16
    servo_position_sample_ov_time_set= CParamItem(QStringLiteral("位置时间"),0,sValue) ;
    //速度时间 servo_speed_sample_ov_time_set 32
    servo_speed_sample_ov_time_set= CParamItem(QStringLiteral("速度时间"),0,sValue) ;
    //速度比例 servo_speed_pid_parm_p_set 2
    servo_speed_pid_parm_p_set[0]= CParamItem(QStringLiteral("速度比例"),0,sValue) ;
    servo_speed_pid_parm_p_set[1]= CParamItem(QStringLiteral("速度比例"),0,sValue) ;
    //速度增量 servo_speed_pid_parm_p_radio 1
    servo_speed_pid_parm_p_radio= CParamItem(QStringLiteral("速度增量"),0,sValue) ;
}
//
CSysLoseParam::CSysLoseParam()
{
    //模式锁定 servo_work_mode_lock_flag_set 0
    servo_work_mode_lock_flag_set= CParamItem(QStringLiteral("模式锁定"),0,0) ;
    //模式设置 servo_work_mode_now 2
    servo_work_mode_now= CParamItem(QStringLiteral("模式设置"),0,2) ;
    //参数范围 servo_pram_config_set 4
    servo_pram_config_set= CParamItem(QStringLiteral("参数范围"),0,4) ;
    //反馈使能 servo_command_fb_flag 1
    servo_command_fb_flag= CParamItem(QStringLiteral("反馈使能"),0,1) ;
    //死区时间 servo_zero_zone_time  1
    servo_zero_zone_time= CParamItem(QStringLiteral("死区时间"),0,1) ;
    //保护比较 servo_protect_pwm_cmpt 1000
    servo_protect_pwm_cmpt= CParamItem(QStringLiteral("保护比较"),0,1000) ;
    //位置比例 servo_position_pid_parm_p_set 1
    servo_position_pid_parm_p_set[0]= CParamItem(QStringLiteral("位置比例"),0,1) ;
    servo_position_pid_parm_p_set[1]= CParamItem(QStringLiteral("位置比例"),0,1) ;
    //位置比率 position_pid_speed_parm_radio 2
    position_pid_speed_parm_radio= CParamItem(QStringLiteral("位置比率"),0,2) ;
    //速度比率 servo_speed_run_sample_k_set 4
    servo_speed_run_sample_k_set[0]= CParamItem(QStringLiteral("速度比率"),0,4) ;
    servo_speed_run_sample_k_set[1]= CParamItem(QStringLiteral("速度比率"),0,4) ;
    //位置时间 servo_position_sample_ov_time_set 16
    servo_position_sample_ov_time_set= CParamItem(QStringLiteral("位置时间"),0,16) ;
    //速度时间 servo_speed_sample_ov_time_set 32
    servo_speed_sample_ov_time_set= CParamItem(QStringLiteral("速度时间"),0,32) ;
    //速度比例 servo_speed_pid_parm_p_set 2
    servo_speed_pid_parm_p_set[0]= CParamItem(QStringLiteral("速度比例"),0,2) ;
    servo_speed_pid_parm_p_set[1]= CParamItem(QStringLiteral("速度比例"),0,2) ;
    //速度增量 servo_speed_pid_parm_p_radio 1
    servo_speed_pid_parm_p_radio= CParamItem(QStringLiteral("速度增量"),0,1) ;
}
CSysLoseParam::CSysLoseParam( const  CSysLoseParam  & old )
{
    //模式锁定 servo_work_mode_lock_flag_set 0
    servo_work_mode_lock_flag_set= old.servo_work_mode_lock_flag_set;
    //模式设置 servo_work_mode_now 2
    servo_work_mode_now= old.servo_work_mode_now;
    //参数范围 servo_pram_config_set 4
    servo_pram_config_set= old.servo_pram_config_set;
    //反馈使能 servo_command_fb_flag 1
    servo_command_fb_flag= old.servo_command_fb_flag;
    //死区时间 servo_zero_zone_time  1
    servo_zero_zone_time= old.servo_zero_zone_time;
    //保护比较 servo_protect_pwm_cmpt 1000
    servo_protect_pwm_cmpt= old.servo_protect_pwm_cmpt;
    //位置比例 servo_position_pid_parm_p_set 1
    servo_position_pid_parm_p_set[0]=old.servo_position_pid_parm_p_set[0];
    servo_position_pid_parm_p_set[1]=old.servo_position_pid_parm_p_set[1];
    //位置比率 position_pid_speed_parm_radio 2
    position_pid_speed_parm_radio= old.position_pid_speed_parm_radio;
    //速度比率 servo_speed_run_sample_k_set 4
    servo_speed_run_sample_k_set[0]=old.servo_speed_run_sample_k_set[0];
    servo_speed_run_sample_k_set[1]=old.servo_speed_run_sample_k_set[1];
    //位置时间 servo_position_sample_ov_time_set 16
    servo_position_sample_ov_time_set= old.servo_position_sample_ov_time_set;
    //速度时间 servo_speed_sample_ov_time_set 32
    servo_speed_sample_ov_time_set=old.servo_speed_sample_ov_time_set;
    //速度比例 servo_speed_pid_parm_p_set 2
    servo_speed_pid_parm_p_set[0]= old.servo_speed_pid_parm_p_set[0];
    servo_speed_pid_parm_p_set[1]= old.servo_speed_pid_parm_p_set[1];
    //速度增量 servo_speed_pid_parm_p_radio 1
    servo_speed_pid_parm_p_radio= old.servo_speed_pid_parm_p_radio;
}
const  CSysLoseParam &CSysLoseParam::operator=(const CSysLoseParam &old)
{
    //模式锁定 servo_work_mode_lock_flag_set 0
    servo_work_mode_lock_flag_set= old.servo_work_mode_lock_flag_set;
    //模式设置 servo_work_mode_now 2
    servo_work_mode_now= old.servo_work_mode_now;
    //参数范围 servo_pram_config_set 4
    servo_pram_config_set= old.servo_pram_config_set;
    //反馈使能 servo_command_fb_flag 1
    servo_command_fb_flag= old.servo_command_fb_flag;
    //死区时间 servo_zero_zone_time  1
    servo_zero_zone_time= old.servo_zero_zone_time;
    //保护比较 servo_protect_pwm_cmpt 1000
    servo_protect_pwm_cmpt= old.servo_protect_pwm_cmpt;
    //位置比例 servo_position_pid_parm_p_set 1
    servo_position_pid_parm_p_set[0]=old.servo_position_pid_parm_p_set[0];
    servo_position_pid_parm_p_set[1]=old.servo_position_pid_parm_p_set[1];
    //位置比率 position_pid_speed_parm_radio 2
    position_pid_speed_parm_radio= old.position_pid_speed_parm_radio;
    //速度比率 servo_speed_run_sample_k_set 4
    servo_speed_run_sample_k_set[0]=old.servo_speed_run_sample_k_set[0];
    servo_speed_run_sample_k_set[1]=old.servo_speed_run_sample_k_set[1];
    //位置时间 servo_position_sample_ov_time_set 16
    servo_position_sample_ov_time_set= old.servo_position_sample_ov_time_set;
    //速度时间 servo_speed_sample_ov_time_set 32
    servo_speed_sample_ov_time_set=old.servo_speed_sample_ov_time_set;
    //速度比例 servo_speed_pid_parm_p_set 2
    servo_speed_pid_parm_p_set[0]= old.servo_speed_pid_parm_p_set[0];
    servo_speed_pid_parm_p_set[1]= old.servo_speed_pid_parm_p_set[1];
    //速度增量 servo_speed_pid_parm_p_radio 1
    servo_speed_pid_parm_p_radio= old.servo_speed_pid_parm_p_radio;

    return *this;
}
CSysLoseParam::~CSysLoseParam()
{

}



CSysParam::CSysParam()
{

}
CSysParam::CSysParam( const  CSysParam  & old )
{
    //常规舵机参数
    m_NormSteeringEngineParam = old.m_NormSteeringEngineParam;
    //串口舵机参数
    m_COMMSteeringEngineParam = old.m_COMMSteeringEngineParam;
    //常规舵机测试
    m_NormSteeringEngineTest = old.m_NormSteeringEngineTest;
    //串口舵机测试
    m_COMMSteeringEngineTest = old.m_COMMSteeringEngineTest;
    //附加信息
    m_AdditionalVariable = old.m_AdditionalVariable;

    //附加信息
    m_SysLoseParam = old.m_SysLoseParam;
}
const  CSysParam &CSysParam::operator=(const CSysParam &old)
{
    //常规舵机参数
    m_NormSteeringEngineParam = old.m_NormSteeringEngineParam;
    //串口舵机参数
    m_COMMSteeringEngineParam = old.m_COMMSteeringEngineParam;
    //常规舵机测试
    m_NormSteeringEngineTest = old.m_NormSteeringEngineTest;
    //串口舵机测试
    m_COMMSteeringEngineTest = old.m_COMMSteeringEngineTest;
    //附加信息
    m_AdditionalVariable = old.m_AdditionalVariable;

    //附加信息
    m_SysLoseParam = old.m_SysLoseParam;
    return *this;
}
CSysParam::~CSysParam()
{

}




