FROM fedora:36

RUN mkdir -p /layercake/

RUN dnf group install -y "Development Tools"

RUN dnf -y install slurm-slurmd slurm-devel \
                   libcurl libcurl-devel \
    && dnf clean all

COPY layercake-podman.c podman.h podman.c /layercake/

WORKDIR /layercake
RUN mkdir -p /usr/lib/slurm \
&& gcc -fPIC -lcurl -shared -o /usr/lib/slurm/layercake-podman.so layercake-podman.c podman.c

WORKDIR /

COPY entrypoint.sh /

ENTRYPOINT ["/entrypoint.sh"]
