/*
 * esercizio-2020-05-23-thread-string.c
 *
 *  Created on: May 21, 2020
 *      Author: marco
 */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <errno.h>
#include <pthread.h>

// barriera per N threads realizzata con pthread_barrier_t
// vedere anche:
// https://github.com/marcotessarotto/esercizio-C-2020-05-20-barrier

// https://linux.die.net/man/3/pthread_barrier_init
// https://linux.die.net/man/3/pthread_barrier_wait


#define N 3

pthread_barrier_t thread_barrier;

int number_of_threads = N;
pthread_t threads[N];

int conta_vocali(char * content){
	int counter=0;
	char * p;
	p=content;

	for(int i=0;i<strlen(content);++i)
		if(content[i]=='a' | content[i]=='e' | content[i]=='i' | content[i]=='o' | content[i]=='u' |
				content[i]=='A' | content[i]=='E' | content[i]=='I' | content[i]=='O' | content[i]=='U' )
			++counter;
	return counter;
}

int conta_consonanti(char * content){
	int counter=0;
	for(int i=0;i<strlen(content);++i)
		if(content[i]!='a' && content[i]!='e' && content[i]!='i' && content[i]!='o' && content[i]!='u' &&
				content[i]!='A' && content[i]!='E' && content[i]!='I' && content[i]!='O' && content[i]!='U'
						&& content[i]!=' ' && content[i]!='\n' && content[i]!='.'&& content[i]!=',' )
			++counter;
	return counter;
}

int conta_caratteri(char * content){
	int counter=0;
	char * p;
	p=content;

	for(int i=0;i<strlen(content);++i)
		if( content[i]==' ' | content[i]=='\n' | content[i]==',' | content[i]=='.')
			++counter;
	return counter;
}

void * thread_function(void * arg) {
	int x;
	char * content = (char *) arg;


	for(int i=0;i<number_of_threads;++i){
		if((unsigned long int)pthread_self() == (unsigned long int)threads[i])
			if(i==0){
				x=conta_vocali(content);
				printf("Tread [%d]: numero vocali contante: %d\n",i,x);
			}else if(i==1){
				x=conta_consonanti(content);
				printf("Tread [%d]: numero consonanti contante: %d\n",i,x);
			}else if(i==2){
				x=conta_caratteri(content);
				printf("Tread [%d]: numero altri caratteri contanti: %d\n",i,x);
			}
	}




	printf("Aspetto...\n");


	int s = pthread_barrier_wait(&thread_barrier);

	printf("Arrivato!\n");

	pthread_exit(x);
}



#define CHECK_ERR(a,msg) {if ((a) == -1) { perror((msg)); exit(EXIT_FAILURE); } }
#define CHECK_ERR2(a,msg) {if ((a) != 0) { perror((msg)); exit(EXIT_FAILURE); } }

int main() {
	char * content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Mattis rhoncus urna neque viverra justo nec ultrices. Pretium quam vulputate dignissim suspendisse in est ante. Vitae congue mauris rhoncus aenean. Blandit cursus risus at ultrices mi. Ut lectus arcu bibendum at varius vel pharetra vel. Etiam non quam lacus suspendisse faucibus interdum posuere. Eget sit amet tellus cras adipiscing enim eu turpis egestas. Lectus magna fringilla urna porttitor rhoncus dolor purus non. Sit amet consectetur adipiscing elit duis tristique sollicitudin nibh. Nec tincidunt praesent semper feugiat nibh. Sapien pellentesque habitant morbi tristique senectus et netus et malesuada.";

	int s;

	// https://linux.die.net/man/3/pthread_barrier_init
	s = pthread_barrier_init(&thread_barrier, NULL, N);
	CHECK_ERR(s,"pthread_barrier_init")

	for (int i=0; i < number_of_threads; i++) {
		s = pthread_create(&threads[i], NULL, thread_function, content);
		CHECK_ERR2(s,"pthread_create")
	}
	int res;
	int vocali;
	int consonanti;
	int caratteri;
	for (int i=0; i < number_of_threads; i++) {
		if(i==0){
			s = pthread_join(threads[i], &res);
			vocali=res;
			CHECK_ERR2(s,"pthread_join");
		}
		if(i==1){
			s = pthread_join(threads[i], &res);
			consonanti=res;
			CHECK_ERR2(s,"pthread_join");
		}
		if(i==2){
			s = pthread_join(threads[i], &res);
			caratteri=res;
			CHECK_ERR2(s,"pthread_join");
		}
	}

	printf("Stampo ciò che ho ricevuto: vocali = %d, consonanti= %d, caratteri= %d",vocali,consonanti,caratteri);

	// https://linux.die.net/man/3/pthread_barrier_init
	s = pthread_barrier_destroy(&thread_barrier);
	CHECK_ERR(s,"pthread_barrier_destroy")

	printf("bye\n");

	return 0;
}
