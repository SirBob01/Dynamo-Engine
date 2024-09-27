#pragma once

#include <Sound/Filter.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Stereo panner filter implementation
     *
     */
    class Stereo : public Filter {
        Sound _remixed;
        Sound _output;

      public:
        Sound &apply(Sound &src,
                     const unsigned offset,
                     const unsigned length,
                     const Material &material,
                     const ListenerProperties &listener) override;
    };
} // namespace Dynamo::Sound