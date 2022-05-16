FROM fedora:36

RUN dnf -y install \
           munge \
           util-linux \
    && dnf clean all

COPY entrypoint.sh /

ENTRYPOINT ["/entrypoint.sh"]
