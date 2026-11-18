const app = document.getElementById("app");

function createEl(tag, className, text) {
    const el = document.createElement(tag);
    if (className) el.className = className;
    if (text !== undefined) el.textContent = text;
    return el;
}

async function fetchJSON(url) {
    const response = await fetch(url);
    if (!response.ok) {
        throw new Error("Erro " + response.status + " em " + url);
    }
    return await response.json();
}

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

function createCharacterCard(c) {
    const card = createEl("div", "character-card");
    card.addEventListener("click", () => card.classList.toggle("expanded"));

    const name = createEl("div", "character-name");
    name.appendChild(createEl("h2", "", c.name));

    const imageBox = createEl("div", "character-image");
    const img = document.createElement("img");
    img.src = c.image;
    img.alt = c.name;
    imageBox.appendChild(img);

    const role = createEl("div", "character-role");
    role.appendChild(createEl("h3", "", c.role));

    const data = createEl("div", "character-data");
    c.stats.forEach(stat => {
        const row = document.createElement("p");
        row.appendChild(createEl("strong", "", stat.label));
        row.appendChild(createEl("span", "", stat.value));
        data.appendChild(row);
    });

    const info = createEl("div", "character-info");
    const content = createEl("div", "character-info-content");
    [
        ["Aparência", c.appearance],
        ["Sobre o personagem", c.about],
        ["Função no jogo", c.gameFunction]
    ].forEach(([title, text]) => {
        content.appendChild(createEl("h3", "", title));
        content.appendChild(createEl("p", "", text));
    });
    info.appendChild(content);

    card.append(name, imageBox, role, data, info);
    return card;
}

async function showCharacters() {
    app.innerHTML =
    `<section id="characters-section">
        <div id="dev-guide">
            <h2>Clique em um personagem para saber mais</h2>
        </div>
        <div id="character-container"></div>
    </section>`;

    const container = document.getElementById("character-container");

    try {
        const characters = await fetchJSON("/api/characters");
        if (!container.isConnected) return; 
        characters.forEach(c => container.appendChild(createCharacterCard(c)));
    } catch (error) {
        console.error(error);
        container.textContent = "Não foi possível carregar os personagens.";
    }
}

function createDevCard(dev) {
    const card = createEl("div", "dev-card");
    card.addEventListener("click", () => card.classList.toggle("expanded"));

    const nickname = createEl("div", "dev-nickname");
    nickname.appendChild(createEl("h3", "", dev.nickname));

    const imageBox = createEl("div", "dev-image");
    const img = document.createElement("img");
    img.src = dev.image;
    img.alt = dev.name;
    imageBox.appendChild(img);

    const job = createEl("div", "dev-job");
    job.appendChild(createEl("h3", "", dev.job));

    const info = createEl("div", "dev-info");
    info.appendChild(createEl("p", "", dev.info));

    card.append(nickname, imageBox, job, info);
    return card;
}

async function showDevs() {
    app.innerHTML =
    `<section id="devs-section">
        <div id="dev-guide">
            <h2>Clique em um desenvolvedor para interagir</h2>
        </div>
        <div id="devs-container"></div>
    </section>`;

    const container = document.getElementById("devs-container");

    try {
        const devs = await fetchJSON("/api/developers");
        if (!container.isConnected) 
            return;
            devs.forEach(dev => container.appendChild(createDevCard(dev)));
    } 
    
    catch (error) {
        console.error(error);
        container.textContent = "Não foi possível carregar os desenvolvedores.";
    }
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