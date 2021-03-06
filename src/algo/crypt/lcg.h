#pragma once

#include <memory>
#include "types.h"

namespace au {
namespace algo {
namespace crypt {

    enum class LcgKind : u8
    {
        MicrosoftVisualC,
        ParkMiller,
        ParkMillerRevised,
    };

    class Lcg final
    {
    public:
        Lcg(LcgKind kind, u32 seed);
        ~Lcg();
        u32 next();

    private:
        struct Priv;
        std::unique_ptr<Priv> p;
    };

} } }
