# Testando as estratégias de escalonamento


## 1. Funções usadas para os testes

```bash
# sched_test0
# Função usada para testar a chamada de sistema wait(), gerando dois processos,
# um processo pai e um filho. Retornando 0, caso for aprovado no teste,
# ou algo diferente de 0 caso contrário e o seu tempo de execução;
#
# sched_test1
# Função usada para o teste do problema de inversão de prioridade,
# ajustando as prioridades dos processos e executando trabalho intensivo em processos
# diferentes, a fim de verificar como o escalanador lida com as prioridades dinâmicas.
# Retornando 0, caso for aprovado no teste, ou algo diferente de 0
# caso contrário e o seu tempo de execução;
#
# sched_test2
# Função usada para criar vários processos que estressam o escalonador,
# alguns executando e outros em pausa, a fim de verificar como o escalonador lida
# com uma carga variada de processos. Retornando 0, caso for aprovado no teste,
# ou algo diferente de 0 caso contrário e o seu tempo de execução;
#
# sched_test3
# Função usada para criar vários processos, forçando o sistema estressar o escalonador,
# a fim de verificar como o escalonador lida com uma carga elevada de processos.
# Retornando 0, caso for aprovado no teste, ou algo diferente de 0
# caso contrário e o seu tempo de execução;
```

## 2. Teste no escalonador Round Robin

```bash
imagem dos testes
```

## 3. Teste no escalonador Fila de Prioridades

```bash
imagem dos testes
```

## 4. Comparação entre os escalonadores

```bash
imagem dos testes
```

## 5. Conclusões 

```bash
#
#
#
#
```

