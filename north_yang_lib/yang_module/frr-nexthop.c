/* autogenerated by embedmodel.py.  DO NOT EDIT */

#include "northbound.h"

static const char model[] =
	"// SPDX-License-Identifier: BSD-2-Clause\n"
	"module frr-nexthop {\n"
	"  yang-version 1.1;\n"
	"  namespace \"http://frrouting.org/yang/nexthop\";\n"
	"  prefix frr-nexthop;\n"
	"\n"
	"  import ietf-inet-types {\n"
	"    prefix inet;\n"
	"  }\n"
	"\n"
	"  import ietf-routing-types {\n"
	"    prefix rt-types;\n"
	"  }\n"
	"\n"
	"  import frr-interface {\n"
	"    prefix frr-interface;\n"
	"  }\n"
	"\n"
	"  import frr-vrf {\n"
	"    prefix frr-vrf;\n"
	"  }\n"
	"\n"
	"  organization\n"
	"    \"FRRouting\";\n"
	"  contact\n"
	"    \"FRR Users List: <mailto:frog@lists.frrouting.org>\n"
	"     FRR Development List: <mailto:dev@lists.frrouting.org>\";\n"
	"  description\n"
	"    \"This module defines a model for managing FRR nexthop information.\n"
	"\n"
	"     Copyright 2020 FRRouting\n"
	"\n"
	"     Redistribution and use in source and binary forms, with or without\n"
	"     modification, are permitted provided that the following conditions\n"
	"     are met:\n"
	"\n"
	"     1. Redistributions of source code must retain the above copyright notice,\n"
	"     this list of conditions and the following disclaimer.\n"
	"\n"
	"     2. Redistributions in binary form must reproduce the above copyright\n"
	"     notice, this list of conditions and the following disclaimer in the\n"
	"     documentation and/or other materials provided with the distribution.\n"
	"\n"
	"     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS\n"
	"     \\\"AS IS\\\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT\n"
	"     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR\n"
	"     A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT\n"
	"     HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,\n"
	"     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT\n"
	"     LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,\n"
	"     DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY\n"
	"     THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n"
	"     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE\n"
	"     OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\";\n"
	"\n"
	"  revision 2019-08-15 {\n"
	"    description\n"
	"      \"Initial revision.\";\n"
	"  }\n"
	"\n"
	"  typedef optional-ip-address {\n"
	"    type union {\n"
	"      type inet:ip-address;\n"
	"      type string {\n"
	"        length \"0\";\n"
	"      }\n"
	"    }\n"
	"  }\n"
	"\n"
	"  /*\n"
	"   * Nexthop types.\n"
	"   */\n"
	"  typedef nexthop-type {\n"
	"    type enumeration {\n"
	"      enum \"ifindex\" {\n"
	"        value 1;\n"
	"        description\n"
	"          \"Specific interface.\";\n"
	"      }\n"
	"      enum \"ip4\" {\n"
	"        value 2;\n"
	"        description\n"
	"          \"IPv4 address.\";\n"
	"      }\n"
	"      enum \"ip4-ifindex\" {\n"
	"        value 3;\n"
	"        description\n"
	"          \"IPv4 address and interface.\";\n"
	"      }\n"
	"      enum \"ip6\" {\n"
	"        value 4;\n"
	"        description\n"
	"          \"IPv6 address.\";\n"
	"      }\n"
	"      enum \"ip6-ifindex\" {\n"
	"        value 5;\n"
	"        description\n"
	"          \"IPv6 address and interface.\";\n"
	"      }\n"
	"      enum \"blackhole\" {\n"
	"        value 6;\n"
	"        description\n"
	"          \"Unreachable or prohibited.\";\n"
	"      }\n"
	"    }\n"
	"    description\n"
	"      \"Nexthop types.\";\n"
	"  }\n"
	"\n"
	"  /*\n"
	"   * Blockhole nexthop type.\n"
	"   */\n"
	"  typedef blackhole-type {\n"
	"    type enumeration {\n"
	"      enum \"unspec\" {\n"
	"        value 0;\n"
	"        description\n"
	"          \"Generic unreachable.\";\n"
	"      }\n"
	"      enum \"null\" {\n"
	"        value 1;\n"
	"        description\n"
	"          \"Null type.\";\n"
	"      }\n"
	"      enum \"reject\" {\n"
	"        value 2;\n"
	"        description\n"
	"          \"ICMP unreachable.\";\n"
	"      }\n"
	"      enum \"prohibited\" {\n"
	"        value 3;\n"
	"        description\n"
	"          \"ICMP admin-prohibited.\";\n"
	"      }\n"
	"    }\n"
	"    default \"null\";\n"
	"    description\n"
	"      \"Nexthop blackhole types.\";\n"
	"  }\n"
	"\n"
	"  typedef nexthop-group-ref {\n"
	"    type leafref {\n"
	"      path \"/frr-nexthop:frr-nexthop-group/frr-nexthop:nexthop-groups/frr-nexthop:name\";\n"
	"      require-instance false;\n"
	"    }\n"
	"  }\n"
	"\n"
	"  /*\n"
	"   * Common nexthop attributes grouping.\n"
	"   */\n"
	"  grouping frr-nexthop-attributes {\n"
	"    leaf nh-type {\n"
	"      type nexthop-type;\n"
	"      mandatory true;\n"
	"      description\n"
	"        \"The nexthop type.\";\n"
	"    }\n"
	"\n"
	"    leaf vrf {\n"
	"      type frr-vrf:vrf-ref;\n"
	"      description\n"
	"        \"The nexthop vrf name, if different from the route.\";\n"
	"    }\n"
	"\n"
	"    leaf gateway {\n"
	"      type frr-nexthop:optional-ip-address;\n"
	"      description\n"
	"        \"The nexthop gateway address.\";\n"
	"    }\n"
	"\n"
	"    leaf interface {\n"
	"      type frr-interface:interface-ref;\n"
	"      description\n"
	"        \"The nexthop egress interface.\";\n"
	"    }\n"
	"\n"
	"    leaf bh-type {\n"
	"      type blackhole-type;\n"
	"      description\n"
	"        \"A blackhole sub-type, if the nexthop is a blackhole type.\";\n"
	"    }\n"
	"\n"
	"    leaf onlink {\n"
	"      type boolean;\n"
	"      default \"false\";\n"
	"      description\n"
	"        \"Nexthop is directly connected.\";\n"
	"    }\n"
	"\n"
	"    leaf srte-color {\n"
	"      when \"../nh-type = \'ip4\' or\n"
	"            ../nh-type = \'ip6\' or\n"
	"            ../nh-type = \'ip4-ifindex\' or\n"
	"            ../nh-type = \'ip6-ifindex\'\";\n"
	"      type uint32;\n"
	"      description\n"
	"        \"The nexthop SR-TE color\";\n"
	"    }\n"
	"\n"
	"    uses rt-types:mpls-label-stack {\n"
	"      description\n"
	"        \"Nexthop\'s MPLS label stack.\";\n"
	"    }\n"
	"\n"
	"    uses srv6-segs-stack {\n"
	"      description\n"
	"        \"Nexthop\'s SRv6 segs SIDs stack.\";\n"
	"    }\n"
	"  }\n"
	"\n"
	"  /*\n"
	"   * operational common attributes for nexthop\n"
	"   */\n"
	"  grouping frr-nexthop-operational {\n"
	"    leaf duplicate {\n"
	"      type empty;\n"
	"      config false;\n"
	"      description\n"
	"        \"Duplicate nexthop\";\n"
	"    }\n"
	"\n"
	"    leaf recursive {\n"
	"      type empty;\n"
	"      config false;\n"
	"      description\n"
	"        \"Nexthop resolved through another gateway.\";\n"
	"    }\n"
	"\n"
	"    leaf active {\n"
	"      type empty;\n"
	"      config false;\n"
	"      description\n"
	"        \"Nexthop is active.\";\n"
	"    }\n"
	"\n"
	"    leaf fib {\n"
	"      type empty;\n"
	"      config false;\n"
	"      description\n"
	"        \"Nexthop is installed in fib.\";\n"
	"    }\n"
	"\n"
	"    leaf weight {\n"
	"      type uint8;\n"
	"      config false;\n"
	"      description\n"
	"        \"Weight to be used by the nexthop for purposes of ECMP\";\n"
	"    }\n"
	"  }\n"
	"\n"
	"  grouping nexthop-grouping {\n"
	"    list nexthop {\n"
	"      key \"nh-type vrf gateway interface\";\n"
	"      min-elements 1;\n"
	"      description\n"
	"        \"A list of nexthop objects.\";\n"
	"      uses frr-nexthop-attributes;\n"
	"    }\n"
	"  }\n"
	"\n"
	"  /*\n"
	"   * Single nexthop grouping.\n"
	"   */\n"
	"  grouping frr-nexthop {\n"
	"    container frr-nexthops {\n"
	"      description\n"
	"        \"FRR nexthop object.\";\n"
	"      uses nexthop-grouping;\n"
	"    }\n"
	"  }\n"
	"\n"
	"\n"
	"  /*\n"
	"   * Container for FRR nexthop group.\n"
	"   */\n"
	"  grouping frr-nexthop-grouping {\n"
	"    list nexthop-groups {\n"
	"      key \"name\";\n"
	"      description\n"
	"        \"List of nexthop groups, each contains group of nexthops\";\n"
	"      leaf name {\n"
	"        type string;\n"
	"        description\n"
	"          \"The nexthop-group name.\";\n"
	"      }\n"
	"\n"
	"      uses frr-nexthop;\n"
	"    }\n"
	"  }\n"
	"\n"
	"  /* Operational nexthop-group */\n"
	"  grouping frr-nexthop-group-operational {\n"
	"    container nexthop-group {\n"
	"      description\n"
	"        \"A group of nexthops.\";\n"
	"      leaf id {\n"
	"        type uint32;\n"
	"        description\n"
	"          \"The nexthop-group id.\";\n"
	"      }\n"
	"\n"
	"      uses nexthop-grouping;\n"
	"    }\n"
	"  }\n"
	"\n"
	"  /* Contaner for SRv6 segs SIDs */\n"
	"  grouping srv6-segs-stack {\n"
	"    description\n"
	"      \"This grouping specifies an SRv6 segs SIDs stack. The segs\n"
	"      SIDs stack is encoded as a list of SID entries. The\n"
	"      list key is an identifier that indicates the relative\n"
	"      ordering of each entry.\";\n"
	"      container srv6-segs-stack {\n"
	"        description\n"
	"          \"Container for a list of SRv6 segs SIDs entries.\";\n"
	"          list entry {\n"
	"            key \"id\";\n"
	"            description\n"
	"              \"List of SRv6 segs SIDs entries.\";\n"
	"\t    leaf id {\n"
	"              type uint8;\n"
	"              description\n"
	"              \"Identifies the entry in a sequence of SRv6 segs SIDs\n"
	"              entries.\";\n"
	"            }\n"
	"            leaf seg {\n"
	"                type inet:ipv6-address;\n"
	"            }\n"
	"          }\n"
	"      }\n"
	"  }\n"
	"  container frr-nexthop-group {\n"
	"    description\n"
	"      \"A nexthop-group, represented as a list of nexthop objects.\";\n"
	"    uses frr-nexthop-grouping;\n"
	"  }\n"
	"\n"
	"  /*\n"
	"   * Augment weight attributes to nexthop group.\n"
	"   */\n"
	"  augment \"/frr-nexthop-group/nexthop-groups/frr-nexthops/nexthop\" {\n"
	"    leaf weight {\n"
	"      type uint8;\n"
	"      description\n"
	"        \"Weight to be used by the nexthop for purposes of ECMP\";\n"
	"    }\n"
	"  }\n"
	"}\n"
	"";

static struct yang_module_embed embed = {
	.mod_name = "frr-nexthop",
	.mod_rev = "2019-08-15",
	.sub_mod_name = "",
	.sub_mod_rev = "",
	.data = model,
	.format = LYS_IN_YANG,
};

static void embed_register(void) __attribute__((_CONSTRUCTOR(2000)));
static void embed_register(void)
{
	yang_module_embed(&embed);
}
