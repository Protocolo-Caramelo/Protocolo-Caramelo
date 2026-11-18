package com.example.Protocolo_Caramelo;

import java.util.List;

public record GameCharacter(
        String id,
        String name,
        String role,
        String image,
        List<Stat> stats,
        String appearance,
        String about,
        String gameFunction
) {}