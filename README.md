# meltdown_note
## pdf
这里包括
+ meltdown 原文：`Meltdown Reading Kernel Memory from User Space`
+ 个人学习笔记：`MeltdownNote`
+ meltdown_Lab 指导书（form [SeedLab](https://seedsecuritylabs.org/Labs_16.04/System/Meltdown_Attack/)）：`Meltdown_Attack`   , `Meltdown_Spectre_ex_problem`

## meltdown_Lab
+ 一个基于 [SeedLab](https://seedsecuritylabs.org/Labs_16.04/System/Meltdown_Attack/) 完成的 Meltdown DEMO
+ 实验环境：
    ```
    Architecture:          x86_64
    CPU op-mode(s):        32-bit, 64-bit
    Byte Order:            Little Endian
    CPU(s):                8
    On-line CPU(s) list:   0-7
    Thread(s) per core:    2
    Core(s) per socket:    4
    Socket(s):             1
    NUMA node(s):          1
    Vendor ID:             GenuineIntel
    CPU family:            6
    Model:                 94
    Model name:            Intel(R) Core(TM) i7-6700 CPU @ 3.40GHz
    Stepping:              3
    CPU MHz:               3800.305
    CPU max MHz:           4000.0000
    CPU min MHz:           800.0000
    BogoMIPS:              6816.00
    Virtualization:        VT-x
    L1d cache:             32K
    L1i cache:             32K
    L2 cache:              256K
    L3 cache:              8192K
    NUMA node0 CPU(s):     0-7
    Flags:                 fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc aperfmperf tsc_known_freq pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch epb intel_pt tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm mpx rdseed adx smap clflushopt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp
    ```