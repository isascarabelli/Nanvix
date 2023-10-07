# Testando as estratégias de escalonamento


## 1. Funções usadas para os testes

>Funções implementadas em src/sbin/test/test.c

```bash
# sched_test0
Função usada para testar a chamada de sistema wait(), gerando dois processos,
um processo pai e um filho. Retornando 0, caso for aprovado no teste,
ou algo diferente de 0 caso contrário e o seu tempo de execução;

# sched_test1
Função usada para o teste do problema de inversão de prioridade,
ajustando as prioridades dos processos e executando trabalho intensivo em processos
diferentes, a fim de verificar como o escalanador lida com as prioridades dinâmicas.
Retornando 0, caso for aprovado no teste, ou algo diferente de 0
caso contrário e o seu tempo de execução;

# sched_test2
Função usada para criar vários processos que estressam o escalonador,
alguns executando e outros em pausa, a fim de verificar como o escalonador lida
com uma carga variada de processos. Retornando 0, caso for aprovado no teste,
ou algo diferente de 0 caso contrário e o seu tempo de execução;

# sched_test3
Função usada para criar vários processos, forçando o sistema estressar o escalonador,
a fim de verificar como o escalonador lida com uma carga elevada de processos.
Retornando 0, caso for aprovado no teste, ou algo diferente de 0
caso contrário e o seu tempo de execução;
```
## 2. Algoritmo fila de prioridades

> Implementação de duas formas para representação da fila de prioridades.

```bash
Para os dois códigos utilizamos como prioridade efetiva a soma da prioridade aplicada pelo próprio sistema operacional através
do atributo 'priority' no bloco de controle de processo (prioridade base estática), mais a prioridade escolhida pelo usuário
(atributo 'nice' do bloco de controle de processo também, alterada através da chamada de sistema nice()), junto com o
tempo de espera daquele processo na fila (prioridade dinâmica). Para a prioridade dinâmica, incrementamos o atributo
'epriority' a cada vez que incrementamos o atributo 'counter'.

Com a criação do atributo 'epriority', tivemos que alterar o bloco de controle de processos assim como a inicialização desse
atributo do processo 'IDLE' e de um processo 'x' criado pela chamada de sistemas fork().

O primeiro processo a ser executado recebe um quantum (PROC_QUANTUM) e após o término desse quantum, ele volta para
'yields' para ver se chegou algum processo com maior prioridade. Caso sim, esse segundo processo é escalonado, caso não,
o mesmo processo executa por mais um quantum.

A diferença entre a primeira e a segunda representação está na comparação entre as prioridades. Na primeira representação,
comparamos prioridades maiores e igual a prioridade do processo atual. Processos com a mesma prioridade revezam entre si
até chegar um processo com maior prioridade. Na segunda representação, consideramos processos apenas com prioridade maior.
Caso as prioridades sejam a mesma, aquele processo vai ficar mais um ciclo na fila e terá sua prioridade incrementada.
Dessa forma, no próximo quantum aquela prioridade será maior que a prioridade que está sendo executada.
```
## 3. Teste no escalonador Round Robin

```bash
## Tempo de Execução: 
# prints dos testes
imagens dos testes
```

## 4. Teste no escalonador Fila de Prioridades

```bash

Prints dos testes:

ROUND-ROBIN natural do Nanvix
(https://github.com/isascarabelli/Nanvix/assets/73960096/03c23cf5-9131-4079-9d54-0712934786f0)
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
Fila de prioridades 1 - padrão

![WhatsApp Image 2023-10-07 at 11 47 11](https://github.com/isascarabelli/Nanvix/assets/73960096/b8e6fe96-2c8f-45d2-b736-454f10e45aee)

![WhatsApp Image 2023-10-07 at 11 32 45](https://github.com/isascarabelli/Nanvix/assets/73960096/e42a177d-ba5f-4140-a787-4d9ffe31a2ba)

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
Fila de prioridades 2

![WhatsApp Image 2023-10-07 at 11 37 17](https://github.com/isascarabelli/Nanvix/assets/73960096/0f57661c-9c15-4ecb-81f6-6d40092555d6)

![WhatsApp Image 2023-10-07 at 11 49 25](https://github.com/isascarabelli/Nanvix/assets/73960096/46d26e06-ab97-4c77-8471-44ed793d3bb8)

```

## 5. Comparação entre os escalonadores

> Levando em conta os testes feit![WhatsApp Image 2023-10-07 at 11 39 07]
os, ......

## 6. Conclusões 

> Levando em conta os testes feitos, ......

