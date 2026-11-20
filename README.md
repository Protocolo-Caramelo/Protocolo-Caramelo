# Protocolo-Caramelo
> Em meio ao desespero da vida pessoal e financeira de um mundo em decadência, Pedro, um pacato cidadão da civilização, reencontra não só um velho amigo, mas também uma forma de recomeçar a viver a vida. Acompanhe-o, guie-o e tenha fé pela sua jornada, enquanto administra a sua renda de *caramelos* e relações humanas.
## Sumário
<p align="center">
  <a href="">1 ……………………………………………………… Introdução</a>
  <br>
  <!-- -->
  <a href="">2 ………………………………………………………… Execução</a>
  <br>
  <a href="">2.1 ……………………………………………………… Exe. Web</a>
  <br>
  <a href="">2.2 ………………………………………………………… Exe. P-C</a>
  <br>
  <!-- -->
  <a href="">3 …………………………………………………… Ficha técnica</a>
  <br>
  <a href="">3.1 …………………………………………………… Arquitetura</a>
  <br>
  <a href="">3.1.1 …………………………………………………… Arq. Web</a>
  <br>
  <a href="">3.1.2 ……………………………………………………… Arq. P-C</a>
  <br>
  <!-- -->
  <a href="">4 ………………………………………………………… Sobre nós</a>
</p>

## 1 Introdução
Protocolo-Caramelo é um jogo _Point&Click_ sobre mineração de criptomoedas, no qual você controla Pedro e administra tanto o dinheiro quanto suas relações pessoais para sobreviver até o final do jogo. O principal objetivo do jogo é fornecer uma experiência imersiva e introspectiva sobre um indivíduo comum que vive e experiencia uma sociedade em profunda degeneração e autodestruição.

Para minerar criptomoedas, você deve resolver problemas matemáticos. Embora pareça, à primeira vista, chato e monótono resolver essa categoria de problemas, um dos pilares do jogo é demonstrar que, se feito da forma certa para o público-alvo certo, é possível disseminar conhecimento de qualquer natureza. A lógica dos problemas matemáticos estão explicados no documento da história do jogo, entretanto, desejamos e esperamos que você jogue o jogo primeiro antes de ler qualquer material dele(por favor, não tome spoilers!)

Por último, mas não menos importante, o Protocolo-Caramelo pretende ser um jogo leve e portátil a qualquer dispositivo, dado que está sendo desenvolvido na linguagem _C_ usando da biblioteca gráfica _**SDL**_.

## 2 Execução
### 2.1 Execução Web
Para executar o site do jogo localmente em sua máquina, você deve, após a clonagem do repositório, acessar o diretório <a href="https://github.com/Protocolo-Caramelo/Protocolo-Caramelo/tree/web/web">./web</a> e executar ./run.sh em qualquer terminal que suporte a linguagem shell, tal como _Bash_, _Powershell_, _Zh_ etc. Garanta que você tenha **docker-desktop** e **docker-compose** propriamente instalados em seu sistema.
```bash
./run.sh
```

Logo após, execute o comando 
```bash
./mvnw spring-boot:run
```
E... pronto, você terá o site do jogando localmente em seu computador!

### 2.1 Execução Protocolo-Caramelo

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

## 4 Sobre nós
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
