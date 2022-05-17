
/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/

#include "servo_serial.h"
#include "string.h"
#include "factory_data.h"

//const struct Servo_Data_Stru_ factory_para[] =
//{
//{2100,1500,900,3100,2048,996,2,0,0,1,1,1,4021,1,15,0,0,1500,10,1,1,10,1360,100,20,1020,900,1350,1500,1500,0,50,6,3,8,8,20,1,1,1,1,1,1}, //04021=V1.1 PGC-D12-00.bat"
//{2100,1500,900,3100,2048,996,2,0,0,1,1,1,4100,1,15,0,1,1500,10,1,1,10,1080,100,20,100,900,1350,1500,1500,0,50,8,3,9,8,20,1,1,1,1,1,1}, //04100=V1.1 S1206G2-00.bat"
//{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10100,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10100=V1.2 D1206G2-00.bat"
//{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10200,1,15,0,1,1500,10,1,1,10,800,50,20,5,800,1000,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10200=V1.2 WH20KG-00.bat"
//{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10300,1,15,0,1,1500,10,1,1,10,800,50,20,5,800,1000,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10300=V1.2 WH30KG-00.bat"
//{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10400,1,15,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10400=V1.2 WH40KG-00.bat"
//{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10500,1,15,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10500=V1.2 PGC-Drift-00.bat"
//{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10600,1,15,0,0,1500,10,1,1,10,1210,50,20,5,800,1200,1500,1500,0,20,8,1,35,4,20,1,50,1,1,1,1}, //10600=V1.2 PGC-D15-00.bat"
//{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10700,1,15,0,0,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,8,1,35,4,20,1,50,1,1,1,1}, //10700=V1.2 PGC-R12S-00.bat"
//{2100,1500,900,3100,2048,996,3,0,0,1,1,1,11000,1,15,0,1,1500,10,1,1,10,1010,50,500,5,800,1000,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //11000=V1.2 PGC-A40-00.bat"
//{2100,1500,900,3100,2048,996,3,0,0,1,1,1,11100,1,15,0,1,1500,10,1,1,10,1360,50,500,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //11100=V1.2 PGC-A50-00.bat"
//};

const struct Servo_Data_Stru_ factory_para[] =
{
{2100,1500,900,3100,2048,996,2,0,0,1,1,1,15021,1,15,0,0,1500,10,1,1,10,1080,100,20,1020,900,1350,1500,1500,0,50,8,3,9,8,20,1,1,1,1,1,1}, //15021=v1.2 CLS-CH712S.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,16100,1,15,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //16100=v1.2 CLS-D1-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,16300,1,15,0,1,1500,10,1,1,10,800,50,20,5,800,1000,1500,1500,0,50,6,1,20,4,20,1,50,1,1,1,1}, //16300=v1.2 CLS-W35-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,16400,1,15,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,50,6,1,20,4,20,1,50,1,1,1,1}, //16400=v1.2 CLS-W45-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,16600,1,15,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //16600=v1.2 CLS-D1S-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,16800,1,15,0,1,1500,10,1,1,10,1010,50,500,5,800,1000,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //16800=v1.2 CLS-T45-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,16900,1,15,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //16900=v1.2 CLS-T55-00.bat"
{2100,1500,900,2900,2048,1200,2,10,0,0,0,7,10800,0,18,0,0,1500,10,1,1,8,1510,100,100,10,500,1500,800,3000,0,10,7,1,28,2,8,1,0,2,0,0,0}, //10800-PGC-M8.-00.bat"
};

//const struct Servo_Tai_Data_ factory_tai_para[]=
//{
//	{100, 1100, 1, 270, 155, 120, 2048, 0, 0, 45, 0},
//	{95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
//	{95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
//	{95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
//	{100, 1100, 1, 270, 155, 120, 2048, 0, 0, 45, 0},
//};
const struct Servo_Tai_Data_ factory_tai_para[]=
{
	{100, 1100, 1, 270, 155, 120, 2048, 0, 0, 45, 0},
	{95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
	{95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
	{95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
	{95, 1100, 1, 130, 155, 35, 2048, 0, 0, 45, 0},
};



//char *servo_version[] = {"PGC-D12", "S1206G2", "D1206G2", "WH-20KG", "WH-30KG", "WH-40KG",
//                         "PGC-DRF", "PGC-D15", "PGC-R12",
//                         "PGC-A40", "PGC-A50", "PGB-S15", "PGB-S25", "PGB-S35", "PGB-S50",
//						 "PGB-B7S"
//                        };
char *servo_version[] = {"CLS-712", "CLS-D1X", "CLS-W35", "CLS-W45", "CLS-D1S", "CLS-T45", "CLS-T55", "PGC-M8.", 
													"BLS-815", "BLS-922", "BLS-927", "BLS-T65", "BLS-T75"
};
//int servo_version_num[] = {40, 41, 1, 2, 3, 4, 5, 6, 7, 10, 11, 80, 81, 82, 83, 84};
						
int servo_version_num[] = {50, 61, 63, 64, 66, 68, 69, 8, 71, 72, 73, 76, 77};


int get_total_num(void)
{
    return sizeof(servo_version_num) / sizeof(int);
}

char *get_ver_char(int num)
{
    return (servo_version[num]);
}


int get_ver_num(int num)
{
    return servo_version_num[num];
}

void get_factory_data(struct Servo_Data_Stru_ *data, int num)
{
    memcpy(data, &factory_para[num], sizeof(struct Servo_Data_Stru_));
}

#define MAX_POWER_INDEX 	13
#define BOOST_INDEX 		79
#define TENSION_INDEX		81
#define DEAD_BAND_INDEX		55
#define FORCE_INDEX			61
#define BRAKE_INDEX			65
#define CENTER_INDEX		43
#define SOFT_START_INDEX	91

void get_tai_factory_data(uint8_t *data, int num)
{
	int index = num - sizeof(factory_para)/ sizeof(struct Servo_Data_Stru_);
	data[MAX_POWER_INDEX] = (uint8_t)(factory_tai_para[index].max_power);
	data[MAX_POWER_INDEX+1] = (uint8_t)(factory_tai_para[index].max_power >> 8);
	
	data[BOOST_INDEX] = (uint8_t)(factory_tai_para[index].boost);
	data[BOOST_INDEX+1] = (uint8_t)(factory_tai_para[index].boost >> 8);

	data[DEAD_BAND_INDEX] = (uint8_t)(factory_tai_para[index].deadband);
	data[DEAD_BAND_INDEX+1] = (uint8_t)(factory_tai_para[index].deadband >> 8);

	data[TENSION_INDEX] = (uint8_t)(factory_tai_para[index].tension);
	data[TENSION_INDEX+1] = (uint8_t)(factory_tai_para[index].tension >> 8);

	data[FORCE_INDEX] = (uint8_t)(factory_tai_para[index].farce);
	data[FORCE_INDEX+1] = (uint8_t)(factory_tai_para[index].farce >> 8);

	data[BRAKE_INDEX] = (uint8_t)(factory_tai_para[index].brake);
	data[BRAKE_INDEX+1] = (uint8_t)(factory_tai_para[index].brake >> 8);

	data[CENTER_INDEX] = (uint8_t)(factory_tai_para[index].senter);
	data[CENTER_INDEX+1] = (uint8_t)(factory_tai_para[index].senter >> 8);

	data[SOFT_START_INDEX] = (uint8_t)(factory_tai_para[index].init_speed);
	data[SOFT_START_INDEX+1] = (uint8_t)(factory_tai_para[index].init_speed >> 8);
	
	data[SOFT_START_INDEX+2] = (uint8_t)(factory_tai_para[index].init_timeout);
	data[SOFT_START_INDEX+3] = (uint8_t)(factory_tai_para[index].init_timeout >> 8);
    
}
