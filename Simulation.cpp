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
sem_t sem1, sem2;

/**
 * The above code defines a struct called "Swimmer" with various properties related to swimming speed
 * and time.
 * @property {string} name - The name of the swimmer.
 * @property {double} freeStyleSpeed - The speed at which the swimmer can swim using the freestyle
 * stroke, measured in meters per second (m/s).
 * @property {double} backStrokeSpeed - The backStrokeSpeed property represents the speed at which the
 * swimmer can swim using the backstroke technique, measured in meters per second (m/s).
 * @property {double} butterflyStyleSpeed - The speed at which the swimmer can swim using the butterfly
 * stroke, measured in meters per second.
 * @property {double} breastStrokeSpeed - The breastStrokeSpeed property represents the speed at which
 * the swimmer can swim using the breaststroke technique, measured in meters per second (m/s).
 * @property {double} selectedSpeed - The selectedSpeed property is used to store the speed selected by
 * the swimmer. This value will change depending on the speed selected by the swimmer for a particular
 * race.
 * @property {double} swimmingTime - The swimmingTime property represents the time it takes for the
 * swimmer to complete the race, measured in seconds.
 */
struct Swimmer
{
    string name;

    double freeStyleSpeed; // m/s
    double backStrokeSpeed; // m/s
    double butterflyStyleSpeed; // m/s
    double breastStrokeSpeed; // m/s
    double selectedSpeed; //This will change depending on the seped selected

    double swimmingTime; // seconds for the swimmer to end the race
};

Swimmer SwimmerList[6], buffer[1];

//This method works as a productor also.
/**
 * The function calculates the swimming time for a swimmer, updates the swimmer's swimming time, and
 * adds the swimmer to a buffer.
 * 
 * @param arg The arg parameter is a void pointer that is used to pass the Swimmer object to the
 * SwimmingTime function. The Swimmer object contains information about the swimmer, such as their
 * selected speed and swimming time.
 * 
 * @return a void pointer, but the actual return statement is unreachable because it is placed after
 * the pthread_exit() function. Therefore, the return statement will not be executed.
 */
void* SwimmingTime (void *arg) 
{
    Swimmer *swimmer;

    swimmer = (Swimmer *) arg;
    
    double time;

    time = (poolLength / swimmer->selectedSpeed);
    swimmer->swimmingTime = time;

    usleep(time * 900000);
    sem_wait(&sem1);
    pthread_mutex_lock(&m);
    buffer[0] = *swimmer;
    pthread_mutex_unlock(&m);
    
    sem_post(&sem2);
    

    pthread_exit(NULL);
    return 0;
}

/**
 * The function "Lee" prints out information about a swimmer's name and swimming time.
 * 
 * @param place The parameter "place" represents the position or rank of a swimmer in a competition.
 */
void Lee(int place)
{
    sem_wait(&sem2);
    pthread_mutex_lock(&m);
    cout << place << ". " << buffer[0].name 
         << " ha terminado con un tiempo de " << buffer[0].swimmingTime 
         << " segundos." << endl;

    pthread_mutex_unlock(&m);
    sem_post(&sem1);
} 

/**
 * The main function initializes variables, prompts the user for input, creates threads to calculate
 * swimming times, and waits for the threads to finish before ending.
 * 
 * @param argc The parameter `argc` is the number of command-line arguments passed to the program. It
 * stands for "argument count".
 * @param argv The `argv` parameter is an array of strings that represents the command-line arguments
 * passed to the program. Each element of the array is a null-terminated string. The first element
 * (`argv[0]`) is the name of the program itself.
 */
int main(int argc, char *argv[])
{

    pthread_t thread[6];
    pthread_mutex_init(&m,NULL);
    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 0);

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

    cout << "Ingresa la longitud de la piscina (metros): ";
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

    cout << "Resultados de la carrera: " << endl;

    for(int i = 0; i < 6; i++){
        pthread_create(&thread[i], NULL, SwimmingTime, (void *)&SwimmerList[i]);
    }

    //Call of the consumer of the program
    for(int j = 0; j < 6; ++j)
    {
        Lee(j + 1);
    }

    cout << endl;

    for (int i = 0; i < 6; i++)
    {
        pthread_join(thread[i], NULL);
    }
}