# docker build -t simdjson_node_objectwrap:latest -f- https://github.com/croteaucarine/simdjson_node_objectwrap.git

FROM gcc:8.3

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





