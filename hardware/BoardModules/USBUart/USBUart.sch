EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4325 3975 4800 3975
Wire Wire Line
	4425 3875 4800 3875
Wire Wire Line
	5800 5400 5950 5400
Wire Wire Line
	5700 5400 5800 5400
Connection ~ 5800 5400
Wire Wire Line
	5800 5400 5800 5275
Connection ~ 5700 5400
Wire Wire Line
	5600 5400 5700 5400
Wire Wire Line
	5700 5275 5700 5400
Wire Wire Line
	5600 5275 5600 5400
Wire Wire Line
	5600 5400 5400 5400
Wire Wire Line
	5400 5400 5400 5275
Connection ~ 5600 5400
Wire Wire Line
	5400 5400 4700 5400
Wire Wire Line
	4700 5400 4700 4975
Wire Wire Line
	4700 4975 4800 4975
Connection ~ 5400 5400
Wire Wire Line
	7275 3975 7275 4075
$Comp
L Device:C C14
U 1 1 6285F321
P 7275 4225
F 0 "C14" H 7300 4325 50  0000 L CNN
F 1 "0.1uF" H 7300 4125 50  0000 L CNN
F 2 "General:0805_HS" H 7313 4075 50  0001 C CNN
F 3 "" H 7275 4225 50  0000 C CNN
	1    7275 4225
	1    0    0    -1  
$EndComp
Wire Wire Line
	7275 4375 7275 4425
$Comp
L Device:R R6
U 1 1 628852E5
P 7275 4600
F 0 "R6" V 7355 4600 50  0000 C CNN
F 1 "10k" V 7275 4600 50  0000 C CNN
F 2 "General:0805_HS" V 7205 4600 50  0001 C CNN
F 3 "" H 7275 4600 50  0000 C CNN
	1    7275 4600
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:2N3904 Q2
U 1 1 62897E52
P 7625 4425
F 0 "Q2" H 7816 4471 50  0000 L CNN
F 1 "NPN" H 7816 4380 50  0000 L CNN
F 2 "General:SOT-23_HS" H 7825 4350 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/2N3903-D.PDF" H 7625 4425 50  0001 L CNN
	1    7625 4425
	1    0    0    -1  
$EndComp
Wire Wire Line
	7425 4425 7275 4425
Connection ~ 7275 4425
Wire Wire Line
	7275 4425 7275 4450
Wire Wire Line
	7275 4750 7275 4775
Wire Wire Line
	7275 4775 7725 4775
Wire Wire Line
	7725 4775 7725 4625
Wire Wire Line
	7725 4775 7725 4850
Connection ~ 7725 4775
$Comp
L Angrywasp-General:GND #PWR030
U 1 1 628D3CF8
P 7725 4850
F 0 "#PWR030" H 7725 4600 50  0001 C CNN
F 1 "GND" H 7730 4677 50  0000 C CNN
F 2 "" H 7725 4850 50  0000 C CNN
F 3 "" H 7725 4850 50  0000 C CNN
	1    7725 4850
	1    0    0    -1  
$EndComp
Text GLabel 7725 4225 1    60   Input ~ 0
RESn
$Comp
L Device:C C10
U 1 1 628EAF1F
P 4625 5550
F 0 "C10" H 4650 5650 50  0000 L CNN
F 1 "0.1uF" H 4650 5450 50  0000 L CNN
F 2 "General:0805_HS" H 4663 5400 50  0001 C CNN
F 3 "" H 4625 5550 50  0000 C CNN
	1    4625 5550
	-1   0    0    1   
$EndComp
$Comp
L Angrywasp-General:GND #PWR021
U 1 1 628FDC4C
P 4625 5700
F 0 "#PWR021" H 4625 5450 50  0001 C CNN
F 1 "GND" V 4630 5572 50  0001 R CNN
F 2 "" H 4625 5700 50  0000 C CNN
F 3 "" H 4625 5700 50  0000 C CNN
	1    4625 5700
	1    0    0    -1  
$EndComp
$Comp
L Angrywasp-General:+5V #PWR023
U 1 1 6298B482
P 5925 2800
F 0 "#PWR023" H 5925 2650 50  0001 C CNN
F 1 "+5V" H 5940 2973 50  0000 C CNN
F 2 "" H 5925 2800 50  0000 C CNN
F 3 "" H 5925 2800 50  0000 C CNN
	1    5925 2800
	0    1    1    0   
$EndComp
$Comp
L Angrywasp-General:LED D2
U 1 1 62B5D809
P 6675 4525
F 0 "D2" H 6675 4740 50  0001 C CNN
F 1 "BLUE" H 6675 4648 50  0000 C CNN
F 2 "General:LED_0805_HS" H 6675 4525 50  0001 C CNN
F 3 "" H 6675 4525 50  0000 C CNN
	1    6675 4525
	1    0    0    -1  
$EndComp
$Comp
L Angrywasp-General:LED D3
U 1 1 62B5FBAA
P 6675 4725
F 0 "D3" H 6675 4940 50  0001 C CNN
F 1 "RED" H 6675 4848 50  0000 C CNN
F 2 "General:LED_0805_HS" H 6675 4725 50  0001 C CNN
F 3 "" H 6675 4725 50  0000 C CNN
	1    6675 4725
	1    0    0    -1  
$EndComp
Wire Wire Line
	6475 4725 6475 4675
Wire Wire Line
	6475 4575 6475 4525
Wire Wire Line
	6875 4725 6900 4725
Wire Wire Line
	6900 4725 6900 4525
Wire Wire Line
	6900 4525 6875 4525
$Comp
L Device:R R4
U 1 1 62BA8F14
P 6900 4350
F 0 "R4" V 6980 4350 50  0000 C CNN
F 1 "100" V 6900 4350 50  0000 C CNN
F 2 "General:0805_HS" V 6830 4350 50  0001 C CNN
F 3 "" H 6900 4350 50  0000 C CNN
	1    6900 4350
	1    0    0    -1  
$EndComp
$Comp
L Angrywasp-General:+5V #PWR027
U 1 1 62BA8F1A
P 6900 4200
F 0 "#PWR027" H 6900 4050 50  0001 C CNN
F 1 "+5V" H 6915 4373 50  0000 C CNN
F 2 "" H 6900 4200 50  0000 C CNN
F 3 "" H 6900 4200 50  0000 C CNN
	1    6900 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 4500 6900 4525
Connection ~ 6900 4525
NoConn ~ 4800 4475
NoConn ~ 4800 4675
NoConn ~ 6400 4775
NoConn ~ 6400 4975
NoConn ~ 6400 4275
NoConn ~ 6400 4175
NoConn ~ 6400 4075
NoConn ~ 6400 4875
Wire Wire Line
	5700 2800 5925 2800
Wire Wire Line
	4000 1850 4000 2675
Wire Wire Line
	4000 2675 4225 2675
Text GLabel 6400 3575 2    60   Input ~ 0
RX
Connection ~ 5700 2800
Wire Wire Line
	5500 3275 5500 3250
$Comp
L Angrywasp-General:GND #PWR0101
U 1 1 637FC241
P 5950 5400
F 0 "#PWR0101" H 5950 5150 50  0001 C CNN
F 1 "GND" H 5955 5227 50  0000 C CNN
F 2 "" H 5950 5400 50  0000 C CNN
F 3 "" H 5950 5400 50  0000 C CNN
	1    5950 5400
	1    0    0    -1  
$EndComp
Text GLabel 6400 3675 2    60   Input ~ 0
TX
Wire Wire Line
	4625 3575 4800 3575
NoConn ~ 6400 3775
NoConn ~ 6400 3875
NoConn ~ -4025 1550
Wire Wire Line
	6475 4675 6400 4675
Wire Wire Line
	6400 4575 6475 4575
Wire Wire Line
	6400 3975 7275 3975
$Comp
L Interface_USB:FT232RL USB1
U 1 1 61A1346F
P 5600 4275
F 0 "USB1" H 5600 5456 50  0000 C CNN
F 1 "FT232L" H 5600 5365 50  0000 C CNN
F 2 "Package_SO:SSOP-28_5.3x10.2mm_P0.65mm" H 6700 3375 50  0001 C CNN
F 3 "https://www.ftdichip.com/Support/Documents/DataSheets/ICs/DS_FT232R.pdf" H 5600 4275 50  0001 C CNN
	1    5600 4275
	1    0    0    -1  
$EndComp
Wire Wire Line
	4625 3575 4625 5400
NoConn ~ 4800 4275
NoConn ~ 4835 4540
Wire Wire Line
	4625 3250 5500 3250
Wire Wire Line
	4625 3250 4625 3575
Connection ~ 4625 3575
Wire Wire Line
	5700 2800 5700 3275
Wire Wire Line
	4000 1850 5700 1850
Wire Wire Line
	5700 1850 5700 2800
Wire Wire Line
	4425 2600 4425 3875
Wire Wire Line
	4325 2600 4325 3975
$Comp
L power:GND #PWR018
U 1 1 619A6625
P 4525 2600
F 0 "#PWR018" H 4525 2350 50  0001 C CNN
F 1 "GND" H 4525 2450 50  0000 C CNN
F 2 "" H 4525 2600 50  0000 C CNN
F 3 "" H 4525 2600 50  0000 C CNN
	1    4525 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4225 2675 4225 2600
$Comp
L Angrywasp-Conn:CONN_01X07 P1
U 1 1 630BD4F6
P 4525 2400
F 0 "P1" V 4533 2012 50  0000 R CNN
F 1 "CONN_01X07" V 4442 2012 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x07_P2.54mm_Vertical" V 4351 2012 50  0000 R CNN
F 3 "" H 4525 2400 50  0000 C CNN
	1    4525 2400
	0    -1   -1   0   
$EndComp
Text GLabel 4725 2600 3    60   Input ~ 0
RX
Text GLabel 4625 2600 3    60   Input ~ 0
TX
Text GLabel 4825 2600 3    60   Input ~ 0
RESn
$EndSCHEMATC
