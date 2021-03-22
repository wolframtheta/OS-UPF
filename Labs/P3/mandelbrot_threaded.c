#include <stdio.h>
#include <string.h>
#include <unistd.h>    // Unix-like system calls read and write
#include <fcntl.h>     // Unix-like system calls to open and close
#include <pthread.h>
#include "myutils.h"

#define R 4  // Constant indicating the image divisions RxR
#define N 100 //number of threads for the interchanges
enum { width=1024, height=1024 };

//enum { width=8192, height=8192 };

// Data structs for the thread functions
struct thread_data{
	int ti;
	int tj;
	unsigned char* p;
}thread_data;

struct thread_interchange{
	int ti,tj,si,sj;
	unsigned char* p;
}thread_interchange;

monitor_lock monitors[R][R]; 

int tga_write_header(int fd, int width, int height) {
  static unsigned char tga[18];
  int nbytes;
  tga[2] = 2;
  tga[12] = 255 & width;
  tga[13] = 255 & (width >> 8);
  tga[14] = 255 & height;
  tga[15] = 255 & (height >> 8);
  tga[16] = 24;
  tga[17] = 32;
  nbytes = write(fd, tga, sizeof(tga));
  return nbytes == sizeof(tga);
}


void write_tga(char* fname, unsigned char *pixels, int width, int height)
{
  int fd = open(fname,  O_CREAT | O_RDWR, 0640);
  tga_write_header(fd, width, height);
  printf("Created file %s: Writing pixels size %d bytes\n", fname, 3*width*height);
  write(fd, pixels,3*width*height);
  close(fd);
}



void tga_read_header(int fd, int* width, int* height) {
  static unsigned char tga[18];
  read(fd, tga, 12);
  read(fd, width, 2);
  read(fd, height, 2);
  read(fd, &tga[16], 2);
}


int compute_iter(int i, int j, int width, int height) {
  int itermax = 255/2;  
  int iter;
  double x,xx,y,cx,cy;
  cx = (((float)i)/((float)width)-0.5)/1.3*3.0-0.7;
  cy = (((float)j)/((float)height)-0.5)/1.3*3.0;
  x = 0.0; y = 0.0;
  for (iter=1;iter<itermax && x*x+y*y<itermax;iter++)  {
    xx = x*x-y*y+cx;
    y = 2.0*x*y+cy;
    x = xx;
  }
  return iter;
}

void generate_mandelbrot_region(unsigned char *p, int ti, int tj){
  int k,l;
  int n = width / R;
  for (k=0;k<n;k++) {
    int t_index = ti*n*width + tj*n + k*width;
    int i = t_index / width;
    int j = t_index % width;
    unsigned char *p_region = &p[3*t_index];
    for (l=0; l<n; l++) {
      *p_region++ = 255 * ((float)(j+l) / height);
      *p_region++ = 255 * ((float)i / width);
      *p_region++ = 2 * compute_iter(i, (j+l), width, height);
    }
  }
}

void interchange(int si, int sj, int ti, int tj, unsigned char *p, int width, int height) {
  int k;
  int n = width / R;
  unsigned char* square = malloc(n*n*3);
  memset(square, 0, n*n*3);
  //entering in the critical section
  mon_lock(&monitors[si][sj]);
  mon_lock(&monitors[ti][tj]);
  for (k=0;k<n;k++) {
    int t_index = ti*n*3*width + tj*3*n + k*3*width;
    memcpy(&square[k*3*n], &p[t_index], n*3);
  }
  for (k=0;k<n;k++) {
    int s_index = si*n*3*width + sj*3*n + k*3*width;
    int t_index = ti*n*3*width + tj*3*n + k*3*width;
    memcpy(&p[t_index], &p[s_index], n*3);
  }
  for (k=0;k<n;k++) {
    int s_index = si*n*3*width + sj*3*n + k*3*width;
    memcpy(&p[s_index], &square[k*3*n], n*3);
  }
  //leaving critical section
  mon_unlock(&monitors[si][sj]);
  mon_unlock(&monitors[ti][tj]);
  free(square);
}

// ------------ THREAD FUNCTIONS -------------

void* thread_function (void *received_data){
  struct thread_data *my_data = (struct thread_data*) received_data; //recast to struct
  int i = my_data->ti;
  int j = my_data->tj;
  unsigned char* p = my_data->p;
  generate_mandelbrot_region(p,i,j);
}

void* thread_i_function(void *received_data){
  struct thread_interchange *my_data = (struct thread_interchange*) received_data;
  int ti = my_data->ti;
  int tj = my_data->tj;
  int si = my_data->si;
  int sj = my_data->sj;
  unsigned char* p = my_data->p;
  int f = 1000/N;
  for (int i = 0; i<f;i++){
    si = rand()%R;
    sj = rand()%R;
    ti = rand()%R;
    tj = rand()%R;
    // check what region is the lower one
    if (si<ti){
    	interchange(si,sj,ti,tj, p,width,height); //first region is lower
    }else if (si == ti && sj<tj){
    	interchange(si,sj,ti,tj, p,width,height); //first region is lower
    }else if(si == ti && sj>tj){
    	interchange(ti,tj,si,sj, p,width,height); //second regions is lower, swap values
    }else if (si>ti){
    	interchange(ti,tj,si,sj, p,width,height); //second regions is lower, swap values
    }
    
  } 
}
// ---------------------------------------------

int main(void) {
  int i, si, sj, ti, tj;
  unsigned char pixels[width * height * 3];

  struct thread_data thread_data_array[R][R];
  pthread_t tids [R][R];
  
  //THREADED MANDELBROAD GENERATION
  startTimer(0);
  for (int i = 0; i<R;i++){
    for (int j = 0; j<R;j++){
      thread_data_array[i][j].ti = i;
      thread_data_array[i][j].tj = j;
      thread_data_array[i][j].p = pixels;
      pthread_create(&tids[i][j],NULL,thread_function,(void*)&thread_data_array[i][j]);
    }
  }
  //WAITING THE THREADS TO FINISH
  for (int i = 0; i<R;i++){
    for (int j = 0; j<R;j++){
    	pthread_join(tids[i][j],NULL);
    }
  }
  

  printf("Time spent generate_mandelbrot: %ldms\n", endTimer(0));
  write_tga("image2.tga", pixels, width, height);
  
  
  //THREADED MANDELBROAD INTERCHANGE
  pthread_t tids_i [N];
  struct thread_interchange thread_array [N];

  startTimer(0);
  
  for (int i = 0; i<R;i++){
    for (int j = 0; j<R;j++){
      mon_lock_init(&monitors[i][j]); //initializing monitors
    }
  }
  for (int i = 0; i<N; i++){
    thread_array[i].ti = 0; //initializing the arguments
    thread_array[i].tj = 0;
    thread_array[i].si = 0;
    thread_array[i].sj = 0;
    thread_array[i].p = pixels;
    pthread_create(&tids_i[i],NULL,thread_i_function,(void*)&thread_array[i]);
  }
  for (int i = 0; i<N; i++){
    pthread_join(tids_i[i],NULL);
  }
  
  printf("Time spent during interchange: %ldms\n", endTimer(0));
  write_tga("image_scrambled_thread_m.tga", pixels, width, height);

  return 0;
}
