/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "stm32f10x.h"
////////////////////////////////////////////////////////////////////////////////////////
//通讯包数据格式：
//数据头       1个字节   0XFA	 
//舵机地址     1个字节   
//命令字       1个字节
//状态字       1个字节
//参数ID       1个字节
//参数值高位   1个字节
//参数值低位   1个字节
//参数ID       1个字节
//参数值高位   1个字节
//参数值低位   1个字节
//校验和       1个字节   校验和 = 舵机地址+命令字+参数1ID+参数2ID
//结束码       1个字节   0XED 
/////////////////////////////////////////////////////////////////////////////////////////
//命令字(0-19)
//命令字
#define SERVO_COMMAND_SERVO_UART_EN         		1 	//舵机通讯命令
#define SERVO_COMMAND_SERVO_COM_PACKAGE         	2   //舵机数据包命令
#define SERVO_COMMAND_SERVO_ADDRESS_SET     		3		//舵机地址命令
#define SERVO_COMMAND_SERVO_PARM_CONFIG     		4		//舵机配置命令
#define SERVO_COMMAND_SERVO_PARM_WORK    			5		//舵机工作命令
#define SERVO_COMMAND_SERVO_PARM_SETUP     			6		//舵机设置命令
#define SERVO_COMMAND_SERVO_PARM_DEBUG    			7		//舵机调试命令
#define SERVO_COMMAND_SERVO_PARM_DOWNLOAD      		8		//舵机下载命令
#define SERVO_COMMAND_SERVO_FB       	        	9		//舵机反馈命令
#define SERVO_COMMAND_SERVO_TARGET        			10		//舵机目标命令
#define SERVO_COMMAND_SERVO_RESET       			15		//舵机复位命令	
#define SERVO_COMMAND_SERVO_RUN                 	16 		//舵机运行命令
#define SERVO_COMMAND_SERVO_STOP        	    	17	//舵机停止命令	
#define SERVO_COMMAND_SERVO_DUMP        	    	18 	//舵机掉电命令
#define SERVO_COMMAND_CHECH_CODE                19	//舵机校验命令
#define SERVO_COMMAND_NONE                      20	//舵机校验命令
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//状态字
#define SERVO_STATE_COM        	   							1 	//接收状态
#define SERVO_STATE_FB        	   							2 	//反馈状态
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//指令参数ID
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_UART_EN  		    	1
//【参数ID1】
#define SERVO_COMMAND_SERVO_UART_COM_ENABLE     1		//配置使能				默认值0xAAAA	
#define SERVO_COMMAND_SERVO_UART_COM_UNABLE    	2		//配置退出				默认值0x5555	
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_COM_PACKAGE 			2
//【参数ID1】
#define SERVO_COMMAND_SERVO_COM_PACK_START  		3		//命令包开始				默认值0	
#define SERVO_COMMAND_SERVO_COM_PACK_END  			4		//命令包结束	       	默认值0	
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_ADDRESS_SET       3
//【参数ID1】						
#define MENU_CONFIG_SERVO_COMMON_ADDRESS_SET  	5		//舵机通用地址	取值0-254			默认值248   
#define MENU_CONFIG_SERVO_UNIQUE_ADDRESS_SET    6		//舵机设备地址	取值0-65534		默认值16 
#define MENU_CONFIG_SERVO_RESET_ADDRESS_SET     7		//舵机设备地址	取值0-65534		默认值16 
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_PARM_CONFIG  			4
//【参数ID1】
#define MENU_CONFIG_SERVO_PARM0            	    10	//bit 								servo_work_mode_lock_flag_set 
#define MENU_CONFIG_SERVO_PARM1            	    11	//unsigned 	char 			servo_work_mode_now
#define MENU_CONFIG_SERVO_PARM2              	  12	//unsigned 	char     	servo_pram_config_set
#define MENU_CONFIG_SERVO_PARM3              	  14	//bit               	servo_command_fb_flag
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_PARM_WORK					5
//【参数ID1】
#define MENU_WORK_SERVO_PARM0             			20	//unsigned 	int      	control_pulse_upper 
#define MENU_WORK_SERVO_PARM1             			21	//unsigned 	int      	control_pulse_mid	
#define MENU_WORK_SERVO_PARM2            				22	//unsigned 	int      	control_pulse_lower		
#define MENU_WORK_SERVO_PARM3             			23	//unsigned 	int      	control_set_upper	
#define MENU_WORK_SERVO_PARM4          					24	//unsigned 	int      	control_set_mid
#define MENU_WORK_SERVO_PARM5            				25	//unsigned 	int      	control_set_lower
#define MENU_WORK_SERVO_PARM6            				26	//unsigned 	char     	servo_zero_zone_set
#define MENU_WORK_SERVO_PARM7            				27	//unsigned 	char     	servo_zero_zone_time
#define MENU_WORK_SERVO_PARM8            				28	//bit               	servo_free_lock_flag
#define MENU_WORK_SERVO_PARM9            				29	//bit               	servo_dir_inverse_flag
#define MENU_WORK_SERVO_PARM10            			30	//bit               	servo_pwm_inverse_flag
#define MENU_WORK_SERVO_PARM11            			31	//signed   	char     	insig_value_diff_set
#define MENU_WORK_SERVO_PARM12            			32	//unsigned	int   		servo_program_version
#define MENU_WORK_SERVO_PARM13            			33	//bit   			servo_epprom_parm_reset_flag
#define MENU_WORK_SERVO_PARM14            			34	//unsigned  int 			servo_unique_address_id_set
#define MENU_WORK_SERVO_PARM15            			35	//unsigned  int  	  	servo_common_command_address_set
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_PARM_SETUP	      6
//【参数ID1】
#define MENU_SETUP_SERVO_PARM0              		40	//bit               	servo_reset_flag_set
#define MENU_SETUP_SERVO_PARM1          			41	//unsigned 	int      	servo_reset_position
#define MENU_SETUP_SERVO_PARM2            			42	//unsigned 	char     	servo_reset_time_set
#define MENU_SETUP_SERVO_PARM3           	  		43	//unsigned 	char     	servo_reset_step_set
#define MENU_SETUP_SERVO_PARM4           			44	//unsgined 	char     	control_interval_time_set
#define MENU_SETUP_SERVO_PARM5         			45	//signed   	char     	control_interval_step_set
#define MENU_SETUP_SERVO_PARM6       				46	//unsigned 	char     	servo_halt_zone_set
#define MENU_SETUP_SERVO_PARM7       				47	//unsigned 	char     	servo_halt_time_set
#define MENU_SETUP_SERVO_PARM8             			48	//unsigend 	int      	servo_protect_time_set
#define MENU_SETUP_SERVO_PARM9             			49	//signed   	int      	servo_protect_pwm_cmp
#define MENU_SETUP_SERVO_PARM10        			50	//signed   	int      	servo_protect_pwm_set
#define MENU_SETUP_SERVO_PARM11        			51	//signed   	int      	servo_max_pwm_set
#define MENU_SETUP_SERVO_PARM12        	  		52	//signed   	int      	servo_motor_work_pwm_cycle
#define MENU_SETUP_SERVO_PARM13            			53	//unsigned 	int      	motor_pwm_cycle
#define MENU_SETUP_SERVO_PARM14            			54  //bit               	servo_init_flag_set
#define MENU_SETUP_SERVO_PARM15            			55	//signed 	  int      	servo_work_base_voltage	
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_PARM_DEBUG       	7	
//【参数ID1】
#define MENU_DEBUG_SERVO_PARM0    					60 //signed 		int	servo_position_pid_parm_p_set	
#define MENU_DEBUG_SERVO_PARM1   					61 //signed 		int    	position_pid_speed_parm_radio	
#define MENU_DEBUG_SERVO_PARM2  					62 //signed 		int     	servo_speed_run_sample_k_set
#define MENU_DEBUG_SERVO_PARM3        				63 //unsigned 	int     	servo_position_sample_ov_time_set
#define MENU_DEBUG_SERVO_PARM4      				64 //signed 		int     	servo_speed_sample_ov_time_set
#define MENU_DEBUG_SERVO_PARM5       				65 //signed 		int     	servo_speed_pid_parm_p_set	
#define MENU_DEBUG_SERVO_PARM6       				66 //signed 		int     	servo_speed_pid_parm_p_radio
#define MENU_DEBUG_SERVO_PARM7     					67 //	
#define MENU_DEBUG_SERVO_PARM8      					68 //
#define MENU_DEBUG_SERVO_PARM9   					69 //	
#define MENU_DEBUG_SERVO_PARM10   					70 //	
#define MENU_DEBUG_SERVO_PARM11   					71 //	
#define MENU_DEBUG_SERVO_PARM12   					72 //	
#define MENU_DEBUG_SERVO_PARM13   					73 //	
#define MENU_DEBUG_SERVO_PARM14   					74 //	
#define MENU_DEBUG_SERVO_PARM15   					75 //	

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_DOWNLOAD          8
//【参数ID1】
#define MENU_DOWMLOAD_WORK_PARM               	76  //download work_parm
#define MENU_DOWMLOAD_SETUP_PARM                77  //download setup_parm
#define MENU_DOWMLOAD_DEBUG_PARM                78  //download debug_parm
#define MENU_DOWMLOAD_CONFIG_PARM               79  //download config_parm
#define MENU_DOWMLOAD_ALL_PARM               	80  //download all_parm
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_FB                9
//【参数ID1】														
#define MENU_FB_SERVO_WORK_PARM0    		    81	//unsigned 	int      	control_pulse_upper 			-->6
#define MENU_FB_SERVO_WORK_PARM1                82	//unsigned 	int      	control_pulse_mid
#define MENU_FB_SERVO_WORK_PARM2                83	//unsigned 	int      	control_pulse_lower 			-->5
#define MENU_FB_SERVO_WORK_PARM3                84	//unsigned 	int      	control_set_upper 			-->9
#define MENU_FB_SERVO_WORK_PARM4                85	//unsigned 	int      	control_set_mid  			-->8
#define MENU_FB_SERVO_WORK_PARM5                86	//unsigned 	int      	control_set_lower 			-->7
#define MENU_FB_SERVO_WORK_PARM6                87	//unsigned 	char     	servo_zero_zone_set			-->2
#define MENU_FB_SERVO_WORK_PARM7                88  //unsigned 	char     	servo_zero_zone_time
#define MENU_FB_SERVO_WORK_PARM8                89	//bit               	servo_free_lock_flag 		-->11
#define MENU_FB_SERVO_WORK_PARM9                90	//bit               	servo_dir_inverse_flag		-->12
#define MENU_FB_SERVO_WORK_PARM10               91	//bit               	servo_pwm_inverse_flag
#define MENU_FB_SERVO_WORK_PARM11               92	//signed   	char     	insig_value_diff_set
#define MENU_FB_SERVO_WORK_PARM12               93	//unsigned	int   		servo_program_version 		//-->1
#define MENU_FB_SERVO_WORK_PARM13               94	//bit     		servo_epprom_parm_reset_flag
#define MENU_FB_SERVO_WORK_PARM14               95	//unsigned  int 			servo_unique_address_id_set
#define MENU_FB_SERVO_WORK_PARM15               96	//unsigned  int  	  	servo_common_command_address_set
#define MENU_FB_SERVO_SETUP_PARM0               100	//bit               	servo_reset_flag_set 		-->10
#define MENU_FB_SERVO_SETUP_PARM1               101	//unsigned 	int      	servo_reset_position
#define MENU_FB_SERVO_SETUP_PARM2               102	//unsigned 	char     	servo_reset_time_set
#define MENU_FB_SERVO_SETUP_PARM3               103	//unsigned 	char     	servo_reset_step_set
#define MENU_FB_SERVO_SETUP_PARM4               104     //unsgined 	char     	control_interval_time_set
#define MENU_FB_SERVO_SETUP_PARM5               105	//signed   	char     	control_interval_step_set
#define MENU_FB_SERVO_SETUP_PARM6               106	//unsigned 	char     	servo_halt_zone_set 			-->15
#define MENU_FB_SERVO_SETUP_PARM7               107	//unsigned 	char     	servo_halt_time_set 
#define MENU_FB_SERVO_SETUP_PARM8               108	//unsigend 	int      	servo_protect_time_set 			-->16
#define MENU_FB_SERVO_SETUP_PARM9               109	//signed   	int      	servo_protect_pwm_cmp
#define MENU_FB_SERVO_SETUP_PARM10              110	//signed   	int      	servo_protect_pwm_set 			-->17
#define MENU_FB_SERVO_SETUP_PARM11              111	//signed   	int      	servo_max_pwm_set 				-->3
#define MENU_FB_SERVO_SETUP_PARM12              112	//signed   	int      	servo_motor_work_pwm_cycle 		-->13
#define MENU_FB_SERVO_SETUP_PARM13              113	//unsigned  int       	motor_pwm_cycle 				-->14
#define MENU_FB_SERVO_SETUP_PARM14              114	//bit                 servo_init_flag_set
#define MENU_FB_SERVO_SETUP_PARM15              115	//signed 		int       servo_work_base_voltage 		-->4

#define MENU_FB_SERVO_DEBUG_PARM0               120	//signed 		int       servo_position_pid_parm_p_set
#define MENU_FB_SERVO_DEBUG_PARM1               121	//signed 		int       position_pid_speed_parm_radio
#define MENU_FB_SERVO_DEBUG_PARM2               122	//signed 		int       servo_speed_run_sample_k_set
#define MENU_FB_SERVO_DEBUG_PARM3               123	//unsigned 	int       servo_position_sample_ov_time_set
#define MENU_FB_SERVO_DEBUG_PARM4               124	//signed 		int       servo_speed_sample_ov_time_set
#define MENU_FB_SERVO_DEBUG_PARM5               125	//signed 		int       servo_speed_pid_parm_p_set
#define MENU_FB_SERVO_DEBUG_PARM6               126	//signed 		int       servo_speed_pid_parm_p_radio
#define MENU_FB_SERVO_DEBUG_PARM7               127	//

#define MENU_FB_SERVO_CONFIG_PARM0              130	//bit     						servo_work_mode_lock_flag_set
#define MENU_FB_SERVO_CONFIG_PARM1              131	//unsigned  char     	servo_work_mode_now
#define MENU_FB_SERVO_CONFIG_PARM2              133	//unsigned  char     	servo_pram_config_set
#define MENU_FB_SERVO_CONFIG_PARM3              135	//bit     						servo_command_fb_flag

#define MENU_FB_SERVO_STATE_PARM0              	140 //unsigned	char  		servo_error_value
#define MENU_FB_SERVO_STATE_PARM1              	141 //signed    int     	servo_position_value_now
#define MENU_FB_SERVO_STATE_PARM2              	142 //unsigned  int     	servo_speed_value_now
#define MENU_FB_SERVO_STATE_PARM3              	143 //unsigned  int     	servo_current_value_now
#define MENU_FB_SERVO_STATE_PARM4              	144 //unsigned  int     	servo_tempture_value_now
#define MENU_FB_SERVO_STATE_PARM5              	145 //unsigned  int     	servo_vddin_value_now
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_TARGET    		    10
//【参数ID1】															
#define MENU_SERVO_RUN_POSITION_VALUE         	150	//目标位置  		取值0-65535   默认值 0
//【参数ID2】															
#define MENU_SERVO_RUN_SPEED_VALUE              151	//目标速度  		取值0-65535   默认值 0
//参数ID2参数值高位	= 速度周期
//参数ID2参数值低位	= 速度步长
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_RESET             15 
//舵机地址 = XX  单个舵机动作
//舵机地址 = 248 全部舵机动作
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_RUN               16
//舵机地址 = XX  单个舵机动作
//舵机地址 = 248 全部舵机动作
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_STOP              17
//舵机地址 = XX  单个舵机动作
//舵机地址 = 248 全部舵机动作
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_SERVO_DUMP             	18
//舵机地址 = XX  单个舵机动作
//舵机地址 = 248 全部舵机动作
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//命令字：SERVO_COMMAND_CHECH_CODE              19	//舵机校验码命令
//【参数ID1】
#define MENU_SERVO_CHECK_CODE        						152	//校验码(舵机地址+程序版本号)
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//参数值
//舵机信号源参数
//MENU_CONFIG_SERVO_INSIG_SET
#define SERVO_INSIG_PPM       									0//PPM信号源
#define SERVO_INSIG_UART       									1//串口信号源
//舵机类型参数值
//MENU_CONFIG_SERVO_TYPE_SET
#define SERVO_TYPE_NORM       									0//普通舵机
#define SERVO_TYPE_UART     										1//串口舵机
//舵机错误参数值
//MENU_FB_SERVO_ERROR
#define SERVO_EORR_OV_CURR    			        		1//过流报警
#define SERVO_EORR_OV_VLOT    			        		2//过压报警
#define SERVO_EORR_OV_TEMP    			        		3//过温报警
#define SERVO_EORR_OV_PARM                      		4//数据包错误

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
#define SERVO_UART_EN_ID                  			251
#define SERVO_SET_ADDRESS_ID                  		252
#define SERVO_CHECK_CODE_ID                  		253


/////////////////////////////////////////////////////////////////////////////////////////
#define  UART_DELAY_OV_TIME                 20000

//---------------------------------------------------------------------------------------
struct Servo_Data_Stru_{ //64 byte
	int16_t work_p0;
	int16_t work_p1;
	int16_t work_p2;
	int16_t work_p3;
	int16_t work_p4;
	int16_t work_p5;
	int16_t work_p6;
	int16_t work_p7;
	int16_t work_p8;
	int16_t work_p9;
	int16_t work_p10;
	int16_t work_p11;
	int16_t work_p12;
	int16_t work_p13;
	int16_t work_p14;
	int16_t work_p15;
	int16_t set_p0;
	int16_t set_p1;
	int16_t set_p2;
	int16_t set_p3;
	int16_t set_p4;
	int16_t set_p5;
	int16_t set_p6;
	int16_t set_p7;
	int16_t set_p8;
	int16_t set_p9;
	int16_t set_p10;
	int16_t set_p11;
	int16_t set_p12;
	int16_t set_p13;
	int16_t set_p14;
	int16_t set_p15;
	int16_t debug_p0;
	int16_t debug_p1;
	int16_t debug_p2;
	int16_t debug_p3;
	int16_t debug_p4;
	int16_t debug_p5;
	int16_t debug_p6;
	int16_t debug_p7;
	int16_t debug_p8;
	int16_t debug_p9;
	int16_t debug_p10;
};


void uart_receive_data(uint8_t data);
void uart_send_command(uint8_t id, uint8_t cmd, uint8_t state, uint8_t para_id1, uint16_t para1, uint8_t para_id2, uint16_t para2);

void save_servo_data_in_flash(uint8_t seq,uint16_t version);
void read_servo_data_in_flash(uint8_t seq);
void test_falsh_progrm(void);

void menu_combine_position(uint16_t pos);
uint8_t menu_combine_fb_work_parm(void);
void menu_combine_prom_work_parm(void);
void menu_combine_prom_work_clear_parm(void);
uint8_t menu_combine_verify_work_parm(void);

#define 	FACTORY_OUT_SPACE	0
#define 	CUSTOMER_OUT_SPACE	9
#define 	USER_OUT_SPACE		108



