# These are needed on our virtual cluster to keep openmpi from trying
# to use the virtualbox interface
export OMPI_MCA_btl_tcp_if_include=eth1
export OMPI_MCA_oob_tcp_if_include=eth1

# This is needed to keep UCX from trying to use the virtualbox interface
UCX_NET_DEVICES=eth1
