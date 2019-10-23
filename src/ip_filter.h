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
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
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
    for(std::string line; std::getline(*input, line);)
    {
        std::vector<std::string> v = split(line, '\t');
        Ip ip;
        for(auto rawIpPart : split(v.at(0), '.'))
        {
            ip.push_back(std::stoi(rawIpPart));
        }
        ipPool.push_back(ip);
    }
    return ipPool;
}

std::string toString(const IpPool& pool)
{
    std::stringstream output;
    for(const auto& ip : pool)
    {
//        std::copy(ip.begin(), std::prev(ip.end()),std::ostream_iterator<int>(output, "."));
        auto res = ip | ranges::views::all;
        output << res << std::endl;
    }
    return output.str();
}

template<class T>
IpPool filterInternal(const IpPool& pool, size_t index, T first)
{
    IpPool result;
    if (pool.size() <= 0 || index >= pool.begin()->size())
    {
        return result;
    }

    auto iterFilterFirst = std::find_if(pool.begin(), pool.end(), [&](const auto& ip)
    {
        return ip[index] == first;
    });

    auto iterFilterLast = std::find_if(iterFilterFirst, pool.end(), [&](const auto& ip)
    {
        return ip[index] < first;
    });

    std::copy(iterFilterFirst, iterFilterLast, std::back_inserter(result));
    return result;
}

template<class T, class... Types>
IpPool filterInternal(const IpPool& pool, size_t index, T first, Types... args)
{
    IpPool result;
    if (pool.size() <= 0)
    {
        return result;
    }

    result = filterInternal(pool, index, first);
    return filterInternal(result, index + 1, args...);
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
    IpPool result;
    std::copy_if(pool.begin(), pool.end(), std::back_inserter(result), [&](const auto& ip)
    {
        return std::find_first_of(ip.begin(), ip.end(), filterList.begin(), filterList.end()) != ip.end();
    });
    return result;
}


