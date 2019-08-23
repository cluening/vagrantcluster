# -*- mode: ruby -*-
# vi: set ft=ruby :

# All Vagrant configuration is done below. The "2" in Vagrant.configure
# configures the configuration version (we support older styles for
# backwards compatibility). Please don't change it unless you know what
# you're doing.
Vagrant.configure("2") do |config|
  # The most common configuration options are documented and commented below.
  # For a complete reference, please see the online documentation at
  # https://docs.vagrantup.com.

  # Every Vagrant development environment requires a box. You can search for
  # boxes at https://atlas.hashicorp.com/search.
  # config.vm.box = "centos/7"

  # Note: insecure hack!
  # NFS mounting of /home from the frontend results in the same authorized_keys
  # file on every node.  With this option vagrant won't generate a new key for
  # each host.  But don't use it in production!
  config.ssh.insert_key = false

  config.vm.define "master" do |master|
    master.vm.box = "centos/7"
    master.vm.hostname = "master"
    master.vm.network :private_network, ip: "192.168.100.2"
  end

  config.vm.define "fe1" do |fe1|
    fe1.vm.box = "centos/7"
    fe1.vm.hostname = "fe1"
    fe1.vm.network :private_network, ip: "192.168.100.3"
  end

  config.vm.define "node01" do |node01|
    node01.vm.box = "centos/7"
    node01.vm.hostname = "node01"
    node01.vm.network :private_network, ip: "192.168.100.101"
  end

  config.vm.define "node02" do |node02|
    node02.vm.box = "centos/7"
    node02.vm.hostname = "node02"
    node02.vm.network :private_network, ip: "192.168.100.102"

    node02.vm.provider "virtualbox" do |v|
      v.memory = 1024
      v.gui = true
      v.customize [
        'modifyvm', :id,
        '--nicbootprio2', '1',
        '--boot1', 'net',
        '--boot2', 'none',
        '--boot3', 'none',
        '--boot4', 'none'
      ]
    end
  end

  config.vm.define "node03" do |node03|
    node03.vm.box = "centos/7"
    node03.vm.hostname = "node03"
    node03.vm.network :private_network, ip: "192.168.100.103"
  end

  config.vm.define "node04" do |node04|
    node04.vm.box = "centos/7"
    node04.vm.hostname = "node04"
    node04.vm.network :private_network, ip: "192.168.100.104"
  end

  # Disable automatic box update checking. If you disable this, then
  # boxes will only be checked for updates when the user runs
  # `vagrant box outdated`. This is not recommended.
  # config.vm.box_check_update = false

  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine. In the example below,
  # accessing "localhost:8080" will access port 80 on the guest machine.
  # config.vm.network "forwarded_port", guest: 80, host: 8080

  # Create a private network, which allows host-only access to the machine
  # using a specific IP.
  # config.vm.network "private_network", ip: "192.168.33.10"

  # Create a public network, which generally matched to bridged network.
  # Bridged networks make the machine appear as another physical device on
  # your network.
  # config.vm.network "public_network"

  # Share an additional folder to the guest VM. The first argument is
  # the path on the host to the actual folder. The second argument is
  # the path on the guest to mount the folder. And the optional third
  # argument is a set of non-required options.
  # config.vm.synced_folder "../data", "/vagrant_data"

  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
  # Example for VirtualBox:
  #
  # config.vm.provider "virtualbox" do |vb|
  #   # Display the VirtualBox GUI when booting the machine
  #   vb.gui = true
  #
  #   # Customize the amount of memory on the VM:
  #   vb.memory = "1024"
  # end
  #
  # View the documentation for the provider you are using for more
  # information on available options.

  config.vm.provider :virtualbox do |vb|
    vb.customize ["modifyvm", :id, "--natdnshostresolver1", "on"]
  end

  # Enable provisioning with a shell script. Additional provisioners such as
  # Puppet, Chef, Ansible, Salt, and Docker are also available. Please see the
  # documentation for more information about their specific syntax and use.
  # config.vm.provision "shell", inline: <<-SHELL
  #   apt-get update
  #   apt-get install -y apache2
  # SHELL

  config.vm.provision "shell", inline: <<-SHELL
    if [ -f /vagrant/localenv.sh ]; then
      . /vagrant/localenv.sh
    fi
    yum install -y https://github.com/openhpc/ohpc/releases/download/v1.3.GA/ohpc-release-1.3-1.el7.x86_64.rpm
    yum install -y ansible

    ansible-playbook -c local -i /vagrant/ansiblerepo/inventory/hosts -l `hostname` /vagrant/ansiblerepo/site.yaml
  SHELL

end
