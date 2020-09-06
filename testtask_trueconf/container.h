#ifndef CONTAINER_H
#define CONTAINER_H
#include <random>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <cassert>

/** @class noncopyable_t
 * @brief Non-copyable class-helper
**/
class noncopyable_t
{
public:
    noncopyable_t() = default;
    ~noncopyable_t() = default;

private:
    noncopyable_t(const noncopyable_t&) = delete;
    noncopyable_t& operator=(const noncopyable_t&) = delete;
};

/** @class Icontainer_t
    @brief Interface class for containers
**/
class Icontainer_t : public noncopyable_t
{
public:
    /// Type for element of container
    typedef std::uint8_t item_t;
    /// Type for index of container
    typedef std::uint32_t idx_t;
    /// Number of digits
    static constexpr item_t m_numbers = 9;
    /// Index of empty element
    static constexpr idx_t m_emtyidx = idx_t(-1);
    /// Type for histogram
    typedef std::array<item_t, m_numbers> histogram_t;
    /// Constructor
    Icontainer_t() : m_rnggen(m_rd()), m_dist(1, m_numbers)
    {  }
    /// Operator of subscription
    virtual item_t& operator[](idx_t) = 0;
    /// Remove random value from container
    virtual void removeRandom() = 0;
    /// Remove mismatched values from container
    virtual void removeMismatched(const Icontainer_t& _cont) = 0;
    /// Fill histogram
    virtual void fillHistogram(histogram_t& _hist) const = 0;
    /// Get maximum index
    virtual idx_t max() const = 0;
    /// Check, Does exist index in container
    virtual bool isIndexFound(idx_t _idx) const = 0;
    /// Get histogram report
    virtual std::string getHistReport() const
    {
        histogram_t histogram;
        fillHistogram(histogram);
        std::string result;
        for (idx_t i = 0; i < histogram.size(); i++)
        {
            result += std::to_string(histogram[i]);
            result += ' ';
        }
        return result;
    }
    /// Get name of container
    virtual std::string getName() const = 0;
    /// Destructor
    virtual ~Icontainer_t() = default;
protected:
    /// Type of generator
    typedef std::mt19937 rnggen_t;
    /// Source of random numbers
    std::random_device m_rd;
    /// Random generator
    rnggen_t m_rnggen;
    /// Random distribution
    std::uniform_int_distribution<item_t> m_dist;
};

/// @class vcontainer_t
class vcontainer_t : public Icontainer_t
{
public:
    vcontainer_t(const idx_t _num)
        : m_distidx(0, idx_t(_num?_num - 1:0))
    {
        for (idx_t i = 0; i < _num; i++)
        {
            item_t tmp = m_dist(m_rnggen);
            assert(tmp <= m_numbers);
            m_vcon.push_back(tmp);
        }
    }
    vcontainer_t(std::initializer_list<item_t> _list)
        : m_distidx(0, idx_t(_list.size()?_list.size() - 1:0))
    {
        for (auto it = _list.begin(); it != _list.end(); it++)
        {
            item_t tmp = *it;
            assert(tmp <= m_numbers);
            m_vcon.push_back(tmp);
        }
    }
    virtual item_t& operator[](idx_t _idx) override
    { erasePending(); return  m_vcon[_idx]; }
    virtual void removeRandom() override
    {
        if ((m_vcon.size() - m_pendErase) == 0) return;
        idx_t tmp_idx = m_distidx(m_rnggen) % (m_vcon.size() - m_pendErase);
        auto it = m_vcon.begin();
        std::advance(it, tmp_idx);
        std::rotate(it, it + 1, m_vcon.end());
        m_pendErase++;
    }
    bool isIndexFound(idx_t _idx) const override
    { return  (_idx < (m_vcon.size()) - m_pendErase); }
    virtual void removeMismatched(const Icontainer_t& _cont) override
    {
        erasePending();
        histogram_t histogram_own, histogram_alien;
        fillHistogram(histogram_own);
        _cont.fillHistogram(histogram_alien);
        auto it = m_vcon.end();
        for (item_t mval = 1; mval <= m_numbers; mval++)
        {
            bool domatch = (histogram_own[mval - 1] != 0);
            bool matched = (domatch)?( histogram_alien[mval - 1] != 0): true;
            if (!matched)
            {
                auto it_new = std::remove(m_vcon.begin(), it, mval);
                it = it_new;
            }
        }
        m_vcon.erase(it, m_vcon.end());
    }
    idx_t max() const override
    {
        auto isempty = m_vcon.size() - m_pendErase;
        return ((isempty > 0)? idx_t(isempty - 1):m_emtyidx);
    }
    virtual std::string getName() const override
    { return "vector"; }
    virtual void fillHistogram(histogram_t& _hist) const override
    {
        std::fill(_hist.begin(), _hist.end(), 0);
        for (auto& value: m_vcon)
        {
            _hist[value - 1]++;
        }
    }
    virtual ~vcontainer_t() override = default;
private:
    /// Type for base container
    typedef std::vector<item_t> vcont_t;
    /// Base container
    vcont_t m_vcon;
    /// Number of pending erase operations (size of garbage at tail)
    idx_t m_pendErase = 0;
    /// Object of random index distribution
    std::uniform_int_distribution<idx_t> m_distidx;
    /// Erase garbage from base container
    void erasePending()
    {
        if (m_pendErase > 0)
        {
            auto idx = m_vcon.size() - m_pendErase;
            auto it = m_vcon.begin();
            std::advance(it, idx);
            m_vcon.erase(it, m_vcon.end());
            m_pendErase = 0;
        }
    }
};

/// @class mcontainer_t
class mcontainer_t : public Icontainer_t
{
public:
    mcontainer_t(const idx_t _num)
        : m_distidx(0, idx_t(_num?_num - 1:0))
    {
        for (idx_t i = 0; i < _num; i++)
        {
            item_t tmp = m_dist(m_rnggen);
            assert(tmp <= m_numbers);
            m_mcon[i] = tmp;
        }
    }
    mcontainer_t(std::initializer_list<item_t> _list)
        : m_distidx(0, idx_t(_list.size()?_list.size() - 1:0))
    {
        idx_t i = 0;
        for (auto it = _list.begin(); it != _list.end(); it++)
        {
            item_t tmp = *it;
            assert(tmp <= m_numbers);
            m_mcon[i++] = tmp;
        }
    }
    virtual item_t& operator[](idx_t _idx) override
    { return m_mcon[_idx]; }
    virtual void removeRandom() override
    {
        if (m_mcon.empty()) return;
        idx_t tmp_idx = m_distidx(m_rnggen) % m_mcon.size(); // UB on zero
        auto it = m_mcon.lower_bound(tmp_idx);
        if (it == m_mcon.end())
        {
            it = m_mcon.upper_bound(tmp_idx);
            if (it == m_mcon.end()) { return; }
        }
        m_mcon.erase(it);
    }
    bool isIndexFound(idx_t _idx) const override
    { return  (m_mcon.find(_idx) != m_mcon.end()); }
    virtual void removeMismatched(const Icontainer_t& _cont) override
    {
        histogram_t histogram_alien;
        _cont.fillHistogram(histogram_alien);
        for (auto it = m_mcon.begin(); it != m_mcon.end();)
        {
            auto [unused, val] = *it;
            (void)unused;
            if (histogram_alien[val - 1] == 0)
            {
                m_mcon.erase(it++);
            }
            else
            {
                ++it;
            }
        }
    }
    idx_t max() const override
    {
        if (!m_mcon.empty())
            { return m_mcon.rbegin()->first; }
        else
            { return m_emtyidx; }
    }
    virtual std::string getName() const override
    { return "map"; }
    virtual void fillHistogram(histogram_t& _hist) const override
    {
        std::fill(_hist.begin(), _hist.end(), 0);
        for (auto& [ unused, value]: m_mcon)
        {
            (void)unused;
            _hist[value - 1]++;
        }
    }
    virtual ~mcontainer_t() override = default;
private:
    /// type of base container
    typedef std::map<idx_t, item_t> mcont_t;
    /// base container
    mcont_t m_mcon;
    /// Object of random index distribution
    std::uniform_int_distribution<idx_t> m_distidx;
};

#endif // CONTAINER_H
