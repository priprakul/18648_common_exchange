
/*
 * setitimer.c - simple use of the interval timer
 */
#include <stdio.h>
#include <sys/time.h>		/* for setitimer */
#include <unistd.h>		/* for pause */
#include <signal.h>		/* for signal */
#include <stdlib.h>


int c; 



/* function prototype */
void DoStuff(void);

int main(int argc, char *argv[]) {

  struct itimerval it_val;	/* for setting itimer */

    int t,cpuid; 

    c = atoi(argv[1]); 
    t = atoi(argv[2]);

    if (c > 60000 || t > 60000){
        printf("Time is not supported");
        return -1; 
    }
    //cpuid = atoi(argv[3]);

    // cpu_set_t set;
    // CPU_ZERO(&set);        // clear cpu mask
    // CPU_SET(cpuid, &set);      // set cpu 0

   /* TODO: can't set cpu affinity for a nonlinux machine; need to test */ 
    //ret = sched_setaffinity(0, sizeof(cpu_set_t), &set);  // 0 is the calling process

    // if (ret != 0){
    //     printf("Couldn't set CPUID \n");
    //     return -1; 
    // }

  /* Upon SIGALRM, call DoStuff().
   * Set interval timer.  We want frequency in ms, 
   * but the setitimer call needs seconds and useconds. */

  if (signal(SIGALRM, (void (*)(int)) DoStuff) == SIG_ERR) {
    perror("Unable to catch SIGALRM");
    exit(1);
  }
  it_val.it_value.tv_sec =     t/1000;
  it_val.it_value.tv_usec =    (t*1000) % 1000000;	
  it_val.it_interval = it_val.it_value;
  if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
    perror("error calling setitimer()");
    exit(1);
  }

  while (1) 
    pause();

}

/*
 * DoStuff
 */
void DoStuff(void) {

    printf("Timer went off.\n");
    int i=0; 
    struct itimerval start_time, current_time,;
    long elapsed_time; 
    //gettimeofday(&start_time, NULL);
    //gettimeofday(&current_time, NULL);

    getitimer(ITIMER_VIRTUAL, &current_time);
    elapsed_time =  (current_time.it_value.tv_sec-start_time.it_value.tv_sec)*1000 +  (current_time.it_value.tv_usec-start_time.it_value.tv_usec);
    printf("Done Time : %ld\n", elapsed_time);
    while (elapsed_time< c){
        i++;
        gettimeofday(&current_time, NULL);
        elapsed_time =  (current_time.tv_sec-start_time.tv_sec)*1000 +  (current_time.tv_usec-start_time.tv_usec);
        printf("Done Time : %ld\n", elapsed_time);
    }
   


}