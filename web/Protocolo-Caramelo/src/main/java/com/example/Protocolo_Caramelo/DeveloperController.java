package com.example.Protocolo_Caramelo;

import java.util.List;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api/developers")
public class DeveloperController {

    private final List<Developer> developers = List.of(
        new Developer("cris", "CastCris", "Cris", "img/cris.jpeg", "Desenvolvedor",
            "Líder do grupo, sendo o responsável pela modelagem e articulação do código fonte do jogo. "
          + "Além disso, desenvolveu, junto ao JVVS, o bruto da parte criativa, a exemplo do tema, história, "
          + "personagens e ambientação do Protocolo-Caramelo."),

        new Developer("joao", "JVVS", "João", "img/joao.png", "Diretor Criativo && Dev. Web",
            "Diretor criativo do jogo, sendo responsável pela criação e desenvolvimento da história do jogo e seus personagens. "
          + "Além disso, é responsável pela criação de efeitos sonoros e músicas ambientes dentro do jogo. "
          + "Por fim, é responsável pela criação e desenvolvimento do site do jogo."),

        new Developer("jean", "JeanCFreitas", "Jean", "img/jean.jpeg", "Artista",
            "Artista do grupo, responsável pelas artes conceituais do trabalho e pelo lado mais artístico. "
          + "Desenvolveu o design do quarto e dos personagens."),

        new Developer("diogo", "DiogoBSousa", "Diogo", "img/diogo.jpeg", "Artista",
            "Artista responsável pela criação dos elementos gráficos e imagens do jogo, além de contribuir "
          + "com ideias para o desenvolvimento da história, ambientação e identidade visual do Protocolo-Caramelo.")
    );

    @GetMapping
    public List<Developer> getAll() {
        return developers;
    }
}