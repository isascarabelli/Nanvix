# Testando as estratégias de escalonamento


## 1. Funções usadas para os testes

Funções implementadas em src/sbin/test/test.c

> #sched_test0
Função usada para testar a chamada de sistema wait(), gerando dois processos,
um processo pai e um filho. Retornando 0, caso for aprovado no teste,
ou algo diferente de 0 caso contrário e o seu tempo de execução;

> #sched_test1
Função usada para o teste do problema de inversão de prioridade,
ajustando as prioridades dos processos e executando trabalho intensivo em processos
diferentes, a fim de verificar como o escalanador lida com as prioridades dinâmicas.
Retornando 0, caso for aprovado no teste, ou algo diferente de 0
caso contrário e o seu tempo de execução;

> #sched_test2
Função usada para criar vários processos que estressam o escalonador,
alguns executando e outros em pausa, a fim de verificar como o escalonador lida
com uma carga variada de processos. Retornando 0, caso for aprovado no teste,
ou algo diferente de 0 caso contrário e o seu tempo de execução;

> #sched_test3
Função usada para criar vários processos, forçando o sistema estressar o escalonador,
a fim de verificar como o escalonador lida com uma carga elevada de processos.
Retornando 0, caso for aprovado no teste, ou algo diferente de 0
caso contrário e o seu tempo de execução;

## 2. Algoritmo fila de prioridades

Implementação de duas formas para representação da fila de prioridades.

>Para os dois códigos utilizamos como prioridade efetiva a soma da prioridade aplicada pelo próprio sistema operacional através
do atributo 'priority' no bloco de controle de processo (prioridade base estática), mais a prioridade escolhida pelo usuário
(atributo 'nice' do bloco de controle de processo também, alterada através da chamada de sistema nice()), junto com o
tempo de espera daquele processo na fila (prioridade dinâmica). Para a prioridade dinâmica, incrementamos o atributo
'epriority' a cada vez que incrementamos o atributo 'counter'.

>Com a criação do atributo 'epriority', tivemos que alterar o bloco de controle de processos assim como a inicialização desse
atributo do processo 'IDLE' e de um processo 'x' criado pela chamada de sistemas fork().

>O primeiro processo a ser executado recebe um quantum (PROC_QUANTUM) e após o término desse quantum, ele volta para
'yields' para ver se chegou algum processo com maior prioridade. Caso sim, esse segundo processo é escalonado, caso não,
o mesmo processo executa por mais um quantum.

>A diferença entre a primeira e a segunda representação está na comparação entre as prioridades. Na primeira representação,
comparamos prioridades maiores e igual a prioridade do processo atual. Processos com a mesma prioridade revezam entre si
até chegar um processo com maior prioridade. Na segunda representação, consideramos processos apenas com prioridade maior.
Caso as prioridades sejam a mesma, aquele processo vai ficar mais um ciclo na fila e terá sua prioridade incrementada.
Dessa forma, no próximo quantum aquela prioridade será maior que a prioridade que está sendo executada.

## 3. Teste no escalonador Round Robin

>ROUND-ROBIN natural do Nanvix

![WhatsApp Image 2023-10-07 at 11 39 07](https://github.com/isascarabelli/Nanvix/assets/73960096/6869f8fb-7058-4118-be27-5d5ab5c3e1b1)

![WhatsApp Image 2023-10-07 at 11 51 30](https://github.com/isascarabelli/Nanvix/assets/73960096/5ccc87a0-d86b-460b-bac9-7efc56b420c3)

## 4. Teste no escalonador Fila de Prioridades

>Fila de prioridades 1 - padrão

![WhatsApp Image 2023-10-07 at 11 32 45](https://github.com/isascarabelli/Nanvix/assets/73960096/635f0220-91b0-4a7c-af4f-2612bbcdb07f)

![WhatsApp Image 2023-10-07 at 11 47 11](https://github.com/isascarabelli/Nanvix/assets/73960096/6702e378-7f74-4430-852b-96f0d515095f)


- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

>Fila de prioridades 2

![WhatsApp Image 2023-10-07 at 11 37 17](https://github.com/isascarabelli/Nanvix/assets/73960096/502336bd-7d56-4ea0-89f2-7b0ba8ab82eb)


![WhatsApp Image 2023-10-07 at 11 49 25](https://github.com/isascarabelli/Nanvix/assets/73960096/bea6c1dc-daf7-4378-aa5a-cb592b75b2a7)


## 5. Comparação entre os escalonadores

Para este trabalho, resolvemos usar de 2 algoritmos de fila de prioridades mais o algoritmo padrão do nanvix para fim de testes sobre qual dos algoritmos é mais eficiente.

A prioridade dinâmica do algoritmo padrão do nanvix foi satisfatória nos testes de prioridade dinâmica, com um tempo razoável. Porém nos testes de estresse do escalonador o algoritmo se mostrou eficiente, com um tempo baixo em todos os testes.

Para a fila de prioridades 1 (padrão) obteve resultados semelhantes ao do padrão do nanvix no quesito de estresse do escalonador, sendo mais eficiente quando se trata de processos com prioridade dinâmica, o que indica que o algoritmo para as prioridades mostrou ganhos de performance, porém não muito significativos.

Para a fila de prioridades 2, foi o algoritmo com o pior desempenho, tendo sido notável a diferença para os dois anteriores quando se trata de prioridade dinâmica e sendo ligeiramente menos eficiente no estresse de escalonador.

## 6. Conclusões 

> Levando em conta os testes feitos...

Em síntese, os resultados obtidos neste trabalho revelam que, embora o algoritmo padrão do nanvix tenha demonstrado uma performance satisfatória em testes de prioridade dinâmica e eficiência notável em situações de estresse do escalonador, a implementação da fila de prioridades 1 revelou ganhos expressivos, especialmente em processos com prioridade dinâmica. No entanto, a fila de prioridades 2 apresentou o pior desempenho, evidenciando uma clara disparidade em relação aos outros dois algoritmos, tanto em termos de prioridade dinâmica quanto no contexto de estresse do escalonador. Logo, foi possível concluir que o algoritmo de fila de prioridades 1 foi o mais eficiente entre todos, sendo que o seu propósito de dar maior importância para as prioridades dos processos foi cumprido quando comparados aos outros 2 cenários.

