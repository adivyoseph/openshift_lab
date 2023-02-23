# First Steps

Install Fedora 37 Workstation from live-disk

run `$ ssh-keygen` to create SSH keys for GitHub authentication.

run `$ cat .ssh/id_rsa.pub` highlight output and right click mouse `copy`, paste into GitHub profile SSH keys for this project.

Clone the repo `$ git clone git@github.com:adivyoseph/openshift_lab.git`

## env
`enp39s0` motherboard GE port
`wlo1` Wifi port on home Wifi LAN 192.168.2.x



## helper node - this workstation
`$ sudo virsh net-define --file virt-net.xml`   Create a virtual network using this file
output `Network openshift4 defined from virt-net.xml`

Set the network to autostart on boot
`$ sudo virsh net-autostart openshift4
Network openshift4 marked as autostarted`

`$ sudo virsh net-start openshift4
Network openshift4 started`

Confirm that the bridge is available and active:

`$ brctl show`
`bridge name	bridge id		STP enabled	interfaces
openshift4		8000.52540066f962	yes		
virbr0		8000.525400f4c3d3	yes	`

first display available OS templates with command below:

`$ virt-builder -l`

use `fedora-37` to get a reasonable kernel version 

`$ sudo virt-builder fedora-37  --format qcow2 \
  --size 20G -o /var/lib/libvirt/images/ocp-bastion-server.qcow2 \
  --root-password password:StrongRootPassw0rd`

  takes a while to complete
`[   4.7] Downloading: http://builder.libguestfs.org/fedora-37.xz
################################################################################################################################################################### 100.0%################################################################################################################################################################### 100.0%
[ 105.8] Planning how to build this image
[ 105.8] Uncompressing
[ 107.5] Resizing (using virt-resize) to expand the disk to 20.0G
[ 212.2] Opening the new disk
[ 232.1] Setting a random seed
[ 232.3] Setting passwords
[ 239.0] SELinux relabelling
[ 337.2] Finishing off
                   Output file: /var/lib/libvirt/images/ocp-bastion-server.qcow2
                   Output size: 20.0G
                 Output format: qcow2
            Total usable space: 19.9G
                    Free space: 18.5G (92%)
`

virt-install was missing
` $ sudo 	dnf install virt-install

the install it

`$ sudo virt-install \
  --name ocp-bastion-server \
  --ram 4096 \
  --vcpus 2 \
  --disk path=/var/lib/libvirt/images/ocp-bastion-server.qcow2 \
  --os-type linux \
  --os-variant rhel8.0 \
  --network bridge=openshift4 \
  --graphics none \
  --serial pty \
  --console pty \
  --boot hd \
  --import`

  takes a while'

`Fedora Linux 37 (Thirty Seven)
Kernel 6.0.9-300.fc37.x86_64 on an x86_64 (ttyS0)
fedora login: root
Password: StrongRootPassw0rd`

change password passwd

to ocp

`# ip link show
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: enp1s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
    link/ether 52:54:00:55:6d:ec brd ff:ff:ff:ff:ff:ff`

    vi /etc/sysconfig/network-scripts/ifcfg-enp1s0
NAME="enp1s0"            # Set network name, usually same as device name
DEVICE="enp1s0"          # Set your interface name as shown while running ip link show command 
ONBOOT="yes"
NETBOOT="yes"
BOOTPROTO="none"
TYPE="Ethernet"
PROXY_METHOD="none"
BROWSER_ONLY="no"
DEFROUTE="yes"
IPADDR=192.168.100.254     # Set your VM IP address
PREFIX=27                  # Set Netmask Prefix
GATEWAY=192.168.100.1      # Set network gateway IP address
DNS1=8.8.8.8               # Set first DNS server to be used
DNS2=8.8.4.4               # Set secondary DNS server to be used

`# ifup enp1s0`

`# ip link set dev enp1s0 up`

ip a add 192.168.100.254/27 dev enp1s0
ip addr add broadcast 192.168.100.1  dev enp1s0
ip a list enp1s0
ip route add 192.168.100.1 dev enp1s0
ip route add default via 192.168.100.1


