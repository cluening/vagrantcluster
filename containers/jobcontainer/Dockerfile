FROM centos:stream8

RUN mkdir -p /layercake \
&& mkdir -p /etc/layercake \
&& dnf install -y \
    hostname \ 
    which \
&& dnf group install -y "Development Tools"

COPY layercake.sh layercakepause.c /layercake/

RUN gcc -o /layercake/layercakepause /layercake/layercakepause.c

ENTRYPOINT /layercake/layercake.sh
