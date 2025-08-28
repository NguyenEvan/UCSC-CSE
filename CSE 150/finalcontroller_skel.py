# Final Skeleton
from pox.core import core
import pox.openflow.libopenflow_01 as of

log = core.getLogger()

class Final (object):
  """
  A Firewall object is created for each switch that connects.
  A Connection object for that switch is passed to the __init__ function.
  """
  def __init__ (self, connection):
    # Keep track of the connection to the switch so that we can
    # send it messages!
    self.connection = connection

    # This binds our PacketIn event listener
    connection.addListeners(self)
  
  def add_flow(self, packet, packet_in, out_port=None):
    msg = of.ofp_flow_mod()
    msg.match = of.ofp_match.from_packet(packet)
    msg.idle_timeout = 60
    msg.hard_timeout = 60
    if out_port is not None:
        msg.actions.append(of.ofp_action_output(port=out_port))
    msg.data = packet_in
    # msg.data = packet  # send the original packet in case switch was waiting
    self.connection.send(msg)


  def do_final (self, packet, packet_in, port_on_switch, switch_id):
    # This is where you'll put your code. The following modifications have 
    # been made from Lab 3:
    #   - port_on_switch: represents the port that the packet was received on.
    #   - switch_id represents the id of the switch that received the packet.
    #      (for example, s1 would have switch_id == 1, s2 would have switch_id == 2, etc...)
    # You should use these to determine where a packet came from. To figure out where a packet 
    # is going, you can use the IP header information.
    dept_a = {"128.114.1.101", "128.114.1.102", "128.114.1.103", "128.114.1.104"}
    dept_b = {"128.114.2.201", "128.114.2.202", "128.114.2.203", "128.114.2.204"}
    llm_server = "128.114.3.178"
    trusted_host = "192.47.38.109"
    untrusted_host = "108.35.24.113"

    routing_table = {
      1: { 
          "128.114.1.101": 1,
          "128.114.1.102": 2
      },
      2: {  
          "128.114.1.103": 1,
          "128.114.1.104": 2
      },
      3: {
          "128.114.2.201": 1,
          "128.114.2.202": 2
      },
      4: {  
          "128.114.2.203": 1,
          "128.114.2.204": 2
      },
      5: {  
          "128.114.3.178": 1  
      },
      6: {  
          "192.47.38.109": 1,  
          "108.35.24.113": 2, 
          "128.114.1.101": 3,
          "128.114.1.102": 3,
          "128.114.1.103": 4,
          "128.114.1.104": 4,
          "128.114.2.201": 5,
          "128.114.2.202": 5,
          "128.114.2.203": 6,
          "128.114.2.204": 6,
          "128.114.3.178": 7
      }
    }

    if switch_id == 6: 
      if packet.type != packet.IP_TYPE:
        self.add_flow(packet, packet_in, of.OFPP_FLOOD)
        return
      else:
        ip_header = packet.find('ipv4')
        src = str(ip_header.srcip)
        dst = str(ip_header.dstip)

        # Block all IP traffic to LLM server from untrusted/trusted
        if dst == llm_server and src in {untrusted_host, trusted_host}:
          self.add_flow(packet, packet_in, None) 
          return

        # Prevent IP traffic between trusted/untrusted host to LLM server
        if (src == untrusted_host or src == trusted_host) and dst == llm_server:
          self.add_flow(packet, packet_in, None)
          return

        # Block all ICMP traffic from untrusted to any internal device
        if ip_header.protocol == ip_header.ICMP_PROTOCOL:
            if src == untrusted_host and (dst in dept_a or dst in dept_b or dst == llm_server):
              self.add_flow(packet, packet_in, None)  
              return
            if src == trusted_host and (dst in dept_b or dst == llm_server):
              self.add_flow(packet, packet_in, None)
              return
            if (src in dept_a and dst in dept_b) or (src in dept_b and dst in dept_a):
              self.add_flow(packet, packet_in, None)  
              return

    if packet.type != packet.IP_TYPE:
      self.add_flow(packet, packet_in, of.OFPP_FLOOD)
    else:
      ip_header = packet.find('ipv4')
      src = str(ip_header.srcip)
      dst = str(ip_header.dstip)
      
      curr_routing = routing_table.get(switch_id, {})
      output_port = curr_routing.get(dst)
      
      if output_port is not None:
        self.add_flow(packet, packet_in, output_port)
      else:
        self.add_flow(packet, packet_in, 3)

  

  def _handle_PacketIn (self, event):
    """
    Handles packet in messages from the switch.
    """
    packet = event.parsed # This is the parsed packet data.
    if not packet.parsed:
      log.warning("Ignoring incomplete packet")
      return

    packet_in = event.ofp # The actual ofp_packet_in message.
    self.do_final(packet, packet_in, event.port, event.dpid)

def launch ():
  """
  Starts the component
  """
  def start_switch (event):
    log.debug("Controlling %s" % (event.connection,))
    Final(event.connection)
  core.openflow.addListenerByName("ConnectionUp", start_switch)
