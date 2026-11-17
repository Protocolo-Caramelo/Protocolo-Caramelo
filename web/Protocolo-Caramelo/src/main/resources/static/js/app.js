const app = document.getElementById("app");

function showHome() {
    app.innerHTML = 
    `<section id="Home">
        <div id="logo"> 
            <img src="img/cachorro.png" alt="dog">
        </div>
        <h1>Sinopse</h1>
        <div class="card">
            <p>
                Esse mundo se passa no ano de 2046, onde o planeta inteiro foi completamente dominado pelas megacorporações de I.A,
                a qual foi capaz de substituir grande parte dos empregos acadêmicos e artísticos existentes. O resultado disso foi catastrófico, 
                com essa nova tecnologia em mãos a humanidade perdeu o bom senso, milhares de pessoas de todos os setores ficaram desempregadas
                e o mundo se tornou frio.
                <br><br>
                Diante disso, Pedro Monteiro Rocha, um pacato cidadão da civilação que nasceu em uma área rural longe 
                dessas tecnologias se esforça para sobreviver quando sua vida chega no fundo do poço. Durante o caminho ele se reencontra com um 
                velho amigo que mostra para ele uma forma de recomeçar sua vida, tanto financeiramente quanto socialmente.
            </p>
        </div>

        <h1>O que é?</h1>
        <div class="card">
            <p>
                Protocolo-Caramelo é um jogo Point & Click sobre mineração de criptomoedas, no qual você controla Pedro e administra tanto
                o dinheiro quanto suas relações pessoais para sobreviver até o final do jogo. O principal objetivo do jogo é fornecer uma
                experiência imersiva e introspectiva sobre um indivíduo comum que vive e experiencia uma sociedade em profunda degeneração
                e autodestruição. 
                <br><br>
                Para minerar criptomoedas, você deve resolver problemas matemáticos. Embora pareça, à primeira vista, chato e monótono
                resolver essa categoria de problemas, um dos pilares do jogo é demonstrar que, se feito da forma certa para o público-alvo 
                certo, é possível disseminar conhecimento de qualquer natureza. A lógica dos problemas matemáticos estão explicados na aba 
                Mecânicas.
            </p>
        </div>

        <h1>Portabilidade</h1>
        <div class="card">
            <p>
                O Protocolo-Caramelo pretende ser um jogo leve e portátil a qualquer dispositivo, 
                dado que está sendo desenvolvido na linguagem C usando da biblioteca gráfica SDL.
            </p>
        </div>

        <button>Jogue agora</button>           
    </section>`;
}
function showMechanics() {
    app.innerHTML =
    `<section id="mechanics">
        <h2 style="color: #ff4d4d;">Saúde</h2>
        <div class="card" style="border-color: #ff4d4d;">
            <p class="card-intro" style="background-color: #ff4d4d;">
                A saúde é uma métrica da qualidade de sono do personagem e o recurso mais vital presente no jogo.
            </p>
            <div class="card-row">
                <div class="card-text">
                    <p>
                        Em uma partida a saúde do personagem será representada por uma barra vermelha localizada no canto
                        superior esquerdo da tela, durante o jogo o jogador deve prestar atenção e
                        regular suas ações para manter sua vida cheia.
                    </p>
                </div>
                <div class="card-image">
                    <img src="img/HpBar.webp" alt="saúde">
                </div>
            </div>
            <h3 style="color: #ff4d4d;">Condições</h3>
            <div class="card-extra" style="border-color: #ff4d4d;">
                <p>
                    Caso ela se esgote, o personagem será morto e a partida será finalizada. A cada vez que
                    o jogador deixa de dormir ele perde uma fração de sua saúde. Porém, ela é recuperada ligeiramente
                    quando o medidor de sono está cheio ou no uso de medkit.
                </p>
            </div>
        </div>

        <h2 style="color: #2ecc71;">Energia</h2>
        <div class="card" style="border-color: #2ecc71;">
            <p class="card-intro" style="background-color: #2ecc71;">
                A energia é a métrica da disposição do personagem durante o jogo.
            </p>
            <div class="card-row">
                <div class="card-text">
                    <p>
                        Em uma partida a energia do personagem será representada por uma barra verde localizada no canto
                        superior esquerdo da tela, durante o jogo cada ação que o jogador fizer consumirá uma parcela de
                        energia, como minerar, conversar ou comprar um item.
                    </p>
                </div>
                <div class="card-image">
                    <img src="img/EnergyBar.webp" alt="energia">
                </div>
            </div>
            <h3 style="color: #2ecc71;">Condições</h3>
            <div class="card-extra" style="border-color: #2ecc71;">
                <p>
                    Se a energia se esgotar, a tela ficará mais escura e os problemas matemáticos da mineração serão mais difíceis.
                    O jogador recupera uma parcela de sua energia toda vez que dorme ou quando consome itens como
                    café ou energéticos.
                </p>
            </div>
        </div>

        <h2 style="color: #c8c8c8;">Sono</h2>
        <div class="card" style="border-color: #c8c8c8;">
            <p class="card-intro" style="background-color: #c8c8c8; color: #111111;">
                O sono é a métrica do cansaço do personagem durante o jogo.
            </p>
            <div class="card-row">
                <div class="card-text">
                    <p>
                        Em uma partida o sono do personagem será representado por uma barra cinza com a imagem de um olho
                        localizada no canto superior esquerdo da tela, durante o jogo conforme o tempo for avançando o personagem
                        começará a ficar com sono.
                    </p>
                </div>
                <div class="card-image">
                    <img src="img/SleepBar.webp" alt="sono">
                </div>
            </div>
            <h3 style="color: #c8c8c8;">Condições</h3>
            <div class="card-extra" style="border-color: #8a9199;">
                <p>
                    Se o medidor esgotar, o personagem começará a perder saúde até sua morte.
                    O sono é reduzido apenas quando o personagem descansa, e é reduzido exponencialmente quanto mais
                    horas o jogador dorme continuamente. Além disso, ele pode usar um sonífero para aumentar a quantidade
                    recuperada por descanso.
                </p>
            </div>
        </div>

        <h2 style="color: #ff9f1c;">Caramelos</h2>
        <div class="card" style="border-color: #ff9f1c;">
            <p class="card-intro" style="background-color: #ff9f1c;">
                Os caramelos são a moeda do jogo que o jogador recebe após finalizar uma sessão de mineração.
            </p>
            <div class="card-row">
                <div class="card-text">
                    <p>
                        Em uma partida os caramelos serão representados por um símbolo de uma moeda localizada no
                        canto superior esquerdo da tela. Durante o jogo os caramelos serão utilizados principalmente
                        para pagar as contas e comprar os itens disponíveis na loja do jogo.
                    </p>
                </div>
                <div class="card-image">
                    <img src="img/caramel.webp" alt="caramelo">
                </div>
            </div>
        </div>

        <h1>Mineração</h1>
        <div class="card">
            <p class="card-intro">
                A mineração é realizada quando o jogador interage com o computador e em seguida clica no aplicativo
                "mineração".
            </p>
            <p>
                Quando uma sessão se inicia, uma nova tela se abre no computador contendo uma sequência de problemas
                matemáticos acompanhada de um timer que finaliza a sessão automaticamente quando se esgota.
                Entretanto, é importante lembrar que as sessões não são infinitas e após a finalização leva um tempo até
                que o jogador possa realizar uma nova tentativa.
            </p>
            <div class="card-image-M">
                <img src="img/quarto.png" alt="quarto">
                <img src="img/tela_pc.png" alt="PC">
            </div>
            <h3>Fim de uma sessão</h3>
            <div class="card-extra">
                <p>
                    Quando uma sessão se encerra, o jogador recebe uma quantidade variável de caramelos dependendo de vários
                    fatores, sendo eles: nível do problema, tempo para finalizar a sessão e quantidade de problemas resolvidos.
                </p>
            </div>
        </div>

        <h1>Problemas matemáticos</h1>
        <div class="card">
            <p class="card-intro">
                Os problemas matemáticos são a principal forma de desafio que o jogo oferece para os jogadores "quebrarem" a cabeça.
            </p>
            <p>
                Dentro de uma sessão, os problemas matemáticos dentro do jogo envolvem questões que trabalham com equações do 1º e 2º grau,
                binômios de Newton e funções lineares, quadráticas, exponenciais e logarítmicas. Nos problemas de função, o objetivo é encontrar
                o resultado para um valor arbitrário de X.
            </p>
            <h3>Níveis de dificuldade</h3>
            <div class="card-extra">
                <p>
                    Os problemas possuem quatro níveis distintos para dar versatilidade ao jogo.
                </p>
            </div>
        </div>

        <div id="levels-container">
            <div class="mini-card">
                <h3>Nível 1 - Fácil</h3>
                <p>
                    Contas simples, apenas com os quatro operadores básicos da matemática.
                    <br><br>
                    Frequência: 10%
                </p>
            </div>

            <div class="mini-card">
                <h3>Nível 2 - Médio</h3>
                <p>
                    Equações simples com as quatro operações básicas e funções afim. É o nível mais comum do jogo.
                    <br>
                    Frequência: 70%
                </p>
            </div>

            <div class="mini-card">
                <h3>Nível 3 - Difícil</h3>
                <p>
                    Contas complicadas de fazer de cabeça: números quebrados, equações do 2º grau e binômio de Newton.
                    <br>
                    Frequência: 15%
                </p>
            </div>

            <div class="mini-card">
                <h3>Nível 4 - Impossível</h3>
                <p>
                    Equações e funções logarítmicas e exponenciais. Não dá para resolver de cabeça.
                    <br><br>
                    Frequência: 5%
                </p>
            </div>
        </div>

        <div id="down">
        <h1>Relações</h1>
        <div class="card">
            <p class="card-intro">
                Durante uma partida, o jogador pode entrar em contato com vários personagens diferentes,
                cada um possuindo personalidade e vontades próprias. Graças a isso, você pode conversar
                com cada um deles seja para descobrir sobre sua história, sobre o mundo em que vivemos
                ou para negociar caramelos em troca de recompensas.
            </p>
            <h3>Sistema de amizades</h3>
            <div class="card-extra">
                <p>
                    O jogo possui um sistema de amizade onde cada diálogo com outros personagens importa,
                    o jogador deve prestar atenção nas características exclusivas e tomar cuidado com o
                    que fala ou faz com cada pessoa. Cada ação possui um valor de amizade que pode ser
                    ganho ou perdido a cada decisão.
                </p>
                <p>
                    O valor de amizade vai de 0 a 100 com um valor inicial de 20, se o usuário atingir um
                    nível de amizade, recompensas exclusivas podem aparecer, mas se ele possuir um baixo
                    nível, os personagens podem ficar offline por até 2 dias e podem cobrar preços maiores
                    por negócios.
                </p>
            </div>
        </div>
        

        <h1>Loja de itens</h1>
        <div class="card">
            <p class="card-intro">
                Ao interagir com o computador o jogador pode clicar no aplicativo "Entregas Rápidas"
                que é responsável por ser o comércio do jogo. Ou seja, o jogador pode comprar vários itens
                com diferentes efeitos e preços para auxiliar durante a partida.
            </p>
            <h3>Reestoque</h3>
            <div class="card-extra">
                <p>
                    No início do jogo, não haverá muitos itens que o jogador poderá comprar, mas conforme o
                    avanço da partida, de tempos em tempos, haverá uma notificação informando sobre o "reestoque"
                    que é justamente o momento onde a loja receberá um aumento em sua gama de opções.
                </p>
            </div>
        </div>

        <h1>Desafios</h1>
        <div class="card">
            <p class="card-intro">
                Além dos importunos naturais como sono ou os problemas matemáticos das sessões, durante uma
                partida o jogador vai ter que superar diversos desafios que podem ou não "ferrar" com a vida dele.
            </p>
            <h3>Contas pendentes</h3>
            <div class="card-extra">
                <p>
                    Para sobreviver nesse mundo, o personagem deve constantemente pagar suas contas a cada 5 dias para evitar
                    ser despejado de seu apartamento. Caso a conta não seja paga até o fim do prazo, a partida se encerra.
                    O valor inicial da conta é equivalente a 500 caramelos e cada conta é 1.5X maior que a anterior.
                </p>
            </div>
            <h3>Eventos aleatórios</h3>
            <div class="card-extra">
                <p>
                    Durante uma partida coisas inesperadas podem acontecer, conforme os dias forem avançando, existe a chance
                    de ocorrer um evento aleatório que pode ser tanto positivo quanto negativo para o jogador. Os principais exemplos
                    são: Blackouts, desconto no Entregas Rápidas e sessões de mineração especiais.
                </p>
            </div>
        </div>
        </div>
    </section>`;
}

function showCharacters(){
    app.innerHTML =
    `<section id="characters-section">
        <div id="dev-guide">
            <h2>Clique em um personagem para saber mais</h2>
        </div>

        <div id="character-container">

            <div class="character-card" onclick="expandCard(this)">
                <div class="character-name">
                    <h2>Pedro Monteiro Rocha</h2>
                </div>
                <div class="character-image">
                    <img src="img/pedro.JPG" alt="Pedro">
                </div>
                <div class="character-role">
                    <h3>Protagonista</h3>
                </div>
                <div class="character-data">
                    <p><strong>Idade</strong><span>24</span></p>
                    <p><strong>Altura</strong><span>1,75 m</span></p>
                    <p><strong>Peso</strong><span>69 kg</span></p>
                    <p><strong>Gostos</strong><span>Jogar videogames e dormir</span></p>
                    <p><strong>Cor favorita</strong><span>Verde</span></p>
                    <p><strong>Gênero</strong><span>Homem</span></p>
                    <p><strong>Ocupação</strong><span>Minerador de caramelos</span></p>
                    <p><strong>Personalidade</strong><span>Determinado, curioso e sensato</span></p>
                </div>
                <div class="character-info">
                    <div class="character-info-content">
                        <h3>Aparência</h3>
                        <p>
                            Cabelos lisos, curtos e pretos, pele parda e olhos grandes e expressivos. Tem um rosto 
                            quadrado que se destaca e uma expressão geralmente cansada. Veste camisas comuns e calça 
                            jeans, e em uma das camisas aparece a logo do Space-Brillig.
                        </p>
                        <h3>Sobre o personagem</h3>
                        <p>
                            Pedro nasceu no interior, longe da evolução tecnológica e principalmente da Inteligência
                            Artificial, mas após a morte de seus pais ele é forçado a ir para a cidade grande em busca
                            de uma vida melhor. Porém, ele não conseguiu acompanhar a velocidade de produção esmagadora
                            da IA e foi rapidamente demitido. Como última alternativa, decide se juntar à comunidade
                            de mineradores de caramelo para sobreviver e manter sua casa. Agora Pedro deve fazer de tudo
                            para sobreviver por 30 dias, superando desafios e homens de terno enquanto descobre segredos
                            de seus conhecidos e sobre o mundo em que vive.
                        </p>
                        <h3>Função no jogo</h3>
                        <p>
                            É o personagem controlado pelo jogador. Cabe a VOCÊ administrar o dinheiro, a energia, o sono
                            e as relações de Pedro para sobreviver até o final do jogo.
                        </p>
                    </div>
                </div>
            </div>

            <div class="character-card" onclick="expandCard(this)">
                <div class="character-name">
                    <h2>Felipe Brito</h2>
                </div>
                <div class="character-image">
                    <img src="img/felipe.png" alt="Felipe">
                </div>
                <div class="character-role">
                    <h3>Coadjuvante</h3>
                </div>
                <div class="character-data">
                    <p><strong>Idade</strong><span>25</span></p>
                    <p><strong>Altura</strong><span>1,80 m</span></p>
                    <p><strong>Peso</strong><span>72 kg</span></p>
                    <p><strong>Gostos</strong><span>Conversar e jogar videogames</span></p>
                    <p><strong>Cor favorita</strong><span>Ciano</span></p>
                    <p><strong>Gênero</strong><span>Homem</span></p>
                    <p><strong>Ocupação</strong><span>Minerador de caramelos</span></p>
                    <p><strong>Personalidade</strong><span>Otimista, engraçado e extrovertido</span></p>
                </div>
                <div class="character-info">
                    <div class="character-info-content">
                        <h3>Aparência</h3>
                        <p>
                            Cabelos cacheados de tamanho médio de cor escura, pele clara com algumas manchas e
                            olhos grandes cobertos por óculos escuros. É notavelmente magro, tem o rosto arredondado e
                            uma expressão alegre, cheia de vida. Usa roupas simples, com camisas e calças comuns.
                        </p>
                        <h3>Sobre o personagem</h3>
                        <p>
                            Velho amigo de Pedro e a única família que lhe resta. Amante da vida e do tipo que
                            "desenrola as coisas na hora", sempre está lá quando precisa oferecendo apoio ao Pedro,
                            e ainda por cima é um extrovertido por natureza o que fez ele conhecer e ganhar contato
                            com várias pessoas diferentes. Além disso, ele é muito informado sobre o Protocolo-Caramelo
                            contendo grandes infomações sobre sua história, mesmo que boa parte dela ainda seja um mistério
                            para a sociedade.
                        </p>
                        <h3>Função no jogo</h3>
                        <p>
                            Guia do jogador do inicío ao fim do jogo, ele  é responsável por apresentar o Protocolo-Caramelo
                            para o jogador e como ele funciona, e ele também passa o contato de vários conhecidos para Pedro
                            para que Pedro tenha acesso a diferentes serviços. Por fim, sempre que estiver em dúvida ou com 
                            insegurança sobre o que fazer a seguir, pode contar com seu lindo e maravilhoso amigo Felipe Brito!
                        </p>
                    </div>
                </div>
            </div>

            <div class="character-card" onclick="expandCard(this)">
                <div class="character-name">
                    <h2>José</h2>
                </div>
                <div class="character-image">
                    <img src="img/jose.png" alt="José">
                </div>
                <div class="character-role">
                    <h3>Personagem secundário</h3>
                </div>
                <div class="character-data">
                    <p><strong>Idade</strong><span>36</span></p>
                    <p><strong>Altura</strong><span>1,70 m</span></p>
                    <p><strong>Peso</strong><span>80 kg</span></p>
                    <p><strong>Gostos</strong><span>Café e ler livros</span></p>
                    <p><strong>Cor favorita</strong><span>Preto e branco</span></p>
                    <p><strong>Gênero</strong><span>Homem</span></p>
                    <p><strong>Ocupação</strong><span>Técnico de Informática</span></p>
                    <p><strong>Personalidade</strong><span>Simpático, altruísta e otimista</span></p>
                </div>
                <div class="character-info">
                    <div class="character-info-content">
                        <h3>Aparência</h3>
                        <p>
                            Homem de postura firme, com cabelo curto e preto, com um pequeno topete na frente.
                            pele negra e barba. Sua expressão é neutra, e no dia a dia usa roupas formais de escritório.
                        </p>
                        <h3>Sobre o personagem</h3>
                        <p>
                            Colega de Pedro no último emprego e a pessoa mais simpática que ele conheceu ali. Foi quem
                            ajudou Pedro a entrar na comunidade Caramelo. José é um homem simples que o único objetivo
                            é ajudar o maior número possível de pessoas até o dia de sua morte, pelo menos depois de não
                            ter conseguido ajudar a pessoa que mais precisava dele...
                        </p>
                        <h3>Função no jogo</h3>
                        <p>
                            Contato da comunidade que ajuda o jogador com informações sobre a VERA. José é um técnico 
                            disposto a fazer upgrades em seu computador por um preço amigável, e ainda por cima sempre
                            vai estar disposto a conversar ou formatar seu computador quando precisar como um gentleman. 
                        </p>
                    </div>
                </div>
            </div>

            <div class="character-card" onclick="expandCard(this)">
                <div class="character-name">
                    <h2>Ana</h2>
                </div>
                <div class="character-image">
                    <img src="img/ana.png" alt="Ana">
                </div>
                <div class="character-role">
                    <h3>Personagem secundária</h3>
                </div>
                <div class="character-data">
                    <p><strong>Idade</strong><span>28</span></p>
                    <p><strong>Altura</strong><span>1,62 m</span></p>
                    <p><strong>Peso</strong><span>50 kg</span></p>
                    <p><strong>Gostos</strong><span>Desenhar e gatos</span></p>
                    <p><strong>Cor favorita</strong><span>Vermelho</span></p>
                    <p><strong>Gênero</strong><span>Mulher</span></p>
                    <p><strong>Ocupação</strong><span>Artista</span></p>
                    <p><strong>Personalidade</strong><span>Forte e decisiva</span></p>
                </div>
                <div class="character-info">
                    <div class="character-info-content">
                        <h3>Aparência</h3>
                        <p>
                            Cabelo longo, liso e ruivo alaranjado, que chega até a cintura, com uma franja que cobre a testa.
                            Seus olhos são pequenos e difíceis de decifrar. Tem postura ruim por passar muito tempo
                            sentada, pele clara e bem cuidada e rosto arredondado. Sua expressão é de alguém estressada e
                            insatisfeita, e as roupas comuns têm lápis nos bolsos e leves manchas de tinta.
                        </p>
                        <h3>Sobre o personagem</h3>
                        <p>
                            Artista por natureza, ficou sem espaço quando as pessoas deixaram de valorizar o trabalho humano 
                            para vangloriar a velocidade de criação da IA, desde então ninguém mais valorizou a arte feita à mão.
                            Mesmo assim ela ainda tenta vender seus desenhos e espalha grafites pelas ruas com a esperança que algum
                            dia a humanidade acorde.
                        </p>
                        <h3>Função no jogo</h3>
                        <p>
                            Personagem da comunidade com quem o jogador pode conversar e negociar para conseguir artes, quadros
                            e todos os tipos de decorações para sua casa.
                        </p>
                    </div>
                </div>
            </div>

            <div class="character-card" onclick="expandCard(this)">
                <div class="character-name">
                    <h2>Ricardo</h2>
                </div>
                <div class="character-image">
                    <img src="img/ricardo.png" alt="Ricardo">
                </div>
                <div class="character-role">
                    <h3>Personagem secundário</h3>
                </div>
                <div class="character-data">
                    <p><strong>Idade</strong><span>40</span></p>
                    <p><strong>Altura</strong><span>1,89 m</span></p>
                    <p><strong>Peso</strong><span>85 kg</span></p>
                    <p><strong>Gostos</strong><span>Jogos de azar</span></p>
                    <p><strong>Cor favorita</strong><span>nenhuma</span></p>
                    <p><strong>Gênero</strong><span>Homem</span></p>
                    <p><strong>Ocupação</strong><span>Apostador</span></p>
                    <p><strong>Personalidade</strong><span>Competitivo e cheio de energia</span></p>
                </div>
                <div class="character-info">
                    <div class="character-info-content">
                        <h3>Aparência</h3>
                        <p>
                            Careca, com olhos médios e pele clara. Normalmente tem o rosto de um homem
                            modesto e de postura normal, mas quando está jogando fica com uma expressão elétrica, de olhos
                            bem abertos. Usa roupas estranhas e sem nexo, e claramente não tem bom gosto em moda.
                        </p>
                        <h3>Sobre o personagem</h3>
                        <p>
                            Ricardo é um homem competitivo, viciado em jogos de azar e refém do próprio vício. Um dia ele
                            já esteve no topo do mundo, mas hoje ele perdeu tudo, mesmo assim, Ricardo acredita fortemente 
                            que não exista emoção melhor no mundo do que apostar com nada mais que sua própria sorte.
                        </p>
                        <h3>Função no jogo</h3>
                        <p>
                            Ricardo pode convidar o jogador para partidas de vinte e um valendo uma quantia de caramelos com  
                            o vencedor levando o bolo. Se você estiver se sentindo com venha até o cassino se divertir um pouco!
                        </p>
                    </div>
                </div>
            </div>

            <div class="character-card" onclick="expandCard(this)">
                <div class="character-name">
                    <h2>Dono do Prédio</h2>
                </div>
                <div class="character-image">
                    <img src="img/dono.JPG" alt="Dono do prédio">
                </div>
                <div class="character-role">
                    <h3>Vilão</h3>
                </div>
                <div class="character-data">
                    <p><strong>Idade</strong><span>64</span></p>
                    <p><strong>Altura</strong><span>1,69 m</span></p>
                    <p><strong>Peso</strong><span>78 kg</span></p>
                    <p><strong>Gostos</strong><span>Dinheiros e vinho</span></p>
                    <p><strong>Cor favorita</strong><span>amarelo</span></p>
                    <p><strong>Gênero</strong><span>Homem</span></p>
                    <p><strong>Ocupação</strong><span>Proprietário do prédio</span></p>
                    <p><strong>Personalidade</strong><span>Estressado, orgulhoso e rancoroso</span></p>
                </div>
                <div class="character-info">
                    <div class="character-info-content">
                        <h3>Aparência</h3>
                        <p>
                            Senhor de cabelo grisalho, liso, curto e muito bem cuidado, com olhos escuros.
                            Tem pele clara e velha, com rugas e pintas pelo corpo. Seu rosto é o de um homem cansado e
                            carregado de raiva, mas mantém a postura de alguém modesto. Quando se irrita, perde a postura
                            e o cabelo fica bagunçado. Usa roupas caras e formais.
                        </p>
                        <h3>Sobre o personagem</h3>
                        <p>
                            Dono da casa popular onde Pedro mora. Estressa-se com extrema facilidade e, por algum motivo,
                            parece não gostar de Pedro.
                        </p>
                        <h3>Função no jogo</h3>
                        <p>
                            Responsável pelas cobranças do jogo. O dono vem de 5 em 5 dias bater na porta de Pedro cobrando 
                            contas cada vez mais absurdas forçando o jogador a correr contra o tempo para não ser despejado de seu apartamento.
                        </p>
                    </div>
                </div>
            </div>
        </div>
    </section>`;
}

function showDevs(){
    app.innerHTML = 
    `<section id="devs-section">
        <div id="dev-guide">
            <h2>Clique em um desenvolvedor para interagir</h2>
        </div>
        <div id="devs-container">
            <div class="dev-card" onclick="expandCard(this)">
                <div class="dev-nickname">
                    <h3>CastCris</h3>
                </div>
                <div class="dev-image">
                    <img src="img/cris.jpeg" alt="Cris">
                </div>
                <div class="dev-job">
                    <h3>Desenvolvedor</h3>
                </div>
                <div class="dev-info">
                    <p>
                        Líder do grupo, sendo o responsável pela modelagem e articulação do código fonte do jogo.
                        Além disso, desenvolveu, junto ao JVVS, o bruto da parte criativa, a exemplo do tema, história,
                        personagens a ambientação do Protocolo-Caramelo.
                    </p>
                </div>
            </div>

            <div class="dev-card" onclick="expandCard(this)">
                <div class="dev-nickname">
                    <h3>JVVS</h3>
                </div>
                <div class="dev-image">
                    <img src="img/joao.png" alt="João">
                </div>
                <div class="dev-job">
                    <h3>Diretor Criativo && Dev. Web</h3>
                </div>
                <div class="dev-info">
                    <p>
                        Diretor criativo do jogo, sendo responsável pela criação e desenvolvimento da história do jogo e seus personagens.
                        Além disso, é responsável pela criação de efeitos sonoros e músicas ambientes dentro do jogo.
                        Por fim, é responsável pela criação e desenvolvimento do site do jogo.
                    </p>
                </div>
            </div>

            <div class="dev-card" onclick="expandCard(this)">
                <div class="dev-nickname">
                    <h3>JeanCFreitas</h3>
                </div>
                <div class="dev-image">
                    <img src="img/jean.jpeg" alt="Jean">
                </div>
                <div class="dev-job">
                    <h3>Artista</h3>
                </div>
                <div class="dev-info">
                    <p>
                        Artista do grupo, responsável pelas artes conceituais do trabalho e pelo lado mais artístico.
                        Desenvolveu o design do quarto e dos personagens.
                    </p>
                </div>
            </div>

            <div class="dev-card" onclick="expandCard(this)">
                <div class="dev-nickname">
                    <h3>DiogoBSousa</h3>
                </div>
                <div class="dev-image">
                    <img src="img/diogo.jpeg" alt="Diogo">
                </div>
                <div class="dev-job">
                    <h3>Artista</h3>
                </div>
                <div class="dev-info">
                    <p>
                        Artista responsável pela criação dos elementos gráficos e imagens do jogo, além de contribuir
                        com ideias para o desenvolvimento da história, ambientação e identidade visual do Protocolo-Caramelo.
                    </p>
                </div>
            </div>
        </div>
    </section>`;
}

function expandCard(card){
    card.classList.toggle("expanded");
}

function loadPage() {

    const page = window.location.hash;
    const path = window.location.pathname;

     if (path !== "/") {
        app.innerHTML = 
        `<section>
            <div id="error-alert">
                <h1>Erro 404</h1>
                <p>Página não encontrada.</p>
            </div>
        </section>`;
        return;
    }

    switch(page) {

        case "#home":
        case "":
            showHome();
            break;

        case "#mechanics":
            showMechanics();
            break;

        case "#characters":
            showCharacters();
            break;

        case "#devs":
            showDevs();
            break;

        default:
            app.innerHTML = 
            `<section>
                <div id="error-alert">
                <h1>Erro 404</h1>
                <p>Página não encontrada.</p>
                </div>
            </section>`;
    }
}

loadPage();
window.addEventListener("hashchange", loadPage);