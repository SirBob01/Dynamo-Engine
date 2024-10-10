#pragma once

#include <Sound/Buffer.hpp>
#include <Sound/Filter.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Simple stereo panner filter implementation
     *
     */
    class Stereo : public Filter {
        Buffer _mono;

      public:
        void apply(const Buffer &src, Buffer &dst, const Source &source, const Listener &listener) override;
    };
} // namespace Dynamo::Sound