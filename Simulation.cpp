#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>

using namespace std;

pthread_mutex_t m;

// Global Variables
int swimmingStyle;
int poolLength;
sem_t sem1;

struct Swimmer
{
    string name;

    double freeStyleSpeed; // m/s
    double backStrokeSpeed; // m/s
    double butterflyStyleSpeed; // m/s
    double breastStrokeSpeed; // m/s
    double selectedSpeed; //This will change depending on the seped selected

    double swimmingTime; // seconds
};

Swimmer SwimmerList[6], buffer[1];

void* SwimmingTime (void *arg) 
{
    Swimmer *swimmer;

    swimmer = (Swimmer *) arg;

    pthread_mutex_lock(&m);
    cout << swimmer->name << ", Selected speed: " << swimmer->selectedSpeed << endl;
    pthread_mutex_unlock(&m);
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    pthread_t thread[6];

    // Main Menu
    printf("***************************\n");
    printf("* Competencia de Natacion *\n");
    printf("***************************\n");

    cout << "Estilos de competencia:" << endl << endl;

    cout << "1. Libre" << endl 
         << "2. Espalda" << endl 
         << "3. Mariposa" << endl 
         << "4. Pecho" << endl 
         << endl;
    
    cout << "Tu eleccion: ";
    cin >> swimmingStyle;
    cout << endl;

    cout << "Ingresa la longitud de la piscina:";
    cin >> poolLength;
    cout << endl;

    // Swimmer structures
    // Best freestyle swimmer
    SwimmerList[0] = {"David Popovici", 2.13, 1.87, 1.68, 1.15};
    // Best backstroke swimmer
    SwimmerList[1] = {"Thomas Ceccon", 1.34, 1.94, 1.06, 1.41};
    // Best butterfly swimmer
    SwimmerList[2] = {"Caeleb Dressel", 1.97, 1.17, 2.02, 1.20};
    // Best breaststroke swimmer
    SwimmerList[3] = {"Adam Peaty", 1.01, 1.25, 1.55, 1.76};
    // Mythical swimmer
    SwimmerList[4] = {"Michael Phelps", 1.90, 1.30, 1.69, 1.10};
    // Guatemalan swimmer xd
    SwimmerList[5] = {"Gisela Morales", 2.01, 1.52, 1.01, 1.13};

    //updating the selected sped variable for all swimmers
    for(int i = 0; i < 6; i++){
        if(swimmingStyle == 1){
            SwimmerList[i].selectedSpeed = SwimmerList[i].freeStyleSpeed;
        }
        else if(swimmingStyle == 2){
            SwimmerList[i].selectedSpeed = SwimmerList[i].backStrokeSpeed;
            
        }
        else if(swimmingStyle == 3){
            SwimmerList[i].selectedSpeed = SwimmerList[i].butterflyStyleSpeed;
            
        }
        else if(swimmingStyle == 4){
            SwimmerList[i].selectedSpeed = SwimmerList[i].breastStrokeSpeed;
        }
    }

    for(int i = 0; i < 6; i++){
        
        Swimmer nSwimmer = SwimmerList[i];
        pthread_create(&thread[i], NULL, SwimmingTime, (void *)&SwimmerList[i]);
    }

    for (int i = 0; i < 6; i++)
    {
        pthread_join(thread[i], NULL);
    }

}