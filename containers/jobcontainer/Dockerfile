FROM centos:stream8

RUN mkdir -p /layercake \
&& dnf install -y \
    hostname \ 
    which \
&& dnf group install -y "Development Tools"

COPY layercakepause.c /layercake/

RUN gcc -o /layercake/layercakepause /layercake/layercakepause.c

ENTRYPOINT /layercake/layercakepause
