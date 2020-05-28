kCPUClock:  .EQU 18432000       ;CPU clock speed in Hz
kCPUClock:  .SET 18432000       ;CPU clock speed in Hz
kDelayOH:   .EQU 29             ;Overhead for each 1ms in Tcycles
kDelayLP:   .EQU 21             ;Inner loop time in Tcycles (20 in theory!)
kDelayTA:   .EQU kCPUClock / 1000 ;CPU clock cycles per millisecond
kDelayTB:   .EQU kDelayTA - kDelayOH  ;Cycles required for inner loop
kDelayCnt:  .EQU kDelayTB / kDelayLP  ;Loop counter for inner loop

			.CODE
			.ORG 0000h
kNewLine: 	.EQU 5
kZ180Base:  .EQU 0xC0           ;Z180 internal register base address
@kChar:     .EQU kZ180Base / 16 ;Z180 base address high nibble
@szHwZ180:  .DB  "1 = Z180 port A    (@",HEXCHAR @kChar,"6)",kNewLine
