cmd_/home/celema/workspace/sdk/Vego_PI_zuluko_sdk/v1b6/Vego-SDK-arbutus-zuluko.Linux-x86_64/out/arbutus-zuluko/compile_dir/target/linux-arbutus-zuluko/linux-3.4.39/user_headers/include/scsi/.install := perl scripts/headers_install.pl /home/celema/workspace/sdk/Vego_PI_zuluko_sdk/v1b6/Vego-SDK-arbutus-zuluko.Linux-x86_64/lichee/linux-3.4/include/scsi /home/celema/workspace/sdk/Vego_PI_zuluko_sdk/v1b6/Vego-SDK-arbutus-zuluko.Linux-x86_64/out/arbutus-zuluko/compile_dir/target/linux-arbutus-zuluko/linux-3.4.39/user_headers/include/scsi arm scsi_bsg_fc.h scsi_netlink.h scsi_netlink_fc.h; perl scripts/headers_install.pl /home/celema/workspace/sdk/Vego_PI_zuluko_sdk/v1b6/Vego-SDK-arbutus-zuluko.Linux-x86_64/lichee/linux-3.4/include/scsi /home/celema/workspace/sdk/Vego_PI_zuluko_sdk/v1b6/Vego-SDK-arbutus-zuluko.Linux-x86_64/out/arbutus-zuluko/compile_dir/target/linux-arbutus-zuluko/linux-3.4.39/user_headers/include/scsi arm ; perl scripts/headers_install.pl /home/celema/workspace/sdk/Vego_PI_zuluko_sdk/v1b6/Vego-SDK-arbutus-zuluko.Linux-x86_64/lichee/linux-3.4/include/generated/scsi /home/celema/workspace/sdk/Vego_PI_zuluko_sdk/v1b6/Vego-SDK-arbutus-zuluko.Linux-x86_64/out/arbutus-zuluko/compile_dir/target/linux-arbutus-zuluko/linux-3.4.39/user_headers/include/scsi arm ; for F in ; do echo "\#include <asm-generic/$$F>" > /home/celema/workspace/sdk/Vego_PI_zuluko_sdk/v1b6/Vego-SDK-arbutus-zuluko.Linux-x86_64/out/arbutus-zuluko/compile_dir/target/linux-arbutus-zuluko/linux-3.4.39/user_headers/include/scsi/$$F; done; touch /home/celema/workspace/sdk/Vego_PI_zuluko_sdk/v1b6/Vego-SDK-arbutus-zuluko.Linux-x86_64/out/arbutus-zuluko/compile_dir/target/linux-arbutus-zuluko/linux-3.4.39/user_headers/include/scsi/.install