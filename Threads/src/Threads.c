#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N_THREADS 4 //numero das threads para definir n primos
#define MAX_NUM 5000000 //numero maximo verificado

int prox_num = 2; //verifica o prox num. sendo primo ou nao
int count = 0; //armazena numero de primos
pthread_mutex_t lock; //mutex para garantir a integridade das variaveis


//funcao que verifica os num. primos.
void *checa_primo(void *arg) {
	int num, e_primo;

	while(1) {
		pthread_mutex_lock(&lock); //acesso exclusivo da variavel
		num = prox_num++; //acha o prox numero e incrementa
		pthread_mutex_unlock(&lock); //acesso exclusivo a variavel count

		if (num > MAX_NUM) { //todos os num. sao verificados
			break;
		}

		e_primo = 1;
		for (int i = 2; i * i <= num; i++) {
			if (num % i == 0) {
				e_primo = 0;
				break;
			}
		}

		if (e_primo) {
			pthread_mutex_lock(&lock);
			count++; //incrementa os numero verificados primos aqui
			pthread_mutex_unlock(&lock);

		}
	}

	return NULL;
}

int main() {
	pthread_t threads[N_THREADS]; //array de threads

	pthread_mutex_init(&lock, NULL);

	//criacao das threads

	for (int i = 0; i < N_THREADS; i++) {
		pthread_create(&threads[i], NULL, checa_primo, NULL);
	}

	for (int i = 0; i < N_THREADS; i++) {
		pthread_join(threads[i], NULL);//main aguarda que as threads sejam concluidas
	}

	printf("Quantidade de numeros primos encontrados neste intervalo: %d \n", count);

	pthread_mutex_destroy(&lock); //destroi o mutex

	return 0;

}
