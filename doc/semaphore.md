# Chamadas de biblioteca no espaço de usuário 
// Implementadas em ubin/nomedocomando/nomedocomando.c

- Semget: chama semget (para passar para o espaço em kernel) e faz a verificação dos parâmetros: valor tem que estar na faixa de valores dos ids para semáforos que foram disponibilizados, entre 0 e SEM_MAX.
  
- Semop: chama semop (para passar para o espaço em kernel) e faz a verificação dos parâmetros: semid tem que estar na faixa de valores dos semáforos, e op pode ser qualquer valor positivo ou negativo, diferente de 0.
  
- Semctl: chama semctl (para passar para o espaço em kernel) e faz a verificação dos parâmetros: semid tem que estar na faixa de valores dos semáforos; cmd precisa ser 0, 1 ou 3; val pode ser qualquer valor maior que 0.

## Chamadas de sistema da implementação do semáforo

## O semáforo
include/sys/sem.h

A estrutura do semáforo está presente no arquivo `sem.h`, que é responsável por conter tudo o que o semáforo precisa. Para isso, foi criado a seguinte `struct semaphore`:

```c
struct semaphore
	{
		int id;		/**< Semaphore ID. */
		unsigned val;		/**< Top value of semaphore. */
		unsigned curr_val;	/**< Current value of semaphore. */

	};
```
Nesse mesmo arquivo, foi criado a tabela de semáforos, que mantém todos os semáforos e é definida por uma valor máximo de semáforos, nesse trabalho definido como 10, mas sendo esse valor alternado em `SEM_MAX`.

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

A função `up_sem()` decrementa o semáforo se ele for maior que 0. Se não for, o processo atual deverá ser posto para dormir, pois tentou decrementar um semáforo zerado. Ao acordar, o processo terá a prioridade definida por `PRIO_USER`.

```c
PUBLIC void down_sem(struct semaphore *sem, int semid){

    if (sem->curr_val > 0)
        semtab[semid].curr_val--;
    else
        sleep(curr_proc->chain, PRIO_USER);

}
```

A função `down_sem()` incrementa o semáforo apenas se o valor atual for menor que o valor máximo do semáforo. Também acorda o processo atual caso o valor atual do semáforo seja 0.

```c
PUBLIC void up_sem(struct semaphore *sem, int semid){

    if (sem->curr_val == 0 && sem->curr_val < sem->val){
        wakeup(curr_proc->chain);
        semtab[semid].curr_val++;
    } else if (sem->curr_val > 0 && sem->curr_val < sem->val)
        semtab[semid].curr_val++;

}
```

## Função para Leitores e Escritores da `semaphore_test4`

A função `semaphore_test4`, implementada em src/sbin/test/test.c demonstra um cenário simples de comunicação entre processos usando semáforos. Ela cria um buffer e dois processos: um leitor e um escritor. Os processos se comunicam lendo e escrevendo itens inteiros no buffer. Semáforos são utilizados para controlar o acesso ao buffer compartilhado e sincronizar as operações entre os processos.

### Retorno

- Retorna 0 em caso de execução bem-sucedida.
- Retorna -1 em caso de erro.

### Debugger

1. **Criar Arquivo de Buffer:**
   - Abrir ou criar um arquivo chamado "buffer" com permissões de leitura e escrita.
   - Retornar -1 em caso de erro.

```c
buffer_fd = open("buffer", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
if (buffer_fd < 0)
    return (-1);
```

2. **Criar Semáforos:**
   - Criar três semáforos: `mutex`, `empty`, e `full`.

```c
SEM_CREATE(mutex, 1);
SEM_CREATE(empty, 2);
SEM_CREATE(full, 3);
```

3. **Inicializar Semáforos:**
   - Inicializar os semáforos: `full` é configurado para 0, `empty` é configurado para o tamanho do buffer, e `mutex` é configurado para 1.

```c
SEM_INIT(full, 0);
SEM_INIT(empty, BUFFER_SIZE);
SEM_INIT(mutex, 1);
```

4. **Criar Processo Filho:**
   - Criar um processo filho usando `fork`.
   - Retornar -1 em caso de erro.

```c
if ((pid = fork()) < 0)
    return (-1);
```

5. **Processos Leitor e Escritor:**
   - O processo pai representa o escritor, e o processo filho representa o leitor.
   - O leitor lê do buffer, e o escritor escreve no buffer.
   - Semáforos são usados para controlar o acesso ao buffer e sincronizar as operações.

```c
if (pid == 0) {
    //  Leitor
} else {
    //  Escritor
}
```

6. **Destruir Semáforos:**
   - Destruir os semáforos criados.

```c
SEM_DESTROY(mutex);
SEM_DESTROY(empty);
SEM_DESTROY(full);
```

7. **Fechar e Limpar:**
   - Fechar o arquivo de buffer e removê-lo.

```c
close(buffer_fd);
unlink("buffer");
```

8. **Retornar Sucesso:**
   - Retornar 0 para indicar execução bem-sucedida.

```c
return (0);
```

## Conclusão 



## Exemplos 
