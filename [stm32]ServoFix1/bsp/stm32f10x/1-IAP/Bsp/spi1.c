/******************** (C) COPYRIGHT 2016 ********************
* Filename:
* Description:
* Platform:
* Other info:
* Author         Date          Notes
xiangbin.huang   2017.04.11    The first version template
***************************************************************/

#include "spi1.h"

#define		SELF_TEST_X		0x0D
#define		SELF_TEST_Y		0x0E
#define		SELF_TEST_Z		0x0F
#define		SELF_TEST_A		0x10
#define		SMPLRT_DIV		0x19
#define		CONFIG				0x1A
#define		GYRO_CONFIG		0x1B
#define		ACCEL_CONFIG	0x1C
#define		MOT_THR				0x1F
#define		FIFO_EN				0x23

#define		INT_PIN_CFG		0x37
#define		INT_ENABLE		0x38
#define		INT_STATUS		0x3A

#define		ACCEL_XOUT_H	0x3B
#define		ACCEL_XOUT_L	0x3C
#define		ACCEL_YOUT_H	0x3D
#define		ACCEL_YOUT_L	0x3E
#define		ACCEL_ZOUT_H	0x3F
#define		ACCEL_ZOUT_L	0x40
#define		TEMP_OUT_H		0x41
#define		TEMP_OUT_L		0x42
#define		GYRO_XOUT_H		0x43
#define		GYRO_XOUT_L		0x44
#define		GYRO_YOUT_H		0x45
#define		GYRO_YOUT_L		0x46
#define		GYRO_ZOUT_H		0x47
#define		GYRO_ZOUT_L		0x48


/*----------------------------------------------------------------------------*/
#define MPU6000_CSN_HIGH()     GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define MPU6000_CSN_LOW()      GPIO_ResetBits(GPIOB, GPIO_Pin_1)

#define HMC5983_CSN_HIGH()     GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define HMC5983_CSN_LOW()      GPIO_ResetBits(GPIOA, GPIO_Pin_4)

#define MS5611_CSN_HIGH()     GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define MS5611_CSN_LOW()      GPIO_ResetBits(GPIOA, GPIO_Pin_3)

/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void spi1_init(void)
{
  SPI_InitTypeDef    SPI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  MPU6000_CSN_HIGH(); 
	HMC5983_CSN_HIGH(); 
	MS5611_CSN_HIGH(); 
	
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 		       
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 	     
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		 	      
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;			       
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   	      
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;  
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_Cmd(SPI1, ENABLE);
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
static u8 SPI1_Read_Write(u8 dat)
{  	
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPI1, dat);		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(SPI1);
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
static u8 SPI1_6000_WriteReg(u8 reg, u8 dat)
{
  u8 status;
	reg |=0x80;
  MPU6000_CSN_LOW();
  status = SPI1_Read_Write(reg);
  SPI1_Read_Write(dat); 	   
  MPU6000_CSN_HIGH();	
  return(status);
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
static u8 SPI1_6000_ReadReg(u8 reg)
{
  u8 reg_val;
  reg |=0x80;
  MPU6000_CSN_LOW();
  SPI1_Read_Write(reg); 
  reg_val = SPI1_Read_Write(0xFF);
  MPU6000_CSN_HIGH();		
  return reg_val;
}	
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void SPI1_6000_WriteReg_NByte(u8 reg, u8 *data, u8 size)
{
	u8 i;
	for(i=0; i<size; i++)
	{
		SPI1_6000_WriteReg(reg, data[i]);
		reg ++;
	}
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void SPI1_6000_ReadReg_NByte(u8 reg, u8 *data, u8 size)
{
	u8 i;
	for(i=0; i<size; i++)
	{
		data[i] = SPI1_6000_ReadReg(reg);
		reg ++;
	}
}	

/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
static u8 SPI1_5983_WriteReg(u8 reg, u8 dat)
{
  u8 status;
  HMC5983_CSN_LOW();
  status = SPI1_Read_Write(reg);
  SPI1_Read_Write(dat); 	   
  HMC5983_CSN_HIGH();	
  return(status);
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
static u8 SPI1_5983_ReadReg(u8 reg)
{
  u8 reg_val;
  reg |=0x80;
  HMC5983_CSN_LOW();
  SPI1_Read_Write(reg); 
  reg_val = SPI1_Read_Write(0xFF);
  HMC5983_CSN_HIGH();		
  return reg_val;
}	
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void SPI1_5883_WriteReg_NByte(u8 reg, u8 *data, u8 size)
{
	u8 i;
	for(i=0; i<size; i++)
	{
		SPI1_5983_WriteReg(reg, data[i]);
		reg ++;
	}
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void SPI1_5883_ReadReg_NByte(u8 reg, u8 *data, u8 size)
{
	u8 i;
	for(i=0; i<size; i++)
	{
		data[i] = SPI1_5983_ReadReg(reg);
		reg ++;
	}
}	


/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
static u8 SPI1_5611_WriteReg(u8 reg, u8 dat)
{
  u8 status;
  MS5611_CSN_LOW();
  status = SPI1_Read_Write(reg);
  SPI1_Read_Write(dat); 	   
  MS5611_CSN_HIGH();	
  return(status);
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
static u8 SPI1_5611_ReadReg(u8 reg)
{
  u8 reg_val;
  reg |=0x80;
  MS5611_CSN_LOW();
  SPI1_Read_Write(reg); 
  reg_val = SPI1_Read_Write(0xFF);
  MS5611_CSN_HIGH();		
  return reg_val;
}	
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void SPI1_5611_WriteReg_NByte(u8 reg, u8 *data, u8 size)
{
	u8 i;
	for(i=0; i<size; i++)
	{
		SPI1_5611_WriteReg(reg, data[i]);
		reg ++;
	}
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void SPI1_5611_ReadReg_NByte(u8 reg, u8 *data, u8 size)
{
	u8 i;
	for(i=0; i<size; i++)
	{
		data[i] = SPI1_5611_ReadReg(reg);
		reg ++;
	}
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
u8 SPI1_6000_Who_Am_I(void)
{
	return SPI1_6000_ReadReg(0x75);
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
//void LSM6DS33_Init(void)
//{
//  u8 Temp;
//  //400HZ,????
//  Temp = LSM6DS33_XG_FIFO_ODR_800HZ | LSM6DS33_XG_FIFO_MODE_CONTINUOUS_OVERWRITE;
//  SPI_NRF_WriteReg(LSM6DS33_XG_FIFO_CTRL5,Temp);
//  //416HZ,+-500°/s
//  Temp = LSM6DS33_G_ODR_833HZ | LSM6DS33_G_FS_500;
//  SPI_NRF_WriteReg(LSM6DS33_XG_CTRL2_G,Temp);
//  //???Z,Y,X??
//  Temp = LSM6DS33_G_ZEN_ENABLE | LSM6DS33_G_YEN_ENABLE | LSM6DS33_G_XEN_ENABLE;
//  SPI_NRF_WriteReg(LSM6DS33_XG_CTRL10_C,Temp);
//  //?????INT2??
//  Temp = LSM6DS33_XG_INT2_INTERUPT_GYRO;
//  SPI_NRF_WriteReg(LSM6DS33_XG_INT2_CTRL,Temp);
//  //416HZ,+-4g, 400HZ
//  Temp = LSM6DS33_XL_ODR_833HZ | LSM6DS33_XL_FS_4G | LSM6DS33_XL_BW_400HZ;
//  SPI_NRF_WriteReg(LSM6DS33_XG_CTRL1_XL ,Temp);
//  //????Z,Y,X??
//  Temp = LSM6DS33_XL_ZEN_ENABLE  | LSM6DS33_XL_YEN_ENABLE  | LSM6DS33_XL_XEN_ENABLE; 
//  SPI_NRF_WriteReg(LSM6DS33_XG_CTRL9_XL,Temp);
//  //??????INT1??
//  Temp = LSM6DS33_XG_INT1_INTERUPT_ACCEL;
//  SPI_NRF_WriteReg(LSM6DS33_XG_INT1_CTRL,Temp);
//}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
//void LSM6DS33_Read_Raw_Data(void)
//{
//  s16 Temp;
//  Temp=(s16)(SPI_NRF_ReadReg(LSM6DS33_XG_OUT_TEMP_H)<<8);
//  LSM6DS33_Raw_Data.Temper = Temp | SPI_NRF_ReadReg(LSM6DS33_XG_OUT_TEMP_L);
//  Temp=(s16)(SPI_NRF_ReadReg(LSM6DS33_XG_OUT_X_H_G)<<8);
//  LSM6DS33_Raw_Data.Gyro_x = Temp | SPI_NRF_ReadReg(LSM6DS33_XG_OUT_X_L_G );
//  Temp=(s16)(SPI_NRF_ReadReg(LSM6DS33_XG_OUT_Y_H_G)<<8);
//  LSM6DS33_Raw_Data.Gyro_y = Temp | SPI_NRF_ReadReg(LSM6DS33_XG_OUT_Y_L_G );
//  Temp=(s16)(SPI_NRF_ReadReg(LSM6DS33_XG_OUT_Z_H_G)<<8);
//  LSM6DS33_Raw_Data.Gyro_z = Temp | SPI_NRF_ReadReg(LSM6DS33_XG_OUT_Z_L_G );
//  Temp=(s16)(SPI_NRF_ReadReg(LSM6DS33_XG_OUT_X_H_XL)<<8);
//  LSM6DS33_Raw_Data.Acce_x= Temp | SPI_NRF_ReadReg(LSM6DS33_XG_OUT_X_L_XL );
//  Temp=(s16)(SPI_NRF_ReadReg(LSM6DS33_XG_OUT_Y_H_XL)<<8);
//  LSM6DS33_Raw_Data.Acce_y = Temp | SPI_NRF_ReadReg(LSM6DS33_XG_OUT_Y_L_XL );
//  Temp=(s16)(SPI_NRF_ReadReg(LSM6DS33_XG_OUT_Z_H_XL)<<8);
//  LSM6DS33_Raw_Data.Acce_z = Temp | SPI_NRF_ReadReg(LSM6DS33_XG_OUT_Z_L_XL );
//}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
//#define Linear_acceleration_sensitivity_2g  0.061f
//#define Linear_acceleration_sensitivity_4g  0.122f
//#define Linear_acceleration_sensitivity_8g  0.244f
//#define Linear_acceleration_sensitivity_16g 0.488f
//#define Angular_rate_sensitivity_125   0.004375f
//#define Angular_rate_sensitivity_245   0.00875f
//#define Angular_rate_sensitivity_500   0.0175f
//#define Angular_rate_sensitivity_1000  0.035f
//#define Angular_rate_sensitivity_2000  0.070f
//#define Degree_to_Angel  0.0174533f;
//void LSM6DS33_Trans_True_Data(void)
//{
//  LSM6DS33_Trans_Data.Temper_f = LSM6DS33_Raw_Data.Temper/16.0f+25;
//  LSM6DS33_Trans_Data.Gyro_x_f = (LSM6DS33_Raw_Data.Gyro_x - gyro_accel_magn_offset.Gyro_x_Offset) * Angular_rate_sensitivity_500 * Degree_to_Angel;
//  LSM6DS33_Trans_Data.Gyro_y_f = (LSM6DS33_Raw_Data.Gyro_y - gyro_accel_magn_offset.Gyro_y_Offset) * Angular_rate_sensitivity_500 * Degree_to_Angel;
//  LSM6DS33_Trans_Data.Gyro_z_f = (LSM6DS33_Raw_Data.Gyro_z - gyro_accel_magn_offset.Gyro_z_Offset) * Angular_rate_sensitivity_500 * Degree_to_Angel;
//  LSM6DS33_Trans_Data.Acce_x_f = (LSM6DS33_Raw_Data.Acce_x - gyro_accel_magn_offset.Accel_x_Offset) * Linear_acceleration_sensitivity_4g*0.0098;
//  LSM6DS33_Trans_Data.Acce_y_f = (LSM6DS33_Raw_Data.Acce_y - gyro_accel_magn_offset.Accel_y_Offset) * Linear_acceleration_sensitivity_4g*0.0098;
//  LSM6DS33_Trans_Data.Acce_z_f = (LSM6DS33_Raw_Data.Acce_z - gyro_accel_magn_offset.Accel_z_Offset) * Linear_acceleration_sensitivity_4g*0.0098;
//  /*if ((-0.02<LSM6DS33_Trans_Data.Gyro_z_f)&&(LSM6DS33_Trans_Data.Gyro_z_f<0.02))
//  {
//    LSM6DS33_Trans_Data.Gyro_z_f=0.0f;
//  }*/
//}

/******************* (C) COPYRIGHT 2016*****END OF FILE****/

