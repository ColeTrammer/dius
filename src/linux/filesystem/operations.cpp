#include "di/assert/prelude.h"
#include "di/container/algorithm/prelude.h"
#include "di/vocab/error/prelude.h"
#include "dius/filesystem/prelude.h"
#include "dius/linux/error.h"
#include "dius/linux/system_call.h"

#ifdef DIUS_USE_RUNTIME
#include <linux/fcntl.h>
#else
#include <fcntl.h>
#endif

namespace dius::filesystem {
namespace linux {
    static auto mknod(di::PathView path, FileType type, Perms perms) -> di::Expected<int, di::GenericCode> {
        auto raw_data = path.data();
        char null_terminated_string[4097];
        ASSERT_LT(raw_data.size(), sizeof(null_terminated_string) - 1);

        di::copy(raw_data, null_terminated_string);
        null_terminated_string[raw_data.size()] = '\0';

        return system::system_call<int>(system::Number::mknodat, AT_FDCWD, null_terminated_string,
                                        static_cast<u32>(perms) | static_cast<u32>(type), 0);
    }

    static auto mkdir(di::PathView path, Perms perms) -> di::Expected<int, di::GenericCode> {
        auto raw_data = path.data();
        char null_terminated_string[4097];
        ASSERT_LT(raw_data.size(), sizeof(null_terminated_string) - 1);

        di::copy(raw_data, null_terminated_string);
        null_terminated_string[raw_data.size()] = '\0';

        return system::system_call<int>(system::Number::mkdirat, AT_FDCWD, null_terminated_string,
                                        static_cast<u32>(perms));
    }
}

namespace detail {
    auto CreateRegularFileFunction::operator()(di::PathView path) const -> di::Result<bool> {
        auto result = linux::mknod(path, FileType::Regular, Perms::All);
        if (!result) {
            if (result.error() == PosixError::FileExists) {
                return false;
            }
            return di::Unexpected(result.error());
        }
        return true;
    }

    auto CreateDirectoryFunction::operator()(di::PathView path) const -> di::Result<bool> {
        auto result = linux::mkdir(path, Perms::All);
        if (!result) {
            if (result.error() == PosixError::FileExists) {
                return false;
            }
            return di::Unexpected(result.error());
        }
        return true;
    }
}
}
