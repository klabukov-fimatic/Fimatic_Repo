
// Declare physical addresses UART 16C750 registers
//

#define UART1CM  		0x44e0006C			// UART1. Clock management Base:0x44e00000 ; CM_PER_UART1_CLKCTRL offset: 0x6C
#define UART1_base 		0x48022000			// UART1. Base:0x48022000

#define THR 			0x48022000 			// Transmit Holding Register
#define RHR 			0x48022000			// Receiver Holding Register
#define DLL 			0x48022000			// Divisor Latches Low Register

#define IER_UART 		0x48022004 			// Interrupt Enable Register (UART/IrDA/CIR)
#define DLH 			0x48022004 			// Divisor Latches High Register

#define EFR 			0x48022008			// Enhanced Feature Register
#define IIR_UART 		0x48022008 			// Interrupt Identification Register (UART/IrDA/CIR)
#define FCR 			0x48022008 			// FIFO Control Register

#define LCR 			0x4802200C			// Line Contrled_valueol Register

#define MCR 			0x48022010			// Modem Control Register
#define XON1_ADDR1 		0x48022010 			// XON1/ADDR1 Register

#define XON2_ADDR2 		0x48022014 			// XON2/ADDR2 Register
#define LSR_UART		0x48022014 			// Line Status Register (UART/IrDA/CIR)

#define TCR 			0x48022018 			// Transmission Control Register
#define MSR 			0x48022018 			// Modem Status Register
#define XOFF1 			0x48022018 			// XOFF1 Register

#define SPR 			0x4802201C 			// Scratchpad Register
#define TLR 			0x4802201C 			// Trigger Level Register
#define XOFF2 			0x4802201C 			// XOFF2 Register

#define MDR1 			0x48022020 			// Mode Definition Register 1
#define MDR2 			0x48022024			// Mode Definition Register 2

#define TXFLL 			0x48022028			// Transmit Frame Length Low Register
#define SFLSR 			0x48022028			// Status FIFO Line Status Register

#define RESUME 		0x4802202C			// RESUME Register
#define TXFLH 			0x4802202C			// Transmit Frame Length High Register

#define RXFLL 			0x48022030			// Received Frame Length Low Register
#define SFREGL 		0x48022030			// Status FIFO Register Low

#define SFREGH 		0x48022034			// Status FIFO Register High
#define RXFLH 			0x48022034			// Received Frame Length High

#define BLR 			0x48022038			// BOF Control Register
#define UASR 			0x48022038			// UART Autobauding Status Register

#define ACREG 			0x4802203C			// Auxiliary Control Register
#define SCR 			0x48022040			// Supplementary Control Register
#define SSR 			0x48022044			// Supplementary Status Register
#define EBLR 			0x48022048			// BOF Length Register
#define MVR 			0x48022050			// Module Version Register
#define SYSC 			0x48022054			// System Configuration Register
#define SYSS 			0x48022058			// System Status Register
#define WER 			0x4802205C			// Wake-Up Enable Register
#define CFPS 			0x48022060			// Carrier Frequency Prescaler Register
#define RXFIFO_LVL 		0x48022064 			// Received FIFO Level Register
#define TXFIFO_LVL 		0x48022068			// Transmit FIFO Level Register
#define IER2 			0x4802206C			// IER2 Register
#define ISR2 			0x48022070			// ISR2 Register
#define FREQ_SEL 		0x48022074 			// FREQ_SEL Register
#define MDR3 			0x48022080			// Mode Definition Register 3
#define TX_DMA_THRESHOLD 	0x48022084 			// TX DMA Threshold Register




