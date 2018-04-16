/*
 * File:   readIR.c
 * Author: BEAN
 *
 * Created on April 16, 2018, 21:00
 */

 /*
  * このプログラムについて
  * ^^^^^^^^^^^^^^^^^^^
  * 赤外線センサより赤外線信号を読み、解析してそのデータを表示するRaspberry Pi 3用のプログラム
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <wiringPi.h>
// GPIO指定をわかりやすくする自作ヘッダファイル
#include "rpi-pin.h"

// 赤外線センサのOutputピンに接続するGPIOピンの指定
#define READPIN GPIO_23

#define NO_SIGNAL_THRESHOLD 20000

typedef struct irDataStruct{
    unsigned int onTime;
    unsigned int offTime;
    struct irDataStruct *next;
} irData_t;

int readIR(irData_t *head) {

    struct timeval start, end;
    unsigned int noSignalCount = 0;
    irData_t *newIrData = (irData_t *)malloc(sizeof(irData_t));
    irData_t *tmp = head;

    while(tmp -> next != NULL)
            tmp = tmp -> next;

    while(digitalRead( READPIN ));
    gettimeofday(&start, NULL);

    while(!digitalRead( READPIN ));
    gettimeofday(&end, NULL);
    newIrData -> onTime = (unsigned int)( end.tv_usec - start.tv_usec );

    gettimeofday(&start, NULL);
    while(digitalRead( READPIN )) {
        if(noSignalCount > NO_SIGNAL_THRESHOLD) {
            newIrData -> offTime = 0;
            newIrData -> next = NULL;
            return 0;
        }
        delayMicroseconds(10);
        noSignalCount++;
    }
    gettimeofday(&end, NULL);
    newIrData -> offTime = (unsigned int)( end.tv_usec - start.tv_usec );

    newIrData -> next = NULL;

    tmp -> next = newIrData;

    return 1;

}

void writeIrData(irData_t *head, FILE *fp){

    fprintf(fp, "%6d\t%6d\n", head -> onTime, head ->offTime);
    //printf("%6d:%6d\n", head -> onTime, head ->offTime);

    if(head -> next == NULL)
        return;

    writeIrData(head -> next, fp);

}

void freeIrData(irData_t *head){

    if(head -> next != NULL) {

        freeIrData(head -> next);

    }

    free(head);

}

int main(int argc, char *argv[]){

    FILE *fp;
    irData_t *rawIrData;
    char fileName[32];

    rawIrData = (irData_t *)malloc(sizeof(irData_t));

    rawIrData -> onTime = 0;
    rawIrData -> offTime = 0;
    rawIrData -> next = NULL;

    if (argc > 1 && argc < 3) {
        strcpy(fileName, argv[1]);
    }else if(argc == 1) {
        strcpy(fileName, "IR-Data.dat");
    }else{
        puts("Usage: ./readIR [Output file name]");
    }

    if ( (fp = fopen(fileName, "w")) == NULL ) {
        perror("Failed to open IR-Data file !!");
        return 1;
    }

    if(wiringPiSetup() == -1) return 1;

    pinMode(READPIN, INPUT);

    puts("赤外線受信可能");

    while(readIR(rawIrData));

    puts("赤外線受信完了");

    writeIrData(rawIrData, fp);

    freeIrData(rawIrData);

    fclose(fp);

    return 0;
}
