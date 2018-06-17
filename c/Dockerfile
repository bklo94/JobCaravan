FROM php:5.6-fpm
RUN apt-get update
RUN apt-get install -y libpq-dev \
    && docker-php-ext-configure pgsql -with-pgsql=/usr/local/pgsql \
    && docker-php-ext-install pdo pdo_pgsql pgsql
FROM gcc
ADD . /cbackend
WORKDIR /cbackend
RUN make
ENTRYPOINT "test/debug.sh"
CMD sleep 5 && ./bin/main

EXPOSE 10800
