FROM ubuntu:latest
LABEL authors=CastCris,JVVS

WORKDIR /Protocolo-Caramelo/web
RUN apt-get update && apt install -y \
build-essential \
openjdk-21-jdk

COPY ./Protocolo-Caramelo .
