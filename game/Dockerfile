FROM ubuntu:latest
LABEL author=CastCris

WORKDIR /game

RUN apt-get update && apt install -y \
    build-essential \
    libsdl2-dev=2.32.10+dfsg-6 \
    libsdl2-image-dev=2.8.8+dfsg-2 \
    libsdl2-ttf-dev=2.24.0+dfsg-3 \
    libsdl2-mixer-dev \
    cmake \
    mingw-w64 \
    wget \
    # SDL2 2.32.10
    && wget https://www.libsdl.org/release/SDL2-devel-2.32.10-mingw.tar.gz -O SDL2-mingw.tar.gz \
    # SDL2_image 2.8.8
    && wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.8-mingw.tar.gz -O SDL2_image-mingw.tar.gz \
    # SDL2_ttf 2.24.0
    && wget https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.24.0-mingw.tar.gz -O SDL2_ttf-mingw.tar.gz \
    # 
    && tar -xzf SDL2-mingw.tar.gz \
    && tar -xzf SDL2_image-mingw.tar.gz \
    && tar -xzf SDL2_ttf-mingw.tar.gz \
    #
    && mkdir -p /opt/sdl-mingw \
    #
    && mv SDL2-2.32.10 /opt/sdl-mingw/ \
    && mv SDL2_image-2.8.8 /opt/sdl-mingw/ \
    && mv SDL2_ttf-2.24.0 /opt/sdl-mingw/

COPY ./game .
