/* ECE319K_Lab4main.c
* Traffic light FSM
* ECE319H students must use pointers for next state
* ECE319K students can use indices or pointers for next state
* Put your names here or look silly
* NIRMAY DAS & NOLAN A.
*/
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h" // student's Lab 3
#include <stdio.h>
#include <string.h>
// put both EIDs in the next two\
lines
const char EID1[] = "nd22696"; // ;replace abc123 with your EID
const char EID2[] = "cna865"; // ;replace abc123 with your EID
struct State {
uint32_t Out;
uint32_t Time;
uint32_t Next[8];
};
typedef const struct State State_t;
#define GoS 0
#define WaitS 1
#define GoW 2
#define WaitW 3
#define GoWalk 4
#define RedWalk1 5
#define RedWalk2 6
#define WalkOff1 7
#define WalkOff2 8
#define AllRed1 9
#define AllRed2 10
#define AllRed3 11
State_t FSM[12]={
{0x04000101,300,{GoS,WaitS,GoS,WaitS,WaitS,WaitS,WaitS,WaitS}}, //GoS
{0x04000102,50,
{AllRed1,AllRed1,AllRed1,AllRed2,AllRed1,AllRed2,AllRed2,AllRed2}}, //WaitS
{0x04000044,300,{GoW,GoW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW}}, //GoW
{0x04000084,50,
{AllRed1,AllRed1,AllRed1,AllRed1,AllRed1,AllRed1,AllRed1,AllRed3}}, //WaitW
{0x0c400104,300,
{GoWalk,RedWalk1,RedWalk1,RedWalk1,GoWalk,RedWalk1,RedWalk1,RedWalk1}}, //GoWalk
{0x04000104,50,
{WalkOff1,WalkOff1,WalkOff1,WalkOff1,WalkOff1,WalkOff1,WalkOff1,WalkOff1}},
//RedWalk1
{0x04000104,50,
{WalkOff2,WalkOff2,WalkOff2,WalkOff2,WalkOff2,WalkOff2,WalkOff2,WalkOff2}},
//RedWalk2
{0x00000104,50,
{RedWalk2,RedWalk2,RedWalk2,RedWalk2,RedWalk2,RedWalk2,RedWalk2,RedWalk2}},
//WalkOff1
{0x00000104,50,
{AllRed1,AllRed1,AllRed1,AllRed1,AllRed1,AllRed2,AllRed2,AllRed1}}, //WalkOff2
{0x04000104,300,{AllRed1,GoW,GoS,GoS,GoWalk,GoWalk,GoS,GoW}}, //AllRed1
{0x04000104,300,{AllRed2,GoW,GoS,GoW,GoWalk,GoW,GoWalk,GoWalk}}, //AllRed2
{0x04000104,300,{AllRed3,GoW,GoS,GoS,GoWalk,GoWalk,GoS,GoS}}, //AllRed3
};
uint32_t S = 0;
// initialize 6 LED outputs and 3 switch inputs
// assumes LaunchPad_Init resets and powers A and B
void Traffic_Init(void){ // assumes LaunchPad_Init resets and powers A and B
// write this
//South Red PB2
//South Yellow PB1 working
//South Green PB0 working
//West Red PB8
//West Yellow PB7
//West Green PB6
//Walk Red pb26
//Walk White Pb22,pb26,pb27
//Walk "Off" ^ all off ^
//Sensor South PB16 INPUT
//Sensor West PB15 INPUT
//Walk Sensor PB17 INPUT
IOMUX->SECCFG.PINCM[31] = 0x00040081; // GPIO pb15 input west sensor
IOMUX->SECCFG.PINCM[32] = 0x00040081; // GPIO pb16 input south sensor
IOMUX->SECCFG.PINCM[42] = 0x00040081; // GPIO pb17 input walk sensor
IOMUX->SECCFG.PINCM[11] = 0x00000081; // GPIO pb0 output
IOMUX->SECCFG.PINCM[12] = 0x00000081; // GPIO pb1 output
IOMUX->SECCFG.PINCM[14] = 0x00000081; // GPIO pb2 output
IOMUX->SECCFG.PINCM[22] = 0x00000081; // GPIO pb6 output
IOMUX->SECCFG.PINCM[23] = 0x00000081; // GPIO pb7 output
IOMUX->SECCFG.PINCM[24] = 0x00000081; // GPIO pb8 output
GPIOB->DOE31_0 = (GPIOB->DOE31_0 | 0x381C7);//enable all outputs mask: 0011
1000 0001 1100 0111
// GPIOB->DOUT31_0 = (GPIOB->DOUT31_0 | 0x381C7); //delete this line later this
is just for testing
}
/* Activate LEDs
* Inputs: data1,data2,data3
* specify what these means
* Output: none
* Feel free to change this. But, if you change the way it works, change the test
programs too
* Be friendly*/
/* Read sensors
* Input: none
* Output: sensor values
* specify what these means
* Feel free to change this. But, if you change the way it works, change the test
programs too
*/
uint32_t Traffic_In(void){
// write this
uint32_t input = 0;
input = GPIOB->DIN31_0 & 0x00038000;
return input; // replace this line
}
void Traffic_Out(){
// write this
uint32_t inputTest = Traffic_In();
uint32_t inputFinal = inputTest>>15;
GPIOB->DOUT31_0 = (GPIOB->DOUT31_0&(0x00000030))|FSM[S].Out;
SysTick_Wait10ms(FSM[S].Time);
S=FSM[S].Next[inputFinal];
}
// use main1 to determine Lab4 assignment
void Lab4Grader(int mode);
void Grader_Init(void);
int main1(void){ // main1
Clock_Init80MHz(0);
LaunchPad_Init();
Traffic_Init(); //temp delete later
Lab4Grader(0); // print assignment, no grading
int32_t Input = 0;
while(1){
}
}
// use main2 to debug LED outputs
int main2(void){ // main2
Clock_Init80MHz(0);
LaunchPad_Init();
Grader_Init(); // execute this line before your code
Traffic_Init(); // your Lab 4 initialization
if((GPIOB->DOE31_0 & 0x20)==0){
UART_OutString("access to GPIOB->DOE31_0 should be friendly.\n\r");
}
Debug_Init(); // Lab 3 debugging
UART_Init();
UART_OutString("Lab 4, Spring 2024, Step 1. Debug LEDs\n\r");
UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
while(1){
//write debug code to test your Traffic_Out
// Call Traffic_Out testing all LED patterns
// Lab 3 dump to record output values
uint32_t temp1 = 0;
Traffic_Out();
GPIOB->DOUT31_0 = GPIOB->DOUT31_0 & 0;
temp1 = GPIOB->DOUT31_0 | 0x01;
Debug_Dump(temp1);
temp1 = GPIOB->DOUT31_0 ^ 0x01;
temp1 = GPIOB->DOUT31_0 | 0x02;
Debug_Dump(temp1);
temp1 = GPIOB->DOUT31_0 ^ 0x02;
temp1 = GPIOB->DOUT31_0 | 0x04;
Debug_Dump(temp1);
temp1 = GPIOB->DOUT31_0 ^ 0x04;
temp1 = GPIOB->DOUT31_0 | 0x100;
Debug_Dump(temp1);
temp1 = GPIOB->DOUT31_0 ^ 0x100;
temp1 = GPIOB->DOUT31_0 | 0x80;
Debug_Dump(temp1);
temp1 = GPIOB->DOUT31_0 ^ 0x80;
temp1 = GPIOB->DOUT31_0 | 0x40;
Debug_Dump(temp1);
temp1 = GPIOB->DOUT31_0 ^ 0x40;
Clock_Delay(40000000); // 0.5s
if((GPIOB->DOUT31_0&0x20) == 0){
UART_OutString("DOUT not friendly\n\r");
}
}
}
// use main3 to debug the three input switches
int main3(void){ // main3
uint32_t last=0,now;
Clock_Init80MHz(0);
LaunchPad_Init();
Traffic_Init(); // your Lab 4 initialization
Debug_Init(); // Lab 3 debugging
UART_Init();
__enable_irq(); // UART uses interrupts
UART_OutString("Lab 4, Spring 2024, Step 2. Debug switches\n\r");
UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
while(1){
now = Traffic_In(); // Your Lab4 input
if(now != last){ // change
UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
Debug_Dump(now);
}
last = now;
Clock_Delay(800000); // 10ms, to debounce switch
}
}
// use main4 to debug using your dump
// proving your machine cycles through all states
int main4(void){// main4
uint32_t input = 0;
Clock_Init80MHz(0);
LaunchPad_Init();
LaunchPad_LED1off();
Traffic_Init(); // your Lab 4 initialization
Debug_Init(); // Lab 3 debugging
UART_Init();
__enable_irq(); // UART uses interrupts
UART_OutString("Lab 4, Spring 2024, Step 3. Debug FSM cycle\n\r");
UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
// initialize your FSM
SysTick_Init(); // Initialize SysTick for software waits
while(1){
// 1) output depending on state using Traffic_Out
input = 7;
GPIOB->DOUT31_0 = (GPIOB->DOUT31_0&(0x00000030))|FSM[S].Out;
SysTick_Wait10ms(FSM[S].Time);
S=FSM[S].Next[input];
// call your Debug_Dump logging your state number and output
uint32_t temp;
temp = (FSM[S].Out) | (S<<28);
Debug_Dump(temp);
// for(uint32_t i = 0; i<12; i++){ //logs state number
// Debug_Dump(i);
// }
// for(uint32_t i = 0; i<12; i++){ //logs output
// uint32_t x = FSM[i].Out;
// Debug_Dump(x);
// }
// 2) wait depending on state
SysTick_Wait10ms(FSM[S].Time);
// 3) hard code this so input always shows all switches pressed
// 4) next depends on state and input
// S = FSM[S].Next[input];
}
}
// use main5 to grade
int main(void){// main5
Clock_Init80MHz(0);
LaunchPad_Init();
Grader_Init(); // execute this line before your code
Traffic_Init(); // your Lab 4 initialization
SysTick_Init(); // Initialize SysTick for software waits
Debug_Init();
// initialize your FSM
Lab4Grader(1); // activate UART, grader and interrupts
S = GoS;
while(1){
Traffic_Out();
Debug_Dump(S);
// 1) output depending on state using Traffic_Out
// call your Debug_Dump logging your state number and output
// 2) wait depending on state
// 3) input from switches
// 4) next depends on state and input
}
}