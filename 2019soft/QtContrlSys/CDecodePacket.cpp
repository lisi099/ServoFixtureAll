#include"mainui.h"
#include"ui_mainui.h"
#include<QTime>
#include<QCoreApplication>
#include"CSysDefine.h"

//解析舵机反馈数据
void    MainUI::AnalyzeTheFeedbackDataOfSteeringGear(unsigned char  IDByte1,short IDValue1,unsigned char IDByte2,short IDValue2)
{

    //对比检测
    char   checkFlag = 0;

    //表示是常规还是串口0 常规 1 串口
    int    iQueryByte = ui->tabWidget->currentIndex();


    switch (IDByte1)
    {
    case MENU_FB_SERVO_WORK_PARM0:
    {
        //脉宽上限control_pulse_upper
        m_bSendrecvData = false;
        if( iQueryByte == 0 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_UpperPulseWidthlimit.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_NormSteeringEngineParam.m_UpperPulseWidthlimit.GetValue() != IDValue2)
                 //   m_ErrorInfoManger.AddValue(QString::fromUtf8("脉宽上限对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if( iQueryByte == 1 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_UpperPulseWidthlimit.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_COMMSteeringEngineParam.m_UpperPulseWidthlimit.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("脉宽上限对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        break;
    }
    case MENU_FB_SERVO_WORK_PARM1:
    {
        //脉宽中点control_pulse_mid
        m_bSendrecvData = false;
        if( iQueryByte == 0 )
        {
            if( checkFlag == 0 )
                 m_SysParam.m_NormSteeringEngineParam.m_MiddlePulseWidthlimit.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_NormSteeringEngineParam.m_MiddlePulseWidthlimit.GetValue() != IDValue2)
                 //   m_ErrorInfoManger.AddValue(QString::fromUtf8("脉宽中点对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if( iQueryByte == 1 )
        {
            if( checkFlag == 0 )
                 m_SysParam.m_COMMSteeringEngineParam.m_MiddlePulseWidthlimit.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_COMMSteeringEngineParam.m_MiddlePulseWidthlimit.GetValue() != IDValue2)
                 //   m_ErrorInfoManger.AddValue(QString::fromUtf8("脉宽中点对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        break;
    }
    case MENU_FB_SERVO_WORK_PARM2:
    {
        //脉宽下限 control_pulse_lower
        m_bSendrecvData = false;
        if( iQueryByte == 0 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_DownPulseWidthlimit.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_NormSteeringEngineParam.m_DownPulseWidthlimit.GetValue() != IDValue2)
                 //   m_ErrorInfoManger.AddValue(QString::fromUtf8("脉宽下限对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if( iQueryByte == 1 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_DownPulseWidthlimit.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_COMMSteeringEngineParam.m_DownPulseWidthlimit.GetValue() != IDValue2)
               //     m_ErrorInfoManger.AddValue(QString::fromUtf8("脉宽下限对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        break;
    }
    case MENU_FB_SERVO_WORK_PARM3:
    {
        //角度上限 control_set_upper
        m_bSendrecvData = false;
        if( iQueryByte == 0 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_UpperLimitOfAngle.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_NormSteeringEngineParam.m_UpperLimitOfAngle.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("角度上限对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if( iQueryByte == 1 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_UpperLimitOfAngle.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_COMMSteeringEngineParam.m_UpperLimitOfAngle.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("角度上限对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        break;
    }
    case MENU_FB_SERVO_WORK_PARM4:
    {
        //角度中点 control_set_mid
        m_bSendrecvData = false;
        if( iQueryByte == 0 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_AngularMidpoint.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_NormSteeringEngineParam.m_AngularMidpoint.GetValue() != IDValue2)
                 //   m_ErrorInfoManger.AddValue(QString::fromUtf8("角度中点对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if( iQueryByte == 1 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_AngularMidpoint.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_COMMSteeringEngineParam.m_AngularMidpoint.GetValue() != IDValue2)
                 //   m_ErrorInfoManger.AddValue(QString::fromUtf8("角度中点对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        break;
    }
    case MENU_FB_SERVO_WORK_PARM5:
    {
        //角度下限 control_set_lower
        m_bSendrecvData = false;
        if( iQueryByte == 0 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_LowerAngleLimit.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_NormSteeringEngineParam.m_AngularMidpoint.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("角度下限对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if( iQueryByte == 1 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_LowerAngleLimit.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_COMMSteeringEngineParam.m_AngularMidpoint.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("角度中点对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }

        break;
    }
    case MENU_FB_SERVO_WORK_PARM6:
    {
        //死区范围 servo_zero_zone_set
        m_bSendrecvData = false;
        if( iQueryByte == 0 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_DeadZone.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_NormSteeringEngineParam.m_DeadZone.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("死区对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if( iQueryByte == 1 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_DeadZone.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_COMMSteeringEngineParam.m_DeadZone.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("死区对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        break;
    }
    case MENU_FB_SERVO_WORK_PARM7:
    {
        //死区时间 servo_zero_zone_time
        m_bSendrecvData = false;
        if( checkFlag == 0 )
            m_SysParam.m_SysLoseParam.servo_zero_zone_time.SetValue(IDValue2);
        else
        {
        }

        break;
    }
    case MENU_FB_SERVO_WORK_PARM8:
    {
        //自锁设置 servo_free_lock_flag
        m_bSendrecvData = false;
        if( iQueryByte == 0 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_Self_lockingSetting.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_NormSteeringEngineParam.m_Self_lockingSetting.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("自锁设置对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
         else if( iQueryByte == 1 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_Self_lockingSetting.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_COMMSteeringEngineParam.m_Self_lockingSetting.GetValue() != IDValue2)
                 //   m_ErrorInfoManger.AddValue(QString::fromUtf8("自锁设置对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        break;
    }
    case MENU_FB_SERVO_WORK_PARM9:
    {
        //信号方向 servo_dir_inverse_flag
        m_bSendrecvData = false;
        if( iQueryByte == 0 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_SteeringGearDirection.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_NormSteeringEngineParam.m_SteeringGearDirection.GetValue() != IDValue2)
                 //   m_ErrorInfoManger.AddValue(QString::fromUtf8("信号方向对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if( iQueryByte == 1 )
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_SteeringGearDirection.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_NormSteeringEngineParam.m_SteeringGearDirection.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("信号方向对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }

        break;
    }
    case MENU_FB_SERVO_WORK_PARM10:
    {
        //电机方向 servo_pwm_inverse_flag
        if( checkFlag == 0 )
            m_SysParam.m_AdditionalVariable.m_MotorDirection.SetValue(IDValue2);
        else
        {
            //if( m_SysParam.m_AdditionalVariable.m_MotorDirection.GetValue() != IDValue2)
            //    m_ErrorInfoManger.AddValue(QString::fromUtf8("电机方向对比错误"),ui->comboBox_6,ui->comboBox_5);
        }
        break;
    }
    case MENU_FB_SERVO_WORK_PARM11:
    {
        //信号容差 insig_value_diff_set
        if( checkFlag == 0 )
            m_SysParam.m_AdditionalVariable.m_SignalTolerance.SetValue(IDValue2);
        else
        {
            //if( m_SysParam.m_AdditionalVariable.m_SignalTolerance.GetValue() != IDValue2)
             //   m_ErrorInfoManger.AddValue(QString::fromUtf8("信号容差对比错误"),ui->comboBox_6,ui->comboBox_5);
        }

        break;
    }
    case MENU_FB_SERVO_WORK_PARM12:
    {
        //程序版本 servo_program_version
        if( checkFlag == 0 )
            m_SysParam.m_AdditionalVariable.m_ProgramVersion.SetValue(IDValue2);
        else
        {
            //if( m_SysParam.m_AdditionalVariable.m_ProgramVersion.GetValue() != IDValue2)
             //   m_ErrorInfoManger.AddValue(QString::fromUtf8("程序版本对比错误"),ui->comboBox_6,ui->comboBox_5);
        }

        break;
    }
    case MENU_FB_SERVO_WORK_PARM13:
    {
        //掉电使能 servo_epprom_parm_reset_flag
        if( checkFlag == 0 )
            m_SysParam.m_AdditionalVariable.m_PowerDownEnable.SetValue(IDValue2);
        else
        {
            //if( m_SysParam.m_AdditionalVariable.m_PowerDownEnable.GetValue() != IDValue2)
             //   m_ErrorInfoManger.AddValue(QString::fromUtf8("掉电使能对比错误"),ui->comboBox_6,ui->comboBox_5);
        }

        break;
    }
    case MENU_FB_SERVO_WORK_PARM14:
    {
        //舵机ID servo_unique_address_id_set
        if( checkFlag == 0 )
           m_SysParam.m_AdditionalVariable.m_SteeringGearID.SetValue(IDValue2);
        else
        {
            //if( m_SysParam.m_AdditionalVariable.m_SteeringGearID.GetValue() != IDValue2)
            //    m_ErrorInfoManger.AddValue(QString::fromUtf8("舵机ID对比错误"),ui->comboBox_6,ui->comboBox_5);
        }

        break;
    }
    case MENU_FB_SERVO_WORK_PARM15:
    {
        //通用ID servo_common_command_address_set
        if( checkFlag == 0 )
        {
            m_SysParam.m_AdditionalVariable.m_UniversalID.SetValue(IDValue2);
        }
        else
        {
            //if( m_SysParam.m_AdditionalVariable.m_UniversalID.GetValue() != IDValue2)
            //    m_ErrorInfoManger.AddValue(QString::fromUtf8("通用ID对比错误"),ui->comboBox_6,ui->comboBox_5);
        }
        break;
    }
    case MENU_FB_SERVO_SETUP_PARM0:
    {
        //复位使能(信号复位) servo_reset_flag_set
        m_bSendrecvData = false;
        if(iQueryByte == 0)
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_SignalReset.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_NormSteeringEngineParam.m_SignalReset.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("信号复位对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if(iQueryByte == 1)
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_SignalReset.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_COMMSteeringEngineParam.m_SignalReset.GetValue() != IDValue2)
                 //   m_ErrorInfoManger.AddValue(QString::fromUtf8("信号复位对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        break;
    }
    case MENU_FB_SERVO_SETUP_PARM1:
    {
        //复位位置 servo_reset_position
        if( checkFlag == 0 )
            m_SysParam.m_AdditionalVariable.m_ResetPosition.SetValue(IDValue2);
        else
        {
            //if( m_SysParam.m_AdditionalVariable.m_ResetPosition.GetValue() != IDValue2)
            //    m_ErrorInfoManger.AddValue(QString::fromUtf8("复位位置对比错误"),ui->comboBox_6,ui->comboBox_5);
        }
        break;
    }
    case MENU_FB_SERVO_SETUP_PARM2:
    {
        //复位时间
        if( checkFlag == 0 )
            m_SysParam.m_AdditionalVariable.m_ResetTime.SetValue(IDValue2);
        else
        {
            //if( m_SysParam.m_AdditionalVariable.m_ResetTime.GetValue() != IDValue2)
            //    m_ErrorInfoManger.AddValue(QString::fromUtf8("复位时间对比错误"),ui->comboBox_6,ui->comboBox_5);
        }
        break;
    }
    case MENU_FB_SERVO_SETUP_PARM3:
    {
        //复位步长 servo_reset_step_set
        if( checkFlag == 0 )
            m_SysParam.m_AdditionalVariable.m_ResetStepSize.SetValue(IDValue2);
        else
        {
            //if( m_SysParam.m_AdditionalVariable.m_ResetStepSize.GetValue() != IDValue2)
            //    m_ErrorInfoManger.AddValue(QString::fromUtf8("复位步长对比错误"),ui->comboBox_6,ui->comboBox_5);
        }
        break;
    }
    case MENU_FB_SERVO_SETUP_PARM4:
    {
        //启动时间 control_interval_time_set
        if( checkFlag == 0 )
            m_SysParam.m_AdditionalVariable.m_StartTime.SetValue(IDValue2);
        else
        {
            //if( m_SysParam.m_AdditionalVariable.m_StartTime.GetValue() != IDValue2)
            //    m_ErrorInfoManger.AddValue(QString::fromUtf8("启动时间对比错误"),ui->comboBox_6,ui->comboBox_5);
        }
        break;
    }
    case MENU_FB_SERVO_SETUP_PARM5:
    {
        //启动步长 control_interval_step_set
        if( checkFlag == 0 )
            m_SysParam.m_AdditionalVariable.m_StartingStepSize.SetValue(IDValue2);
        else
        {
            //if( m_SysParam.m_AdditionalVariable.m_StartingStepSize.GetValue() != IDValue2)
            //    m_ErrorInfoManger.AddValue(QString::fromUtf8("启动步长对比错误"),ui->comboBox_6,ui->comboBox_5);
        }
        break;
    }
    case MENU_FB_SERVO_SETUP_PARM6:
    {
        //20200520 堵转保护和保护对比对换
        //堵转保护 servo_halt_zone_set
        m_bSendrecvData = false;
        if(iQueryByte == 0)
        {
            if( checkFlag == 0 )
                m_SysParam.m_SysLoseParam.servo_protect_pwm_cmpt.SetValue(IDValue2);
                //m_SysParam.m_NormSteeringEngineParam.m_Locked_rotorProtection.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_NormSteeringEngineParam.m_Locked_rotorProtection.GetValue() != IDValue2)
               //     m_ErrorInfoManger.AddValue(QString::fromUtf8("堵转区间对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if(iQueryByte == 1)
        {
            if( checkFlag == 0 )
                m_SysParam.m_SysLoseParam.servo_protect_pwm_cmpt.SetValue(IDValue2);
                //m_SysParam.m_COMMSteeringEngineParam.m_Locked_rotorProtection.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_COMMSteeringEngineParam.m_Locked_rotorProtection.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("堵转区间对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        break;
    }
    case MENU_FB_SERVO_SETUP_PARM7:
    {
        //堵转时间 servo_halt_time_set
        if( checkFlag == 0 )
            m_SysParam.m_AdditionalVariable.m_BlockingTime.SetValue(IDValue2);
        else
        {
            //if( m_SysParam.m_AdditionalVariable.m_BlockingTime.GetValue() != IDValue2)
             //   m_ErrorInfoManger.AddValue(QString::fromUtf8("堵转时间对比错误"),ui->comboBox_6,ui->comboBox_5);
        }
        break;
    }
    case MENU_FB_SERVO_SETUP_PARM8:
    {
        //保护时间 servo_protect_time_set
        m_bSendrecvData = false;
        if(iQueryByte == 0)
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_ProtectionTime.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_NormSteeringEngineParam.m_ProtectionTime.GetValue() != IDValue2)
               //     m_ErrorInfoManger.AddValue(QString::fromUtf8("保护时间对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else  if(iQueryByte == 1)
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_ProtectionTime.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_COMMSteeringEngineParam.m_ProtectionTime.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("保护时间对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        break;
    }
    case MENU_FB_SERVO_SETUP_PARM9:
    {
        //20200520 堵转保护和保护对比对换
        //保护比较 servo_protect_pwm_cmp
        m_bSendrecvData = false;
        if(iQueryByte == 0)
        {
            if( checkFlag == 0 )
                //m_SysParam.m_SysLoseParam.servo_protect_pwm_cmpt.SetValue(IDValue2);
                m_SysParam.m_NormSteeringEngineParam.m_Locked_rotorProtection.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_NormSteeringEngineParam.m_Locked_rotorProtection.GetValue() != IDValue2)
               //     m_ErrorInfoManger.AddValue(QString::fromUtf8("保护比较"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if(iQueryByte == 1)
        {
            if( checkFlag == 0 )
                //m_SysParam.m_SysLoseParam.servo_protect_pwm_cmpt.SetValue(IDValue2);
                m_SysParam.m_COMMSteeringEngineParam.m_Locked_rotorProtection.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_COMMSteeringEngineParam.m_Locked_rotorProtection.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("保护比较"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        break;
    }
    case MENU_FB_SERVO_SETUP_PARM10:
    {
        //保护输出 servo_protect_pwm_set
        m_bSendrecvData = false;
        if(iQueryByte == 0)
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_ProtectionOutput.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_NormSteeringEngineParam.m_ProtectionOutput.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("保护输出对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if(iQueryByte == 1)
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_ProtectionOutput.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_COMMSteeringEngineParam.m_ProtectionOutput.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("保护输出对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }

        break;
    }
    case MENU_FB_SERVO_SETUP_PARM11:
    {
        //最大输出 servo_max_pwm_set
        m_bSendrecvData = false;
        if(iQueryByte == 0)
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_MaximumOutput.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_NormSteeringEngineParam.m_MaximumOutput.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("最大输出对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if(iQueryByte == 1)
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_MaximumOutput.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_COMMSteeringEngineParam.m_MaximumOutput.GetValue() != IDValue2)
                 //   m_ErrorInfoManger.AddValue(QString::fromUtf8("最大输出对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        break;
    }
    case MENU_FB_SERVO_SETUP_PARM12:
    {
        //驱动频率 servo_motor_work_pwm_cycle
        m_bSendrecvData = false;
        if(iQueryByte == 0)
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_DrivingFrequency.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_NormSteeringEngineParam.m_DrivingFrequency.GetValue() != IDValue2)
                 //   m_ErrorInfoManger.AddValue(QString::fromUtf8("驱动频率对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if(iQueryByte == 1)
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_DrivingFrequency.SetValue(IDValue2);
            else
            {
                //if( m_SysParam.m_COMMSteeringEngineParam.m_DrivingFrequency.GetValue() != IDValue2)
                 //   m_ErrorInfoManger.AddValue(QString::fromUtf8("驱动频率对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }

        break;
    }
    case MENU_FB_SERVO_SETUP_PARM13:
    {
        //频率设置 motor_pwm_cycle
        m_bSendrecvData = false;
        if(iQueryByte == 0)
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_FrequencySetting.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_NormSteeringEngineParam.m_FrequencySetting.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("频率设置对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if(iQueryByte == 1)
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_FrequencySetting.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_COMMSteeringEngineParam.m_FrequencySetting.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("频率设置对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }

        break;
    }
    case MENU_FB_SERVO_SETUP_PARM14:
    {
        //上电复位 servo_init_flag_set
        if( checkFlag == 0 )
             m_SysParam.m_AdditionalVariable.m_Power_onReset.SetValue(IDValue2);
        else
        {
           // if( m_SysParam.m_AdditionalVariable.m_Power_onReset.GetValue() != IDValue2)
           //     m_ErrorInfoManger.AddValue(QString::fromUtf8("上电复位对比错误"),ui->comboBox_6,ui->comboBox_5);
        }
        break;
    }
    case MENU_FB_SERVO_SETUP_PARM15:
    {
        //启动电压 servo_work_base_voltage
        m_bSendrecvData = false;
        if(iQueryByte == 0)
        {
            if( checkFlag == 0 )
                m_SysParam.m_NormSteeringEngineParam.m_StartingVoltage.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_NormSteeringEngineParam.m_StartingVoltage.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("启动电压对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }
        else if(iQueryByte == 1)
        {
            if( checkFlag == 0 )
                m_SysParam.m_COMMSteeringEngineParam.m_StartingVoltage.SetValue(IDValue2);
            else
            {
               // if( m_SysParam.m_COMMSteeringEngineParam.m_StartingVoltage.GetValue() != IDValue2)
                //    m_ErrorInfoManger.AddValue(QString::fromUtf8("启动电压对比错误"),ui->comboBox_6,ui->comboBox_5);
            }
        }

        break;
    }
    case MENU_FB_SERVO_DEBUG_PARM0:
    {
        //位置比例 servo_position_pid_parm_p_set
        if(iQueryByte == 0)
        {
            m_SysParam.m_SysLoseParam.servo_position_pid_parm_p_set[0].SetValue(IDValue2);
        }
        else if(iQueryByte == 1)
        {
            m_SysParam.m_SysLoseParam.servo_position_pid_parm_p_set[1].SetValue(IDValue2);
        }
        break;
    }
    case MENU_FB_SERVO_DEBUG_PARM1:
    {
        //位置比率 position_pid_speed_parm_radio
        m_SysParam.m_SysLoseParam.position_pid_speed_parm_radio.SetValue(IDValue2);
        break;
    }
    case MENU_FB_SERVO_DEBUG_PARM2:
    {
        //速度比率 servo_speed_run_sample_k_set
        if(iQueryByte == 0)
        {
            m_SysParam.m_SysLoseParam.servo_speed_run_sample_k_set[0].SetValue(IDValue2);
        }
        else if(iQueryByte == 1)
        {
            m_SysParam.m_SysLoseParam.servo_speed_run_sample_k_set[1].SetValue(IDValue2);
        }
        break;
    }
    case MENU_FB_SERVO_DEBUG_PARM3:
    {
        //位置时间 servo_position_sample_ov_time_set
        m_SysParam.m_SysLoseParam.servo_position_sample_ov_time_set.SetValue(IDValue2);
        break;
    }
    case MENU_FB_SERVO_DEBUG_PARM4:
    {
        //速度时间 servo_speed_sample_ov_time_set
        m_SysParam.m_SysLoseParam.servo_speed_sample_ov_time_set.SetValue(IDValue2);
        break;
    }
    case MENU_FB_SERVO_DEBUG_PARM5:
    {
        //速度比例 servo_speed_pid_parm_p_set
        if(iQueryByte == 0)
        {
            m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_set[0].SetValue(IDValue2);
        }
        else if(iQueryByte == 1)
        {
            m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_set[1].SetValue(IDValue2);
        }
        break;
    }
    case MENU_FB_SERVO_DEBUG_PARM6:
    {
        //速度增量 servo_speed_pid_parm_p_radio
        m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_radio.SetValue(IDValue2);
        break;
    }
    case MENU_FB_SERVO_DEBUG_PARM7:
    {
        break;
    }
    case MENU_FB_SERVO_CONFIG_PARM0:
    {
        //模式锁定 servo_work_mode_lock_flag_set
        m_SysParam.m_SysLoseParam.servo_work_mode_lock_flag_set.SetValue(IDValue2);
        break;
    }
    case MENU_FB_SERVO_CONFIG_PARM1:
    {
        //模式设置 servo_work_mode_now
        m_SysParam.m_SysLoseParam.servo_work_mode_now.SetValue(IDValue2);
        break;
    }
    case MENU_FB_SERVO_CONFIG_PARM2:
    {
        //参数范围 servo_pram_config_set
        m_SysParam.m_SysLoseParam.servo_pram_config_set.SetValue(IDValue2);
        break;
    }
    case MENU_FB_SERVO_CONFIG_PARM3:
    {
        //反馈使能 servo_command_fb_flag
        m_SysParam.m_SysLoseParam.servo_command_fb_flag.SetValue(IDValue2);
        break;
    }
    case MENU_FB_SERVO_STATE_PARM0:
    {
        //servo_error_value = IDValue2;
        break;
    }
    case MENU_FB_SERVO_STATE_PARM1:
    {
        //位置
        //servo_position_value_now = IDValue2;
        m_SysParam.m_NormSteeringEngineTest.m_Position.SetValue(IDValue2);
        Refweizhi(IDValue2);
//        ui->qwtPlot->replot();
        break;
    }
    case MENU_FB_SERVO_STATE_PARM2:
    {
        //速度
        //servo_speed_value_now = IDValue2;
        break;
    }
    case MENU_FB_SERVO_STATE_PARM3:
    {
        //电流
        //servo_current_value_now = IDValue2;
        m_SysParam.m_NormSteeringEngineTest.m_ElectricWave.SetValue(IDValue2);
        Refdianliu(IDValue2);
//        ui->qwtPlot->replot();
        break;
    }
    case MENU_FB_SERVO_STATE_PARM4:
    {
        //温度
        //servo_tempture_value_now = IDValue2;
        m_SysParam.m_NormSteeringEngineTest.m_Temperature.SetValue(IDValue2);
        Refwendu(IDValue2);
//        ui->qwtPlot->replot();
        break;
    }
    case MENU_FB_SERVO_STATE_PARM5:
    {
        //电压
        //servo_vddin_value_now = IDValue2;
        m_SysParam.m_NormSteeringEngineTest.m_Voltage.SetValue(IDValue2);
        Refdianya(IDValue2);
//        ui->qwtPlot->replot();
        break;
    }
    default:
        break;
    }


    switch (iQueryByte)
    {
    case 0:
    {
        SetUI_NormSteeringEngineParam();
        break;
    }
    case 1:
    {
        SetUI_COMMSteeringEngineParam();
        break;
    }
    case 2:
    {
        SetUI_SteeringEngineTest();
        break;
    }
    case 3:
    {
        break;
    }
    default:
        break;
    }

    SetUI_AdditionalVariable();
}
