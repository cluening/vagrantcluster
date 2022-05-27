FROM fedora:36

RUN mkdir -p /layercake/

RUN dnf group install -y "Development Tools"

RUN dnf -y install slurm-slurmd slurm-devel \
    && dnf clean all

COPY layercake-podman.c /layercake/

RUN mkdir -p /usr/lib/slurm \
&& gcc -fPIC -shared -o /usr/lib/slurm/layercake-podman.so /layercake/layercake-podman.c

COPY entrypoint.sh /

ENTRYPOINT ["/entrypoint.sh"]
