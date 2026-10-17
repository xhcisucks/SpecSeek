#include <specseek.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "args.h"

#define ANSI_RESET   "\033[0m"
#define ANSI_BOLD    "\033[1m"
#define ANSI_DIM     "\033[2m"
#define ANSI_GREEN   "\033[32m"
#define ANSI_RED     "\033[31m"

static const char *g_reset   = ANSI_RESET;
static const char *g_bold    = ANSI_BOLD;
static const char *g_dim     = ANSI_DIM;
static const char *g_green   = ANSI_GREEN;
static const char *g_red     = ANSI_RED;
static const char *C_PRIMARY = "";
static const char *C_ACCENT  = "";

static void init_theme(specseek_cpu_vendor_t vendor, int no_ansi) {
    if (no_ansi) {
        g_reset = g_bold = g_dim = g_green = g_red = "";
        return;
    }
    if (vendor == AMD) {
        C_PRIMARY = "\033[31m";
        C_ACCENT  = "\033[91m";
    } else {
        C_PRIMARY = "\033[34m";
        C_ACCENT  = "\033[94m";
    }
}

static void box_top(const char *title) {
    printf("\n%s%s=== %s ===%s\n", C_PRIMARY, g_bold, title, g_reset);
}

static void box_mid(const char *title) {
    printf("\n%s%s--- %s ---%s\n", C_PRIMARY, g_bold, title, g_reset);
}

static void box_bot(void) {
    printf("\n");
}

static void kv(const char *key, const char *fmt, ...) {
    char val[256];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(val, sizeof(val), fmt, ap);
    va_end(ap);

    printf("  %-16s: %s%s%s\n", key, C_ACCENT, val, g_reset);
}

static void kv_bool(const char *key, int val) {
    const char *valstr = val ? "true" : "false";
    printf("  %-16s: %s%s%s\n", key, val ? g_green : g_red, valstr, g_reset);
}


#define CELL_NAME_W 22

static int feat_col = 0;

static void feat_begin(void) {
    feat_col = 0;
}

static void feat_item(const char *name, int val) {
    const char *valstr = val ? "true " : "false";
    const char *valcol = val ? g_green : g_red;

    if (feat_col == 0) {
        printf("  ");
    }

    printf("%-*s: %s%s%s", CELL_NAME_W, name, valcol, valstr, g_reset);

    feat_col++;
    if (feat_col == 2) {
        printf("\n");
        feat_col = 0;
    } else {
        printf("    "); // gap
    }
}

static void feat_end(void) {
    if (feat_col != 0) {
        printf("\n");
    }
    feat_col = 0;
}

#define F(field) feat_item(#field, (f)->field)

void print_cpu_features_minimal(const specseek_cpu_features *f) {
    if (!f) return;

    box_top("FEATURES");
    feat_begin();
    
    F(sse);      F(sse2);
    F(sse3);     F(ssse3);
    F(sse4_1);   F(sse4_2);

    F(avx);      F(avx2);
    F(f16c);     F(fma);
    F(avx512f);

    F(vmx);      F(svm);
    F(hypervisor);

    F(nx);       F(smep);
    F(smap);     F(pdpe1gb);

    F(popcnt);   F(bmi1);
    F(bmi2);     F(lm);

    feat_end();
    box_bot();
}

void print_cpu_features(const specseek_cpu_features *f) {
    if (!f) return;

    box_top("FEATURES");

    box_mid("SSE / Base SIMD");
    feat_begin();
    F(sse);     F(sse2);    F(sse3);    F(ssse3);
    F(sse4_1);  F(sse4_2);  F(sse4a);
    F(fpu);     F(mmx);     F(mmxext);
    feat_end();

    box_mid("AVX / AVX-512");
    feat_begin();
    F(avx);             F(avx2);
    F(avx_vnni);        F(avx_ifma);
    F(f16c);            F(fma);
    F(fma4);            F(avx512f);
    F(avx512dq);        F(avx512bw);
    F(avx512vl);        F(avx512cd);
    F(avx512er);        F(avx512pf);
    F(avx512ifma);      F(avx512vbmi);
    F(avx512vbmi2);     F(avx512vnni);
    F(avx512bitalg);    F(avx512vpopcntdq);
    F(avx512_4vnniw);   F(avx512_4fmaps);
    F(avx512vp2intersect); F(avx512fp16);
    F(avx512_bf16);
    feat_end();

    box_mid("AMX");
    feat_begin();
    F(amx_tile);  F(amx_int8);
    F(amx_bf16);  F(amx_fp16);
    feat_end();

    box_mid("Crypto / Security");
    feat_begin();
    F(aes);         F(vaes);
    F(sha);         F(sha512);
    F(sm3);         F(sm4);
    F(pclmulqdq);   F(vpclmulqdq);
    F(gfni);        F(rdrnd);
    F(rdseed);      F(sgx);
    F(sgx_lc);      F(kl);
    F(pku);         F(ospke);
    F(umip);        F(tme);
    F(shstk);       F(ibt);
    F(pks);         F(ibrs_ibpb);
    F(stibp);       F(ssbd);
    F(l1d_flush);   F(md_clear);
    F(ia32_arch_cap); F(ia32_core_cap);
    feat_end();

    box_mid("Virtualisation");
    feat_begin();
    F(vmx);       F(smx);
    F(svm);       F(hypervisor);
    F(x2apic);    F(apic);
    feat_end();

    box_mid("Memory / Paging");
    feat_begin();
    F(pae);       F(pse);
    F(pse36);     F(pge);
    F(pat);       F(mtrr);
    F(mca);       F(mce);
    F(pcid);      F(invpcid);
    F(smep);      F(smap);
    F(pdpe1gb);   F(nx);
    F(la57);      F(lam);
    F(mpx);       F(pqm);
    F(pqe);       F(clflushopt);
    F(clwb);      F(clfsh);
    F(cldemote);
    feat_end();

    box_mid("Branching / Control Flow");
    feat_begin();
    F(cmov);        F(cx8);
    F(cx16);        F(bmi1);
    F(bmi2);        F(abm);
    F(popcnt);      F(movbe);
    F(movdiri);     F(movdir64b);
    F(serialize);   F(xsave);
    F(osxsave);     F(fsgsbase);
    F(tsc);         F(tsc_adjust);
    F(tsc_deadline); F(rdtscp);
    F(rdpid);       F(lahf_lm);
    F(syscall);     F(sep);
    F(fxsr);        F(fxsr_opt);
    feat_end();

    box_mid("Transactions / ISA Ext");
    feat_begin();
    F(hle);             F(rtm);
    F(rtm_always_abort); F(tsx_force_abort);
    F(tsxldtrk);        F(enqcmd);
    F(waitpkg);         F(uintr);
    F(wdt);             F(lwp);
    F(skinit);          F(xop);
    F(tbm);             F(rao_int);
    F(cmpccxadd);       F(lkgs);
    F(wrmsrns);         F(msrlist);
    F(fred);            F(fsrm);
    F(fzrm);            F(fsrs);
    F(rsrcs);           F(hreset);
    F(prefetchwt1);     F(prefetch3dnow);
    F(amd3dnow);        F(amd3dnowext);
    F(misalignsse);
    feat_end();

    box_mid("Debug / Monitor / Misc");
    feat_begin();
    F(monitor);     F(mwaitx);
    F(ds);          F(dtes64);
    F(ds_cpl);      F(pdcm);
    F(dca);         F(pt);
    F(lbr);         F(cnxt_id);
    F(sdbg);        F(psn);
    F(pbe);         F(htt);
    F(tm);          F(tm2);
    F(eist);        F(xtpr);
    F(acpi);        F(msr);
    F(vme);         F(de);
    F(ss);          F(ibs);
    F(osvw);        F(extapic);
    F(cr8_legacy);  F(topoext);
    F(perfctr_core); F(perfctr_nb);
    F(perfctr_llc); F(ptsc);
    F(bpext);       F(nodeid_msr);
    F(tce);         F(mp);
    F(lm);          F(ia64);
    F(fdp_excptn);  F(erms);
    F(pconfig);     F(hybrid);
    F(archperfmon_ext); F(lass);
    F(cmp_legacy);  F(srbds_ctrl);
    feat_end();

    box_bot();
}

int main(int argc, char **argv) {
    specseek_args args = args_parse(argc, argv);

    specseek_cpu_identifiers identifiers = {0};
    specseek_set_cpuid_identifiers(&identifiers);

    if (args.feature_mode) {
        args_run_feature_query(&args);
        // wont return btw
    }

    init_theme(identifiers.cpu_vendor_type, args.no_ansi);

    // CPU Identity  
    box_top("CPU IDENTITY");
    kv("Vendor",   identifiers.cpu_vendor_string);
    kv("Brand",    identifiers.cpu_brand_string);
    kv("Model",    "0x%X (base 0x%X, ext 0x%X)",
       identifiers.cpu_model, identifiers.cpu_model_base, identifiers.cpu_model_extended);
    kv("Family",   "0x%X (base 0x%X, ext 0x%X)",
       identifiers.cpu_family, identifiers.cpu_family_base, identifiers.cpu_family_extended);
    kv("Revision", "0x%X", identifiers.cpu_revision);
    box_bot();

    // Cache  
    specseek_cpu_cache cpu_cache_info = {0};
    specseek_set_cpuid_cache(&cpu_cache_info);

    box_top("CACHE");
    kv("L1 Instruction", "%d KB", cpu_cache_info.L1_instruction_cache);
    kv("L1 Data",        "%d KB", cpu_cache_info.L1_data_cache);
    kv("L2",             "%d KB", cpu_cache_info.L2_cache);
    kv("L3",             "%d KB", cpu_cache_info.L3_cache);
    box_bot();

    // Topology  
    specseek_cpu_specifications specifications = {0};
    specseek_set_cpuid_specifications(&specifications);

    box_top("TOPOLOGY");
    kv("Physical cores",     "%ld", specifications.physical_core_count);
    kv("Threads per core",   "%ld", specifications.threads_per_core);
    kv("Logical processors", "%ld", specifications.logical_processor_count);
    box_bot();

    // Address Space  
    specseek_cpu_address_space address_space = {0};
    specseek_set_cpuid_address_space(&address_space);
    unsigned long long phys_tib = (1ULL << address_space.physical_address_bits) / (1024ULL*1024*1024*1024);
    unsigned long long virt_tib = (1ULL << address_space.virtual_address_bits)  / (1024ULL*1024*1024*1024);

    box_top("ADDRESS SPACE");
    kv("Physical", "%u bits (%llu TiB max)", address_space.physical_address_bits, phys_tib);
    kv("Virtual",  "%u bits (%llu TiB max)", address_space.virtual_address_bits,  virt_tib);
    box_bot();

    // Frequency  
    specseek_cpu_frequency freq = {0};
    specseek_set_cpuid_frequency(&freq);

    box_top("FREQUENCY");
    if (freq.base_mhz == 0 && freq.max_mhz == 0 && freq.bus_mhz == 0) {
        printf("  %snot reported by this CPU%s\n", g_dim, g_reset);
    } else {
        kv("Base", "%u MHz", freq.base_mhz);
        kv("Max",  "%u MHz", freq.max_mhz);
        kv("Bus",  "%u MHz", freq.bus_mhz);
    }
    box_bot();

    // Hypervisor  
    specseek_cpu_hypervisor hypervisor = {0};
    specseek_set_cpuid_hypervisor(&hypervisor);

    box_top("HYPERVISOR");
    if (hypervisor.present) {
        kv("Vendor", "%s", hypervisor.vendor_string);
    } else {
        printf("  %snot detected%s\n", g_dim, g_reset);
    }
    box_bot();

    // Thermal  
    specseek_cpu_thermal thermal = {0};
    specseek_set_cpuid_thermal(&thermal);

    box_top("THERMAL");
    kv_bool("Digital Thermal Sensor",    thermal.dts);
    kv_bool("Turbo Boost",               thermal.turbo_boost);
    kv_bool("Always-running APIC Timer", thermal.arat);
    kv_bool("HW Coord. Feedback",        thermal.hwp_feedback);
    kv_bool("Energy-Efficient Turbo",    thermal.energy_efficient_turbo);
    box_bot();

    // Performance Monitoring  
    specseek_cpu_perfmon perfmon = {0};
    specseek_set_cpuid_perfmon(&perfmon);

    box_top("PERFORMANCE MONITORING");
    if (perfmon.version == 0) {
        printf("  %snot available%s\n", g_dim, g_reset);
    } else {
        kv("Version",          "%u", perfmon.version);
        kv("General counters", "%u (%u-bit wide)",
           perfmon.general_counter_count, perfmon.general_counter_width);
    }
    box_bot();

    // Features  
    specseek_cpu_features features = {0};
    specseek_set_cpuid_features(&features);
    if (args.verbose) {
        print_cpu_features(&features);
    } else {
        print_cpu_features_minimal(&features);
    }

    if (args.pause) {
        args_wait_for_exit();
    }

    return 0;
}