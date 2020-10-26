#include"mainui.h"
#include "ui_mainui.h"
#include<QTime>
#include<QDebug>
#include<QCoreApplication>
#include"CSysDefine.h"
#include"ccrcthread.h"
//发送舵机位置速度
void        MainUI::OnSendPoistion( char  iID, short sPos,short  sSpeed )
{
    QString str;
    str.sprintf("%d -  %d  -  %d",iID,sPos,sSpeed);
    //qDebug(str.toLatin1().data());

    m_SysComm.Encode(SERVO_COMMAND_SERVO_TARGET, iID,SERVO_STATE_COM, MENU_SERVO_RUN_POSITION_VALUE,sPos,MENU_SERVO_RUN_SPEED_VALUE, sSpeed);
    sleep(delyTime);

    m_SysComm.Encode(SERVO_COMMAND_SERVO_RUN, iID,SERVO_STATE_COM, 0,0,0,0);
    sleep(delyTime);

    str.sprintf("%d",m_SendCount);
    ui->lineEdit_95->setText(str);
}
//常规舵机下发界面配置指令
void        MainUI::OnNormSendUI()
{
    struct servo_params params;
    char  cID = GetID();
    //获得界面参数
    GetUI_NormSteeringEngineParam();
    GetUI_AdditionalVariable();

    //脉宽上限
    short  sValue = m_SysParam.m_NormSteeringEngineParam.m_UpperPulseWidthlimit.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM0,sValue,0,0);
    sleep(delyTime);
    params.data[0] = sValue;

    //脉宽中点
    sValue = m_SysParam.m_NormSteeringEngineParam.m_MiddlePulseWidthlimit.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM1,sValue,0,0);
    sleep(delyTime);
    params.data[1] = sValue;
    //脉宽下限
    sValue = m_SysParam.m_NormSteeringEngineParam.m_DownPulseWidthlimit.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM2,sValue,0,0);
    sleep(delyTime);
    params.data[2] = sValue;
    //角度上限
    sValue = m_SysParam.m_NormSteeringEngineParam.m_UpperLimitOfAngle.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM3,sValue,0,0);
    sleep(delyTime);
    params.data[3] = sValue;
    //角度中点
    sValue = m_SysParam.m_NormSteeringEngineParam.m_AngularMidpoint.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM4,sValue,0, 0);
    sleep(delyTime);
    params.data[4] = sValue;
    //角度下限
    sValue = m_SysParam.m_NormSteeringEngineParam.m_LowerAngleLimit.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM5,sValue,0, 0);
    sleep(delyTime);
    params.data[5] = sValue;
    //死区(范围)
    sValue = m_SysParam.m_NormSteeringEngineParam.m_DeadZone.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM6,sValue,0,0);
    sleep(delyTime);
    params.data[6] = sValue;
    //死区时间---
    sValue = m_SysParam.m_SysLoseParam.servo_zero_zone_time.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM7,sValue,0,0);
    sleep(delyTime);
    params.data[7] = sValue;
    //自锁设置
    sValue = m_SysParam.m_NormSteeringEngineParam.m_Self_lockingSetting.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM8,sValue,0, 0);
    sleep(delyTime);
    params.data[8] = sValue;

    //舵机方向(信号方向)
    sValue = m_SysParam.m_NormSteeringEngineParam.m_SteeringGearDirection.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM9,sValue,0, 0);
    sleep(delyTime);
    params.data[9] = sValue;
    //电机方向---
    sValue = m_SysParam.m_AdditionalVariable.m_MotorDirection.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM10,sValue,0, 0);
    sleep(delyTime);
    params.data[10] = sValue;
    //信号容差--
    sValue = m_SysParam.m_AdditionalVariable.m_SignalTolerance.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM11,sValue,0, 0);
    sleep(delyTime);
    params.data[11] = sValue;
    //软件版本--
    sValue = m_SysParam.m_AdditionalVariable.m_ProgramVersion.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM12,sValue,0, 0);
    sleep(delyTime);
    params.data[12] = sValue;
    //掉电使能--
    sValue = m_SysParam.m_AdditionalVariable.m_PowerDownEnable.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM13,sValue,0, 0);
    sleep(delyTime);
    params.data[13] = sValue;
    //舵机ID--
    //sValue = m_SysParam.m_AdditionalVariable.m_SteeringGearID.GetValue();
    //m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM14,sValue,0, 0);
    //sleep(100);

    //通用ID--
    //sValue = m_SysParam.m_AdditionalVariable.m_UniversalID.GetValue();
    //m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM15,sValue,0, 0);
    //sleep(100);

    //信号复位(复位使能)
    sValue = m_SysParam.m_NormSteeringEngineParam.m_SignalReset.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM0,sValue,0,0);
    sleep(delyTime);
    params.data[16] = sValue;


    //复位位置--
    sValue = m_SysParam.m_AdditionalVariable.m_ResetPosition.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM1,sValue,0, 0);
    sleep(delyTime);
    params.data[17] = sValue;
    //复位位置--
    sValue = m_SysParam.m_AdditionalVariable.m_ResetTime.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM2,sValue,0, 0);
    sleep(delyTime);
    params.data[18] = sValue;
    //复位步长--
    sValue = m_SysParam.m_AdditionalVariable.m_ResetStepSize.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM3,sValue,0, 0);
    sleep(delyTime);
    params.data[19] = sValue;
    //启动时间--
    sValue = m_SysParam.m_AdditionalVariable.m_StartTime.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM4,sValue,0, 0);
    sleep(delyTime);
    params.data[20] = sValue;
    //启动步长--
    sValue = m_SysParam.m_AdditionalVariable.m_StartingStepSize.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM5,sValue,0, 0);
    sleep(delyTime);
    params.data[21] = sValue;
    //堵转保护(堵转区间)
    sValue = m_SysParam.m_NormSteeringEngineParam.m_Locked_rotorProtection.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM6,sValue,0,0);
    sleep(delyTime);
    params.data[22] = sValue;

    //堵转时间--
    sValue = m_SysParam.m_AdditionalVariable.m_BlockingTime.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM7,sValue,0, 0);
    sleep(delyTime);
    params.data[23] = sValue;
    //保护时间
    sValue = m_SysParam.m_NormSteeringEngineParam.m_ProtectionTime.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP,cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM8,sValue,0, 0);
    sleep(delyTime);
    params.data[24] = sValue;
    //保护比较---
    sValue = m_SysParam.m_SysLoseParam.servo_protect_pwm_cmpt.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM9,sValue,0, 0);
    sleep(delyTime);
    params.data[25] = sValue;
    //保护输出
    sValue = m_SysParam.m_NormSteeringEngineParam.m_ProtectionOutput.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM10,sValue,0, 0);
    sleep(delyTime);
    params.data[26] = sValue;
    //最大输出
    sValue = m_SysParam.m_NormSteeringEngineParam.m_MaximumOutput.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM11,sValue,0,0);
    sleep(delyTime);
    params.data[27] = sValue;
    //驱动频率
    sValue = m_SysParam.m_NormSteeringEngineParam.m_DrivingFrequency.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM12,sValue,0,0);
    sleep(delyTime);
    params.data[28] = sValue;
    //频率设置
    sValue = m_SysParam.m_NormSteeringEngineParam.m_FrequencySetting.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM13,sValue,0,0);
    sleep(delyTime);
    params.data[29] = sValue;
    //上电复位
    sValue = m_SysParam.m_AdditionalVariable.m_Power_onReset.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM14,sValue,0,0);
    sleep(100);
    params.data[30] = sValue;
    //启动电压
    sValue = m_SysParam.m_NormSteeringEngineParam.m_StartingVoltage.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM15,sValue,0,0);
    sleep(delyTime);
    params.data[31] = sValue;
    //生效指令
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DOWNLOAD,cID,SERVO_STATE_COM, MENU_DOWMLOAD_WORK_PARM,0,0,0);
    sleep(delyTime);

    //位置比例---
    sValue = m_SysParam.m_SysLoseParam.servo_position_pid_parm_p_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM0,sValue,0,0);
    sleep(delyTime);
    params.data[32] = sValue;
    //位置比率---
    sValue = m_SysParam.m_SysLoseParam.position_pid_speed_parm_radio.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM1,sValue,0,0);
    sleep(delyTime);
    params.data[33] = sValue;
    //速度比率---
    sValue = m_SysParam.m_SysLoseParam.servo_speed_run_sample_k_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM2,sValue,0,0);
    sleep(delyTime);
    params.data[34] = sValue;
    //位置时间---
    sValue = m_SysParam.m_SysLoseParam.servo_position_sample_ov_time_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM3,sValue,0,0);
    sleep(delyTime);
    params.data[35] = sValue;
    //速度时间---
    sValue = m_SysParam.m_SysLoseParam.servo_speed_sample_ov_time_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM4,sValue,0,0);
    sleep(delyTime);
    params.data[36] = sValue;
    //速度比例---
    sValue = m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM5,sValue,0,0);
    sleep(delyTime);
    params.data[37] = sValue;
    //速度增量---
    sValue = m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_radio.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM6,sValue,0,0);
    sleep(delyTime);
    params.data[38] = sValue;
    //模式锁定---
    sValue = m_SysParam.m_SysLoseParam.servo_work_mode_lock_flag_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_CONFIG, cID,SERVO_STATE_COM, MENU_CONFIG_SERVO_PARM0,sValue,0,0);
    sleep(delyTime);
    params.data[39] = sValue;
    //模式设置---
    sValue = m_SysParam.m_SysLoseParam.servo_work_mode_now.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_CONFIG, cID,SERVO_STATE_COM, MENU_CONFIG_SERVO_PARM1,sValue,0,0);
    sleep(delyTime);
    params.data[40] = sValue;
    //参数范围---
    sValue = m_SysParam.m_SysLoseParam.servo_pram_config_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_CONFIG, cID,SERVO_STATE_COM, MENU_CONFIG_SERVO_PARM2,sValue,0,0);
    sleep(delyTime);
    params.data[41] = sValue;
    //反馈使能---
    sValue = m_SysParam.m_SysLoseParam.servo_command_fb_flag.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_CONFIG, cID,SERVO_STATE_COM, MENU_CONFIG_SERVO_PARM3,sValue,0,0);
    sleep(delyTime);
    params.data[42] = sValue;

    //生效指令
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DOWNLOAD,cID,SERVO_STATE_COM, MENU_DOWMLOAD_DEBUG_PARM,0,0,0);
    sleep(delyTime);
    QString data_print;
    data_print = "const struct data ={";
    for (int i=0; i<43; i++){
        data_print = data_print + QString::number(params.data[i]);
        if(i !=42){
            data_print = data_print + ", ";
        }
    }
    data_print = data_print +"}";
    qDebug() << data_print;
}
//串口舵机下发界面配置指令
void        MainUI::OnCOMMSendUI()
{
    //获得界面参数
    GetUI_COMMSteeringEngineParam();
    GetUI_AdditionalVariable();

    char  cID = m_SysParam.m_COMMSteeringEngineParam.m_CurID.GetValue();

    //脉宽上限
    short  sValue = m_SysParam.m_COMMSteeringEngineParam.m_UpperPulseWidthlimit.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM0,sValue,0,0);
    sleep(delyTime);

    //脉宽中点
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_MiddlePulseWidthlimit.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM1,sValue,0,0);
    sleep(delyTime);

    //脉宽下限
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_DownPulseWidthlimit.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM2,sValue,0,0);
    sleep(delyTime);

    //角度上限
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_UpperLimitOfAngle.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM3,sValue,0,0);
    sleep(delyTime);

    //角度中点
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_AngularMidpoint.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM4,sValue,0, 0);
    sleep(delyTime);

    //角度下限
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_LowerAngleLimit.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM5,sValue,0, 0);
    sleep(delyTime);

    //死区(范围)
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_DeadZone.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM6,sValue,0,0);
    sleep(delyTime);

    //死区时间---
    sValue = m_SysParam.m_SysLoseParam.servo_zero_zone_time.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM7,sValue,0,0);
    sleep(delyTime);

    //自锁设置
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_Self_lockingSetting.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM8,sValue,0, 0);
    sleep(delyTime);


    //舵机方向(信号方向)
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_SteeringGearDirection.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM9,sValue,0, 0);
    sleep(delyTime);

    //电机方向---
    sValue = m_SysParam.m_AdditionalVariable.m_MotorDirection.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM10,sValue,0, 0);
    sleep(delyTime);

    //信号容差--
    sValue = m_SysParam.m_AdditionalVariable.m_SignalTolerance.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM11,sValue,0, 0);
    sleep(delyTime);

    //软件版本--
    sValue = m_SysParam.m_AdditionalVariable.m_ProgramVersion.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM12,sValue,0, 0);
    sleep(delyTime);

    //掉电使能--
    sValue = m_SysParam.m_AdditionalVariable.m_PowerDownEnable.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM13,sValue,0, 0);
    sleep(delyTime);

    //舵机ID--
    //sValue = m_SysParam.m_AdditionalVariable.m_SteeringGearID.GetValue();
    //m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM14,sValue,0, 0);
    //sleep(100);

    //通用ID--
    //sValue = m_SysParam.m_AdditionalVariable.m_UniversalID.GetValue();
    //m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_WORK, cID,SERVO_STATE_COM, MENU_WORK_SERVO_PARM15,sValue,0, 0);
    //sleep(100);

    //信号复位(复位使能)
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_SignalReset.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM0,sValue,0,0);
    sleep(delyTime);


    //复位位置--
    sValue = m_SysParam.m_AdditionalVariable.m_ResetPosition.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM1,sValue,0, 0);
    sleep(delyTime);

    //复位位置--
    sValue = m_SysParam.m_AdditionalVariable.m_ResetTime.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM2,sValue,0, 0);
    sleep(delyTime);

    //复位步长--
    sValue = m_SysParam.m_AdditionalVariable.m_ResetStepSize.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM3,sValue,0, 0);
    sleep(delyTime);

    //启动时间--
    sValue = m_SysParam.m_AdditionalVariable.m_StartTime.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM4,sValue,0, 0);
    sleep(delyTime);

    //启动步长--
    sValue = m_SysParam.m_AdditionalVariable.m_StartingStepSize.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM5,sValue,0, 0);
    sleep(delyTime);

    //堵转保护(堵转区间)
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_Locked_rotorProtection.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM6,sValue,0,0);
    sleep(delyTime);


    //堵转时间--
    sValue = m_SysParam.m_AdditionalVariable.m_BlockingTime.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM7,sValue,0, 0);
    sleep(delyTime);

    //保护时间
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_ProtectionTime.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP,cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM8,sValue,0, 0);
    sleep(delyTime);

    //保护比较---
    sValue = m_SysParam.m_SysLoseParam.servo_protect_pwm_cmpt.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM9,sValue,0, 0);
    sleep(delyTime);

    //保护输出
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_ProtectionOutput.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM10,sValue,0, 0);
    sleep(delyTime);

    //最大输出
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_MaximumOutput.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM11,sValue,0,0);
    sleep(delyTime);

    //驱动频率
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_DrivingFrequency.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM12,sValue,0,0);
    sleep(delyTime);

    //频率设置
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_FrequencySetting.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM13,sValue,0,0);
    sleep(delyTime);

    //上电复位
    sValue = m_SysParam.m_AdditionalVariable.m_Power_onReset.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM14,sValue,0,0);
    sleep(delyTime);

    //启动电压
    sValue = m_SysParam.m_COMMSteeringEngineParam.m_StartingVoltage.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_SETUP, cID,SERVO_STATE_COM, MENU_SETUP_SERVO_PARM15,sValue,0,0);
    sleep(delyTime);

    //生效指令
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DOWNLOAD,cID,SERVO_STATE_COM, MENU_DOWMLOAD_WORK_PARM,0,0,0);
    sleep(delyTime);

    //位置比例---
    sValue = m_SysParam.m_SysLoseParam.servo_position_pid_parm_p_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM0,sValue,0,0);
    sleep(delyTime);

    //位置比率---
    sValue = m_SysParam.m_SysLoseParam.position_pid_speed_parm_radio.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM1,sValue,0,0);
    sleep(delyTime);

    //速度比率---
    sValue = m_SysParam.m_SysLoseParam.servo_speed_run_sample_k_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM2,sValue,0,0);
    sleep(delyTime);

    //位置时间---
    sValue = m_SysParam.m_SysLoseParam.servo_position_sample_ov_time_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM3,sValue,0,0);
    sleep(delyTime);

    //速度时间---
    sValue = m_SysParam.m_SysLoseParam.servo_speed_sample_ov_time_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM4,sValue,0,0);
    sleep(delyTime);

    //速度比例---
    sValue = m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM5,sValue,0,0);
    sleep(delyTime);

    //速度增量---
    sValue = m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_radio.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DEBUG, cID,SERVO_STATE_COM, MENU_DEBUG_SERVO_PARM6,sValue,0,0);
    sleep(delyTime);

    //模式锁定---
    sValue = m_SysParam.m_SysLoseParam.servo_work_mode_lock_flag_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_CONFIG, cID,SERVO_STATE_COM, MENU_CONFIG_SERVO_PARM0,sValue,0,0);
    sleep(delyTime);

    //模式设置---
    sValue = m_SysParam.m_SysLoseParam.servo_work_mode_now.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_CONFIG, cID,SERVO_STATE_COM, MENU_CONFIG_SERVO_PARM1,sValue,0,0);
    sleep(delyTime);

    //参数范围---
    sValue = m_SysParam.m_SysLoseParam.servo_pram_config_set.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_CONFIG, cID,SERVO_STATE_COM, MENU_CONFIG_SERVO_PARM2,sValue,0,0);
    sleep(delyTime);

    //反馈使能---
    sValue = m_SysParam.m_SysLoseParam.servo_command_fb_flag.GetValue();
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_CONFIG, cID,SERVO_STATE_COM, MENU_CONFIG_SERVO_PARM3,sValue,0,0);
    sleep(delyTime);

    //生效指令
    m_SysComm.Encode(SERVO_COMMAND_SERVO_PARM_DOWNLOAD,cID,SERVO_STATE_COM, MENU_DOWMLOAD_DEBUG_PARM,0,0,0);
    sleep(delyTime);
}
//发送等待
bool       MainUI::Waiting()
{
    return  true;

    //判断是否检测
    bool   bWaiting = true;
    if( bWaiting )
    {
        QTime  dieTime = QTime::currentTime().addMSecs(500);
        while(m_bSendrecvData)
        {
           sleep(10);
           if( QTime::currentTime() >= dieTime )
           {
               QString str;
               str.sprintf("system recv param delay...");
               qDebug(str.toLatin1().data());
               break;
           }
        }
    }
    m_bSendrecvData = false;

    return  true;
}
