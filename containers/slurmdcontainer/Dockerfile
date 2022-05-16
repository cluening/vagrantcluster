FROM fedora:36

RUN dnf -y install slurm-slurmd \
    && dnf clean all

COPY entrypoint.sh /

ENTRYPOINT ["/entrypoint.sh"]
