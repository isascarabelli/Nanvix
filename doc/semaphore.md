# Problema Produtor-Consumidor com Semáforos

## Visão Geral

Este programa demonstra o clássico problema produtor-consumidor usando semáforos em um cenário com um buffer de tamanho 32. O problema produtor-consumidor envolve dois processos, um produtor e um consumidor, que compartilham um buffer comum de tamanho fixo. A tarefa do produtor é produzir dados e colocá-los no buffer, enquanto a tarefa do consumidor é consumir os dados do buffer.

## Inicialização dos Semáforos

A função começa criando um arquivo de buffer e inicializando três semáforos: `mutex` (para exclusão mútua), `empty` (para rastrear posições vazias no buffer) e `full` (para rastrear posições cheias no buffer). O arquivo do buffer é aberto usando `open()` e os semáforos são criados e inicializados usando macros personalizadas (`SEM_CREATE` e `SEM_INIT`).

## Fork dos Processos

Após a inicialização dos semáforos, a função faz um fork de dois processos: um para o leitor e outro para o escritor. O processo leitor lê do buffer, e o processo escritor escreve no buffer. Ambos os processos executam suas respectivas tarefas em um loop, repetindo 10 vezes.

## Processo Leitor

O processo leitor espera por uma posição vazia no buffer (`SEM_DOWN(empty)`), trava o buffer (`SEM_DOWN(mutex)`), lê um item do buffer, destrava o buffer (`SEM_UP(mutex)`) e sinaliza que uma posição cheia está disponível (`SEM_UP(full)`).

## Processo Escritor

O processo escritor espera pelo buffer não estar vazio (`SEM_DOWN(full)`), trava o buffer (`SEM_DOWN(mutex)`), escreve um item no buffer, destrava o buffer (`SEM_UP(mutex)`) e sinaliza que uma posição vazia está disponível (`SEM_UP(empty)`).

## Limpeza

Após a conclusão dos processos, a função aguarda a saída de ambos os processos filhos, fecha o arquivo do buffer e exclui o arquivo do buffer.

## Conclusão 

A função retorna 0 se as operações dos semáforos funcionarem corretamente e diferente de zero caso contrário. Ela verifica o resultado de `SEM_POST(mutex)` para determinar o sucesso.

## Exemplos 
