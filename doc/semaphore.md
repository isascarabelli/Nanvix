# Chamadas de biblioteca no espaço de usuário 
// Implementadas em ubin/nomedocomando/nomedocomando.c

- Semget: chama semget (para passar para o espaço em kernel) e faz a verificação dos parâmetros: valor tem que estar na faixa de valores dos ids para semáforos que foram disponibilizados, entre 0 e SEM_MAX.
  
- Semop: chama semop (para passar para o espaço em kernel) e faz a verificação dos parâmetros: semid tem que estar na faixa de valores dos semáforos, e op pode ser qualquer valor positivo ou negativo, diferente de 0.
  
- Semctl: chama semctl (para passar para o espaço em kernel) e faz a verificação dos parâmetros: semid tem que estar na faixa de valores dos semáforos; cmd precisa ser 0, 1 ou 3; val pode ser qualquer valor maior que 0.

## Função para Leitores e Escritores da `semaphore_test4`

### Descrição

A função `semaphore_test4` implementa um cenário simples de comunicação entre processos usando semáforos. Ela cria um buffer e dois processos: um leitor e um escritor. Os processos se comunicam lendo e escrevendo itens inteiros no buffer. Semáforos são utilizados para controlar o acesso ao buffer compartilhado e sincronizar as operações entre os processos.

### Retorno

- Retorna 0 em caso de execução bem-sucedida.
- Retorna -1 em caso de erro.

### Passos Principais

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
    // Processo Leitor
    // ...
} else {
    // Processo Escritor
    // ...
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
