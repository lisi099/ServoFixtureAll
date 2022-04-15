
/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/

#include "servo_serial.h"
#include "string.h"
#include "factory_data.h"

/*const struct Servo_Data_Stru_ factory_para[] =
{
//{2100,1500,900,3100,2048,996,2,0,0,1,1,1,4021,1,14,0,0,1500,10,1,1,10,1080,100,20,1020,900,1350,1500,1500,0,50,6,3,8,8,20,1,1,1,1,1,1}, //04021=V1.1 PGC-D12-00.bat"
{2100,1500,900,3100,2048,996,2,0,0,1,1,1,4021,1,19,0,0,1500,10,1,1,10,1080,100,20,100,900,1350,1500,1500,0,50,6,3,8,8,20,1,1,1,1,1,1}, //04021=V1.1 PGC-D12-00new.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10100,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10100=V1.2 D1206G2-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10200,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1050,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10200=V1.2 WH20KG-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10300,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1050,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10300=V1.2 WH30KG-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10400,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10400=V1.2 WH40KG-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10500,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10500=V1.2 PGC-Drift-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10600,1,14,0,0,1500,10,1,1,10,960,50,20,5,800,1200,1500,1500,0,20,8,1,35,4,20,1,50,1,1,1,1}, //10600=V1.2 PGC-D15-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10700,1,14,0,0,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,8,1,35,4,20,1,50,1,1,1,1}, //10700=V1.2 PGC-R12S-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10800,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10800=V1.2 PGC-A20-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10900,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10900=V1.2 PGC-A30-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,11000,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //11000=V1.2 PGC-A40-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,11100,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //11100=V1.2 PGC-A50-00.bat"
};*/

const struct Servo_Data_Stru_ factory_para[] =
{
  {2100,1500,900,3100,2048,996,2,0,0,1,1,1,4221,1,21,0,0,1500,10,1,1,10,1360,100,20,1020,900,1350,1500,1500,0,50,6,3,8,8,20,1,1,1,1,1,1}, //04221=D1305L1.bat"
  {2100,1500,900,3100,2048,996,2,0,0,1,1,1,4321,1,21,0,0,1500,10,1,1,10,1360,100,20,1020,900,1350,1500,1500,0,50,6,3,8,8,20,1,1,1,1,1,1}, //04321=D1305L2.bat"
  {2100,1500,900,2900,2048,1200,2,10,0,0,0,7,4421,0,21,0,0,1500,10,1,1,8,1510,100,100,10,500,1500,800,3000,0,10,7,1,28,2,8,1,0,2,0,0,0}, //04421=D1305X1.bat"
};

const struct Servo_Tai_Data_ factory_tai_para[]=
{
    {100, 1100, 1, 270, 155, 120, 2048, 0, 0, 45, 0},
    {95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
};

//        version_map_.insert(42, "D1305L1");
//        version_map_.insert(43, "D1305L2");
//        version_map_.insert(44, "D1305X1");
//        version_map_.insert(20, "S1550L1");
//        version_map_.insert(21, "S1550X1");

char *servo_version[] = {"D1305L1", "D1305L2", "D1305X1", "S1550L1", "S1550X1",
                        };

int servo_version_num[] = {42, 43, 44, 20, 21};

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
