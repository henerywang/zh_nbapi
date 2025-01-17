/* autogenerated by embedmodel.py.  DO NOT EDIT */

#include "northbound.h"

static const char model[] =
	"// SPDX-License-Identifier: BSD-2-Clause\n"
	"module frr-ripd {\n"
	"  yang-version 1.1;\n"
	"  namespace \"http://frrouting.org/yang/ripd\";\n"
	"  prefix frr-ripd;\n"
	"\n"
	"  import ietf-inet-types {\n"
	"    prefix inet;\n"
	"  }\n"
	"  import ietf-yang-types {\n"
	"    prefix yang;\n"
	"  }\n"
	"  import frr-if-rmap {\n"
	"    prefix frr-if-rmap;\n"
	"  }\n"
	"  import frr-bfdd {\n"
	"    prefix frr-bfdd;\n"
	"  }\n"
	"  import frr-filter {\n"
	"    prefix frr-filter;\n"
	"  }\n"
	"  import frr-interface {\n"
	"    prefix frr-interface;\n"
	"  }\n"
	"  import frr-nexthop {\n"
	"    prefix frr-nexthop;\n"
	"  }\n"
	"  import frr-vrf {\n"
	"    prefix frr-vrf;\n"
	"  }\n"
	"  import frr-route-map {\n"
	"    prefix frr-route-map;\n"
	"  }\n"
	"  import frr-route-types {\n"
	"    prefix frr-route-types;\n"
	"  }\n"
	"\n"
	"  organization\n"
	"    \"FRRouting\";\n"
	"  contact\n"
	"    \"FRR Users List:       <mailto:frog@lists.frrouting.org>\n"
	"     FRR Development List: <mailto:dev@lists.frrouting.org>\";\n"
	"  description\n"
	"    \"This module defines a model for managing FRR ripd daemon.\n"
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
	"  revision 2019-09-09 {\n"
	"    description\n"
	"      \"Changed interface references to use\n"
	"      frr-interface:interface-ref typedef\";\n"
	"    reference \"FRRouting\";\n"
	"  }\n"
	"  revision 2017-12-06 {\n"
	"    description\n"
	"      \"Initial revision.\";\n"
	"    reference\n"
	"      \"RFC 1058: Routing Information Protocol.\n"
	"       RFC 2453: RIP Version 2.\";\n"
	"  }\n"
	"\n"
	"  typedef rip-route-type {\n"
	"    type enumeration {\n"
	"      enum normal {\n"
	"        value 0;\n"
	"        description \"Normal RIP route type.\";\n"
	"      }\n"
	"      enum static {\n"
	"        value 1;\n"
	"        description \"Static RIP route type.\";\n"
	"      }\n"
	"      enum default {\n"
	"        value 2;\n"
	"        description \"Default RIP route type.\";\n"
	"      }\n"
	"      enum redistribute {\n"
	"        value 3;\n"
	"        description \"Redistribute RIP route type.\";\n"
	"      }\n"
	"      enum interface {\n"
	"        value 4;\n"
	"        description \"Interface RIP route type.\";\n"
	"      }\n"
	"    }\n"
	"    description\n"
	"      \"Types of RIP routes.\";\n"
	"  }\n"
	"\n"
	"  container ripd {\n"
	"    description \"rip routing instance data\";\n"
	"    list instance {\n"
	"      key \"vrf\";\n"
	"      description\n"
	"        \"RIP routing instance.\";\n"
	"\n"
	"      leaf vrf {\n"
	"        type frr-vrf:vrf-ref;\n"
	"        description\n"
	"          \"VRF name.\";\n"
	"      }\n"
	"      leaf allow-ecmp {\n"
	"        type uint8;\n"
	"        default 0;\n"
	"        description\n"
	"          \"Allow equal-cost multi-path.\";\n"
	"      }\n"
	"      leaf default-information-originate {\n"
	"        type boolean;\n"
	"        default \"false\";\n"
	"        description\n"
	"          \"Control distribution of default route.\";\n"
	"      }\n"
	"      leaf default-metric {\n"
	"        type uint8 {\n"
	"          range \"1..16\";\n"
	"        }\n"
	"        default \"1\";\n"
	"        description\n"
	"          \"Default metric of redistributed routes.\";\n"
	"      }\n"
	"      container distance {\n"
	"        description\n"
	"          \"Administrative distance.\";\n"
	"        leaf default {\n"
	"          type uint8 {\n"
	"            range \"0..255\";\n"
	"          }\n"
	"          default \"0\";\n"
	"          description\n"
	"            \"Default administrative distance.\";\n"
	"        }\n"
	"        list source {\n"
	"          key \"prefix\";\n"
	"          description\n"
	"            \"Custom administrative distance per IP prefix.\";\n"
	"          leaf prefix {\n"
	"            type inet:ipv4-prefix;\n"
	"            description\n"
	"              \"Prefix.\";\n"
	"          }\n"
	"          leaf distance {\n"
	"            type uint8 {\n"
	"              range \"1..255\";\n"
	"            }\n"
	"            mandatory true;\n"
	"            description\n"
	"              \"Administrative distance.\";\n"
	"          }\n"
	"          leaf access-list {\n"
	"            type string;\n"
	"            description\n"
	"              \"Access-list name.\";\n"
	"          }\n"
	"        }\n"
	"      }\n"
	"      leaf-list explicit-neighbor {\n"
	"        type inet:ipv4-address;\n"
	"        description\n"
	"          \"Specifies the RIP neighbors. Useful for a\n"
	"           non-broadcast multiple access (NBMA) network.\";\n"
	"      }\n"
	"      leaf-list network {\n"
	"        type inet:ipv4-prefix;\n"
	"        description\n"
	"          \"Enable RIP on the specified IP network.\";\n"
	"      }\n"
	"      leaf-list interface {\n"
	"        type frr-interface:interface-ref;\n"
	"        description\n"
	"          \"Enable RIP on the specified interface.\";\n"
	"      }\n"
	"      list offset-list {\n"
	"        key \"interface direction\";\n"
	"        description\n"
	"          \"Offset-list to modify route metric.\";\n"
	"        leaf interface {\n"
	"          type union {\n"
	"            type frr-interface:interface-ref;\n"
	"            type enumeration {\n"
	"              enum \'*\' {\n"
	"                description\n"
	"                  \"Match all interfaces.\";\n"
	"              }\n"
	"            }\n"
	"          }\n"
	"          description\n"
	"            \"Interface to match. Use \'*\' to match all interfaces.\";\n"
	"        }\n"
	"        leaf direction {\n"
	"          type enumeration {\n"
	"            enum in {\n"
	"              value 0;\n"
	"              description\n"
	"                \"Incoming updates.\";\n"
	"            }\n"
	"            enum out {\n"
	"              value 1;\n"
	"              description\n"
	"                \"Outgoing updates.\";\n"
	"            }\n"
	"          }\n"
	"          description\n"
	"            \"Incoming or outgoing updates.\";\n"
	"        }\n"
	"        leaf access-list {\n"
	"          type string;\n"
	"          mandatory true;\n"
	"          description\n"
	"            \"Access-list name.\";\n"
	"        }\n"
	"        leaf metric {\n"
	"          type uint8 {\n"
	"            range \"0..16\";\n"
	"          }\n"
	"          mandatory true;\n"
	"          description\n"
	"            \"Route metric.\";\n"
	"        }\n"
	"      }\n"
	"      leaf passive-default {\n"
	"        type boolean;\n"
	"        default \"false\";\n"
	"        description\n"
	"          \"Control whether interfaces are in the passive mode\n"
	"           by default or not.\";\n"
	"      }\n"
	"      leaf-list passive-interface {\n"
	"        when \"../passive-default = \'false\'\";\n"
	"        type frr-interface:interface-ref;\n"
	"        description\n"
	"          \"A list of interfaces where the sending of RIP packets\n"
	"           is disabled.\";\n"
	"      }\n"
	"      leaf-list non-passive-interface {\n"
	"        when \"../passive-default = \'true\'\";\n"
	"        type frr-interface:interface-ref;\n"
	"        description\n"
	"          \"A list of interfaces where the sending of RIP packets\n"
	"           is enabled.\";\n"
	"      }\n"
	"\n"
	"      uses frr-filter:distribute-list-group;\n"
	"\n"
	"      list redistribute {\n"
	"        key \"protocol\";\n"
	"        description\n"
	"          \"Redistributes routes learned from other routing protocols.\";\n"
	"        leaf protocol {\n"
	"          type frr-route-types:frr-route-types-v4;\n"
	"          must \'. != \"rip\"\';\n"
	"          description\n"
	"            \"Routing protocol.\";\n"
	"        }\n"
	"        leaf route-map {\n"
	"          type frr-route-map:route-map-ref;\n"
	"          description\n"
	"            \"Applies the conditions of the specified route-map to\n"
	"             routes that are redistributed into the RIP routing\n"
	"             instance.\";\n"
	"        }\n"
	"        leaf metric {\n"
	"          type uint8 {\n"
	"            range \"0..16\";\n"
	"          }\n"
	"          description\n"
	"            \"Metric used for the redistributed route. If a metric is\n"
	"             not specified, the metric configured with the\n"
	"             default-metric attribute in RIP router configuration is\n"
	"             used. If the default-metric attribute has not been\n"
	"             configured, the default metric for redistributed routes\n"
	"             is 0.\";\n"
	"        }\n"
	"      }\n"
	"\n"
	"      uses frr-if-rmap:if-route-maps-group;\n"
	"\n"
	"      leaf-list static-route {\n"
	"        type inet:ipv4-prefix;\n"
	"        description\n"
	"          \"RIP static routes.\";\n"
	"      }\n"
	"      container timers {\n"
	"        description\n"
	"          \"Settings of basic timers\";\n"
	"        leaf flush-interval {\n"
	"          type uint32 {\n"
	"            range \"5..2147483647\";\n"
	"          }\n"
	"          units \"seconds\";\n"
	"          default \"120\";\n"
	"          description\n"
	"            \"Interval before a route is flushed from the routing\n"
	"             table.\";\n"
	"        }\n"
	"        leaf holddown-interval {\n"
	"          type uint32 {\n"
	"            range \"5..2147483647\";\n"
	"          }\n"
	"          units \"seconds\";\n"
	"          default \"180\";\n"
	"          description\n"
	"            \"Interval before better routes are released.\";\n"
	"        }\n"
	"        leaf update-interval {\n"
	"          type uint32 {\n"
	"            range \"5..2147483647\";\n"
	"          }\n"
	"          units \"seconds\";\n"
	"          default \"30\";\n"
	"          description\n"
	"            \"Interval at which RIP updates are sent.\";\n"
	"        }\n"
	"      }\n"
	"      container version {\n"
	"        description \"version of rip\";\n"
	"        leaf receive {\n"
	"          type enumeration {\n"
	"            enum \"1\" {\n"
	"              value 1;\n"
	"              description\n"
	"                \"Accept RIPv1 updates only.\";\n"
	"            }\n"
	"            enum \"2\" {\n"
	"              value 2;\n"
	"              description\n"
	"                \"Accept RIPv2 updates only.\";\n"
	"            }\n"
	"            enum \"1-2\" {\n"
	"              value 3;\n"
	"              description\n"
	"                \"Accept both RIPv1 and RIPv2 updates.\";\n"
	"            }\n"
	"          }\n"
	"          must\n"
	"            \'(. = \"1\" and ../send = \"1\") or \' +\n"
	"            \'(. = \"2\" and ../send = \"2\") or \' +\n"
	"            \'(. = \"1-2\" and ../send = \"2\")\';\n"
	"          default \"1-2\";\n"
	"          description\n"
	"            \"Advertisement reception - Version control.\";\n"
	"        }\n"
	"        leaf send {\n"
	"          type enumeration {\n"
	"            enum \"1\" {\n"
	"              value 1;\n"
	"              description\n"
	"                \"Send RIPv1 updates only.\";\n"
	"            }\n"
	"            enum \"2\" {\n"
	"              value 2;\n"
	"              description\n"
	"                \"Send RIPv2 updates only.\";\n"
	"            }\n"
	"          }\n"
	"          must\n"
	"            \'(../receive = \"1\" and . = \"1\") or \' +\n"
	"            \'(../receive = \"2\" and . = \"2\") or \' +\n"
	"            \'(../receive = \"1-2\" and . = \"2\")\';\n"
	"          default \"2\";\n"
	"          description\n"
	"            \"Advertisement transmission - Version control.\";\n"
	"        }\n"
	"      }\n"
	"\n"
	"      leaf default-bfd-profile {\n"
	"        type frr-bfdd:profile-ref;\n"
	"        description\n"
	"          \"Use this BFD profile for all peers by default.\";\n"
	"      }\n"
	"\n"
	"      /*\n"
	"       * Operational data.\n"
	"       */\n"
	"      container state {\n"
	"        config false;\n"
	"        description\n"
	"          \"Operational data.\";\n"
	"\n"
	"        container neighbors {\n"
	"          description\n"
	"            \"Neighbor information.\";\n"
	"          list neighbor {\n"
	"            key \"address\";\n"
	"            description\n"
	"              \"A RIP neighbor.\";\n"
	"            leaf address {\n"
	"              type inet:ipv4-address;\n"
	"              description\n"
	"                \"IP address that a RIP neighbor is using as its\n"
	"                 source address.\";\n"
	"            }\n"
	"            leaf last-update {\n"
	"              type yang:date-and-time;\n"
	"              description\n"
	"                \"The time when the most recent RIP update was\n"
	"                 received from this neighbor.\";\n"
	"            }\n"
	"            leaf bad-packets-rcvd {\n"
	"              type yang:counter32;\n"
	"              description\n"
	"                \"The number of RIP invalid packets received from\n"
	"                 this neighbor which were subsequently discarded\n"
	"                 for any reason (e.g. a version 0 packet, or an\n"
	"                 unknown command type).\";\n"
	"            }\n"
	"            leaf bad-routes-rcvd {\n"
	"              type yang:counter32;\n"
	"              description\n"
	"                \"The number of routes received from this neighbor,\n"
	"                 in valid RIP packets, which were ignored for any\n"
	"                 reason (e.g. unknown address family, or invalid\n"
	"                 metric).\";\n"
	"            }\n"
	"          }\n"
	"        }\n"
	"        container routes {\n"
	"          description\n"
	"            \"Route information.\";\n"
	"          list route {\n"
	"            key \"prefix\";\n"
	"            description\n"
	"              \"A RIP IPv4 route.\";\n"
	"            leaf prefix {\n"
	"              type inet:ipv4-prefix;\n"
	"              description\n"
	"                \"IP address (in the form A.B.C.D) and prefix length,\n"
	"                 separated by the slash (/) character. The range of\n"
	"                 values for the prefix-length is 0 to 32.\";\n"
	"            }\n"
	"            container nexthops {\n"
	"              description \"container of nexthops\";\n"
	"              list nexthop {\n"
	"                description \"A list of nexthop objects.\";\n"
	"                leaf nh-type {\n"
	"                  type frr-nexthop:nexthop-type;\n"
	"                  mandatory true;\n"
	"                  description\n"
	"                    \"The nexthop type.\";\n"
	"                }\n"
	"                leaf protocol {\n"
	"                  type frr-route-types:frr-route-types-v4;\n"
	"                  description\n"
	"                    \"The protocol originating this route.\";\n"
	"                }\n"
	"                leaf rip-type {\n"
	"                  type rip-route-type;\n"
	"                  description\n"
	"                    \"The RIP type of route.\";\n"
	"                }\n"
	"                leaf gateway {\n"
	"                  type inet:ipv4-address;\n"
	"                  description\n"
	"                    \"The nexthop gateway address.\";\n"
	"                }\n"
	"                leaf interface {\n"
	"                  type frr-interface:interface-ref;\n"
	"                  description\n"
	"                    \"The nexthop egress interface.\";\n"
	"                }\n"
	"                leaf from {\n"
	"                  type inet:ipv4-address;\n"
	"                  description\n"
	"                    \"The nexthop gateway address.\";\n"
	"                }\n"
	"                leaf tag {\n"
	"                  type uint32;\n"
	"                  default \"0\";\n"
	"                  description\n"
	"                    \"Route tag\";\n"
	"                }\n"
	"                leaf external-metric {\n"
	"                  type uint32;\n"
	"                  description\n"
	"                    \"External metric if learned from external protocol.\";\n"
	"                }\n"
	"                leaf expire-time {\n"
	"                  type uint32;\n"
	"                  description\n"
	"                    \"Seconds before route expires.\";\n"
	"                }\n"
	"              }\n"
	"            }\n"
	"            leaf metric {\n"
	"              type uint8 {\n"
	"                range \"0..16\";\n"
	"              }\n"
	"              description\n"
	"                \"Route metric.\";\n"
	"            }\n"
	"            /*\n"
	"             * Replaced by container `nexthops` above.\n"
	"             */\n"
	"            leaf next-hop {\n"
	"              type inet:ipv4-address;\n"
	"              status deprecated;\n"
	"              description\n"
	"                \"Next hop IPv4 address.\";\n"
	"            }\n"
	"            leaf interface {\n"
	"              type frr-interface:interface-ref;\n"
	"              status deprecated;\n"
	"              description\n"
	"                \"The interface that the route uses.\";\n"
	"            }\n"
	"          }\n"
	"        }\n"
	"      }\n"
	"    }\n"
	"  }\n"
	"\n"
	"  /*\n"
	"   * Per-interface configuration data\n"
	"   */\n"
	"  augment \"/frr-interface:lib/frr-interface:interface\" {\n"
	"    description \"rip interface data\";\n"
	"    container rip {\n"
	"      description\n"
	"        \"RIP interface parameters.\";\n"
	"      leaf split-horizon {\n"
	"        type enumeration {\n"
	"          enum \"disabled\" {\n"
	"            value 0;\n"
	"            description\n"
	"              \"Disables split-horizon processing.\";\n"
	"          }\n"
	"          enum \"simple\" {\n"
	"            value 1;\n"
	"            description\n"
	"              \"Enables simple split-horizon processing.\";\n"
	"          }\n"
	"          enum \"poison-reverse\" {\n"
	"            value 2;\n"
	"            description\n"
	"              \"Enables split-horizon processing with poison\n"
	"               reverse.\";\n"
	"          }\n"
	"        }\n"
	"        default \"simple\";\n"
	"        description\n"
	"          \"Controls RIP split-horizon processing on the specified\n"
	"           interface.\";\n"
	"      }\n"
	"      leaf v2-broadcast {\n"
	"        type boolean;\n"
	"        default \"false\";\n"
	"        description\n"
	"          \"Send IP broadcast v2 update.\";\n"
	"      }\n"
	"      leaf version-receive {\n"
	"        type enumeration {\n"
	"          enum \"unspecified\" {\n"
	"            value 0;\n"
	"            description\n"
	"              \"Inherit configuration from the routing instance.\";\n"
	"          }\n"
	"          enum \"1\" {\n"
	"            value 1;\n"
	"            description\n"
	"              \"Accept RIPv1 updates only.\";\n"
	"          }\n"
	"          enum \"2\" {\n"
	"            value 2;\n"
	"            description\n"
	"              \"Accept RIPv2 updates only.\";\n"
	"          }\n"
	"          enum \"both\" {\n"
	"            value 3;\n"
	"            description\n"
	"              \"Accept both RIPv1 and RIPv2 updates.\";\n"
	"          }\n"
	"          enum \"none\" {\n"
	"            value 4;\n"
	"            description\n"
	"              \"Do not accept neither RIPv1 nor RIPv2 updates.\";\n"
	"          }\n"
	"        }\n"
	"        default \"unspecified\";\n"
	"        description\n"
	"          \"Advertisement reception - Version control.\";\n"
	"      }\n"
	"      leaf version-send {\n"
	"        type enumeration {\n"
	"          enum \"unspecified\" {\n"
	"            value 0;\n"
	"            description\n"
	"              \"Inherit configuration from the routing instance.\";\n"
	"          }\n"
	"          enum \"1\" {\n"
	"            value 1;\n"
	"            description\n"
	"              \"Send RIPv1 updates only.\";\n"
	"          }\n"
	"          enum \"2\" {\n"
	"            value 2;\n"
	"            description\n"
	"              \"Send RIPv2 updates only.\";\n"
	"          }\n"
	"          enum \"both\" {\n"
	"            value 3;\n"
	"            description\n"
	"              \"Send both RIPv1 and RIPv2 updates.\";\n"
	"          }\n"
	"          enum \"none\" {\n"
	"            value 4;\n"
	"            description\n"
	"              \"Do not send neither RIPv1 nor RIPv2 updates.\";\n"
	"          }\n"
	"        }\n"
	"        default \"unspecified\";\n"
	"        description\n"
	"          \"Advertisement transmission - Version control.\";\n"
	"      }\n"
	"      container authentication-scheme {\n"
	"        description\n"
	"          \"Specify the authentication scheme for the RIP interface\";\n"
	"        leaf mode {\n"
	"          type enumeration {\n"
	"            enum \"none\" {\n"
	"              value 0;\n"
	"              description\n"
	"                \"No authentication.\";\n"
	"            }\n"
	"            enum \"plain-text\" {\n"
	"              value 2;\n"
	"              description\n"
	"                \"Plain-text authentication.\";\n"
	"            }\n"
	"            enum \"md5\" {\n"
	"              value 3;\n"
	"              description\n"
	"                \"MD5 authentication.\";\n"
	"            }\n"
	"          }\n"
	"          default \"none\";\n"
	"          description\n"
	"            \"Specify the authentication mode.\";\n"
	"        }\n"
	"        leaf md5-auth-length {\n"
	"          when \"../mode = \'md5\'\";\n"
	"          type enumeration {\n"
	"            enum \"16\" {\n"
	"              value 16;\n"
	"              description\n"
	"                \"RFC compatible.\";\n"
	"            }\n"
	"            enum \"20\" {\n"
	"              value 20;\n"
	"              description\n"
	"                \"Old ripd compatible.\";\n"
	"            }\n"
	"          }\n"
	"          default \"20\";\n"
	"          description\n"
	"            \"MD5 authentication data length.\";\n"
	"        }\n"
	"      }\n"
	"      choice authentication-data {\n"
	"        description\n"
	"          \"Choose whether to use a simple password or a key-chain.\";\n"
	"        leaf authentication-password {\n"
	"          type string {\n"
	"            length \"1..16\";\n"
	"          }\n"
	"          description\n"
	"            \"Authentication string.\";\n"
	"        }\n"
	"        leaf authentication-key-chain {\n"
	"          type string;\n"
	"          description\n"
	"            \"Key-chain name.\";\n"
	"        }\n"
	"      }\n"
	"\n"
	"      container bfd-monitoring {\n"
	"        presence\n"
	"          \"Present if BFD is configured for RIP peers in this interface.\";\n"
	"        description \"Configure BFD use in RIPD\";\n"
	"\n"
	"        leaf enable {\n"
	"          type boolean;\n"
	"          default false;\n"
	"          description\n"
	"            \"Enable/disable BFD monitoring.\";\n"
	"        }\n"
	"\n"
	"        leaf profile {\n"
	"          type frr-bfdd:profile-ref;\n"
	"          description\n"
	"            \"BFD profile to use.\";\n"
	"        }\n"
	"      }\n"
	"    }\n"
	"  }\n"
	"\n"
	"  /*\n"
	"   * RPCs\n"
	"   */\n"
	"  rpc clear-rip-route {\n"
	"    description\n"
	"      \"Clears RIP routes from the IP routing table and routes\n"
	"       redistributed into the RIP protocol.\";\n"
	"\n"
	"    input {\n"
	"      leaf vrf {\n"
	"        type frr-vrf:vrf-ref;\n"
	"        description\n"
	"          \"VRF name identifying a specific RIP instance.\n"
	"           This leaf is optional for the rpc.\n"
	"           If it is specified, the rpc will clear all routes in the\n"
	"           specified RIP instance;\n"
	"           if it is not specified, the rpc will clear all routes in\n"
	"           all RIP instances.\";\n"
	"      }\n"
	"    }\n"
	"  }\n"
	"\n"
	"  /*\n"
	"   * Notifications\n"
	"   */\n"
	"  notification authentication-type-failure {\n"
	"    description\n"
	"      \"This notification is sent when the system\n"
	"       receives a PDU with the wrong authentication type\n"
	"       field.\";\n"
	"    leaf interface-name {\n"
	"      type frr-interface:interface-ref;\n"
	"      description\n"
	"        \"Describes the name of the RIP interface.\";\n"
	"    }\n"
	"    leaf raw-pdu {\n"
	"      type binary;\n"
	"      description\n"
	"        \"Received raw PDU.\";\n"
	"    }\n"
	"  }\n"
	"  notification authentication-failure {\n"
	"    description\n"
	"      \"This notification is sent when the system\n"
	"       receives a PDU with the wrong authentication\n"
	"       information.\";\n"
	"    leaf interface-name {\n"
	"      type frr-interface:interface-ref;\n"
	"      description\n"
	"        \"Describes the name of the RIP interface.\";\n"
	"    }\n"
	"    leaf raw-pdu {\n"
	"      type binary;\n"
	"      description\n"
	"        \"Received raw PDU.\";\n"
	"    }\n"
	"  }\n"
	"}\n"
	"";

static struct yang_module_embed embed = {
	.mod_name = "frr-ripd",
	.mod_rev = "2019-09-09",
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
