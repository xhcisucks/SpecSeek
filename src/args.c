#include <args.h>
#include <specseek.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <io.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stddef.h>

static int get_feature_value(const specseek_cpu_features *f, const char *name) {
    #define MATCH(str, field) if (strcmp(name, str) == 0) return f->field;

    MATCH("sse", sse)
    MATCH("sse2", sse2)
    MATCH("sse3", sse3)
    MATCH("ssse3", ssse3)
    MATCH("sse4.1", sse4_1)
    MATCH("sse4.2", sse4_2)
    MATCH("sse4a", sse4a)
    MATCH("fpu", fpu)
    MATCH("mmx", mmx)
    MATCH("mmxext", mmxext)
    MATCH("avx", avx)
    MATCH("avx2", avx2)
    MATCH("avx_vnni", avx_vnni)
    MATCH("avx_ifma", avx_ifma)
    MATCH("f16c", f16c)
    MATCH("fma", fma)
    MATCH("fma4", fma4)
    MATCH("avx512f", avx512f)
    MATCH("avx512dq", avx512dq)
    MATCH("avx512bw", avx512bw)
    MATCH("avx512vl", avx512vl)
    MATCH("avx512cd", avx512cd)
    MATCH("avx512er", avx512er)
    MATCH("avx512pf", avx512pf)
    MATCH("avx512ifma", avx512ifma)
    MATCH("avx512vbmi", avx512vbmi)
    MATCH("avx512vbmi2", avx512vbmi2)
    MATCH("avx512vnni", avx512vnni)
    MATCH("avx512bitalg", avx512bitalg)
    MATCH("avx512vpopcntdq", avx512vpopcntdq)
    MATCH("avx512_4vnniw", avx512_4vnniw)
    MATCH("avx512_4fmaps", avx512_4fmaps)
    MATCH("avx512vp2intersect", avx512vp2intersect)
    MATCH("avx512fp16", avx512fp16)
    MATCH("avx512_bf16", avx512_bf16)
    MATCH("amx_tile", amx_tile)
    MATCH("amx_int8", amx_int8)
    MATCH("amx_bf16", amx_bf16)
    MATCH("amx_fp16", amx_fp16)
    MATCH("aes", aes)
    MATCH("vaes", vaes)
    MATCH("sha", sha)
    MATCH("sha512", sha512)
    MATCH("sm3", sm3)
    MATCH("sm4", sm4)
    MATCH("pclmulqdq", pclmulqdq)
    MATCH("vpclmulqdq", vpclmulqdq)
    MATCH("gfni", gfni)
    MATCH("rdrnd", rdrnd)
    MATCH("rdseed", rdseed)
    MATCH("sgx", sgx)
    MATCH("sgx_lc", sgx_lc)
    MATCH("kl", kl)
    MATCH("pku", pku)
    MATCH("ospke", ospke)
    MATCH("umip", umip)
    MATCH("tme", tme)
    MATCH("shstk", shstk)
    MATCH("ibt", ibt)
    MATCH("pks", pks)
    MATCH("ibrs_ibpb", ibrs_ibpb)
    MATCH("stibp", stibp)
    MATCH("ssbd", ssbd)
    MATCH("l1d_flush", l1d_flush)
    MATCH("md_clear", md_clear)
    MATCH("ia32_arch_cap", ia32_arch_cap)
    MATCH("ia32_core_cap", ia32_core_cap)
    MATCH("vmx", vmx)
    MATCH("smx", smx)
    MATCH("svm", svm)
    MATCH("hypervisor", hypervisor)
    MATCH("x2apic", x2apic)
    MATCH("apic", apic)
    MATCH("pae", pae)
    MATCH("pse", pse)
    MATCH("pse36",  pse36)
    MATCH("pge", pge)
    MATCH("pat", pat)
    MATCH("mtrr", mtrr)
    MATCH("mca", mca)
    MATCH("mce", mce)
    MATCH("pcid", pcid)
    MATCH("invpcid", invpcid)
    MATCH("smep", smep)
    MATCH("smap", smap)
    MATCH("pdpe1gb", pdpe1gb)
    MATCH("nx", nx)
    MATCH("la57", la57)
    MATCH("lam", lam)
    MATCH("mpx", mpx)
    MATCH("pqm", pqm)
    MATCH("pqe", pqe)
    MATCH("clflushopt", clflushopt)
    MATCH("clwb", clwb)
    MATCH("clfsh", clfsh)
    MATCH("cldemote", cldemote)
    MATCH("cmov", cmov)
    MATCH("cx8", cx8)
    MATCH("cx16", cx16)
    MATCH("bmi1", bmi1)
    MATCH("bmi2", bmi2)
    MATCH("abm", abm)
    MATCH("popcnt", popcnt)
    MATCH("movbe", movbe)
    MATCH("movdiri", movdiri)
    MATCH("movdir64b", movdir64b)
    MATCH("serialize", serialize)
    MATCH("xsave", xsave)
    MATCH("osxsave", osxsave)
    MATCH("fsgsbase", fsgsbase)
    MATCH("tsc", tsc)
    MATCH("tsc_adjust", tsc_adjust)
    MATCH("tsc_deadline", tsc_deadline)
    MATCH("rdtscp", rdtscp)
    MATCH("rdpid", rdpid)
    MATCH("lahf_lm", lahf_lm)
    MATCH("syscall", syscall)
    MATCH("sep", sep)
    MATCH("fxsr", fxsr)
    MATCH("fxsr_opt", fxsr_opt)
    MATCH("hle", hle)
    MATCH("rtm", rtm)
    MATCH("rtm_always_abort", rtm_always_abort)
    MATCH("tsx_force_abort", tsx_force_abort)
    MATCH("tsxldtrk", tsxldtrk)
    MATCH("enqcmd", enqcmd)
    MATCH("waitpkg", waitpkg)
    MATCH("uintr", uintr)
    MATCH("wdt", wdt)
    MATCH("lwp", lwp)
    MATCH("skinit", skinit)
    MATCH("xop", xop)
    MATCH("tbm", tbm)
    MATCH("rao_int", rao_int)
    MATCH("cmpccxadd", cmpccxadd)
    MATCH("lkgs", lkgs)
    MATCH("wrmsrns", wrmsrns)
    MATCH("msrlist", msrlist)
    MATCH("fred", fred)
    MATCH("fsrm", fsrm)
    MATCH("fzrm", fzrm)
    MATCH("fsrs", fsrs)
    MATCH("rsrcs", rsrcs)
    MATCH("hreset", hreset)
    MATCH("prefetchwt1", prefetchwt1)
    MATCH("prefetch3dnow", prefetch3dnow)
    MATCH("amd3dnow", amd3dnow)
    MATCH("amd3dnowext", amd3dnowext)
    MATCH("misalignsse", misalignsse)
    MATCH("monitor", monitor)
    MATCH("mwaitx", mwaitx)
    MATCH("ds", ds)
    MATCH("dtes64", dtes64)
    MATCH("ds_cpl", ds_cpl)
    MATCH("pdcm", pdcm)
    MATCH("dca", dca)
    MATCH("pt", pt)
    MATCH("lbr", lbr)
    MATCH("cnxt_id", cnxt_id)
    MATCH("sdbg", sdbg)
    MATCH("psn", psn)
    MATCH("pbe", pbe)
    MATCH("htt", htt)
    MATCH("tm", tm)
    MATCH("tm2", tm2)
    MATCH("eist", eist)
    MATCH("xtpr", xtpr)
    MATCH("acpi", acpi)
    MATCH("msr", msr)
    MATCH("vme", vme)
    MATCH("de", de)
    MATCH("ss", ss)
    MATCH("ibs", ibs)
    MATCH("osvw", osvw)
    MATCH("extapic", extapic)
    MATCH("cr8_legacy", cr8_legacy)
    MATCH("topoext", topoext)
    MATCH("perfctr_core", perfctr_core)
    MATCH("perfctr_nb", perfctr_nb)
    MATCH("perfctr_llc", perfctr_llc)
    MATCH("ptsc", ptsc)
    MATCH("bpext", bpext)
    MATCH("nodeid_msr", nodeid_msr)
    MATCH("tce", tce)
    MATCH("mp", mp)
    MATCH("lm", lm)
    MATCH("ia64", ia64)
    MATCH("fdp_excptn", fdp_excptn)
    MATCH("erms", erms)
    MATCH("pconfig", pconfig)
    MATCH("hybrid", hybrid)
    MATCH("archperfmon_ext", archperfmon_ext)
    MATCH("lass", lass)
    MATCH("cmp_legacy", cmp_legacy)
    MATCH("srbds_ctrl", srbds_ctrl)

    #undef MATCH
    return -1;
}

static int stdout_is_tty(void) {
#ifdef _WIN32
    if (!_isatty(_fileno(stdout))) return 0;

    // Older consoles can display color with a bit more setup
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(out, &mode))
        SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    return 1;
#else
    return isatty(fileno(stdout));
#endif
}

specseek_args args_parse(int argc, char **argv) {
    specseek_args result = {0};

    if (!stdout_is_tty()) {
        result.no_ansi = 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--no-ansi") == 0) {
            result.no_ansi = 1;
        } else if (strcmp(argv[i], "--verbose") == 0) {
            result.verbose = 1;
        } else if (strcmp(argv[i], "--feature") == 0) {
            result.feature_mode = 1;
            result.features = (const char **)(argv + i + 1);
            result.feature_count = argc - i - 1;
            if (result.feature_count == 0) {
                fprintf(stderr, "usage: --feature <name> [name ...]\n");
                exit(1);
            }
            break; // everything after --feature is a feature name, so it should always be the last
        } else {
            fprintf(stderr, "unknown option: %s\n", argv[i]);
            exit(1);
        }
    }

    return result;
}

void args_run_feature_query(const specseek_args *args) {
    specseek_cpu_features features = {0};
    specseek_set_cpuid_features(&features);

    for (int i = 0; i < args->feature_count; i++) {
        const char *name = args->features[i];
        
        int val = get_feature_value(&features, name);
        if (val != -1) {
            printf("%s: %s\n", name, val ? "yes" : "no");
        } else {
            fprintf(stderr, "unknown feature: %s\n", name);
            exit(1);
        }
    }
}