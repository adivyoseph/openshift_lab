# Install instructions

Assume all nodes running Fedora 37 WorkStation addition.

each node has two network interfaces:
- Wifi connected via DHCP to home network 192.168.2.x
- GE ethernet port, cluster underlay network 10.10.1.x

Example K8 laptop 

`wlp3s0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.2.159  netmask 255.255.255.0  broadcast 192.168.2.255
        inet6 fe80::776d:f01c:6ab7:d9aa  prefixlen 64  scopeid 0x20<link>
        ether 28:b2:bd:5a:61:8a  txqueuelen 1000  (Ethernet)
`

`enp0s25: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet6 fe80::40cd:4b36:3605:ebf0  prefixlen 64  scopeid 0x20<link>
        ether 28:d2:44:c2:c7:ee  txqueuelen 1000  (Ethernet)`


## network setup
`$sudu ip addr add 10.10.1.1/24 dev enp0s25`

Ping to check.
Set perminantly via GUI or ncli.


On each node do the following

sudo rpm --import https://packages.cloud.google.com/yum/doc/yum-key.gpg https://packages.cloud.google.com/yum/doc/rpm-package-key.gpg
sudo dnf update

sudo dnf module -y install cri-o:1.24/default
sudo dnf -y install iptables-legacy 
sudo systemctl enable --now crio 

sudo dnf -y install kubernetes-kubeadm kubernetes-node kubernetes-client cri-tools iproute-tc container-selinux

sudo nano /etc/kubernetes/kubelet
# line 5 : change

KUBELET_ADDRESS="--address=0.0.0.0
"
# line 8 : uncomment

KUBELET_PORT="--port=10250"
# line 11 : change to your hostname

KUBELET_HOSTNAME="--hostname-override=dlp.srv.world"


sudo nano /etc/systemd/system/kubelet.service.d/kubeadm.conf
# line 6 : add

Environment="KUBELET_EXTRA_ARGS=--cgroup-driver=systemd --container-runtime=remote --container-runtime-endpoint=unix:///var/run/crio/crio.sock
"

sudo systemctl enable kubelet 

sudo nano /etc/sysctl.d/kubernetes.conf
kernel.printk=4
net.ipv4.ip_forward=1
net.bridge.bridge-nf-call-iptables=1
net.bridge.bridge-nf-call-ip6tables=1

sudo nano /etc/yum.repos.d/kubernetes.repo
[kubernetes]
name=Kubernetes
baseurl=https://packages.cloud.google.com/yum/repos/kubernetes-el7-$basearch
enabled=0
gpgcheck=1
repo_gpgcheck=1
gpgkey=https://packages.cloud.google.com/yum/doc/yum-key.gpg https://packages.cloud.google.com/yum/doc/rpm-package-key.gpg 

  253  sudo yum import gpgkey=https://packages.cloud.google.com/yum/doc/yum-key.gpg https://packages.cloud.google.com/yum/doc/rpm-package-key.gpg
  254  sudo rpm import https://packages.cloud.google.com/yum/doc/yum-key.gpg https://packages.cloud.google.com/yum/doc/rpm-package-key.gpg

sudo dnf module -y install cri-o:1.24/default
sudo dnf -y install iptables-legacy 
sudo systemctl enable --now crio 
sudo dnf -y install kubernetes-kubeadm kubernetes-node kubernetes-client cri-tools iproute-tc container-selinux

sudo nano /etc/kubernetes/kubelet
##
# kubernetes kubelet (minion) config

# The address for the info server to serve on (set to 0.0.0.0 or "" for all interfaces)
KUBELET_ADDRESS="--address=0.0.0.0"

# The port for the info server to serve on
KUBELET_PORT="--port=10250"

# You may leave this blank to use the actual hostname
KUBELET_HOSTNAME="--hostname-override=dlp.srv.world"

# Edit the kubelet.kubeconfig to have correct cluster server address
KUBELET_KUBECONFIG="--kubeconfig=/etc/kubernetes/kubelet.kubeconfig"

# Add your own!
KUBELET_ARGS="--cgroup-driver=systemd --fail-swap-on=false"


sudo nano /etc/systemd/system/kubelet.service.d/kubeadm.conf
[Service]
Environment="KUBELET_KUBECONFIG_ARGS=--bootstrap-kubeconfig=/etc/kubernetes/bootstrap-kubelet.conf --kubeconfig=/etc/kubernetes/kubelet.conf --fail-swap-on=false"
Environment="KUBELET_SYSTEM_PODS_ARGS=--pod-manifest-path=/etc/kubernetes/manifests"
Environment="KUBELET_DNS_ARGS=--cluster-dns=10.96.0.10 --cluster-domain=cluster.local"
Environment="KUBELET_AUTHZ_ARGS=--authorization-mode=Webhook --client-ca-file=/etc/kubernetes/pki/ca.crt"
Environment="KUBELET_EXTRA_ARGS=--cgroup-driver=systemd"
#add line below
Environment="KUBELET_EXTRA_ARGS=--cgroup-driver=systemd --container-runtime=remote --container-runtime-endpoint=unix:///var/run/crio/crio.sock"
# This is a file that "kubeadm init" and "kubeadm join" generates at runtime, populating the KUBELET_KUBEADM_ARGS variable dynamically
EnvironmentFile=-/var/lib/kubelet/kubeadm-flags.env

ExecStart=
ExecStart=/usr/bin/kubelet $KUBELET_KUBECONFIG_ARGS $KUBELET_SYSTEM_PODS_ARGS $KUBELET_DNS_ARGS $KUBELET_AUTHZ_ARGS $KUBELET_EXTRA_ARGS $KUBELET_KUBEADM_ARGS

Restart=always
StartLimitInterval=0
RestartSec=10

sudo systemctl enable kubelet 
sudo nano k8s.te
sudo checkmodule -m -M -o k8s.mod k8s.te 
sudo semodule_package --outfile k8s.pp --module k8s.mod
sudo semodule -i k8s.pp 


sudo dnf install podman skopeo buildah runc cri-o cri-tools     containernetworking-plugins bridge-utils telnet jq
sudo kubeadm

sudo systemctl daemon-reload
sudo systemctl enable --now crio
sudo systemctl enable --now kubelet
sudo systemctl disable --now firewalld

  308  free -m
  309  sudo swap -s
  310  sudo swapon -s
  311  free -m
sudo dnf remove zram-generator-defaults
  313  free -m
  314  sudo swapon -s
sudo swapoff -a && sudo sysctl -w vm.swappiness=0
  316  free -m
sudo sysctl -p

sudo lsmod|grep netfilt
sudo modprobe br_netfilter
sudo lsmod|grep netfilter
sudo kubeadm init 	--pod-network-cidr=10.85.0.0/16 	--kubernetes-version=${version} 	--ignore-preflight-errors=Swap



## finally

``Your Kubernetes control-plane has initialized successfully!
 
To start using your cluster, you need to run the following as a regular user:

   mkdir -p $HOME/.kube
  sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
  sudo chown $(id -u):$(id -g) $HOME/.kube/config

Alternatively, if you are the root user, you can run:

  export KUBECONFIG=/etc/kubernetes/admin.conf

You should now deploy a pod network to the cluster.
Run "kubectl apply -f [podnetwork].yaml" with one of the options listed at:
  https://kubernetes.io/docs/concepts/cluster-administration/addons/

Then you can join any number of worker nodes by running the following on each as root:

kubeadm join 192.168.2.159:6443 --token wgheve.jz8l70oc6pq2p7ww \
	--discovery-token-ca-cert-hash sha256:cd24c825f8e79064c37f24b96eccb7fcf29a1e141b85891857300c9e1ed43dfb 
    ``


## client
kubeadm join 192.168.2.159:6443 --token wgheve.jz8l70oc6pq2p7ww \
        --discovery-token-ca-cert-hash sha256:cd24c825f8e79064c37f24b96eccb7fcf29a1e141b85891857300c9e1ed43dfb
[preflight] Running pre-flight checks
error execution phase preflight: [preflight] Some fatal errors occurred:
	[ERROR IsPrivilegedUser]: user is not running as root
[preflight] If you know what you are doing, you can make a check non-fatal with `--ignore-preflight-errors=...`
To see the stack trace of this error execute with --v=5 or higher
[martin@localhost-live k8_only]$ sudo kubeadm join 192.168.2.159:6443 --token wgheve.jz8l70oc6pq2p7ww \
        --discovery-token-ca-cert-hash sha256:cd24c825f8e79064c37f24b96eccb7fcf29a1e141b85891857300c9e1ed43dfb
[preflight] Running pre-flight checks
[preflight] Reading configuration from the cluster...
[preflight] FYI: You can look at this config file with 'kubectl -n kube-system get cm kubeadm-config -o yaml'
[kubelet-start] Writing kubelet configuration to file "/var/lib/kubelet/config.yaml"
[kubelet-start] Writing kubelet environment file with flags to file "/var/lib/kubelet/kubeadm-flags.env"
[kubelet-start] Starting the kubelet
[kubelet-start] Waiting for the kubelet to perform the TLS Bootstrap...

This node has joined the cluster:
* Certificate signing request was sent to apiserver and a response was received.
* The Kubelet was informed of the new secure connection details.

Run 'kubectl get nodes' on the control-plane to see this node join the cluster.


