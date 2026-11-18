# Protocolo-Caramelo
> Em meio ao desespero da vida pessoal e financeira de um mundo em decadência, Pedro, um pacato cidadão da civilização, reencontra não só um velho amigo, mas também uma forma de recomeçar a viver a vida. Acompanhe-o, guie-o e tenha fé pela sua jornada, enquanto administra a sua renda de *caramelos* e relações humanas.
## Sumário
<p align="center">
  <a href="">1 ……………………………………………………… Introdução</a>
  <br>
  <!-- Execução-->
  <a href="">2 ………………………………………………………… Execução</a>
  <br>
  <a href="">2.1 ……………………………………………………… Exe. Web</a>
  <br>
  <a href="">2.2 ………………………………………………………… Exe. P-C</a>
  <br>
  <!-- Ficha técnica -->
  <a href="">3 …………………………………………………… Ficha técnica</a>
  <br>
  <a href="">3.1 …………………………………………………… Arquitetura</a>
  <br>
  <a href="">3.1.1 …………………………………………………… Arq. Web</a>
  <br>
  <a href="">3.1.2 ……………………………………………………… Arq. P-C</a>
  <br>
  <!-- Mecânicas -->
  <a href="">4 ………………………………………………………… Mecânicas</a>
  <br>
  <a href="">4.1 ……………………………………………………………… Saúde</a>
  <br>
  <a href="">4.2 …………………………………………………………… Energia</a>
  <br>
  <a href="">4.3 ………………………………………………………………… Sono</a>
  <br>
  <a href="">4.4 ………………………………………………………… Caramelos</a>
  <br>
  <a href="">4.5 ……………………………………………………………… Tempo</a>
  <br>
  <a href="">4.6 …………………………………… Problemas Matemáticos</a>
  <br>
  <a href="">4.7 ……………………………………………………………… Dívidas</a>
  <br>
  <a href="">4.7 …………………………………………………………… Dialógos</a>
  <br>
  <!--Sobre nós -->
  <a href="">5 …………………………………………………………… Sobre nós</a>
</p>

## 1 Introdução
Protocolo-Caramelo é um jogo _Point&Click_ sobre mineração de criptomoedas, no qual você controla Pedro e administra tanto o dinheiro quanto suas relações pessoais para sobreviver até o final do jogo. O principal objetivo do jogo é fornecer uma experiência imersiva e introspectiva sobre um indivíduo comum que vive e experiencia uma sociedade em profunda degeneração e autodestruição.

Para minerar criptomoedas, você deve resolver problemas matemáticos. Embora pareça, à primeira vista, chato e monótono resolver essa categoria de problemas, um dos pilares do jogo é demonstrar que, se feito da forma certa para o público-alvo certo, é possível disseminar conhecimento de qualquer natureza. A lógica dos problemas matemáticos estão explicados no documento da história do jogo, entretanto, desejamos e esperamos que você jogue o jogo primeiro antes de ler qualquer material dele(por favor, não tome spoilers!)

Por último, mas não menos importante, o Protocolo-Caramelo pretende ser um jogo leve e portátil a qualquer dispositivo, dado que está sendo desenvolvido na linguagem _C_ usando da biblioteca gráfica _**SDL**_.

## 2 Execução
Para compilar o jogo em sua máquina local usando os métoods listados abaixo, garanta que o seu sistema operacional seja Windows 11 com a última versão estável do WSL ou Linux com Wayland.

### 2.1 Execução Web
Para executar o site do jogo localmente em sua máquina, você deve, após a clonagem do repositório, acessar o diretório <a href="https://github.com/Protocolo-Caramelo/Protocolo-Caramelo/tree/web/web">./web</a> e executar ./run.sh em qualquer terminal que suporte a linguagem shell, tal como _Bash_, _Powershell_, _Zh_ etc.
```bash
./run.sh
```

Logo após, execute o comando 
```bash
./mvnw spring-boot:run
```
E... pronto, você terá o site do jogando localmente em seu computador!

### 2.1 Execução Protocolo-Caramelo
Caso deseje contribuir para a grande família Caramelo, clone este repositório em sua máquina local por meio do git clone e acesse a _branch_ game_v2 e o diretório game. Após isso, execute o comando ./run.sh em qualquer terminal que suporte a linguagem bash.

Após isso, caso queria uma build jogável do jogo, sem opções de debug, execute o seguinte comando:
```bash
# Para ambientes Linux(incluso o container usando para realizar essa build)
cmake --preset build
cmake --build __build__
./__Protocolo-Caramelo__

# Para ambientes windows(11)
cmake --preset windows
cmake --build __build_windows__
./__Protocolo-Caramelo_windows__
```

Caso queira ativar as opções de desenvolvedor, siga esses seguintes comandos:
```bash
# Linux
cmake --preset devquote
cmake --build __build_devquote__
./__Protocolo-Caramelo_devquote__

# Windows(11)
cmake --preset windows_devquote
cmake --build __build_windows_devquote__
./__Protocolo-Caramelo_windows_devquote__
```

## 3 Ficha Técnica
### 3.1 Arquitetura Web
### 3.2 Arquitetura Protocolo-Caramelo
### 3.2.1 UML
#### 3.2.1.1 Caso de Uso
<img width="3126" height="1804" alt="use case diagram" src="https://github.com/user-attachments/assets/d4e7467e-6322-4a91-b111-b0ad34fe6aad" />

#### 3.2.1.2 Diagrama de Classes
##### _Graphical_
<img width="3011" height="1706" alt="graphical" src="https://github.com/user-attachments/assets/c37dd9dd-e129-4619-af98-91f6158044d0" />
<hr>

#### _Protocolo-Caramelo_
<img width="2014" height="1707" alt="ProtocoloCaramelo" src="https://github.com/user-attachments/assets/f0f60468-76ff-4a71-b9f5-1343a46a7050" />

## 4 Mecânicas
### 4.1 Saúde
<img height="240" alt="ConceptBoost_Medkit" src="https://github.com/user-attachments/assets/9c54e48b-973b-4e89-a3c2-3b702550e190" />

A saúde é uma métrica da qualidade de sono do personagem e o recurso mais vital presente jogo. Ele irá definir se o jogador deve  perder o jogo ou se ele continuar.

A cada vez que o jogador deixa de dormir ou consome energéticos, ele perde uma fração da sua saúde. Ela é recuperada minimamente 
 a barra de sono está preenchida ou no uso do medkit.

O medkit pode ser adquirido na loja virtual “entrega rápidas”

### 4.2 Energia
<img width="240" alt="ConceptBoost_Arcus" src="https://github.com/user-attachments/assets/fda99efa-d8c8-482a-84f8-a475e2caadd1" />

A energia é adquirida ao dormir ou ao ingerir um energético/café. Ela indica a disposição do personagem. 

É importante notar a diferença entre energia e sono: caso a energia esteja baixa, a tela do jogador irá começar a escurecer e os problemas matemáticos serão mais difíceis; já o sono pode recuperar a saúde caso ela esteja completa ou diminuir caso contrário.

### 4.3 Sono
<img width="240" alt="ConceptBoost_Pílula" src="https://github.com/user-attachments/assets/5c6d15b4-7300-4a1d-9fec-78597eb138a2" />


O descanso é vital para o bem-estar, desenvolvimento e saúde humana, tanto no jogo quanto na vida real. Assim, caso o personagem não durma corretamente, ele começará a perder saúde.

O sono é reduzido apenas quando o personagem descansa, e é reduzido exponencialmente quanto mais horas o jogador dorme continuamente.

O sonífero pode ser utilizado para aumentar qualidade de sono e, portanto, o quanto ele é reduzido.

### 4.4 Caramelos
<img width="240" alt="Caramelos" src="https://github.com/user-attachments/assets/86d85804-249b-4838-b580-9e24d802c9a8" />


O Caramelo é a moeda do jogo, ou seja, é a o recurso que o jogador vai receber sempre que ele finalizar uma sessão de mineração.

O caramelo é utilizado principalmente para pagar as contas e comprar os itens disponíveis na loja do jogo.

O jogo também pode apostar seus caramelos com o Personagem Ricardo caso esteja se sentindo com sorte.

### 4.5 Tempo

### 4.6 Problemas Matemáticos
_Gerado com o auxílio da I.A(Claude)_


### 4.7 Dívidas
_Gerado com o auxílio da I.A(GPT)_

### 4.8 Diálogos
_Gerado com o auxílio da I.A(Claude)_

## 5 Sobre nós
<table>
  <thead>
    <tr>
      <th>CastCris (Desenvolvedor)</th>
      <th>DiogoBSousa (Artista)</th>
      <th>JeanCFreitas (Artista)</th>
      <th>JVVS(Diretor Criativo && Dev. Web)</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td align="center">
        <img src="https://avatars.githubusercontent.com/u/161128558?v=4" height="100">
      </td>
      <td align="center">
        <img src="https://avatars.githubusercontent.com/u/163172524?v=4" height="100">
      </td>
      <td align="center">
        <img src="https://avatars.githubusercontent.com/u/163172637?v=4" height="100">
      </td>
      <td align="center">
        <img src="https://avatars.githubusercontent.com/u/163188697?v=4" height="100">
      </td>
    </tr>
    <tr>
      <td valign="top"><em>Líder</em> do grupo, sendo o responsável pela modelagem e articulação do código fonte do jogo. Além disso, desenvolveu, junto ao JVVS, o bruto da parte criativa, a exemplo do tema, história, personagens a ambientação do Protocolo-Caramelo.</td>
      <td valign="top">Artista responsável pela criação dos elementos gráficos e imagens do jogo, além de contribuir com ideias para o desenvolvimento da história, ambientação e identidade visual do Protocolo-Caramelo.</td>
      <td valign="top">Artista do grupo, responsável pelas artes conceituais do trabalho e pelo lado mais artístico. Desenvolveu o design do quarto e dos personagens.</td>
      <td valign="top">Diretor criativo do jogo, sendo responsável pela criação e desenvolvimento da história do jogo e seus personagens. Além disso, é responsável pela criação de efeitos sonoros e músicas ambientes dentro do jogo. Por fim, auxilia DiogoBSousa e JeanCFreitas no desenvolvimento dos personagens e cenários dentro do jogo.</td>
    </tr>
  </tbody>
</table>
