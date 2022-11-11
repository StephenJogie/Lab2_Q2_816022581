
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "freertos/queue.h" 
#include "freertos/semphr.h"
//For this question, we will use a mutex 

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"

static const char *TAG = "main";
//Use pre-emptive scheduling
//#define configUSE_PREEMPTION 1;
//#define configUSE_MUTEXES 1;

#define GPIO_OUTPUT_IO_0    2
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0))
#define GPIO_INPUT_IO_0     0
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0))

static SemaphoreHandle_t xMutex = NULL;
static TaskHandle_t xHandle_1 = NULL;
static TaskHandle_t xHandle_2 = NULL;
static TaskHandle_t xHandle_3 = NULL;


static void task_1(void *arg)
{   
    bool output_high = 1;
    for(;;){
    
        //Take the mutex before performing task
        //We want a non-blocking task, so we immediately return after 0 ticks check
        if (xSemaphoreTake(xMutex, (TickType_t)1)==pdTRUE) //had ==pdTRUE before
        {   
            printf("task 1 took semaphore thing \n"); //testline
            //bool output_level = 1;
            gpio_set_level(GPIO_OUTPUT_IO_0, output_high);

            //Active delay for 500ms (ish)
            printf("Active delay for 500ms \n");
            for (uint32_t i=0;i<6500000;i++){

            }   
        
            //Task delay for 1000ms

                
            //Give back the mutex after critical task section complete
            printf("Going to attempt to give back the mutex after resisting tempation (task1) \n");
            xSemaphoreGive(xMutex);
            printf("task1 gave back the mutex despite the temptations of inner greed \n");
            printf("task1 task delay for 1000ms \n");
            vTaskDelay(1000 / portTICK_RATE_MS);
        } 
        else
        {
            //Did not take the mutex
            printf("Could not take the mutex (task 1 btw) \n");
        }
    }
}

static void task_2(void *arg)
{
    bool output_low = 0;
    for(;;){
    
        //Take the mutex before performing task
        //We want a non-blocking task, so we immediately return after 0 ticks check
        if (xSemaphoreTake(xMutex, (TickType_t)1)==pdTRUE)
        {
            // bool output_level = 0;
            printf("TASK 2 took semaphore \n");
            gpio_set_level(GPIO_OUTPUT_IO_0, output_low);

            //Active delay for 500ms (ish)
            for (uint32_t i=0;i<6500000;i++){
                //I am unsure if this is the problem with triggering the task watchdog timer or not
                //I will test without and with later to see
            }   
        
            //Give back the mutex after critical task section complete
            xSemaphoreGive(xMutex);

            //Task delay for 1000ms
            vTaskDelay(1000 / portTICK_RATE_MS);
                
        }
        else
        {
            //Did not take the mutex
            printf("could not take the mutex (task2 btw) \n");
        }
    }
}

static void task_3(void *arg)
{
    uint8_t pin_state;
    for(;;){
    
        //Take the mutex before performing task
        //We want a non-blocking task, so we immediately return after 0 ticks check
        if (xSemaphoreTake(xMutex, (TickType_t)1)==pdTRUE)
        {   
            printf("task 3 took mutex \n");
            //Print the output level of the pin if mutex is taken
            pin_state= gpio_get_level(GPIO_OUTPUT_IO_0);
            printf("The output level of the PIN: %d \n", pin_state); 
            
                
            //Give back the mutex after critical task section complete
            xSemaphoreGive(xMutex);
            //Task delay for 1000ms
            vTaskDelay(1000 / portTICK_RATE_MS);
        }
        else
        {
         printf("Task3 can't take the mutex :( \n");   //Did not take the mutex
        }
    }
}

void app_main(void)
{
    xMutex = xSemaphoreCreateMutex();
    if(xMutex != NULL)
    {
        printf("MUTEX CREATED SUCCESSFULLY \n");
    }

    //NOTE: Configuring outside of tasks for now, will try to configure within tasks next, or make configure task 
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO 2
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
    printf("GPIO 2 CONFIGURED AS OUTPUT \n");

    //Start the three tasks
    printf("Going to attempt to make the tasks now \n");
    
    xHandle_1 = xTaskCreate(task_1, "task_1", 2048, NULL, 10, NULL);
    printf("TASK1 made  \n");
    xHandle_2 = xTaskCreate(task_2, "task_2", 2048, NULL, 10, NULL);
    printf("TASK2 made  \n");
    xHandle_3 = xTaskCreate(task_3, "task_3", 2048, NULL, 11, NULL); 
    printf("TASK3 made  \n");

    int cnt = 0;
    //While loop
    while(1){
        //Put main loop function here
        //Will put nothing here as I just want my tasks to do their thing
        cnt++;
        //ESP_LOGI(TAG, "cnt: %d \n", cnt);
        printf("COUNT = %d \n \n",cnt);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }

}


