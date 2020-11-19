#ifndef CSYSPARAM_H
#define CSYSPARAM_H
#include<QString>
#include<vector>
#include<QComboBox>
/*
功能：系统参数对应表
*/

//单独对象
class  CParamItem
{
public:
    CParamItem( QString  strName = "" , int  iID = -1, short  fValue = 0.0);
    CParamItem( const CParamItem &old );
    const CParamItem &operator=(const CParamItem & old);
    ~CParamItem();
public:
    void     SetName(QString  strName);
    QString  GetName();
    void     SetID(int  iID);
    int      GetID();
    void     SetValue(short fValue);
    short    GetValue();
private:
    int      m_ID;
    float    m_Value;
    char     m_Name[20];
};

//常规舵机参数
class  CNormSteeringEngineParam
{
public:
    CNormSteeringEngineParam();
    CNormSteeringEngineParam( const CNormSteeringEngineParam & old );
    const  CNormSteeringEngineParam &operator=( const CNormSteeringEngineParam & old );
    ~CNormSteeringEngineParam();
public:
    //脉宽上限 control_pulse_upper
    CParamItem    m_UpperPulseWidthlimit ;
    //脉宽中点 control_pulse_mid
    CParamItem    m_MiddlePulseWidthlimit ;
    //脉宽下限 control_pulse_lower
    CParamItem    m_DownPulseWidthlimit ;
    //启动电压 启动电压
    CParamItem    m_StartingVoltage;
    //驱动频率 servo_motor_work_pwm_cycle
    CParamItem    m_DrivingFrequency;
    //死区(范围) servo_zero_zone_set
    CParamItem    m_DeadZone;
    //自锁设置 servo_free_lock_flag
    CParamItem    m_Self_lockingSetting;
    //堵转保护(堵转区间先按100 ,选中后这个参数下发是100,没选中下发是0) servo_halt_zone_set
    CParamItem    m_Locked_rotorProtection;
    //信号复位 - 复位使能 servo_reset_flag_set
    CParamItem    m_SignalReset;
    //保护时间 servo_protect_time_set
    CParamItem    m_ProtectionTime;
    //舵机方向 信号方向 servo_dir_inverse_flag
    CParamItem    m_SteeringGearDirection;
    //保护输出 servo_protect_pwm_set
    CParamItem    m_ProtectionOutput;
    //角度上限 control_set_upper
    CParamItem    m_UpperLimitOfAngle;
    //角度中点 control_set_mid
    CParamItem    m_AngularMidpoint;
    //角度下限 control_set_lower
    CParamItem    m_LowerAngleLimit;
    //最大输出 servo_max_pwm_set
    CParamItem    m_MaximumOutput;
    //频率设置 motor_pwm_cycle
    CParamItem    m_FrequencySetting;
public:
    //设置值
    void   SetValue(short  sValue);
    //判断-1
    bool   Jump();
};
//串口舵机参数
class  CCOMMSteeringEngineParam
{
public:
    CCOMMSteeringEngineParam();
    CCOMMSteeringEngineParam( const CCOMMSteeringEngineParam & old );
    const  CCOMMSteeringEngineParam &operator=( const CCOMMSteeringEngineParam & old );
    ~CCOMMSteeringEngineParam();
public:
    //目前ID
    CParamItem    m_CurID;
    //脉宽上限
    CParamItem    m_UpperPulseWidthlimit ;
    //脉宽中点
    CParamItem    m_MiddlePulseWidthlimit ;
    //脉宽下限
    CParamItem    m_DownPulseWidthlimit ;
    //启动电压
    CParamItem    m_StartingVoltage;
    //驱动频率
    CParamItem    m_DrivingFrequency;
    //死区
    CParamItem    m_DeadZone;
    //自锁设置
    CParamItem    m_Self_lockingSetting;
    //堵转保护
    CParamItem    m_Locked_rotorProtection;
    //信号复位
    CParamItem    m_SignalReset;
    //保护时间
    CParamItem    m_ProtectionTime;
    //舵机方向
    CParamItem    m_SteeringGearDirection;
    //保护输出
    CParamItem    m_ProtectionOutput;
    //新设ID
    CParamItem    m_NewID;
    //角度上限
    CParamItem    m_UpperLimitOfAngle;
    //角度中点
    CParamItem    m_AngularMidpoint;
    //角度下限
    CParamItem    m_LowerAngleLimit;
    //最大输出
    CParamItem    m_MaximumOutput;
    //频率设置
    CParamItem    m_FrequencySetting;
public:
    //设置值
    void   SetValue(short  sValue);
    //判断-1
    bool   Jump();
};

//常规舵机测试
class  CNormSteeringEngineTest
{
public:
    CNormSteeringEngineTest();
    CNormSteeringEngineTest( const CNormSteeringEngineTest & old );
    const  CNormSteeringEngineTest &operator=( const CNormSteeringEngineTest & old );
    ~CNormSteeringEngineTest();
public:
    //值
    CParamItem   m_Value;
    //最大值
    CParamItem   m_MaxValue;
    //中间值
    CParamItem   m_MiddleValue;
    //最小值
    CParamItem   m_MinValue;
    //位置 servo_position_value_now
    CParamItem   m_Position;
    //电压 servo_vddin_value_now
    CParamItem   m_Voltage;
    //温度 servo_tempture_value_now
    CParamItem   m_Temperature;
    //电流
    CParamItem   m_ElectricWave;
};
//串口舵机测试
class  CCOMMSteeringEngineTest
{
public:
    CCOMMSteeringEngineTest();
    CCOMMSteeringEngineTest( const CCOMMSteeringEngineTest & old );
    const  CCOMMSteeringEngineTest &operator=( const CCOMMSteeringEngineTest & old );
    ~CCOMMSteeringEngineTest();
public:
    //舵机ID - 8
    CParamItem   m_ID[8];
    //位置1 - 8
    CParamItem   m_Position1[8];
    //位置2 - 8
    CParamItem   m_Position2[8];
    //速度  - 8
    CParamItem   m_Speed[8];
    //停止时间
    CParamItem   m_StopTime;
    //运行次数
    CParamItem   m_NumberOfRuns;
};

//附加信息
class  CAdditionalVariable
{
public:
    CAdditionalVariable();
    CAdditionalVariable( const CAdditionalVariable & old );
    const  CAdditionalVariable &operator=( const CAdditionalVariable & old );
    ~CAdditionalVariable();
public:
    //电机方向 servo_pwm_inverse_flag
    CParamItem   m_MotorDirection;
    //信号容差 insig_value_diff_set
    CParamItem   m_SignalTolerance;
    //程序版本 servo_program_version
    CParamItem   m_ProgramVersion;
    //掉电使能 servo_epprom_parm_reset_flag
    CParamItem   m_PowerDownEnable;
    //舵机ID servo_unique_address_id_set
    CParamItem   m_SteeringGearID;
    //通用ID servo_common_command_address_set
    CParamItem   m_UniversalID;
    //复位位置 servo_reset_position
    CParamItem   m_ResetPosition;
    //复位时间 yes
    CParamItem   m_ResetTime;
    //复位步长 servo_reset_step_set
    CParamItem   m_ResetStepSize;
    //启动时间 control_interval_time_set
    CParamItem   m_StartTime;
    //启动步长 control_interval_step_set
    CParamItem   m_StartingStepSize;
    //堵转时间 servo_halt_time_set
    CParamItem   m_BlockingTime;
    //上电复位 servo_init_flag_set
    CParamItem   m_Power_onReset;
public:
    //设置值
    void   SetValue(short  sValue);
    //判断-1
    bool   Jump();
};

//默认参数
class  CSysLoseParam
{
public:
    CSysLoseParam();
    CSysLoseParam( const  CSysLoseParam  & old );
    const  CSysLoseParam &operator=(const CSysLoseParam &old);
    ~CSysLoseParam();
public:
    //模式锁定 servo_work_mode_lock_flag_set 0
    CParamItem   servo_work_mode_lock_flag_set;
    //模式设置 servo_work_mode_now 2
    CParamItem   servo_work_mode_now;
    //参数范围 servo_pram_config_set 4
    CParamItem   servo_pram_config_set;
    //反馈使能 servo_command_fb_flag 1
    CParamItem   servo_command_fb_flag;

    //死区时间 servo_zero_zone_time  1
    CParamItem   servo_zero_zone_time;
    //保护比较 servo_protect_pwm_cmpt 1000
    CParamItem   servo_protect_pwm_cmpt;

    //位置比例 servo_position_pid_parm_p_set 1
    CParamItem   servo_position_pid_parm_p_set[2];
    //位置比率 position_pid_speed_parm_radio 2
    CParamItem   position_pid_speed_parm_radio;
    //速度比率 servo_speed_run_sample_k_set 4
    CParamItem   servo_speed_run_sample_k_set[2];
    //位置时间 servo_position_sample_ov_time_set 16
    CParamItem   servo_position_sample_ov_time_set;
    //速度时间 servo_speed_sample_ov_time_set 32
    CParamItem   servo_speed_sample_ov_time_set;
    //速度比例 servo_speed_pid_parm_p_set 2
    CParamItem   servo_speed_pid_parm_p_set[2];
    //速度增量 servo_speed_pid_parm_p_radio 1
    CParamItem   servo_speed_pid_parm_p_radio;
public:
    //设置值
    void   SetValue(short  sValue);
    //判断-1
    bool   Jump();
};
//系统参数
class  CSysParam
{
public:
    CSysParam();
    CSysParam( const  CSysParam  & old );
    const  CSysParam &operator=(const CSysParam &old);
    ~CSysParam();
public:
    //常规舵机参数
    CNormSteeringEngineParam m_NormSteeringEngineParam;
    //串口舵机参数
    CCOMMSteeringEngineParam m_COMMSteeringEngineParam;
    //常规舵机测试
    CNormSteeringEngineTest  m_NormSteeringEngineTest;
    //串口舵机测试
    CCOMMSteeringEngineTest  m_COMMSteeringEngineTest;
    //附加信息
    CAdditionalVariable      m_AdditionalVariable;
    //默认参数
    CSysLoseParam            m_SysLoseParam;
};

#endif // CSYSPARAM_H
