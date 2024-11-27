#include <dius/main.h>
#include <dius/test/prelude.h>

namespace dius::test {
static auto main(TestManager::Args& args) -> di::Result<void> {
    return dius::test::TestManager::the().run_tests(args);
}
}

DIUS_MAIN(dius::test::TestManager::Args, dius::test)
