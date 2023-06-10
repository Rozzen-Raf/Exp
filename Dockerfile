FROM ubuntu:latest AS build-stage

RUN apt update && \
    apt install -y cmake git coreutils g++ libc-dev make

RUN g++ -v

RUN git clone https://github.com/Rozzen-Raf/Exp.git

ADD ./run.sh /Exp/run.sh

RUN ["chmod", "+x", "/Exp/run.sh"]

WORKDIR /Exp/

RUN ./run.sh

FROM ubuntu:latest

RUN groupadd -r sample && useradd -r -g sample sample
USER sample

# Скопируем приложение со сборочного контейнера в рабочую директорию
COPY --from=build-stage /Exp/build/examples/ .
COPY ./config.json ./

ENTRYPOINT ["./server-example"]
