cmd_/home/caiyongheng/tina_test/out/tulip-d1/compile_dir/toolchain/linux-dev//include/linux/netfilter_ipv4/.install := bash scripts/headers_install.sh /home/caiyongheng/tina_test/out/tulip-d1/compile_dir/toolchain/linux-dev//include/linux/netfilter_ipv4 /home/caiyongheng/tina_test/lichee/linux-3.10/include/uapi/linux/netfilter_ipv4 ip_tables.h ipt_CLUSTERIP.h ipt_ECN.h ipt_LOG.h ipt_REJECT.h ipt_TTL.h ipt_ULOG.h ipt_ah.h ipt_ecn.h ipt_ttl.h; bash scripts/headers_install.sh /home/caiyongheng/tina_test/out/tulip-d1/compile_dir/toolchain/linux-dev//include/linux/netfilter_ipv4 /home/caiyongheng/tina_test/lichee/linux-3.10/include/linux/netfilter_ipv4 ; bash scripts/headers_install.sh /home/caiyongheng/tina_test/out/tulip-d1/compile_dir/toolchain/linux-dev//include/linux/netfilter_ipv4 /home/caiyongheng/tina_test/lichee/linux-3.10/include/generated/uapi/linux/netfilter_ipv4 ; for F in ; do echo "\#include <asm-generic/$$F>" > /home/caiyongheng/tina_test/out/tulip-d1/compile_dir/toolchain/linux-dev//include/linux/netfilter_ipv4/$$F; done; touch /home/caiyongheng/tina_test/out/tulip-d1/compile_dir/toolchain/linux-dev//include/linux/netfilter_ipv4/.install
