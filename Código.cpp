#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <sstream>
#include "GPIO.h"
#define LDS_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"
#define AIN0 0


using namespace std;
int readAnalog(int number);
GPIO inGPIO1(18);
GPIO inGPIO2(37);
GPIO outGPIO1(28);
GPIO outGPIO2(17);

void * threadFunction(void *arg);
void * threadFunction2(void *arg);

int main (int argc, char *argv[]) {

    inGPIO1.setDirection(INPUT);
    outGPIO1.setDirection(OUTPUT);
    inGPIO2.setDirection(INPUT);
    outGPIO2.setDirection(OUTPUT);

    pthread_t thread_id1, thread_id2;
    void * thread_ret;
    int status;

    status = pthread_create(&thread_id1,NULL,threadFunction,(void*)("1"));
    if(status != 0) {
        printf ("*** Erro na criação da thread do botão ***\n");
        exit(EXIT_FAILURE);
    } else {
        printf ("Thread do botão criada com sucesso.\n\n");
    }

    status = pthread_create(&thread_id2,NULL,threadFunction2,(void*)("2"));
    if(status != 0) {
        printf ("*** Erro na criação da thread do potenciomentro ***\n");
        exit(EXIT_FAILURE);
    } else {
        printf ("Thread do potenciomentro criada com sucesso.\n\n");
    }


    status = pthread_join (thread_id1, &thread_ret);
    if(status != 0) {
        printf ("*** Erro na finalização da thread do botão ***\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Thread do botão finalizada com sucesso.\n\n");
    }

    status = pthread_join (thread_id2, &thread_ret);
    if(status != 0) {
        printf ("*** Erro na finalização da thread do potenciomentro ***\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Thread do potenciomentro finalizada com sucesso.\n\n");
    }
    return 0;
}

void * threadFunction(void *arg) {

    clock_t t_thread;
    t_thread = clock();
    for(;;){
        if((double(clock() - t_thread)/ double(CLOCKS_PER_SEC))*1000 > 100){
            printf("Thread %s executando...\n\n",(char *)arg);
            int potencio;
            float potenciomentro;
            potencio = readAnalog(AIN0);
            potenciomentro = ((potencio * 5.0) / 4095);
            cout << "Valor Potenciometro ... = " << potenciomentro << endl;
            cout << endl;
            if(potenciomentro >= 2.5){
                outGPIO2.setValue(HIGH);
            }else{
                outGPIO2.setValue(LOW);
            }
            t_thread = clock();
        }

    }
    pthread_exit(arg);
}

void * threadFunction2(void *arg) {

    clock_t t_thread;
    t_thread = clock();
    for(;;){
        if((double(clock() - t_thread)/ double(CLOCKS_PER_SEC))*1000 > 100){
            printf("Thread %s executando...\n\n",(char *)arg);
            if(inGPIO1.getValue() == 1){
                outGPIO1.setValue(HIGH);
            }else{
                outGPIO1.setValue(LOW);
            }
            t_thread = clock();
        }

    }
    pthread_exit(arg);
}


int readAnalog(int number){
stringstream ss;
ss << LDS_PATH << number << "_raw";
fstream fs;
fs.open(ss.str().c_str(), fstream::in);
fs >> number;
fs.close();
return number;
}

