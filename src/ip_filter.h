#pragma once

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>
#include "range/v3/all.hpp"

using Ip = std::vector<int>;
using IpPool = std::vector<Ip>;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string& str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos)
    {
        auto&& result = str.substr(start, stop - start);
        r.push_back(result);

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

IpPool fill(std::istream* input)
{
    IpPool ipPool;
    for (std::string line; std::getline(*input, line);)
    {
        std::vector<std::string> v = split(line, '\t');
        Ip ip;
        for (auto rawIpPart : split(v.at(0), '.'))
        {
            ip.push_back(std::stoi(rawIpPart));
        }
        ipPool.push_back(ip);
    }
    return ipPool;
}

std::string toString(const IpPool& pool)
{
    auto res = pool | ranges::views::transform([&](const auto& ip)
    {
        auto res = ip | ranges::views::transform([&](const auto& item)
        {
            return std::to_string(item);
        }) | ranges::views::intersperse(".");
        return ranges::actions::join(res) | ranges::to<std::string>();
    });

    std::stringstream output;
    ranges::copy(res, ranges::ostream_iterator<>(output, "\n"));
    return output.str();

/*    for (const auto& ip : pool)
    {
        std::copy(ip.begin(), std::prev(ip.end()), std::ostream_iterator<int>(output, "."));
        output << ip.back() << std::endl;
    }
    return output.str();*/
}

template<class T>
IpPool filterInternal(const IpPool& pool, size_t index, T first)
{
    if (pool.size() <= 0 || index >= pool.begin()->size())
    {
        return IpPool{};
    }

    return pool | ranges::views::filter([&](const auto& ip)
    {
        return ip[index] == first;
    }) | ranges::to<std::vector>();
}

template<class T, class... Types>
IpPool filterInternal(const IpPool& pool, size_t index, T first, Types... args)
{
    if (pool.size() <= 0)
    {
        return IpPool{};
    }

    auto result = filterInternal(pool, index, first);
    return filterInternal(result, index + 1, args...) | ranges::to<std::vector>();
}

template<class... Types>
IpPool filter(const IpPool& pool, Types... args)
{
    return filterInternal(pool, 0, args...);
}

template<class T, class... Types>
IpPool filter_any(const IpPool& pool, T first, Types... args)
{
    std::vector<T> filterList{first, args...};
    return pool | ranges::views::filter(std::bind(ranges::any_of, std::placeholders::_1, [&](const auto& item)
    {
        return ranges::any_of(filterList, [&](const auto& filterItem)
        {
            return item == filterItem;
        });
    })) | ranges::to<std::vector>();
}


