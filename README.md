encionadas, apesar de estarem mal implementadas por enquanto.

markdown
Copiar
# Projeto de Sistemas Operativos - Monitorização da Qualidade do Ar

Este projeto foi desenvolvido no âmbito da disciplina de **Sistemas Operativos**, com o objetivo de simular um sistema de **monitorização da qualidade do ar** em salas de aula. O sistema utiliza **programação concorrente** e **paralela em C**, explorando técnicas como **multiprocessamento**, **multithreading** e **comunicação inter-processos**.

## Objetivos do Projeto

### Fases Implementadas:

- **Fase 1 - Multiprocessamento e Comunicação entre Processos**  
  Utilização de **pipes** para comunicação entre processos e cálculo de médias dos dados dos sensores.

- **Fase 1E - Comunicação com Unix Domain Sockets**  
  Substituição de **pipes** por **Unix Domain Sockets** para comunicação eficiente entre processos.

- **Fase 2A - Multithreading e Memória Partilhada**  
  Uso de **threads** para processar dados simultaneamente e memória partilhada para armazenar os resultados.

- **Fase 2C - Sistema Produtor-Consumidor com Semáforos**  
  Implementação de um sistema **produtor-consumidor** usando **semáforos** para sincronizar threads.

### Fases Não Implementadas (A Realizar):

- **Fase 2B - Barra de Progresso com Threads**  
  A implementação da barra de progresso está atualmente **incorreta**. Embora tenha sido tentado implementá-la, o comportamento de atualização da barra de progresso não está a funcionar como esperado, saltando de 0% para 100%. A barra de progresso deveria atualizar a cada segundo, mas está a ser ignorada devido à rápida execução do sistema. Esta fase será revisitada após a avaliação do desempenho.

- **Fase 1D - Barra de Progresso**  
  Tal como na fase 2B, a barra de progresso foi implementada, mas está a **falhar** no seu comportamento. Está a ser atualizada de forma errada (saltando diretamente de 0% para 100%) devido ao tempo de execução ser muito curto e os semáforos e threads não estarem a sincronizar corretamente a atualização da barra. A correção desta parte também será feita após a avaliação com o professor.

### Fase 3 - Otimização de Desempenho (Extra) (A Realizar) 
Melhorias no desempenho do sistema, medição do tempo de execução e análise de gargalos.

## Estrutura do Projeto

### Ficheiros principais:

- **`main.c`**: Implementação com **pipes**.
- **`main_sockets.c`**: Implementação com **Unix Domain Sockets**.
- **`main_threads.c`**: Implementação com **multithreading**.
- **`process_sensor.c`**: Processamento dos dados dos sensores com **pipes**.
- **`process_sensor_sockets.c`**: Processamento com **Unix Domain Sockets**.
- **`process_sensor_threads.c`**: Processamento com **threads**.
- **`process_sensor_prodcons.c`**: Sistema **produtor-consumidor** com **semáforos**.

## Como Rodar o Projeto

### Passos Iniciais:

1. **Clone o repositório**:
   ```bash
   git clone https://github.com/teu_usuario/projeto_so.git
   cd projeto_so
Compilar e rodar o código: Escolha a versão que deseja testar:

Versão com Pipes (Fase 1A):

bash
Copiar
make projeto_so
./projeto_so ../data
Versão com Unix Domain Sockets (Fase 1E):

bash
Copiar
make projeto_so_sockets
./projeto_so_sockets ../data
Versão com Threads (Fase 2A):

bash
Copiar
make projeto_so_threads
./projeto_so_threads ../data
Versão Produtor-Consumidor com Semáforos (Fase 2C):

bash
Copiar
make projeto_so_prodcons
./projeto_so_prodcons
Relatório: O programa irá gerar um relatório com os dados processados. O relatório será gravado em relatorio.txt ou relatorio_socket.txt, dependendo da versão escolhida.

Medição de Desempenho
O tempo de execução total será exibido no terminal após o processamento:

bash
Copiar
Tempo total de execução: X.XX segundos
Como Contribuir
Faça um fork deste repositório.

Crie uma branch (git checkout -b feature/xyz).

Realize as alterações necessárias.

Faça um commit (git commit -m 'Adiciona nova funcionalidade').

Envie para o repositório remoto (git push origin feature/xyz).

Abra um pull request.

Licença
Este projeto está licenciado sob a Licença MIT. Veja o arquivo LICENSE para mais detalhes.

Agradecimentos
Professores: Pedro Sobral e João Viana, pela orientação e feedback.

GitHub: Para o controle de versões e integração contínua.