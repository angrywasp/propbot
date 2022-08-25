EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:Angrywasp-IC
LIBS:Angrywasp-General
LIBS:Angrywasp-Conn
LIBS:PSU-cache
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
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
$Comp
L GND #PWR1
U 1 1 583911E7
P 9610 7130
F 0 "#PWR1" H 9610 6880 50  0001 C CNN
F 1 "GND" H 9610 6980 50  0000 C CNN
F 2 "" H 9610 7130 50  0000 C CNN
F 3 "" H 9610 7130 50  0000 C CNN
	1    9610 7130
	1    0    0    -1  
$EndComp
Text GLabel 8205 6985 3    40   Input ~ 0
USB_PWR
Wire Wire Line
	9610 6240 9610 7130
Connection ~ 9610 6640
Connection ~ 9610 6840
Wire Wire Line
	9445 6340 9690 6340
Wire Wire Line
	9410 6440 9690 6440
Wire Wire Line
	9385 6640 9690 6640
Connection ~ 9610 6240
Wire Wire Line
	9540 6140 9690 6140
Wire Wire Line
	9300 6940 9690 6940
Wire Wire Line
	9465 6240 9690 6240
$Comp
L PSU-1.1 U2
U 1 1 58718B00
P 9940 6490
F 0 "U2" H 10040 6940 60  0000 C CNN
F 1 "PSU-1.1" H 10040 5940 60  0000 C CNN
F 2 "Angrywasp:PSU-1.1" H 10340 6540 60  0001 C CNN
F 3 "" H 10340 6540 60  0001 C CNN
	1    9940 6490
	1    0    0    -1  
$EndComp
Wire Wire Line
	9690 6740 9350 6740
Wire Wire Line
	9350 6740 9350 6400
Wire Wire Line
	9350 6400 9300 6400
Wire Wire Line
	9385 6640 9385 6300
Wire Wire Line
	9385 6300 9300 6300
Wire Wire Line
	9410 6440 9410 6200
Wire Wire Line
	9410 6200 9300 6200
Wire Wire Line
	9445 6340 9445 6100
Wire Wire Line
	9445 6100 9300 6100
Wire Wire Line
	9465 6240 9465 6000
Wire Wire Line
	9465 6000 9300 6000
Wire Wire Line
	9540 5905 9540 6140
Wire Wire Line
	9540 5905 9300 5905
Wire Wire Line
	9300 5905 9300 5900
Wire Wire Line
	9690 6540 8775 6540
Wire Wire Line
	8775 6540 8775 6950
Wire Wire Line
	8775 6950 8205 6950
Wire Wire Line
	8205 6725 8205 6985
Connection ~ 8205 6950
Wire Wire Line
	8605 6840 9690 6840
Wire Wire Line
	8605 6840 8605 6725
$Comp
L USB_OTG P1
U 1 1 58BB64D9
P 8405 6425
F 0 "P1" H 8730 6300 50  0000 C CNN
F 1 "USB_OTG" H 8405 6625 50  0000 C CNN
F 2 "Angrywasp:USB_Mini-B" V 8355 6325 50  0001 C CNN
F 3 "" V 8355 6325 50  0000 C CNN
	1    8405 6425
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X07 P2
U 1 1 58BB73C8
P 9100 6200
F 0 "P2" H 9100 6600 50  0000 C CNN
F 1 "CONN_01X07" V 9200 6200 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x07" H 9100 6200 50  0001 C CNN
F 3 "" H 9100 6200 50  0000 C CNN
	1    9100 6200
	-1   0    0    1   
$EndComp
Wire Wire Line
	9300 6500 9300 6940
$EndSCHEMATC
