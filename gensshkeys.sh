#!/bin/bash

# TODO
#  - Add --force option
#  - Add --class option

CLASSLIST="compute fe master"
HOSTcompute="node*,192.168.56.1*"
HOSTfe="fe1,192.168.56.3"
HOSTmaster="master,192.168.56.2"
ETCDIR="ansiblerepo/roles/common/files/etc/"
KEYLIST="ecdsa ed25519 rsa"

# Check that ETCDIR actually exists
# FIXME: should probably check that $ETCDIR/ssh.$CLASS exists too
if ! [ -d $ETCDIR ]; then
  echo Error: ETCDIR $ETCDIR does not exist
  exit 1
fi

# Check if files already exist so we don't overwrite them if they do
FILEEXISTS=false
for class in $CLASSLIST; do
  for key in $KEYLIST; do
    if [ -e $ETCDIR/ssh.${class}/ssh_host_${key}_key ]; then
      FILEEXISTS=true
    fi
    if [ -e $ETCDIR/ssh.${class}/ssh_host_${key}_key.pub ]; then
      FILEEXISTS=true
    fi
  done
  if [ -e $ETCDIR/ssh.${class}/ssh_known_hosts ]; then
    FILEEXISTS=true
  fi
done

# If a file already exists, error out
if [ "$FILEEXISTS" = true ]; then
  echo Error: one or more ssh key or known_hosts files already exist
  # FIXME: could print list of existing files here
  exit 1
fi

# Create all of the types of keys that we can
# FIXME: should probably warn if key generation fails
for class in $CLASSLIST; do
  for key in $KEYLIST; do
    /usr/bin/ssh-keygen -q -t $key -f $ETCDIR/ssh.${class}/ssh_host_${key}_key -C '' -N '' >& /dev/null
  done
done

# Create the known_hosts files
for skhclass in $CLASSLIST; do
  for key in $KEYLIST; do
    for class in $CLASSLIST; do
      if [ -e $ETCDIR/ssh.${class}/ssh_host_${key}_key.pub ]; then
        hostname="HOST$class"
        echo ${!hostname} `cat $ETCDIR/ssh.${class}/ssh_host_${key}_key.pub` >> $ETCDIR/ssh.${skhclass}/ssh_known_hosts
      fi
    done
  done
done
