const app = document.getElementById("app");

function showHome() {
    app.innerHTML = 
    `<section>
        <div id="logo"> 
            <img src="img/cachorro.png" alt="dog">
        </div>
        <h1>Sinopse</h1>
        <div class="card">
            <p>
                Esse mundo se passa no ano de 2046, onde o planeta inteiro foi completamente dominado pelas megacorporações de I.A,
                a qual foi capaz de substituir grande parte dos empregos acadêmicos e artísticos existentes. O resultado disso foi catastrófico, 
                com essa nova tecnologia em mãos a humanidade perdeu o bom senso, milhares de pessoas de todos os setores ficaram desempregadas
                e o mundo se tornou frio. Diante disso, Pedro Monteiro Rocha, um pacato cidadão da civilação que nasceu em uma área rural longe 
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
                <br>
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

function showDevs(){
    app.innerHTML = 
    `<section id="devs-section">

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