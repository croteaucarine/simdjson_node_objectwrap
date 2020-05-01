# docker build -t simdjson_node_objectwrap . && docker run --privileged -t simdjson_node_objectwrap
FROM gcc:9.3

USER root

RUN apt-get update
RUN apt-get -y install curl gnupg
RUN curl -sL https://deb.nodesource.com/setup_12.x  | bash -
RUN apt-get -y install nodejs

WORKDIR /app

COPY package.json package.json

COPY . .

RUN npm install --unsafe-perm
RUN npm run benchmarks

CMD tail -f /dev/null





