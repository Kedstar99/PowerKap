#!/bin/bash

echo "Welcome to my script to create a test interface:"
echo "First please pick a number corresponding to the interface you wish 
to bind."
ip link show |grep UP

 if [ "$EUID" -ne 0 ]
 	then echo "Please run as root"
 	exit
 fi

read -n 1 -p "Input Selection:" interface

input="${interface}:"

stringInterface="$(ip link show |grep $input)"

cutVal=${stringInterface#*:}
chosenInterface=${cutVal%%:*}

sysctl -w net.ipv4.ip_forward=1

ip netns add test_ns

ip link add veth-a type veth peer name veth-b

ip link set veth-a netns test_ns

ip netns exec test_ns ip addr add 192.168.163.1/24 broadcast 192.168.163.255 dev veth-a
ip netns exec test_ns ip link set dev veth-a up


ip netns exec test_ns ip link set dev lo up

ip addr add 192.168.163.254/24 broadcast 192.168.163.255 dev veth-b
ip link set dev veth-b up

ip netns exec test_ns ip route add default via 192.168.163.254 dev veth-a

localIP="$(hostname -I)"

getIP=${localIP%% *}

echo $chosenInterface
echo ${getIP}
iptables -t nat -A POSTROUTING -s 192.168.163.0/24 -o $chosenInterface -j SNAT --to-source $getIP
iptables -A FORWARD -i $chosenInterface -o veth-b -j ACCEPT
iptables -A FORWARD -o $chosenInterface -i veth-b -j ACCEPT