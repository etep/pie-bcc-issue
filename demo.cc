#include <stdint.h>
#include <stdio.h>
#include <sys/mount.h>

#include <bcc/BPF.h>

#include <filesystem>

constexpr char kBCCProgram[] = R"(
  int bpf_uprobe_fn(struct pt_regs* ctx) {
    return 0;
  }
)";

extern "C" {
void target_fn() { printf("I am the target function.\n"); }
}

int main() {
  ebpf::BPF bpf;

  printf("Mounting debugfs to /sys/kernel/debug.\n");
  const int debug_mount = mount("none", "/sys/kernel/debug", "debugfs", 0, nullptr);
  if (debug_mount < 0) {
    printf("Could not mount debugfs to /sys/kernel/debug, exiting now.\n");
    return -1;
  }

  printf("Initializing BPF.\n");
  const auto init = bpf.init(std::string(kBCCProgram), {"-Wno-macro-redefined"});

  if (!init.ok()) {
    printf("Could not initialize BCC, exiting now.\n");
    return -1;
  }

  const auto self = std::filesystem::read_symlink("/proc/self/exe");
  const uint64_t symbol_addr = reinterpret_cast<uint64_t>(&target_fn);

  printf("Found self binary path: %s.\n", self.string().c_str());
  printf("Address for target_fn:  0x%016lx.\n", symbol_addr);

  // If this program is compiled as a "PIE" (position independent executable),
  // then this will fail. If compiled as a "normal" (not PIE) exectuable,
  // the attach will succeed.
  printf("Attaching uprobe (using virtual address for target_fn).\n");
  const auto attach = bpf.attach_uprobe(self.string().c_str(), {}, "bpf_uprobe_fn", symbol_addr);

  if (attach.ok()) {
    printf("Success!!\n");
  } else {
    printf("Fail: %s.\n", attach.msg().c_str());
  }
  return attach.code();
}
