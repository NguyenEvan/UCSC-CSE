#!/usr/bin/python

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel
from mininet.cli import CLI
from mininet.node import RemoteController

class final_topo(Topo):
  def build(self):
    h101 = self.addHost('h101', mac='00:00:00:00:11:01', ip='128.114.1.101/24', defaultRoute="h101-eth0")
    h102 = self.addHost('h102', mac='00:00:00:00:11:02', ip='128.114.1.102/24', defaultRoute="h102-eth0")
    h103 = self.addHost('h103', mac='00:00:00:00:11:03', ip='128.114.1.103/24', defaultRoute="h103-eth0")
    h104 = self.addHost('h104', mac='00:00:00:00:11:04', ip='128.114.1.104/24', defaultRoute="h104-eth0")

    h201 = self.addHost('h201', mac='00:00:00:00:22:01', ip='128.114.2.201/24', defaultRoute="h201-eth0")
    h202 = self.addHost('h202', mac='00:00:00:00:22:02', ip='128.114.2.202/24', defaultRoute="h202-eth0")
    h203 = self.addHost('h203', mac='00:00:00:00:22:03', ip='128.114.2.203/24', defaultRoute="h203-eth0")
    h204 = self.addHost('h204', mac='00:00:00:00:22:04', ip='128.114.2.204/24', defaultRoute="h204-eth0")

    h_trust = self.addHost('h_trust', mac='00:00:00:00:00:01', ip='192.47.38.109/24', defaultRoute="h_trust-eth0")
    h_untrust = self.addHost('h_untrust', mac='00:00:00:00:00:02', ip='108.35.24.113/24', defaultRoute="h_untrust-eth0")
    h_server = self.addHost('h_server', mac='00:00:00:00:00:03', ip='128.114.3.178/24', defaultRoute="h_server-eth0")

    s1_floor1 = self.addSwitch('s1_floor1', dpid="0000000000000001")
    s2_floor1 = self.addSwitch('s2_floor1', dpid="0000000000000002")
    s1_floor2 = self.addSwitch('s1_floor2', dpid="0000000000000003")
    s2_floor2 = self.addSwitch('s2_floor2', dpid="0000000000000004")
    s_dc = self.addSwitch('s_dc', dpid="0000000000000005")
    s_core = self.addSwitch('s_core', dpid="0000000000000006")


    self.addLink(s1_floor1, h101, port1=1, port2=0)
    self.addLink(s1_floor1, h102, port1=2, port2=0)
    self.addLink(s2_floor1, h103, port1=1, port2=0)
    self.addLink(s2_floor1, h104, port1=2, port2=0)

    self.addLink(s1_floor2, h201, port1=1, port2=0)
    self.addLink(s1_floor2, h202, port1=2, port2=0)
    self.addLink(s2_floor2, h203, port1=1, port2=0)
    self.addLink(s2_floor2, h204, port1=2, port2=0)

    self.addLink(s_dc, h_server, port1=1, port2=0)

    self.addLink(s_core, h_trust, port1=1, port2=0)
    self.addLink(s_core, h_untrust, port1=2, port2=0)

    self.addLink(s_core, s1_floor1, port1=3, port2=3)
    self.addLink(s_core, s2_floor1, port1=4, port2=3)
    self.addLink(s_core, s1_floor2, port1=5, port2=3)
    self.addLink(s_core, s2_floor2, port1=6, port2=3)
    self.addLink(s_core, s_dc, port1=7, port2=3)



def configure():
  topo = final_topo()
  net = Mininet(topo=topo, controller=RemoteController)
  net.start()

  CLI(net)
  
  net.stop()


if __name__ == '__main__':
  configure()
