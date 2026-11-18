package com.example.Protocolo_Caramelo;

import java.util.List;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/api/characters")
public class CharacterController {

    private static List<Stat> stats(String idade, String altura, String peso, String gostos,
                                    String corFavorita, String genero, String ocupacao,
                                    String personalidade) {
        return List.of(
            new Stat("Idade", idade),
            new Stat("Altura", altura),
            new Stat("Peso", peso),
            new Stat("Gostos", gostos),
            new Stat("Cor favorita", corFavorita),
            new Stat("Gênero", genero),
            new Stat("Ocupação", ocupacao),
            new Stat("Personalidade", personalidade)
        );
    }

    private final List<GameCharacter> characters = List.of(

        new GameCharacter("pedro", "Pedro Monteiro Rocha", "Protagonista", "img/pedro.JPG",
            stats("24", "1,75 m", "69 kg", "Jogar videogames e dormir", "Verde", "Homem",
                  "Minerador de caramelos", "Determinado, curioso e sensato"),
            "Cabelos lisos, curtos e pretos, pele parda e olhos grandes e expressivos. Tem um rosto "
          + "quadrado que se destaca e uma expressão geralmente cansada. Veste camisas comuns e calça "
          + "jeans, e em uma das camisas aparece a logo do Space-Brillig.",
            "Pedro nasceu no interior, longe da evolução tecnológica e principalmente da Inteligência "
          + "Artificial, mas após a morte de seus pais ele é forçado a ir para a cidade grande em busca "
          + "de uma vida melhor. Porém, ele não conseguiu acompanhar a velocidade de produção esmagadora "
          + "da IA e foi rapidamente demitido. Como última alternativa, decide se juntar à comunidade "
          + "de mineradores de caramelo para sobreviver e manter sua casa. Agora Pedro deve fazer de tudo "
          + "para sobreviver por 30 dias, superando desafios e homens de terno enquanto descobre segredos "
          + "de seus conhecidos e sobre o mundo em que vive.",
            "É o personagem controlado pelo jogador. Cabe a VOCÊ administrar o dinheiro, a energia, o sono "
          + "e as relações de Pedro para sobreviver até o final do jogo."),

        new GameCharacter("felipe", "Felipe Brito", "Coadjuvante", "img/felipe.png",
            stats("25", "1,80 m", "72 kg", "Conversar e jogar videogames", "Ciano", "Homem",
                  "Minerador de caramelos", "Otimista, engraçado e extrovertido"),
            "Cabelos cacheados de tamanho médio de cor escura, pele clara com algumas manchas e "
          + "olhos grandes cobertos por óculos escuros. É notavelmente magro, tem o rosto arredondado e "
          + "uma expressão alegre, cheia de vida. Usa roupas simples, com camisas e calças comuns.",
            "Velho amigo de Pedro e a única família que lhe resta. Amante da vida e do tipo que "
          + "\"desenrola as coisas na hora\", sempre está lá quando precisa oferecendo apoio ao Pedro, "
          + "e ainda por cima é um extrovertido por natureza o que fez ele conhecer e ganhar contato "
          + "com várias pessoas diferentes. Além disso, ele é muito informado sobre o Protocolo-Caramelo "
          + "contendo grandes informações sobre sua história, mesmo que boa parte dela ainda seja um mistério "
          + "para a sociedade.",
            "Guia do jogador do início ao fim do jogo, ele é responsável por apresentar o Protocolo-Caramelo "
          + "para o jogador e como ele funciona, e ele também passa o contato de vários conhecidos para Pedro "
          + "para que Pedro tenha acesso a diferentes serviços. Por fim, sempre que estiver em dúvida ou com "
          + "insegurança sobre o que fazer a seguir, pode contar com seu lindo e maravilhoso amigo Felipe Brito!"),

        new GameCharacter("jose", "José", "Personagem secundário", "img/jose.png",
            stats("36", "1,70 m", "80 kg", "Café e ler livros", "Preto e branco", "Homem",
                  "Técnico de Informática", "Simpático, altruísta e otimista"),
            "Homem de postura firme, com cabelo curto e preto, com um pequeno topete na frente. "
          + "Pele negra e barba. Sua expressão é neutra, e no dia a dia usa roupas formais de escritório.",
            "Colega de Pedro no último emprego e a pessoa mais simpática que ele conheceu ali. Foi quem "
          + "ajudou Pedro a entrar na comunidade Caramelo. José é um homem simples cujo único objetivo "
          + "é ajudar o maior número possível de pessoas até o dia de sua morte, pelo menos depois de não "
          + "ter conseguido ajudar a pessoa que mais precisava dele...",
            "Contato da comunidade que ajuda o jogador com informações sobre a VERA. José é um técnico "
          + "disposto a fazer upgrades em seu computador por um preço amigável, e ainda por cima sempre "
          + "vai estar disposto a conversar ou formatar seu computador quando precisar como um gentleman."),

        new GameCharacter("ana", "Ana", "Personagem secundária", "img/ana.png",
            stats("28", "1,62 m", "50 kg", "Desenhar e gatos", "Vermelho", "Mulher",
                  "Artista", "Forte e decisiva"),
            "Cabelo longo, liso e ruivo alaranjado, que chega até a cintura, com uma franja que cobre a testa. "
          + "Seus olhos são pequenos e difíceis de decifrar. Tem postura ruim por passar muito tempo "
          + "sentada, pele clara e bem cuidada e rosto arredondado. Sua expressão é de alguém estressada e "
          + "insatisfeita, e as roupas comuns têm lápis nos bolsos e leves manchas de tinta.",
            "Artista por natureza, ficou sem espaço quando as pessoas deixaram de valorizar o trabalho humano "
          + "para vangloriar a velocidade de criação da IA, desde então ninguém mais valorizou a arte feita à mão. "
          + "Mesmo assim ela ainda tenta vender seus desenhos e espalha grafites pelas ruas com a esperança que algum "
          + "dia a humanidade acorde.",
            "Personagem da comunidade com quem o jogador pode conversar e negociar para conseguir artes, quadros "
          + "e todos os tipos de decorações para sua casa."),

        new GameCharacter("ricardo", "Ricardo", "Personagem secundário", "img/ricardo.png",
            stats("40", "1,89 m", "85 kg", "Jogos de azar", "Nenhuma", "Homem",
                  "Apostador", "Competitivo e cheio de energia"),
            "Careca, com olhos médios e pele clara. Normalmente tem o rosto de um homem "
          + "modesto e de postura normal, mas quando está jogando fica com uma expressão elétrica, de olhos "
          + "bem abertos. Usa roupas estranhas e sem nexo, e claramente não tem bom gosto em moda.",
            "Ricardo é um homem competitivo, viciado em jogos de azar e refém do próprio vício. Um dia ele "
          + "já esteve no topo do mundo, mas hoje ele perdeu tudo, mesmo assim, Ricardo acredita fortemente "
          + "que não exista emoção melhor no mundo do que apostar com nada mais que sua própria sorte.",
            "Ricardo pode convidar o jogador para partidas de vinte e um valendo uma quantia de caramelos com "
          + "o vencedor levando o bolo. Se você estiver se sentindo com sorte, venha até o cassino se divertir um pouco!"),

        new GameCharacter("dono", "Dono do Prédio", "Vilão", "img/dono.JPG",
            stats("64", "1,69 m", "78 kg", "Dinheiro e vinho", "Amarelo", "Homem",
                  "Proprietário do prédio", "Estressado, orgulhoso e rancoroso"),
            "Senhor de cabelo grisalho, liso, curto e muito bem cuidado, com olhos escuros. "
          + "Tem pele clara e velha, com rugas e pintas pelo corpo. Seu rosto é o de um homem cansado e "
          + "carregado de raiva, mas mantém a postura de alguém modesto. Quando se irrita, perde a postura "
          + "e o cabelo fica bagunçado. Usa roupas caras e formais.",
            "Dono da casa popular onde Pedro mora. Estressa-se com extrema facilidade e, por algum motivo, "
          + "parece não gostar de Pedro.",
            "Responsável pelas cobranças do jogo. O dono vem de 5 em 5 dias bater na porta de Pedro cobrando "
          + "contas cada vez mais absurdas forçando o jogador a correr contra o tempo para não ser despejado de seu apartamento.")
    );

    @GetMapping
    public List<GameCharacter> getAll() {
        return characters;
    }

    @GetMapping("/{id}")
    public ResponseEntity<GameCharacter> getById(@PathVariable String id) {
        return characters.stream()
                .filter(c -> c.id().equals(id))
                .findFirst()
                .map(ResponseEntity::ok)
                .orElse(ResponseEntity.notFound().build());
    }
}