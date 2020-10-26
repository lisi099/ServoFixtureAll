#ifndef CSYSDEFINE_H
#define CSYSDEFINE_H

//命令字
#define SERVO_COMMAND_SERVO_UART_EN         		1 	    //舵机通讯命令
#define SERVO_COMMAND_SERVO_COM_PACKAGE             2       //舵机数据包命令
#define SERVO_COMMAND_SERVO_ADDRESS_SET     		3		//舵机地址命令
#define SERVO_COMMAND_SERVO_PARM_CONFIG     		4		//舵机配置命令
#define SERVO_COMMAND_SERVO_PARM_WORK    		    5		//舵机工作命令
#define SERVO_COMMAND_SERVO_PARM_SETUP     			6		//舵机设置命令
#define SERVO_COMMAND_SERVO_PARM_DEBUG    			7		//舵机调试命令
#define SERVO_COMMAND_SERVO_PARM_DOWNLOAD      	    8		//舵机下载命令
#define SERVO_COMMAND_SERVO_FB       	        	9		//舵机反馈命令
#define SERVO_COMMAND_SERVO_TARGET        			10	    //舵机目标命令
#define SERVO_COMMAND_SERVO_RESET       		    15	    //舵机复位命令
#define SERVO_COMMAND_SERVO_RUN                     16 	    //舵机运行命令
#define SERVO_COMMAND_SERVO_STOP        	    	17	    //舵机停止命令
#define SERVO_COMMAND_SERVO_DUMP        	    	18 	    //舵机掉电命令
#define SERVO_COMMAND_CHECH_CODE                    19	    //舵机校验命令
#define SERVO_COMMAND_NONE                          20	    //舵机校验命令

//状态字
#define SERVO_STATE_COM        	   				    1       //接收状态
#define SERVO_STATE_FB        	   					2 	    //反馈状态

//指令参数ID
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_UART_EN  		    	1
//【参数ID1】
#define SERVO_COMMAND_SERVO_UART_COM_ENABLE         1		//配置使能				默认值0xAAAA
#define SERVO_COMMAND_SERVO_UART_COM_UNABLE    	    2		//配置退出				默认值0x5555
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_COM_PACKAGE 			2
//【参数ID1】
#define SERVO_COMMAND_SERVO_COM_PACK_START  		3		//命令包开始				默认值0
#define SERVO_COMMAND_SERVO_COM_PACK_END  			4		//命令包结束	       	    默认值0
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_ADDRESS_SET       3
//【参数ID1】
#define MENU_CONFIG_SERVO_COMMON_ADDRESS_SET  	    5		//舵机通用地址	取值0-254		默认值248
#define MENU_CONFIG_SERVO_UNIQUE_ADDRESS_SET        6		//舵机设备地址	取值0-65534		默认值16
#define MENU_CONFIG_SERVO_RESET_ADDRESS_SET         7		//舵机设备地址	取值0-65534		默认值16
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_PARM_CONFIG  			4
//【参数ID1】
#define MENU_CONFIG_SERVO_PARM0            	        10	//bit 					servo_work_mode_lock_flag_set  模式锁定  0   N
#define MENU_CONFIG_SERVO_PARM1            	        11	//unsigned 	char 		servo_work_mode_now            模式设置  2   N
#define MENU_CONFIG_SERVO_PARM2              	    12	//unsigned 	char     	servo_pram_config_set          参数范围  4   N
#define MENU_CONFIG_SERVO_PARM3              	    14	//bit               	servo_command_fb_flag          反馈使能  1   N
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_PARM_WORK					5
//【参数ID1】
#define MENU_WORK_SERVO_PARM0             			20	//unsigned 	int      	control_pulse_upper  脉宽上限2200  y
#define MENU_WORK_SERVO_PARM1             			21	//unsigned 	int      	control_pulse_mid    脉宽中点1500  y
#define MENU_WORK_SERVO_PARM2            			22	//unsigned 	int      	control_pulse_lower  脉宽下限800   y
#define MENU_WORK_SERVO_PARM3             			23	//unsigned 	int      	control_set_upper    角度上限3548  y
#define MENU_WORK_SERVO_PARM4          				24	//unsigned 	int      	control_set_mid      角度中点2048  y
#define MENU_WORK_SERVO_PARM5            			25	//unsigned 	int      	control_set_lower    角度下限548   y
#define MENU_WORK_SERVO_PARM6            			26	//unsigned 	char     	servo_zero_zone_set  死区范围20    y
#define MENU_WORK_SERVO_PARM7            			27	//unsigned 	char     	servo_zero_zone_time 死区时间1     N
#define MENU_WORK_SERVO_PARM8            			28	//bit               	servo_free_lock_flag 自锁设置1     y
#define MENU_WORK_SERVO_PARM9            			29	//bit               	servo_dir_inverse_flag信号方向0    y
#define MENU_WORK_SERVO_PARM10            			30	//bit               	servo_pwm_inverse_flag电机方向0    y
#define MENU_WORK_SERVO_PARM11            			31	//signed   	char     	insig_value_diff_set 信号容差2     y
#define MENU_WORK_SERVO_PARM12            			32	//unsigned	int   		servo_program_version程序版本2     y
#define MENU_WORK_SERVO_PARM13            			33	//bit   			    servo_epprom_parm_reset_flag掉电使能0 y(复位1->0)
#define MENU_WORK_SERVO_PARM14            			34	//unsigned  int 	    servo_unique_address_id_set舵机ID 2  y
#define MENU_WORK_SERVO_PARM15            			35	//unsigned  int  	  	servo_common_command_address_set通用ID 2 y
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_PARM_SETUP	      6
//【参数ID1】
#define MENU_SETUP_SERVO_PARM0              		40	//bit               	servo_reset_flag_set       复位使能1 读取0  y
#define MENU_SETUP_SERVO_PARM1          			41	//unsigned 	int      	servo_reset_position       复位位置  1500  y
#define MENU_SETUP_SERVO_PARM2            			42	//unsigned 	char     	servo_reset_time_set       复位时间  10    y
#define MENU_SETUP_SERVO_PARM3           	  		43	//unsigned 	char     	servo_reset_step_set       复位步长  1     y
#define MENU_SETUP_SERVO_PARM4           			44	//unsgined 	char     	control_interval_time_set  启动时间  1     y
#define MENU_SETUP_SERVO_PARM5         			    45	//signed   	char     	control_interval_step_set  启动步长  10    y
#define MENU_SETUP_SERVO_PARM6       				46	//unsigned 	char     	servo_halt_zone_set        堵转区间  5     y
#define MENU_SETUP_SERVO_PARM7       				47	//unsigned 	char     	servo_halt_time_set        堵转时间  100   y
#define MENU_SETUP_SERVO_PARM8             			48	//unsigend 	int      	servo_protect_time_set     保护时间  101   y
#define MENU_SETUP_SERVO_PARM9             			49	//signed   	int      	servo_protect_pwm_cmp      保护比较  1000  N
#define MENU_SETUP_SERVO_PARM10        			    50	//signed   	int      	servo_protect_pwm_set      保护输出  1020  y
#define MENU_SETUP_SERVO_PARM11        			    51	//signed   	int      	servo_max_pwm_set          最大输出  1023  y
#define MENU_SETUP_SERVO_PARM12        	  		    52	//signed   	int      	 驱动频率  3     y
#define MENU_SETUP_SERVO_PARM13            			53	//unsigned 	int      	motor_pwm_cycle            频率设置  0     y
#define MENU_SETUP_SERVO_PARM14            			54  //bit               	servo_init_flag_set        上电复位  8  读取0  y
#define MENU_SETUP_SERVO_PARM15            			55	//signed 	int      	servo_work_base_voltage    启动电压  9     y
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_PARM_DEBUG       	7
//【参数ID1】
#define MENU_DEBUG_SERVO_PARM0    					60 //signed 		int	    servo_position_pid_parm_p_set     位置比例 1  N
#define MENU_DEBUG_SERVO_PARM1   					61 //signed 		int    	position_pid_speed_parm_radio     位置比率 2  N
#define MENU_DEBUG_SERVO_PARM2  					62 //signed 		int     servo_speed_run_sample_k_set      速度比率 4  N
#define MENU_DEBUG_SERVO_PARM3        				63 //unsigned 	    int     servo_position_sample_ov_time_set 位置时间 16 N
#define MENU_DEBUG_SERVO_PARM4      				64 //signed 		int     servo_speed_sample_ov_time_set    速度时间 32 N
#define MENU_DEBUG_SERVO_PARM5       				65 //signed 		int     servo_speed_pid_parm_p_set        速度比例 2  N
#define MENU_DEBUG_SERVO_PARM6       				66 //signed 		int     servo_speed_pid_parm_p_radio      速度增量 1  N
#define MENU_DEBUG_SERVO_PARM7     					67 //
#define MENU_DEBUG_SERVO_PARM8      				68 //
#define MENU_DEBUG_SERVO_PARM9   					69 //
#define MENU_DEBUG_SERVO_PARM10   					70 //
#define MENU_DEBUG_SERVO_PARM11   					71 //
#define MENU_DEBUG_SERVO_PARM12   					72 //
#define MENU_DEBUG_SERVO_PARM13   					73 //
#define MENU_DEBUG_SERVO_PARM14   					74 //
#define MENU_DEBUG_SERVO_PARM15   					75 //

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：0          8
//【参数ID1】
#define MENU_DOWMLOAD_WORK_PARM               	    76  //download work_parm
#define MENU_DOWMLOAD_SETUP_PARM                    77  //download setup_parm
#define MENU_DOWMLOAD_DEBUG_PARM                    78  //download debug_parm
#define MENU_DOWMLOAD_CONFIG_PARM                   79  //download config_parm
#define MENU_DOWMLOAD_ALL_PARM               	    80  //download all_parm
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_FB                9
//【参数ID1】
#define MENU_FB_SERVO_WORK_PARM0    		        81	//unsigned 	int      	control_pulse_upper  ok
#define MENU_FB_SERVO_WORK_PARM1                    82	//unsigned 	int      	control_pulse_mid    ok
#define MENU_FB_SERVO_WORK_PARM2                    83	//unsigned 	int      	control_pulse_lower  ok
#define MENU_FB_SERVO_WORK_PARM3                    84	//unsigned 	int      	control_set_upper    ok
#define MENU_FB_SERVO_WORK_PARM4                    85	//unsigned 	int      	control_set_mid      ok
#define MENU_FB_SERVO_WORK_PARM5                    86	//unsigned 	int      	control_set_lower    ok
#define MENU_FB_SERVO_WORK_PARM6                    87	//unsigned 	char     	servo_zero_zone_set  ok
#define MENU_FB_SERVO_WORK_PARM7                    88  //unsigned 	char     	servo_zero_zone_time ok
#define MENU_FB_SERVO_WORK_PARM8                    89	//bit               	 ok
#define MENU_FB_SERVO_WORK_PARM9                    90	//bit               	servo_dir_inverse_flag  ok
#define MENU_FB_SERVO_WORK_PARM10                   91	//bit               	servo_pwm_inverse_flag  ok
#define MENU_FB_SERVO_WORK_PARM11                   92	//signed   	char     	insig_value_diff_set    ok
#define MENU_FB_SERVO_WORK_PARM12                   93	//unsigned	int   		servo_program_version   ok
#define MENU_FB_SERVO_WORK_PARM13                   94	//bit     		        servo_epprom_parm_reset_flag   ok
#define MENU_FB_SERVO_WORK_PARM14                   95	//unsigned  int 		servo_unique_address_id_set       ok
#define MENU_FB_SERVO_WORK_PARM15                   96	//unsigned  int  	  	servo_common_command_address_set  ok
#define MENU_FB_SERVO_SETUP_PARM0                   100	//bit               	servo_reset_flag_set  ok
#define MENU_FB_SERVO_SETUP_PARM1                   101	//unsigned 	int      	servo_reset_position  ok
#define MENU_FB_SERVO_SETUP_PARM2                   102	//unsigned 	char     	servo_reset_time_set  ok
#define MENU_FB_SERVO_SETUP_PARM3                   103	//unsigned 	char     	servo_reset_step_set  ok
#define MENU_FB_SERVO_SETUP_PARM4                   104 //unsgined 	char     	control_interval_time_set  ok
#define MENU_FB_SERVO_SETUP_PARM5                   105	//signed   	char     	control_interval_step_set  ok
#define MENU_FB_SERVO_SETUP_PARM6                   106	//unsigned 	char     	servo_halt_zone_set   ok
#define MENU_FB_SERVO_SETUP_PARM7                   107	//unsigned 	char     	servo_halt_time_set   ok
#define MENU_FB_SERVO_SETUP_PARM8                   108	//unsigend 	int      	servo_protect_time_set  ok
#define MENU_FB_SERVO_SETUP_PARM9                   109	//signed   	int      	servo_protect_pwm_cmp ok
#define MENU_FB_SERVO_SETUP_PARM10                  110	//signed   	int      	servo_protect_pwm_set ok
#define MENU_FB_SERVO_SETUP_PARM11                  111	//signed   	int      	servo_max_pwm_set     ok
#define MENU_FB_SERVO_SETUP_PARM12                  112	//signed   	int      	servo_motor_work_pwm_cycle  ok
#define MENU_FB_SERVO_SETUP_PARM13                  113	//unsigned  int         motor_pwm_cycle   ok
#define MENU_FB_SERVO_SETUP_PARM14                  114	//bit                   servo_init_flag_set    ok
#define MENU_FB_SERVO_SETUP_PARM15                  115	//signed 	int         servo_work_base_voltage  ok

#define MENU_FB_SERVO_DEBUG_PARM0                   120	//signed 	int         servo_position_pid_parm_p_set   ok
#define MENU_FB_SERVO_DEBUG_PARM1                   121	//signed 	int         position_pid_speed_parm_radio   ok
#define MENU_FB_SERVO_DEBUG_PARM2                   122	//signed 	int         servo_speed_run_sample_k_set    ok
#define MENU_FB_SERVO_DEBUG_PARM3                   123	//unsigned 	int         servo_position_sample_ov_time_set  ok
#define MENU_FB_SERVO_DEBUG_PARM4                   124	//signed 	int         servo_speed_sample_ov_time_set  ok
#define MENU_FB_SERVO_DEBUG_PARM5                   125	//signed 	int         servo_speed_pid_parm_p_set      ok
#define MENU_FB_SERVO_DEBUG_PARM6                   126	//signed 	int         servo_speed_pid_parm_p_radio    ok
#define MENU_FB_SERVO_DEBUG_PARM7                   127	//

#define MENU_FB_SERVO_CONFIG_PARM0                  130	//bit     				servo_work_mode_lock_flag_set   ok
#define MENU_FB_SERVO_CONFIG_PARM1                  131	//unsigned  char     	servo_work_mode_now             ok
#define MENU_FB_SERVO_CONFIG_PARM2                  133	//unsigned  char     	servo_pram_config_set           ok
#define MENU_FB_SERVO_CONFIG_PARM3                  135	//bit     				servo_command_fb_flag           ok

#define MENU_FB_SERVO_STATE_PARM0              	    140 //unsigned	char  		servo_error_value
#define MENU_FB_SERVO_STATE_PARM1              	    141 //signed    int     	servo_position_value_now
#define MENU_FB_SERVO_STATE_PARM2              	    142 //unsigned  int     	servo_speed_value_now
#define MENU_FB_SERVO_STATE_PARM3              	    143 //unsigned  int     	servo_current_value_now
#define MENU_FB_SERVO_STATE_PARM4              	    144 //unsigned  int     	servo_tempture_value_now
#define MENU_FB_SERVO_STATE_PARM5              	    145 //unsigned  int     	servo_vddin_value_now
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_TARGET    		    10
//【参数ID1】
#define MENU_SERVO_RUN_POSITION_VALUE         	    150	//目标位置  		取值0-65535   默认值 0
//【参数ID2】
#define MENU_SERVO_RUN_SPEED_VALUE                  151	//目标速度  		取值0-65535   默认值 0
//参数ID2参数值高位	= 速度周期                                                                      N
//参数ID2参数值低位	= 速度步长                                                                      N
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_RESET                   15
//舵机地址 = XX  单个舵机动作
//舵机地址 = 248 全部舵机动作
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_RUN                     16
//舵机地址 = XX  单个舵机动作
//舵机地址 = 248 全部舵机动作
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_STOP                    17
//舵机地址 = XX  单个舵机动作
//舵机地址 = 248 全部舵机动作
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_DUMP             	    18
//舵机地址 = XX  单个舵机动作
//舵机地址 = 248 全部舵机动作
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_CHECH_CODE                    19	//舵机校验码命令
//【参数ID1】
#define MENU_SERVO_CHECK_CODE        				152	//校验码(舵机地址+程序版本号)
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//参数值
//舵机信号源参数
//MENU_CONFIG_SERVO_INSIG_SET
#define SERVO_INSIG_PPM       						0//PPM信号源
#define SERVO_INSIG_UART       					    1//串口信号源
//舵机类型参数值
//MENU_CONFIG_SERVO_TYPE_SET
#define SERVO_TYPE_NORM       						0//普通舵机
#define SERVO_TYPE_UART     						1//串口舵机
//舵机错误参数值
//MENU_FB_SERVO_ERROR
#define SERVO_EORR_OV_CURR    			        	1//过流报警
#define SERVO_EORR_OV_VLOT    			        	2//过压报警
#define SERVO_EORR_OV_TEMP    			        	3//过温报警
#define SERVO_EORR_OV_PARM                          4//数据包错误

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
#define SERVO_UART_EN_ID                  			251
#define SERVO_SET_ADDRESS_ID                  	    252
#define SERVO_CHECK_CODE_ID                  		253
#endif // CSYSDEFINE_H
