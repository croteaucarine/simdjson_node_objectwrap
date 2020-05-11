# docker build -t simdjson_node_objectwrap . 
# https://docs.docker.com/engine/reference/builder/#arg
FROM gcc:9.3
ARG BENCHMARK_TYPE=all
ARG ITERATIONS=1

RUN apt-get update
RUN apt-get -y install curl gnupg
RUN curl -sL https://deb.nodesource.com/setup_12.x  | bash -
RUN apt-get -y install nodejs

WORKDIR /app

COPY package.json package.json

COPY . .

RUN npm install --unsafe-perm

RUN if [ ${BENCHMARK_TYPE} = "parse" ] ; then bash ./scripts/parse.sh; else npm run benchmarks ; fi

CMD tail -f /dev/null





