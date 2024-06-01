#ifndef _LCOM_UART_H
#define _LCOM_UART_H


#define	RBR		0x00	// For reading 	// SP_LCR_DLAB must be 0
#define	THR		0x00	// For writing	// SP_LCR_DLAB must be 0
#define	IER		0x01	// Interrupt Enable Register
#define	IIR		0x02	// Interrupt Identification Register
#define FCR		0x02	// Write to FIFO Control Register
#define	LCR		0x03	// Line Control Register
#define MCR		0x04
#define	LSR		0x05	// Line Status Register
#define	MSR		0x06
#define	SR		0x07
#define DLL		0x00	// SP_LCR_DLAB must be 1
#define DLM		0x01	// SP_LCR_DLAB must be 1

#endif
