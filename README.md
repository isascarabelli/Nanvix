# Nanvix
💻 Integrantes: Isabela Ferreira Scarabelli, Lucas José de Freitas e Pedro Henrique de Almeida Santos.

📖 Disciplina: Sistemas Operacionais

🏫 Pontifícia Universidade Católica de Minas Gerais

O projeto consiste em um divisão em sprint's com alterações visando a aplicação dos conhecimentos teórico aprendidos em sala. 

Para a primeira Sprint o objetivo foi desenvolver uma chamada de sistema no sistema operacional Nanvix. Essa chamada de sistema visa implementar um bloco de controle de processos, onde as informações sobre cada processo sejam disponibilizadas para consulta através de métodos específicos. As informações acerca de um processo serão acessadas e copiadas para um buffer em espaço de usuário;

Resultado final da implementação da primeira Sprint:

![image](https://github.com/isascarabelli/Nanvix/assets/99020483/e693a51f-571b-47b0-b19f-e4265a2546c6)

Para a continuação do projeto, sendo a segunda sprint, o objetivo foi implementar uma nova estratégia de escalonamento de processos, a fila de prioridades. Continuando, modificamos 4 funções, as quais testam o escalonador implementado e, agora, verificam o tempo de execução, a fim de comparar as duas estratégias, o escalonador nativo do Nanvix (Round Robin), e o escalonador por Fila de Prioridades, além de implementar o novo escalonador;

Resultado da implementação após testes de perfomance do algoritmo padrão implementado pelo grupo utilizando fila de prioridades:

![WhatsApp Image 2023-10-07 at 11 32 45](https://github.com/isascarabelli/Nanvix/assets/73960096/829f4542-f41e-4954-a499-1ab29a2c719d)

