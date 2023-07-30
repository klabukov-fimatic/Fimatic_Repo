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


// Утилита для проверки модулей IO серии Fimatic
// Варианты запуска:

// 1. 	IOFimatic_Checking --help
// 	краткая справка

// 2. 	IOFimatic_Checking [ChType] [Cmd] [NetAddr] [ChNumber] [D0] [D1] ... [D7]
// 	чтение состояния модулей ввода-вывода: аналоговый входной (ток, напряжение, RTD, TX), дискретный входной/ выходной

//	[ChType]	тип модуля (ang - аналоговый; dgt - дискретный)
//	[Cmd]		команда. Для аналоговых: чтение - rd. Для дискретных: чтение(запись) - rd(wr)

//	[NetAddr] 	сетевой адрес на Backplane. Число 1...99
// 	[ChNumber]	количество каналов. Для аналоговых 4(2). Для дискретных 8(4) 

// 	[Di]		Только для управления модулями DO(опции: dgt + wr ): Включение - 1 (Отключение - 0) соответсвующего канала DO.
//
// 	Примеры:
//	IOFimatic_Checking ang rd 4 4 			; прочитать значения и состояние каналов модуля "МСАТС4" (сетевой адрес равен 4)
// 	IOFimatic_Checking ang rd 7 4 			; прочитать значения и состояние каналов модуля "МСАН4" (сетевой адрес равен 7)
//	IOFimatic_Checking dgt rd 2 8				; прочитать состояние каналов сигнализации модуля "МСД8" (сетевой адрес равен 2)
//	IOFimatic_Checking dgt wr 1 8 0 0 1 1 0 0 1 1		; установить реле каналов управления модуля "МВДР8" в состояние: [off][off][on][on][off][off][on][on] (сетевой адрес равен 1)
//	IOFimatic_Checking dgt rd 1 8				; прочитать состояние реле каналов управления модуля "МВДР8" (сетевой адрес равен 1)

int main(int argc, char **argv) {
			
// Объявляем переменные
//
	int i;
	int my_dev;						// дескриптор файла устройства

	struct IOCtl_Fimatic_rdIO Data_IO = {			// Структура IOCtl для чтения данных из модулей IO
		.NetAddr	=  4,
		.ChType 	= -1,
		.ChNumber 	=  4,
		.retCod	= -1,
		.Data		= {-1,-1,-1,-1,-1,-1,-1,-1}	
	};
	
	struct IOCtl_Fimatic_wrIO Data_DO8 = {
		.NetAddr	= 1,
		.ChType 	= 0x91,
		.ChNumber 	= 8,
		.Data		= {0,1,0,1,0,1,0,1}	
	};

// Проверяем корректное количество аргументов
//
	if ((argc!=1)&&(argc!=2)&&(argc!=5)&&(argc!=13)&&(argc!=9)) {
		printf ("incorrect number of arguments ...\n");
		return 0;
		}
	if ((argc==1) || ((argc==2) && !(strcmp(argv[1], "--help")))) {	
		printf ("help ...\n");
		return 0;
		}
	if (   ((!(strcmp(argv[1], "ang"))) || (!(strcmp(argv[1], "dgt")))) && (!(strcmp(argv[2], "rd")))   &&  (argc!=5)    ) {
		printf ("incorrect number of arguments for the format: IOFimatic_Checking [ang] [rd]|[wr] [addr] [chnum]\n");
		return 0;	
	}		
	if (   (!(strcmp(argv[1], "dgt"))) && (!(strcmp(argv[2], "wr")))   &&   (atoi(argv[4])==8) &&  (argc!=13)    ) {	
		printf ("incorrect number of arguments for the format: IOFimatic_Checking [dgt] [wr] [addr] [chnum] [d0] ... [d7]\n");
		return 0;	
	}
	if (   (!(strcmp(argv[1], "dgt"))) && (!(strcmp(argv[2], "wr")))   &&   (atoi(argv[4])==4) &&  (argc!=9)    ) {	
		printf ("incorrect number of arguments for the format: IOFimatic_Checking [dgt] [wr] [addr] [chnum] [d0] ... [d3]\n");
		return 0;	
	}	

// Проверяем корректность самих аргументов
// ...	

	printf ("program run ...\n");
	
	my_dev = open("/dev/fimatic_dev", 0);									// открываем файл устройства
	if (my_dev < 0) {
		perror("Fail to open device file: /dev/fimatic_dev.");
		return 0;
	}
	
	
// Обрабатываем формат: IOFimatic_Checking [ang] [rd] [addr] [chnum]
//
	if ( !(strcmp(argv[1], "ang"))  &&  !(strcmp(argv[2], "rd"))  &&  (argc==5) ) {
		printf ("Format: IOFimatic_Checking [ang] [rd] [addr] [chnum] ...\n");
		
		Data_IO.ChType = ChTYPE_ANG;		
		Data_IO.NetAddr = atoi(argv[3]);
		Data_IO.ChNumber = atoi(argv[4]);		
		
		ioctl(my_dev, cmdIOCtl_Fimatic_rdIO, &Data_IO);
				
		printf("NetAddr = %d\n", Data_IO.NetAddr );
		printf("ChNumber = %d\n", Data_IO.ChNumber );

		for(i=0; i<=3; i++) printf("Data[%d] = %#04x %#04x %#04x %#04x\n", i, Data_IO.Data[i*4], Data_IO.Data[i*4+1], Data_IO.Data[i*4+2], Data_IO.Data[i*4+3]);
		for(i=0; i<=3; i++) printf("ChStatus[%d] = %#04x\n", i, Data_IO.ChStatus[i]);
		printf("retCod = %d\n", Data_IO.retCod );
		printf("resTime = %#04x\n", Data_IO.resTime );				
	}

// Обрабатываем формат: IOFimatic_Checking [dgt] [rd] [addr] [chnum]
//
	if ( !(strcmp(argv[1], "dgt"))  &&  !(strcmp(argv[2], "rd"))  &&  (argc==5) ) {
		printf ("Format: IOFimatic_Checking [dgt] [rd] [addr] [chnum] ...\n");
		
		Data_IO.ChType = ChTYPE_DGT;		
		Data_IO.NetAddr = atoi(argv[3]);
		Data_IO.ChNumber = atoi(argv[4]);
			
		ioctl(my_dev, cmdIOCtl_Fimatic_rdIO, &Data_IO);
				
		printf("NetAddr = %d\n", Data_IO.NetAddr );
		printf("ChNumber = %d\n", Data_IO.ChNumber );
		
		for(i=0; i<=7; i++) printf("Data[%d] = %d\n", i, Data_IO.Data[i]);
		for(i=0; i<=7; i++) printf("ChStatus[%d] = %#04x\n", i, Data_IO.ChStatus[i]);
		printf("retCod = %d\n", Data_IO.retCod );
		printf("resTime = %#04x\n", Data_IO.resTime );				
	}

// Обрабатываем формат: IOFimatic_Checking [dgt] [wr] [addr] [chnum] [d0] ... [d7]
//
	if ( !(strcmp(argv[1], "dgt"))  &&  !(strcmp(argv[2], "wr"))  &&  ((argc==13)||(argc==9)) ) {
		printf ("Format: IOFimatic_Checking [dgt] [rd] [addr] [chnum] [d0] ... [d7]\n");
		
		Data_DO8.ChType = ChTYPE_DGT;		
		Data_DO8.NetAddr = atoi(argv[3]);
		Data_DO8.ChNumber = atoi(argv[4]);	
		
		for(i=0; i<=7; i++) Data_DO8.Data[i] = atoi(argv[i+5]);

		ioctl(my_dev, cmdIOCtl_Fimatic_wrIO, &Data_DO8);
		
		
		printf("NetAddr = %d\n", Data_DO8.NetAddr );
		printf("ChNumber = %d\n", Data_DO8.ChNumber );	
		
		printf("retCod = %d\n", Data_DO8.retCod );
		printf("resTime = %#04x\n", Data_DO8.resTime );	
				
	}
	
	close(my_dev);
	return 0;
}




