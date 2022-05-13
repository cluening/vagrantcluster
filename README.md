Virtual HPC Cluster
===================

This repository contains the framework for a very basic HPC cluster based on Vagrant, Ansible, and OpenHPC.  It is just enough to build four nodes, a frontend, and a head node on your laptop or desktop system.  From there, you can customize it however you wish!

Getting Started
---------------

1. Install Vagrant: https://www.vagrantup.com/
1. Clone this repository
1. Run the `gensshkeys.sh` script to generate ssh keys in the ansible repository
1. (Optional) Copy `localenv.sh.in` to `localenv.sh` and populate it with any local environment variables you need during the vagrant provisioning step (HTTP proxy information, for example)
1. Run `vagrant up` to fire up the cluster
1. Once the cluster is booted, you can run `vagrant ssh head` to log in to the head node, or `vagrant ssh fe1` to log in to the frontend
1. Run `sinfo` on the frontend or head node to see if Slurm sees that your nodes are up.  If they are not, run `sudo scontrol update nodename=node[01-04] state=resume` to wake them up.
1. Start using your cluster!  At this point, you should be able to run a simple test across the cluster (`srun -N 4 /bin/hostname`) or run some more complex jobs.
1. When you are done, shut down your cluster by logging out of it and running `vagrant halt`.
1. If you want to completely rebuild your cluster, run `vagrant destroy`, and then run `vagrant up` again.

A note on security
------------------
This virtual cluster is built around convenience, not security.  It uses Vagrant's default ssh keys for convenience, and it contains some private keys (for munge, for example).  This is good enough to run on an isolated desktop or laptop for experimentation, but you shouldn't plan to base an actual cluster configuration on its ansible repository without doing a good security sanity check.
