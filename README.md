# Lab2_Q2_816022581

# Lore and Hypothesis

Recall the following:
![image](https://user-images.githubusercontent.com/91706020/202894322-8b30e659-cb54-4c35-835c-394f5bb3028c.png)

The above behavior displays correct operation of the system for lab 2. 

In lab 2 q2 the priority levels of task1, task2, and task3 are all set to be equal and pre-emption is enabled. 

Task1 is created first, then Task2 is created, then Task3 is created. 

When task1 and task2 take the mutex and use the gpio pin (shared resource), they have an active delay of 500ms WHILE HOLDING THE MUTEX, they then give back the mutex for another task to use, then they have a TASK DELAY of 1000ms.

When task3 takes the mutex, it simply makes a printf statement for the state of the gpio2 pin and then returns the mutex and task delays itself immediately. 
Therefore, it is expected that task3 utilization should be minimal compared to task1, task2, and IDLE. 

task1, task2, and IDLE should have around a third of the utilization each as shown in the oscilloscope figure. 

The freertos kernel functions and the function to get the runtimestats for the tasks would utilize some of the runtime for the system. But the majority of the utilization percentages should be from task1, task2, and IDLE. 

The function to get and print runtimestats will run once every ten seconds. This is to allow multiple hyper periods. 

# Result screenshots from output (outputq2discuss.out)

The far-rightmost column of the print statement is the percentage utilization. (See documentation on https://www.freertos.org/a00021.html#vTaskGetRunTimeStats)

First statistic print:


![image](https://user-images.githubusercontent.com/91706020/202894600-9068ff4e-b473-40a4-a7e9-a94aa85fecca.png)

Second statistic print:

![image](https://user-images.githubusercontent.com/91706020/202894673-4f2977aa-01ef-4586-add6-5fdb5969c30d.png)

Third statistic print:

![image](https://user-images.githubusercontent.com/91706020/202894697-bcac0809-32ad-4941-a9cf-6d7d05755f19.png)


These verify the hypothesis and show that the system operates as intented. These results are mostly expected (was unsure of the specifics of the other freertos kernel tasks and getting the statistics) and line up with the expected results as the vast majority of utilization is from task1, task2, and IDLE with each accounting for around 30 percent of utilization time.
