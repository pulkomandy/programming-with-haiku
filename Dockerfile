FROM ivotron/pandoc
ENV TZ="Europe/Rome"

RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections \
  && apt-get update \
  && apt-get install -y --no-install-recommends tzdata curl calibre \
  && ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone \
  && dpkg-reconfigure --frontend noninteractive tzdata \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/

WORKDIR /app
COPY . .

CMD ["make", "all"]
