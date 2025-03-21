#include "di/assert/prelude.h"
#include "di/container/string/prelude.h"
#include "di/container/vector/prelude.h"
#include "di/container/view/prelude.h"
#include "di/math/prelude.h"

auto main(int argc, char** argv) -> int {
    auto args = di::Vector<di::TransparentString> {};
    for (auto i : di::range(1, argc)) {
        args.push_back(
            di::TransparentStringView { argv[i], di::to_unsigned(di::distance(di::ZString(argv[i]))) }.to_owned());
    }

    auto expected = di::Array { "hello"_tsv.to_owned(), "world"_tsv.to_owned() } | di::to<di::Vector>();

    ASSERT_EQ(args, expected);

    return 0;
}
