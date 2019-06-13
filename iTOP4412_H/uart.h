#ifndef __4412_UART_H_
#define __4412_UART_H_

/*
*  UART 0~4
*/
#define baseAddress(n) ((0x13800000) | (n) << (16)) // uartn n 0~4

#define offset_ULCONn 0x0000
#define offset_UCONn 0x0004
#define offset_UFCONn 0x0008
#define offset_UMCONn 0x000C
#define offset_UTRSTATn 0x0010
#define offset_UERSTATn 0x0014
#define offset_UFSTATn 0x0018
#define offset_UMSTATn 0x001C
#define offset_UTXHn 0x0020
#define offset_URXHn 0x0024
#define offset_UBRDIVn 0x0028
#define offset_UFRACVALn 0x002C
#define offset_UINTPn 0x0030
#define offset_UINTSPn 0x0034
#define offset_UINTMn 0x0038

#define ULCON(n) (offset_ULCONn + baseAddress(n))
#define UCON(n) (offset_UCONn + baseAddress(n))
#define UFCON(n) (offset_UFCONn + baseAddress(n))
#define UMCON(n) (offset_UMCONn + baseAddress(n))
#define UTRSTAT(n) (offset_UTRSTATn + baseAddress(n))
#define UERSTAT(n) (offset_UERSTATn + baseAddress(n))
#define UFSTAT(n) (offset_UFSTATn + baseAddress(n))
#define UMSTAT(n) (offset_UMSTATn + baseAddress(n))
#define UTXH(n) (offset_UTXHn + baseAddress(n))
#define URXH(n) (offset_URXHn + baseAddress(n))
#define UBRDIV(n) (offset_UBRDIVn + baseAddress(n))
#define UFRACVAL(n) (offset_UFRACVALn + baseAddress(n))
#define UINTP(n) (offset_UINTPn + baseAddress(n))
#define UINTSP(n) (offset_UINTSPn + baseAddress(n))
#define UINTM(n) (offset_UINTMn + baseAddress(n))

#endif 