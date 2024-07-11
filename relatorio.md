# Universidade Federal do Rio de Janeiro
## Ciência da Computação

## RELATÓRIO DO TRABALHO DE PROGRAMAÇÃO DE COMPUTADORES II
### Registro de Estoque

### Rio de Janeiro
### 10 de julho de 2024

**ALUNOS:**

- Andrey Pereira - 123458697
- Matheus Lourenço - 123571916
- Rodrigo Reis - 123585711
- Sara Mendes - 120172103

## Sumário:

- A - Descrição resumida do problema
- B - Pseudocódigo do projeto
- C - Descrição dos casos de teste projetados
- D - Testes realizados e resultados obtidos
- E - Dificuldades encontradas
- F - Conclusão

---

## A. Descrição resumida do problema

Nosso projeto consiste em criar um programa que gerencia um estoque de produtos. Ele mantém uma lista encadeada de produtos, onde cada produto possui um ID, nome, quantidade e data de registro. A lista é inicializada a partir de um arquivo binário ("estoque.bin") que contém os produtos previamente salvos. O programa permite as seguintes operações:

- **Adicionar Produto:** Insere um novo produto na lista com um ID único e data atual.
- **Exibir Tamanho da Lista:** Mostra o número de produtos na lista.
- **Remover Produto:** Remove um produto da lista pelo ID.
- **Listar Produtos:** Exibe todos os produtos na lista, ordenados por data.
- **Buscar Produto:** Procura um produto pelo nome e exibe a quantidade em estoque.

As operações de inserção e remoção registram as ações em um arquivo de log ("log.txt") e atualizam o arquivo binário de estoque para persistência de dados. O programa também mantém e atualiza um ID global para assegurar IDs únicos para novos produtos.

## B. Pseudocódigo do projeto

### 1. Constantes e variáveis globais:
- **`Definir constantes:`**
  - verdadeiro = 1
  - falso = 0
- **`Definir variável global:`**
  - globalID = 0

### 2. Tipos de Dados:
- **Definindo tipos:**
  - DATA (dia, mês, hora, minuto, segundo)
  - PRODUTO (ID, nome, quantidade, data)
  - ELEMENTO (reg, prox)
  - LISTA (inicio)

### 3. Funções:

#### 3.1 `inicializarLista(lista)`
- abrir arquivo "estoque.bin" para leitura
- se o arquivo existe:
  - enquanto ler produto do arquivo:
    - criar novo elemento
    - atribuir produto para o elemento
    - adicionar o elemento no início da lista
  - fechar arquivo

#### 3.2 `tamanho(lista)`
- inicializar contador tam = 0
- para cada elemento na lista:
  - incrementar +1 em tam
- retornar tam

#### 3.3 `exibirLista(lista)`
- para cada um dos elementos na lista:
  - imprimir detalhes do produto

#### 3.4 `load_global_id()`
- abrir arquivo "global_id.txt" para leitura
- se o arquivo existe:
  - ler globalID do arquivo
  - fechar arquivo

#### 3.5 `save_global_id()`
- abrir arquivo "global_id.txt" para escrita
- escrever globalID no arquivo
- fechar arquivo

#### 3.6 `atualizaestoque(lista)`
- abrir arquivo "estoque.bin" para escrita
- para cada um dos elementos na lista:
  - escrever produto no arquivo
- fechar arquivo

#### 3.7 `Registra(produto)`
- abrir arquivo "log.txt" para anexar
- escrever registro de entrada do produto
- fechar arquivo

#### 3.8 `RegistraSaida(produto)`
- abrir arquivo "log.txt" para anexar
- escrever registro de saída do produto
- fechar arquivo

#### 3.9 i`nsere(lista, produto)`
- criar novo elemento
- atribuir ID único ao produto
- salvar globalID
- registrar entrada do produto
- adicionar produto ao início da lista
- ordenar a lista por data
- atualizar arquivo de estoque

#### 3.10 `exclui(lista, id)`
- encontrar elemento com o ID correspondente
- se elemento encontrado:
  - remover elemento da lista
  - registrar saída do produto
  - ordenar lista por data
  - atualizar arquivo de estoque
- senão:
  - imprimir "Item não encontrado!"

#### 3.11 `altera(lista, quantidade, produto)`
- percorre a lista de produtos:
  - se o nome do produto coincidir com algum nome do produto da lista:
    - Adiciona quantidade a quantidade do produto da lista em questão
    - Atualiza a dada do produto da lista em questão
- Atualiza estoque

#### 3.12 `estaNoEstoque(lista, produto)`
- percorre a lista de produtos:
  - se o nome do produto coincidir com algum nome do produto da lista:
    - retornar verdadeiro
- retornar falso

#### 3.13 `reinicializarLista(lista)`
- para cada elemento na lista:
  - liberar memória do elemento

#### 3.14 `buscaSeqPorNome(lista, nome)`
- para cada elemento na lista:
  - se nome do produto coincide:
    - imprimir detalhes do produto
    - retornar verdadeiro
- imprimir "Produto não encontrado!"
- retornar falso

#### 3.15 `comparacaoDatas(data1, data2)`
- compara data1 e data2
- retornar -1, 0 ou 1 baseado na comparação

#### 3.16 `bubbleSortPorData(lista)`
- se lista vazia:
  - retornar
- repetir até que não haja trocas:
  - para cada par de elementos consecutivos na lista:
    - se data do primeiro for menor que a do segundo:
      - trocar elementos

#### 3.17 `ehNumero(string)`
- para cada caracter da string:
  - se não é um digito:
    - retornar false
- retornar verdadeiro

#### 3.18 `menu()`
- imprimir as opções do menu

### 4. Main
- Inicializa variável listaProdutos tipo LISTA
- Inicializa a lista
- Inicializa a variável opcao
- Abre o menu

Se o usuário escolher:
1. Adicionar Produto
	* Ler nome e quantidade do produto
	* preencher data atual no produto
	* inserir ou alterar produto na lista
2. Imprimir tamanho da lista
3. Remover Produto
	* Ler id do produto
	* remover produto da lista pelo `ID`
4. Exibir lista de produtos
5. Buscar Produto
	* Ler nome do produto
	* buscar produto pelo nome
6. Sair do programa

## C. Descrição dos casos de teste projetados

### Teste adicionar produto
Ao selecionar a opção 1 (adicionar produto) o usuário deverá digitar o nome do produto e sua quantidade. Após isso o item deverá ser adicionado à lista que está ordenada por data de adição junto com o seu ID próprio. Se o produto (nome do produto) estiver no estoque, deverá aparecer um campo para adicionar quantidades ao produto. Desse modo, é alterado a quantidade e data do produto em questão.

### Teste remover produto
Ao selecionar a opção 2 (remover produto) o usuário deverá digitar o ID do produto a ser removido. Após isso o item será removido da lista. Se o estoque estiver vazio, deverá aparecer "Lista Vazia".

### Teste tamanho da lista
Ao selecionar a opção 3 (tamanho da lista) deverá aparecer a quantidade de produtos no estoque (não a quantidade total). Se o estoque estiver vazio deverá aparecer que "existem 0 produtos em estoque".

### Teste listar produtos
Ao selecionar a opção 4 (listar produtos) deverão ser listados todos os produtos do estoque ordenados pela data de adição na lista, junto ao seu ID, sua quantidade, e sua data e hora de adição. Se o estoque estiver vazio, não aparece nada.

### Teste buscar produtos
Ao selecionar a opção 5 (buscar produto) o usuário deverá digitar o nome do produto a ser buscado. Após isso deverá ser mostrado o produto e sua quantidade. Se o produto não for encontrado, deverá aparecer que o "produto não foi encontrado no estoque".

### Teste sair do programa
Ao selecionar a opção 6 (sair do programa) o programa deverá ser finalizado e retornar 3 arquivos:
- Um arquivo binário "estoque.bin" onde estarão todos os produtos em estoque.
- Um arquivo "global_id.txt" onde terá o contador do ID, que a cada produto adicionado no estoque adicionará um ao ID, fazendo com que nunca tenha produtos de mesmo ID.
- Um arquivo "log.txt" onde estarão todas as movimentações de adição e exclusão do estoque.

## D. Testes realizados e resultados obtidos

Durante os testes de adição de produtos, obtivemos resultados satisfatórios. Os produtos foram adicionados à lista ordenados por data de adição e com IDs únicos. Nos testes de remoção, também obtivemos os resultados esperados: os produtos foram removidos da lista, mantendo-a ordenada e descartando o ID correspondente. Quando o estoque estava vazio, o programa retornou a mensagem "Lista vazia - sem elementos para remover".

Nos testes para retornar o tamanho da lista, os resultados foram positivos. O programa retornou o tamanho correto da lista, inclusive quando estava vazia (ou seja, 0). Nos testes de listagem de produtos, os resultados também foram positivos. Os produtos foram listados em ordem de adição, juntamente com seus IDs, quantidades e datas e horas de adição.

Nos testes de busca, o programa respondeu corretamente. Quando digitado o nome de um produto presente no estoque, retornou "produto encontrado" e a quantidade disponível. Quando o produto não estava no estoque, retornou "produto não encontrado". Quando o estoque estava vazio, a mensagem foi "sem elementos para buscar".

Nos testes de encerramento do programa, os resultados também foram positivos. O programa salvou corretamente os arquivos de estoque, ID e log.

## E. Dificuldades encontradas

A maior dificuldade que enfrentamos ao escrever o código foi na forma de implementação do ID de cada produto, fazendo com que eles fossem únicos. A solução que encontramos foi criar um arquivo txt que armazena um contador de IDs e é incrementado a cada produto adicionado, de modo que o ID sempre será único.

## F. Conclusão

O desenvolvimento deste programa de gerenciamento de estoque tem como objetivo proporcionar uma solução eficaz para a administração de produtos. Através de uma lista encadeada, o programa permite a inserção, remoção, exibição e busca de produtos de forma eficiente e organizada.

O programa oferece uma maneira prática e automatizada de gerenciar um estoque, reduzindo a possibilidade de erros humanos e aumentando a eficiência na gestão dos produtos. A manutenção de logs detalhados das operações de inserção e remoção proporciona um histórico de movimentações que pode ser útil para análises futuras.
