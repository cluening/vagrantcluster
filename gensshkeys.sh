#!/bin/bash

# TODO
#  - Add --force option
#  - Add --group option

GROUP_LIST="compute fe head"
INVENTORY_DIR="ansiblerepo/inventory"
KEYLIST="ecdsa ed25519 rsa"

TEMPDIR="$(mktemp -d)"
trap 'rm -rf -- "$TEMPDIR"' EXIT

# Check if files already exist so we don't overwrite them if they do
FILEEXISTS=false
for group in $GROUP_LIST; do
  if [ -e "$INVENTORY_DIR/group_vars/${group}/sshkeys.yaml" ]; then
    FILEEXISTS=true
  fi
done

# If a file already exists, exit with an error
if [ "$FILEEXISTS" = true ]; then
  echo Error: one or more ssh key or known_hosts files already exist
  # FIXME: could print list of existing files here
  exit 1
fi

# Create all of the types of keys that we can
# FIXME: should probably warn if key generation fails
for group in $GROUP_LIST; do
  echo "---" > "$INVENTORY_DIR/group_vars/${group}/sshkeys.yaml"
  echo "ssh_keys:" >> "$INVENTORY_DIR/group_vars/${group}/sshkeys.yaml"
  for key in $KEYLIST; do
    /usr/bin/ssh-keygen -q -t "$key" -f "$TEMPDIR/ssh_host_${key}_key" -C '' -N '' >& /dev/null
    {
      echo "  ${key}:"
      echo "    public: '$(cat "$TEMPDIR/ssh_host_${key}_key.pub")'"
      echo "    private: |"
      sed 's/^/      /' < "$TEMPDIR/ssh_host_${key}_key"
    } >> "$INVENTORY_DIR/group_vars/${group}/sshkeys.yaml"
    rm "$TEMPDIR/ssh_host_${key}_key" "$TEMPDIR/ssh_host_${key}_key.pub"
  done
done
