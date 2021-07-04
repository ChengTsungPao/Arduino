// 4105054025、鄭琮寶

#include "pitches.h"

// 按鈕相關參數設定
struct Buttun_Content {
	const String frequence_type;        // 目前音調
	int frequence;                      // 目前頻率
	byte cur_frequence_number;          // 目前幾個八度的音調
	const byte change_frequence_number; // 每個按鈕案三秒後，要調整成高幾個八度的音調
	const String color_type;            // 目前顏色
	const byte color[3];                // 目前顏色之RGB
	const byte port;                    // 輸出的PORT
};

Buttun_Content Buttun[] = {
	{ "DO", NOTE_C6, 6, 2,    "red", {255,   0,   0},  4},
	{ "Re", NOTE_D6, 6, 3, "orange", {255, 165,   0}, A0},
	{ "Mi", NOTE_E6, 6, 4, "yellow", {255, 255,   0}, A1},
	{ "Fa", NOTE_F6, 6, 5,  "green", {  0, 255,   0}, A2},
	{"Sol", NOTE_G6, 6, 6,   "blue", {  0,   0, 255}, A3},
	{ "La", NOTE_A6, 6, 7, "indigo", { 75,   0, 130}, A4},
	{ "Ti", NOTE_B6, 6, 8, "purple", {128,   0, 128}, A5}
};

// 預設LED顏色
const byte default_light[3] = {0, 0, 255};

// 顏色輸出端
const byte Red = 7;
const byte Green = 6;
const byte Blue = 5;
const byte SP_PIN = 9;

void setup() {
	pinMode(Red, OUTPUT);
	pinMode(Green, OUTPUT);
	pinMode(Blue, OUTPUT);
	pinMode(SP_PIN, OUTPUT);
	for(byte i = 0; i < sizeof(Buttun) / sizeof(Buttun_Content); i++){
		pinMode(Buttun[i].port, INPUT);
	}
	SetupLED(default_light);
}

void loop() {
	CheckButtonEvent();
	delay(100);
}

void CheckButtonEvent() {
	for(byte i = 0; i < sizeof(Buttun) / sizeof(Buttun_Content); i++){
		if(digitalRead(Buttun[i].port) && CheckButtonTimer(Buttun[i].port, Buttun[i].frequence)){
			SetupFrequence(Buttun[i].change_frequence_number);
			SetupLED(Buttun[i].color);
		}
	}
}

bool CheckButtonTimer(byte port, int frequence) {
	const int timedelta = 100;
	for(int i = 0; i < 3000; i += timedelta){
		tone(SP_PIN, frequence);
		if(!digitalRead(port)){
			noTone(SP_PIN);
			return false;
		}
		delay(timedelta);
	}
	noTone(SP_PIN);
	return true;
}

void SetupFrequence(byte frequence_number) {
	for(byte i = 0; i < sizeof(Buttun) / sizeof(Buttun_Content); i++){
		Buttun[i].frequence *= pow(2, frequence_number - Buttun[i].cur_frequence_number);
		Buttun[i].cur_frequence_number = frequence_number;
	}	
}

void SetupLED(byte light[3]) {
	analogWrite(Red, light[0]);
	analogWrite(Green, light[1]);
	analogWrite(Blue, light[2]);
}