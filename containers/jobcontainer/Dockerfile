FROM centos:stream8

RUN mkdir -p /layercake \
&& mkdir -p /etc/layercake

RUN dnf config-manager --set-enabled powertools \
&& dnf install -y \
    epel-release epel-next-release

RUN dnf install -y \
    hostname \
    libcurl \
    libcurl-devel \
    openmpi \
    openmpi-devel \
    pmix \
    pmix-devel \
    slurm \
    slurm-devel \
    slurm-nss_slurm \
    slurm-pmi \
    which \
&& dnf group install -y "Development Tools"

#COPY layercake-podman.c podman.h podman.c layercake.sh layercakepause.c /layercake/
COPY layercake.sh layercakepause.c /layercake/

WORKDIR /layercake

RUN gcc -o layercakepause layercakepause.c
#RUN mkdir -p /usr/lib/slurm \
#&& gcc -fPIC -lcurl -shared -o /usr/lib/slurm/layercake-podman.so layercake-podman.c podman.c

RUN sed -i 's/sss files systemd/slurm sss files systemd/g' /etc/nsswitch.conf

WORKDIR /

ENTRYPOINT /layercake/layercake.sh
