#pragma once

#include <boost/signals2/signal.hpp>

namespace Framework {

template<typename T>
using SignalEvent = boost::signals2::signal<void(const T&)>;

using ScopedConnection = boost::signals2::scoped_connection;

template<typename T>
using SignalCallback = const std::function<void(const T&)>&;

}
