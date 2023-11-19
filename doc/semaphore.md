# Chamadas de biblioteca no espaço de usuário 

Chamadas de sistemas implementadas em ubin/nomedocomando/nomedocomando.c

- Semget: chama semget (para passar para o espaço em kernel) e faz a verificação dos parâmetros: valor tem que estar na faixa de valores dos ids para semáforos que foram disponibilizados, entre 0 e SEM_MAX.

  ![Captura de tela 2023-11-19 145737](https://github.com/isascarabelli/Nanvix/assets/73960096/81ddbd0b-c2db-4d8e-8a9e-acd81e51844b)
  
- Semop: chama semop (para passar para o espaço em kernel) e faz a verificação dos parâmetros: semid tem que estar na faixa de valores dos semáforos, e op pode ser qualquer valor positivo ou negativo, diferente de 0.

  ![Captura de tela 2023-11-19 144711](https://github.com/isascarabelli/Nanvix/assets/73960096/f0fc0a9c-354b-462d-8dfa-58da1ffc07dd)
  
- Semctl: chama semctl (para passar para o espaço em kernel) e faz a verificação dos parâmetros: semid tem que estar na faixa de valores dos semáforos; cmd precisa ser 0, 1 ou 3; val pode ser qualquer valor maior que 0.

  ![Captura de tela 2023-11-19 145646](https://github.com/isascarabelli/Nanvix/assets/73960096/b1b7a2ee-99bc-470b-831d-0dc1b44c6109)
  ![Captura de tela 2023-11-19 145531](https://github.com/isascarabelli/Nanvix/assets/73960096/e9095a7b-0ab8-44d0-a209-b57d1460bd98)
  ![Captura de tela 2023-11-19 145507](https://github.com/isascarabelli/Nanvix/assets/73960096/f15ea98d-51f3-4457-9421-982e03754c0f)


## Chamadas de sistema da implementação do semáforo

### O semáforo
include/sys/sem.h

A estrutura do semáforo está presente no arquivo `sem.h`, que é responsável por conter tudo o que o semáforo precisa. Para isso, foi criado a seguinte `struct semaphore`:

```c
struct semaphore
	{
		int id;			/**< Semaphore ID. */
		int val;		/**< Top value of semaphore. */
		int curr_val;		/**< Current value of semaphore. */
		int flag;		/**< Set a state for semaphore. */

	};
```
Nesse mesmo arquivo, foi criado a tabela de semáforos, que mantém todos os semáforos e é definida por uma valor máximo de semáforos, nesse trabalho definido como 10, mas sendo esse valor alternado em `SEM_MAX`.

A variável `flag` é responsavel por salvar se o estado atual do semáforo é bloqueado ou desbloqueado, definido como `LOCKED` e `UNLOCKED`.

Além disso, foi também criado um arquivo `sem.c` que apenas constroi essa tabela de páginas.

Todos os semáforos são criados com o valor 1, caracterizando um `mutex`. Para alterar, será necessário utilizar a chamada de sistema `semctl.c`.

## Chamadas de sistema
src/kernel/sys/sem

### Semget

A chamada de sistema `semget.c` recebe um inteiro como chave e procura na tabela de semáforos por essa chave. Se é encontrado, retorna o valor da mesma. Se não, um novo semáforo é criado com aquela chave e que foi criado é salvo em `semtab`.

```c
semtab[key].id = key;          /**< Creating semaphore. */
semtab[key].val = SETVAL;      /**< Set semaphore with max value (mutex). */
semtab[key].curr_val = SETVAL; /**< Semaphore with current value 1 (mutex). */
```

### Semctl

A chamada de sistema `semctl.c` é responsável pelo controle do semáforo, recebendo o id do semáforo, o que irá ser feito (`cmd`) e um valor. O que irá ser feito no semáforo é regulado pelos seguintes parâmetros especificados em `sem.h`:

```c
	#define GETVAL   0   	/**< Returns the current value of a semaphore.  */
	#define SETVAL   1   	/**< Sets the value of a semaphore.     */
	#define IPC_RMID 3   	/**< Destroys a semaphore.              */

```
Se `cmd == GETVAL`, o valor atual do semáforo é retornado.
O ultimo parâmetro é um valor, que só é utilizado caso `cmd == SETVAL`, o que muda o valor máximo do semáforo para o valor presente em `val`.
Caso `cmd == IPC_RMID`, o semáforo é deletado atribuindo seu id como `-1` e caso o usuário queira criar outro como o mesmo id, deverá chamar por `semget.c`.

### Semop

A chamada de sistema `semop.c` executa as operações de incremento e decremento do semáforo, realizadas pelas funções `up_sem()` e `down_sem()`.

A função `up_sem()` decrementa o semáforo se ele for maior que 0. Se não for, o processo atual deverá ser posto para dormir e o semáforo bloqueado, pois houve uma tentativa de decrementar um semáforo zerado. Ao acordar, o processo terá a prioridade definida por `PRIO_USER`.

```c
PUBLIC void down_sem(struct semaphore *sem, int semid){

    if (sem->curr_val > 0)
        semtab[semid].curr_val--;
    while (sem->curr_val == 0) {
        semtab[semid].flag = LOCKED;
        sleep(curr_proc->chain, PRIO_USER);
    }

}
```

A função `down_sem()` incrementa o semáforo apenas se o valor atual for menor que o valor máximo do semáforo. Também acorda o processo atual caso o estado atual do semáforo seja `LOCKED`.

```c
PUBLIC void up_sem(struct semaphore *sem, int semid){

    if (sem->flag == LOCKED){
        wakeup(curr_proc->chain);
        semtab[semid].flag = UNLOCKED;
        semtab[semid].curr_val++;
    } else if (sem->curr_val > 0 && sem->curr_val < sem->val)
        semtab[semid].curr_val++;

}
```

## Função `semaphore_test4`

A função `semaphore_test4`, implementada em src/sbin/test/test.c, implementa o problema de sincronização dos leitores e escritores usando semáforos para sincronização entre processos. 

### Etapas e Funcionalidades

1. **Criação de Semáforos:**
   - `mutex` e `bd` são criados para garantir a exclusão mútua e controlar a escrita no buffer, respectivamente.
   ```c
   SEM_CREATE(mutex, 1);
   SEM_CREATE(bd, 2);
   ```

2. **Inicialização de Semáforos:**
   - `bd` é inicializado com 1 para permitir a escrita inicial no buffer.
   - `mutex` é inicializado com 1 para garantir a exclusão mútua.
   ```c
   SEM_INIT(bd, 1);
   SEM_INIT(mutex, 1);
   ```

3. **Criação do Buffer Compartilhado:**
   - O arquivo "buffer" é criado ou aberto para leitura e escrita.
   ```c
   buffer_fd = open("buffer", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
   ```

4. **Criação do Processo Filho (Leitor):**
   - Utilizando `fork`, um novo processo (leitor) é criado.
   - O processo filho executa a função `leitor`.
   ```c
   if ((pid = fork()) < 0)
       return -1;
   else if (pid == 0)
       leitor(mutex, bd, buffer_fd);
   ```

5. **Processo Pai (Escritor):**
   - O processo pai (escritor) executa a função `escritor` e aguarda a conclusão do processo filho.
   ```c
   else {
       escritor(bd, buffer_fd);
       wait(NULL);
   }
   ```

6. **Destruição de Semáforos e Limpeza:**
   - Ao final, os semáforos são destruídos e o arquivo de buffer é fechado e removido.
   ```c
   SEM_DESTROY(mutex);
   SEM_DESTROY(bd);
   close(buffer_fd);
   unlink("buffer");
   ```

7. **Métodos Leitor e Escritor:**
   - As funções `leitor` e `escritor`, utilizadas na implementação da função `semaphore_test4`, desempenham papéis específicos no contexto do problema dos leitores e escritores, facilitando a leitura e escrita no buffer compartilhado entre processos.

	Função `leitor`

	A função `leitor` representa o comportamento de um processo leitor no contexto do problema dos leitores e escritores.

	```c
	void leitor(int mutex, int bd, int buffer_fd);
	```

	**Entradas:**
  		- `mutex`: Semáforo de exclusão mútua.
  		- `bd`: Semáforo de controle de escrita no buffer.
  		- `buffer_fd`: Descritor de arquivo do buffer compartilhado.

	- **Comportamento:**
  		1. Realiza a operação DOWN no semáforo `mutex` para obter exclusão mútua.
  		2. Incrementa o contador de leitores.
  		3. Se for o primeiro leitor, realiza a operação DOWN no semáforo `bd` para impedir escritas no buffer.
  		4. Libera o semáforo `mutex`.
  		5. Lê um item do buffer compartilhado.
  		6. Obtém novamente a exclusão mútua.
  		7. Decrementa o contador de leitores.
  		8. Se não houver mais leitores, realiza a operação UP no semáforo `bd` para permitir escritas no buffer.
  		9. Libera o semáforo `mutex`.

	### Função `escritor`

	A função `escritor` representa o comportamento de um processo escritor no contexto do problema dos leitores e escritores.

	```c
	void escritor(int bd, int buffer_fd);
	```

	- **Entradas:**
  		- `bd`: Semáforo de controle de escrita no buffer.
  		- `buffer_fd`: Descritor de arquivo do buffer compartilhado.

	- **Comportamento:**
  		1. Gera um novo item de dados.
  		2. Realiza a operação DOWN no semáforo `bd` para impedir leituras e escritas simultâneas no buffer.
  		3. Escreve o novo item no buffer compartilhado.
  		4. Libera o semáforo `bd` para permitir operações subsequentes.

	8. **Retorno:**
   		- Retorna 0 em caso de execução bem-sucedida.
		- Retorna -1 em caso de erro.
