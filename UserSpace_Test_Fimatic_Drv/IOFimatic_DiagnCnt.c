/*
 *  A simple application to call helloworld character driver's ioctl.
 *
 */
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "../Fimatic_Drv/Fimatic_Drv.h"



int main(int argc, char **argv) {
			
// Объявляем переменные
//
// IOFimatic_DiagnCnt [Cmd] [DgnSlot]
//

// [Cmd] Команды

// ChooseSlot	- установить номер текущего слота диагностики
// SlotCurrent	- прочитать номер текущего слота диагностики
// ResCnt 	- сбросить счетчики текущего слота диагностики
// ResAll	- сбросить счетчики всех слотов диагностики
// RdCnt 	- прочитать состояние счетчиков диагностики для текущего слота
// RdCntAll	- прочитать состояние счетчиков диагностики для всех слотов
//
// [DgnSlot] номер слота для команды 0...31
//


	int i, rc;
	int my_dev;								// дескриптор файла устройства
	int DgnSlot=1;
	
	struct IOCtl_Fimatic_Dgn_RdCnt Data_Dgn = {				// Структура IOCtl для чтения счетчиков текущего слота диагностики
		.cntErr	= -1,
		.cntErr_to 	= -1,
		.cntErr_crc	= -1,
		.cntErr_rc	= -1,	
	};
	
	my_dev = open("/dev/fimatic_dev", 0);					// открываем файл устройства
	if (my_dev < 0) {
		perror("Fail to open device file: /dev/fimatic_dev.");
		return 0;
	}
	
	ioctl(my_dev, cmdIOCtl_Dgn_SlotCurrent, &DgnSlot);			// инициализируем номер текущего слота диагностики

// Проверяем корректное количество аргументов и корректность ввода
//	
	if (argc>3) {
		printf ("incorrect number of arguments ...\n");
		return 0;
		}
	if ((argc==1) || ((argc==2) && !(strcmp(argv[1], "--help")))) {
		printf ("\n");	
		printf ("---- Diagnostic utility for the driver of the I/O modules of Fimatic controllers. v1.1 ----\n");
		printf ("\n");

		printf ("command input format: IOFimatic_DiagnCnt [Cmd] [DgnSlot]\n");
		printf ("\n");
		printf ("[Cmd]\n");
		printf ("ChooseSlot	- set the number of the current diagnostic slot\n");
		printf ("SlotCurrent	- read the number of the current diagnostic slo\n");
		printf ("ResCnt 		- reset counters of the current diagnostic slot\n");
		printf ("ResAll		- reset counters of all diagnostic slots\n");
		printf ("RdCnt 		- read the status of diagnostic counters for the current slot\n");
		printf ("RdCntAll	- read the status of diagnostic counters for all slots\n");
		printf ("\n");
		printf ("[DgnSlot] 	  the slot number for the team is 0...31\n");
				
		return 0;
		}


// Обрабатываем формат: IOFimatic_DiagnCnt [ChooseSlot] [DgnSlot] - Выбрать номер слота диагностики (сделать его текущим)
//	
	if ( (argc==3) && !(strcmp(argv[1], "ChooseSlot")) ){	
	
		DgnSlot = atoi(argv[2]);
		printf ("Change Dgn Slot to: %d\n", DgnSlot);
		ioctl(my_dev, cmdIOCtl_Dgn_ChooseSlot, &DgnSlot);
		
		return 0;
		}	

		
// Обрабатываем формат: IOFimatic_DiagnCnt [SlotCurrent] - Прочитать номер текущего слота диагностики
//		
	if ( (argc==2) && !(strcmp(argv[1], "SlotCurrent")) ){	
	
		ioctl(my_dev, cmdIOCtl_Dgn_SlotCurrent, &DgnSlot);
		printf ("Current Dgn Slot: %d\n", DgnSlot);
		
		return 0;
		}	

				
// Обрабатываем формат: IOFimatic_DiagnCnt [ResCnt] - Сбросить счетчики текущего слота диагностики, выбранного командой [ChooseSlot]
//	
	if ( (argc==2) && !(strcmp(argv[1], "ResCnt")) ){	
	
		printf ("Clear current Dgn Slot ...\n");		
		ioctl(my_dev, cmdIOCtl_Dgn_ResCnt);
		
		return 0;
		}			

		
// Обрабатываем формат: IOFimatic_DiagnCnt [ResAll] - Сбросить счетчики всех слотов диагностики
//	
	if ( (argc==2) && !(strcmp(argv[1], "ResAll")) ){	
	
		printf ("Clear  All Dgn Slot ...");		
		ioctl(my_dev, cmdIOCtl_Dgn_ResAll);
		
		return 0;
		}		
	

// Обрабатываем формат: IOFimatic_DiagnCnt [RdCntAll] - Прочитать счетчики текущего слота диагностики
//		
	if (  !(strcmp(argv[1], "RdCnt")) ){	
	
		if (argc==3) {
			DgnSlot = atoi(argv[2]);
			ioctl(my_dev, cmdIOCtl_Dgn_ChooseSlot, &DgnSlot);
		}
		
		ioctl(my_dev, cmdIOCtl_Dgn_RdCnt, &Data_Dgn);
		
		printf ("Read counters in the current diagnostic slot \n");
		printf ("Dgn Slot: %d\n", DgnSlot);
		printf ("cntErr: %d\n", Data_Dgn.cntErr);
		printf ("cntErr_to: %d\n", Data_Dgn.cntErr_to);
		printf ("cntErr_crc: %d\n", Data_Dgn.cntErr_crc);
		printf ("cntErr_rc: %d\n", Data_Dgn.cntErr_rc);		
				
		return 0;
		}

// Обрабатываем формат: IOFimatic_DiagnCnt [RdCntAll] - Прочитать состояние счетчиков диагностики для всех слотов
//		
	if (  !(strcmp(argv[1], "RdCntAll")) ){	
		
		printf ("Read counters in the All diagnostic slot \n");
		for (i=0; i<=31; i++) {
		
			DgnSlot = i;
			ioctl(my_dev, cmdIOCtl_Dgn_ChooseSlot, &DgnSlot);
			ioctl(my_dev, cmdIOCtl_Dgn_RdCnt, &Data_Dgn);
		
			printf ("Dgn Slot: %d - ", DgnSlot);
			printf ("cntErr: %d - ", Data_Dgn.cntErr);
			printf ("cntErr_to: %d - ", Data_Dgn.cntErr_to);
			printf ("cntErr_crc: %d - ", Data_Dgn.cntErr_crc);
			printf ("cntErr_rc: %d", Data_Dgn.cntErr_rc);		
			printf ("\n");
		}			
		return 0;
	}

	close(my_dev);
	return 0;
}




