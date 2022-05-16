#!/bin/bash

exec /usr/sbin/slurmd -D --conf-server slurm $@
