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
NoConn ~ -4025 1550
$Comp
L Regulator_Linear:AMS1117-3.3 U1
U 1 1 630C4B40
P 4800 3000
F 0 "U1" H 4800 3242 50  0000 C CNN
F 1 "AMS1117-3.3" H 4800 3151 50  0000 C CNN
F 2 "General:SOT-223" H 4800 3200 50  0001 C CNN
F 3 "" H 4900 2750 50  0001 C CNN
	1    4800 3000
	1    0    0    -1  
$EndComp
$Comp
L Angrywasp-General:CP C2
U 1 1 630C5F90
P 5300 3200
F 0 "C2" H 5418 3291 50  0000 L CNN
F 1 "CP" H 5418 3200 50  0000 L CNN
F 2 "Capacitors:5x5.3" H 5418 3109 50  0000 L CNN
F 3 "" H 5300 3200 50  0000 C CNN
	1    5300 3200
	1    0    0    -1  
$EndComp
$Comp
L Angrywasp-General:CP C1
U 1 1 630C7489
P 4300 3200
F 0 "C1" H 4418 3291 50  0000 L CNN
F 1 "CP" H 4418 3200 50  0000 L CNN
F 2 "Capacitors:5x5.3" H 4418 3109 50  0000 L CNN
F 3 "" H 4300 3200 50  0000 C CNN
	1    4300 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3000 5300 3000
$Comp
L Angrywasp-General:+3V3 #PWR0101
U 1 1 630CA4BB
P 5600 3000
F 0 "#PWR0101" H 5600 2850 50  0001 C CNN
F 1 "+3V3" H 5615 3173 50  0000 C CNN
F 2 "" H 5600 3000 50  0000 C CNN
F 3 "" H 5600 3000 50  0000 C CNN
	1    5600 3000
	1    0    0    -1  
$EndComp
$Comp
L Angrywasp-General:+5V #PWR0102
U 1 1 630CBA10
P 3900 3000
F 0 "#PWR0102" H 3900 2850 50  0001 C CNN
F 1 "+5V" H 3915 3173 50  0000 C CNN
F 2 "" H 3900 3000 50  0000 C CNN
F 3 "" H 3900 3000 50  0000 C CNN
	1    3900 3000
	1    0    0    -1  
$EndComp
$Comp
L Angrywasp-General:GND #PWR0103
U 1 1 630CC10A
P 5300 3400
F 0 "#PWR0103" H 5300 3150 50  0001 C CNN
F 1 "GND" H 5305 3227 50  0000 C CNN
F 2 "" H 5300 3400 50  0000 C CNN
F 3 "" H 5300 3400 50  0000 C CNN
	1    5300 3400
	1    0    0    -1  
$EndComp
$Comp
L Angrywasp-General:GND #PWR0104
U 1 1 630CD99D
P 4800 3400
F 0 "#PWR0104" H 4800 3150 50  0001 C CNN
F 1 "GND" H 4805 3227 50  0000 C CNN
F 2 "" H 4800 3400 50  0000 C CNN
F 3 "" H 4800 3400 50  0000 C CNN
	1    4800 3400
	1    0    0    -1  
$EndComp
$Comp
L Angrywasp-General:GND #PWR0105
U 1 1 630CE469
P 4300 3400
F 0 "#PWR0105" H 4300 3150 50  0001 C CNN
F 1 "GND" H 4305 3227 50  0000 C CNN
F 2 "" H 4300 3400 50  0000 C CNN
F 3 "" H 4300 3400 50  0000 C CNN
	1    4300 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3000 4300 3050
Connection ~ 4300 3000
Wire Wire Line
	4300 3000 4500 3000
Wire Wire Line
	5300 3050 5300 3000
Connection ~ 5300 3000
Wire Wire Line
	5300 3000 5100 3000
$Comp
L Angrywasp-General:+5V #PWR0106
U 1 1 630D8A3D
P 5200 1800
F 0 "#PWR0106" H 5200 1650 50  0001 C CNN
F 1 "+5V" H 5215 1973 50  0000 C CNN
F 2 "" H 5200 1800 50  0000 C CNN
F 3 "" H 5200 1800 50  0000 C CNN
	1    5200 1800
	-1   0    0    1   
$EndComp
$Comp
L Angrywasp-General:GND #PWR0107
U 1 1 630D9AE3
P 5300 1800
F 0 "#PWR0107" H 5300 1550 50  0001 C CNN
F 1 "GND" H 5305 1627 50  0000 C CNN
F 2 "" H 5300 1800 50  0000 C CNN
F 3 "" H 5300 1800 50  0000 C CNN
	1    5300 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3400 4300 3350
Wire Wire Line
	4800 3400 4800 3300
Wire Wire Line
	5300 3400 5300 3350
Wire Wire Line
	3900 3000 4300 3000
$Comp
L Angrywasp-General:GND #PWR0108
U 1 1 630E8277
P 5500 1800
F 0 "#PWR0108" H 5500 1550 50  0001 C CNN
F 1 "GND" H 5505 1627 50  0000 C CNN
F 2 "" H 5500 1800 50  0000 C CNN
F 3 "" H 5500 1800 50  0000 C CNN
	1    5500 1800
	1    0    0    -1  
$EndComp
$Comp
L Angrywasp-General:+3V3 #PWR0109
U 1 1 630E87B1
P 5400 1800
F 0 "#PWR0109" H 5400 1650 50  0001 C CNN
F 1 "+3V3" H 5415 1973 50  0000 C CNN
F 2 "" H 5400 1800 50  0000 C CNN
F 3 "" H 5400 1800 50  0000 C CNN
	1    5400 1800
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 630EA9E6
P 5300 1600
F 0 "J1" V 5264 1312 50  0000 R CNN
F 1 "Conn_01x04" V 5173 1312 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 5300 1600 50  0001 C CNN
F 3 "~" H 5300 1600 50  0001 C CNN
	1    5300 1600
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
