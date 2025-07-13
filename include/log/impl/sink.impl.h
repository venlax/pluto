//
// Created by xixi on 2025-07-11.
//

namespace pluto {
    template<typename T> requires Outputable<T>
    TLogSink<T>::TLogSink(T& sink) : sink_(sink) {
    }

    template<typename T> requires Outputable<T>
    TLogSink<T>::~TLogSink() {
    }

    template<typename T> requires Outputable<T>
    void TLogSink<T>::output(std::string_view str) {
        std::println(sink_, "{}", str);
    }

}


