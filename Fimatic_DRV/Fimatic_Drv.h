
/* Description of common structures */

// Types of I/O modules
//
//[mnemo] 	[Mod Cod]	[Description]		[Channel type]								[Channel type Cod ]						[Size/Chann]
//
// msat4 	0x12		AI Current 		(4-20ma; 0-20ma) 							0x45; 0x46							float(4byte)
// msan4	0x19		AI Voltage		(0-5V;   0-10V)  							0x41; 0x42							float(4byte)
// msatp4	0x13 		AI thermocouple	(K;L;E;T;J;mV)	 							0x51;0x52;0x53;0x54;0x55;0x5F;				float(4byte)
// msats4	0x14		AI rtd			(Pt100;Pt500;pt1000;50P;100P;50M8;100V8;Ni100;50M6;100M6;om)		0x61;0x62;0x63;0x64;0x65;0x66;0x67;0x68;0x69;0x6A;0x6F;	float(4byte)

// msd8(4)	0x18(0x17)	DI digital input	(DI;Hz;Count;Faze;acceleration)					0x11;0x21;0x22;0x23;0x24;					char(1)/ float(4)

// mwdo8	0x15		DO 24vdc		(DO; Hz)								0x91;0xA1;							char(1)/ float(4)
// mwdr8	0x16		DO Relays		(DO_Relays)									0x91;								char(1)

// mwatn4(2)	0x1A(0x11)	AO Current/ Voltage	(0-5V;0-10V;+-5V;+-10V;4-20ma;0-20ma;0-22ma;+5.5V;+11V;+-5.5V;+-11V)	0xC1;0xC2;0xC3;0xC4;0xC5;0xC6;0xC7;0xC8;0xC9;0xCA;0xCB;	float(4byte)


#define IOC_MAGIC 'k' 											// IOCtl magic number

#define cmdIOCtl_Fimatic_rdIO		_IOWR(IOC_MAGIC, 10, struct IOCtl_Fimatic_rdIO)		//
#define cmdIOCtl_Fimatic_wrIO		_IOWR(IOC_MAGIC, 12, struct IOCtl_Fimatic_wrIO)		//


struct IOCtl_Fimatic_rdIO {		// Структура данных для чтения информации с модулей IO (AI,DI,DO,AO) (все типы: Входные и выходные)
	int 	NetAddr;		// -> Адрес целевого модуля в крейте модулей IO
	int	ChType;		// -> тип запрашиваемых каналов IO в целевом модуле ввода-вывода (определяется спецификацией прикладного протокола Fimatic)
	int 	ChNumber;		// -> количество каналов в целевом модуле IO
	int 	retCod;		// <- код возврата. Устанавливается драйвером по итогам вызова IOCtl функции
	char	Data[16];		// <- массив из 16 байт. Возвращается от драйвера в UserSpace. Кодировка определяется типом модуля
					//    модули AI - [4байта(float)/канал]x4 = 16 байт; модули DI - [1байт(On-Off)/канал]x8 = 8 байт. Нумерация каналов, по индексу Data, от младших к старшим
	char    ChStatus[8];		// <- Статус канала в модуле: 0x00-"Отключен", "Ошибка канала" или тип канала IO
	int	resTime;		// <- время в мс (количество системных тиков) ожидания ответа модуля IO					
};

struct IOCtl_Fimatic_wrIO {		// Структура данных для управления модулем DO (типы модулей: МВДР8, МВДО8)
					//
	int 	NetAddr;		// -> Адрес целевого Выходного модуля в крейте модулей IO
	int	ChType;		// -> тип каналов в целевом модуле Вывода (определяется спецификацией прикладного протокола Fimatic)
	int 	ChNumber;		// -> количество каналов в целевом Выходном модуле 
	int 	retCod;		// <- код возврата. Устанавливается драйвером по итогам вызова IOCtl функции
	char	Data[8];		// -> массив из 8 байт. Задается в UserSpace и определяет драйверу значения для каждого из DO каналов (1-"Включить", 0-"Отключить")
					//    Нумерация каналов, по индексу массива Data,от младших к старшим
	int	resTime;		// <- время в мс (количество системных тиков) ожидания ответа модуля IO
};

#define ChTYPE_ANG 0x1			// Тип аналоговых каналов IO
#define ChTYPE_DGT 0x2			// Тип дискретных каналов IO


// Структуры для работы со счетчиками диагностики
//

// Выбрать номер слота диагностики (сделать его текущим)
#define cmdIOCtl_Dgn_ChooseSlot	_IOW(IOC_MAGIC, 20, int)

// Прочитать номер текущего слота диагностики
#define cmdIOCtl_Dgn_SlotCurrent	_IOR(IOC_MAGIC, 21, int)

// Сбросить счетчики текущего слота диагностики
#define cmdIOCtl_Dgn_ResCnt		_IOW(IOC_MAGIC, 22, int)

// Сбросить счетчики всех слотов диагностики
#define cmdIOCtl_Dgn_ResAll		_IOW(IOC_MAGIC, 23, int)

// Прочитать счетчики текущего слота диагностики
#define cmdIOCtl_Dgn_RdCnt		_IOR(IOC_MAGIC, 24, struct IOCtl_Fimatic_Dgn_RdCnt)

struct IOCtl_Fimatic_Dgn_RdCnt {	// Структура счетчиков диагностики. Для текущего слота диагностики выбранного командой "cmdIOCtl_Dgn_ChooseSlot"
					//
	int	cntErr;		// <- Err: общий счетчик ошибок
	int 	cntErr_to;		// <- Err: ошибки time-out
	int 	cntErr_crc;		// <- Err: ошибки контрольной суммы
	int	cntErr_rc;		// <- Err: ошибки в ответе модуля IO
};













