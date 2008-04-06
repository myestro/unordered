
// Copyright (C) 2003-2004 Jeremy B. Maitin-Shepard.
// Copyright (C) 2005-2008 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/unordered for documentation

#ifndef BOOST_UNORDERED_SET_HPP_INCLUDED
#define BOOST_UNORDERED_SET_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>

#include <functional>
#include <memory>

#include <boost/unordered/detail/hash_table.hpp>
#include <boost/functional/hash.hpp>

namespace boost
{
    template <class Value,
        class Hash = hash<Value>,
        class Pred = std::equal_to<Value>,
        class Alloc = std::allocator<Value> >
    class unordered_set
    {
        typedef boost::unordered_detail::hash_types_unique_keys<
            Value, Value, Hash, Pred, Alloc
        > implementation;

        BOOST_DEDUCED_TYPENAME implementation::hash_table base;

    public:

        // types

        typedef Value key_type;
        typedef Value value_type;
        typedef Hash hasher;
        typedef Pred key_equal;

        typedef Alloc allocator_type;
        typedef BOOST_DEDUCED_TYPENAME allocator_type::pointer pointer;
        typedef BOOST_DEDUCED_TYPENAME allocator_type::const_pointer const_pointer;
        typedef BOOST_DEDUCED_TYPENAME allocator_type::reference reference;
        typedef BOOST_DEDUCED_TYPENAME allocator_type::const_reference const_reference;

        typedef BOOST_DEDUCED_TYPENAME implementation::size_type size_type;
        typedef BOOST_DEDUCED_TYPENAME implementation::difference_type difference_type;

        typedef BOOST_DEDUCED_TYPENAME implementation::const_iterator iterator;
        typedef BOOST_DEDUCED_TYPENAME implementation::const_iterator const_iterator;
        typedef BOOST_DEDUCED_TYPENAME implementation::const_local_iterator local_iterator;
        typedef BOOST_DEDUCED_TYPENAME implementation::const_local_iterator const_local_iterator;

        // construct/destroy/copy

        explicit unordered_set(
                size_type n = boost::unordered_detail::default_initial_bucket_count,
                const hasher &hf = hasher(),
                const key_equal &eql = key_equal(),
                const allocator_type &a = allocator_type())
            : base(n, hf, eql, a)
        {
        }

        // TODO: Should this be explicit?
        unordered_set(allocator_type const& a)
            : base(boost::unordered_detail::default_initial_bucket_count,
                hasher(), key_equal(), a)
        {
        }

        unordered_set(unordered_set const& other, allocator_type const& a)
            : base(other.base, a)
        {
        }

#if defined(BOOST_HAS_RVALUE_REFS)
        unordered_set(unordered_set&& other)
            : base(std::move(other.base))
        {
        }

        unordered_set(unordered_set&& other, allocator_type const& a)
            : base(std::move(other.base), a)
        {
        }
#endif

        template <class InputIterator>
        unordered_set(InputIterator f, InputIterator l)
            : base(f, l, boost::unordered_detail::default_initial_bucket_count,
                hasher(), key_equal(), allocator_type())
        {
        }

        template <class InputIterator>
        unordered_set(InputIterator f, InputIterator l, size_type n,
                const hasher &hf = hasher(),
                const key_equal &eql = key_equal(),
                const allocator_type &a = allocator_type())
            : base(f, l, n, hf, eql, a)
        {
        }

// TODO: Do this properly?
#if defined(BOOST_HAS_RVALUE_REFS)
        unordered_set& operator=(unordered_set&& x)
        {
            swap(x);
            return *this;
        }
#endif

    private:

        BOOST_DEDUCED_TYPENAME implementation::iterator_base const&
            get(const_iterator const& it)
        {
            return boost::unordered_detail::iterator_access::get(it);
        }

    public:

        allocator_type get_allocator() const
        {
            return base.get_allocator();
        }

        // size and capacity

        bool empty() const
        {
            return base.empty();
        }

        size_type size() const
        {
            return base.size();
        }

        size_type max_size() const
        {
            return base.max_size();
        }

        // iterators

        iterator begin()
        {
            return iterator(base.data_.begin());
        }

        const_iterator begin() const
        {
            return const_iterator(base.data_.begin());
        }

        iterator end()
        {
            return iterator(base.data_.end());
        }

        const_iterator end() const
        {
            return const_iterator(base.data_.end());
        }

        const_iterator cbegin() const
        {
            return const_iterator(base.data_.begin());
        }

        const_iterator cend() const
        {
            return const_iterator(base.data_.end());
        }

        // modifiers

        std::pair<iterator, bool> insert(const value_type& obj)
        {
            return boost::unordered_detail::pair_cast<iterator, bool>(
                    base.insert(obj));
        }

        iterator insert(const_iterator hint, const value_type& obj)
        {
            return iterator(base.insert(get(hint), obj));
        }

        template <class InputIterator>
            void insert(InputIterator first, InputIterator last)
        {
            base.insert(first, last);
        }

        iterator erase(const_iterator position)
        {
            return iterator(base.erase(get(position)));
        }

        size_type erase(const key_type& k)
        {
            return base.erase(k);
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            return iterator(base.erase(get(first), get(last)));
        }

        void clear()
        {
            base.data_.clear();
        }

        void swap(unordered_set& other)
        {
            base.swap(other.base);
        }

        // observers

        hasher hash_function() const
        {
            return base.hash_function();
        }

        key_equal key_eq() const
        {
            return base.key_eq();
        }

        // lookup

        const_iterator find(const key_type& k) const
        {
            return const_iterator(base.find(k));
        }

        size_type count(const key_type& k) const
        {
            return base.count(k);
        }

        std::pair<const_iterator, const_iterator>
            equal_range(const key_type& k) const
        {
            return boost::unordered_detail::pair_cast<const_iterator, const_iterator>(
                    base.equal_range(k));
        }

        // bucket interface

        size_type bucket_count() const
        {
            return base.bucket_count();
        }

        size_type max_bucket_count() const
        {
            return base.max_bucket_count();
        }

        size_type bucket_size(size_type n) const
        {
            return base.data_.bucket_size(n);
        }

        size_type bucket(const key_type& k) const
        {
            return base.bucket(k);
        }

        local_iterator begin(size_type n)
        {
            return local_iterator(base.data_.begin(n));
        }

        const_local_iterator begin(size_type n) const
        {
            return const_local_iterator(base.data_.begin(n));
        }

        local_iterator end(size_type n)
        {
            return local_iterator(base.data_.end(n));
        }

        const_local_iterator end(size_type n) const
        {
            return const_local_iterator(base.data_.end(n));
        }

        const_local_iterator cbegin(size_type n) const
        {
            return const_local_iterator(base.data_.begin(n));
        }

        const_local_iterator cend(size_type n) const
        {
            return const_local_iterator(base.data_.end(n));
        }

        // hash policy

        float load_factor() const
        {
            return base.load_factor();
        }

        float max_load_factor() const
        {
            return base.max_load_factor();
        }

        void max_load_factor(float m)
        {
            base.max_load_factor(m);
        }

        void rehash(size_type n)
        {
            base.rehash(n);
        }
    }; // class template unordered_set

    template <class T, class H, class P, class A>
    void swap(unordered_set<T, H, P, A> &m1,
            unordered_set<T, H, P, A> &m2)
    {
        m1.swap(m2);
    }

    template <class Value,
        class Hash = hash<Value>,
        class Pred = std::equal_to<Value>,
        class Alloc = std::allocator<Value> >
    class unordered_multiset
    {
        typedef boost::unordered_detail::hash_types_equivalent_keys<
            Value, Value, Hash, Pred, Alloc
        > implementation;

        BOOST_DEDUCED_TYPENAME implementation::hash_table base;

    public:

        //types

        typedef Value key_type;
        typedef Value value_type;
        typedef Hash hasher;
        typedef Pred key_equal;

        typedef Alloc allocator_type;
        typedef BOOST_DEDUCED_TYPENAME allocator_type::pointer pointer;
        typedef BOOST_DEDUCED_TYPENAME allocator_type::const_pointer const_pointer;
        typedef BOOST_DEDUCED_TYPENAME allocator_type::reference reference;
        typedef BOOST_DEDUCED_TYPENAME allocator_type::const_reference const_reference;

        typedef BOOST_DEDUCED_TYPENAME implementation::size_type size_type;
        typedef BOOST_DEDUCED_TYPENAME implementation::difference_type difference_type;

        typedef BOOST_DEDUCED_TYPENAME implementation::const_iterator iterator;
        typedef BOOST_DEDUCED_TYPENAME implementation::const_iterator const_iterator;
        typedef BOOST_DEDUCED_TYPENAME implementation::const_local_iterator local_iterator;
        typedef BOOST_DEDUCED_TYPENAME implementation::const_local_iterator const_local_iterator;

        // construct/destroy/copy

        explicit unordered_multiset(
                size_type n = boost::unordered_detail::default_initial_bucket_count,
                const hasher &hf = hasher(),
                const key_equal &eql = key_equal(),
                const allocator_type &a = allocator_type())
          : base(n, hf, eql, a)
        {
        }

        // TODO: Should this be explicit?
        unordered_multiset(allocator_type const& a)
            : base(boost::unordered_detail::default_initial_bucket_count,
                hasher(), key_equal(), a)
        {
        }

        unordered_multiset(unordered_multiset const& other, allocator_type const& a)
            : base(other.base, a)
        {
        }

#if defined(BOOST_HAS_RVALUE_REFS)
        unordered_multiset(unordered_multiset&& other)
            : base(std::move(other.base))
        {
        }

        unordered_multiset(unordered_multiset&& other, allocator_type const& a)
            : base(std::move(other.base), a)
        {
        }
#endif

        template <class InputIterator>
        unordered_multiset(InputIterator f, InputIterator l)
            : base(f, l, boost::unordered_detail::default_initial_bucket_count,
                hasher(), key_equal(), allocator_type())
        {
        }

        template <class InputIterator>
        unordered_multiset(InputIterator f, InputIterator l, size_type n,
                const hasher &hf = hasher(),
                const key_equal &eql = key_equal(),
                const allocator_type &a = allocator_type())
          : base(f, l, n, hf, eql, a)
        {
        }

// TODO: Do this properly?
#if defined(BOOST_HAS_RVALUE_REFS)
        unordered_multiset& operator=(unordered_multiset&& x)
        {
            swap(x);
            return *this;
        }
#endif

    private:

        BOOST_DEDUCED_TYPENAME implementation::iterator_base const&
            get(const_iterator const& it)
        {
            return boost::unordered_detail::iterator_access::get(it);
        }

    public:

        allocator_type get_allocator() const
        {
            return base.get_allocator();
        }

        // size and capacity

        bool empty() const
        {
            return base.empty();
        }

        size_type size() const
        {
            return base.size();
        }

        size_type max_size() const
        {
            return base.max_size();
        }

        // iterators

        iterator begin()
        {
            return iterator(base.data_.begin());
        }

        const_iterator begin() const
        {
            return const_iterator(base.data_.begin());
        }

        iterator end()
        {
            return iterator(base.data_.end());
        }

        const_iterator end() const
        {
            return const_iterator(base.data_.end());
        }

        const_iterator cbegin() const
        {
            return const_iterator(base.data_.begin());
        }

        const_iterator cend() const
        {
            return const_iterator(base.data_.end());
        }

        // modifiers

        iterator insert(const value_type& obj)
        {
            return iterator(base.insert(obj));
        }

        iterator insert(const_iterator hint, const value_type& obj)
        {
            return iterator(base.insert(get(hint), obj));
        }

        template <class InputIterator>
            void insert(InputIterator first, InputIterator last)
        {
            base.insert(first, last);
        }

        iterator erase(const_iterator position)
        {
            return iterator(base.erase(get(position)));
        }

        size_type erase(const key_type& k)
        {
            return base.erase(k);
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            return iterator(base.erase(get(first), get(last)));
        }

        void clear()
        {
            base.data_.clear();
        }

        void swap(unordered_multiset& other)
        {
            base.swap(other.base);
        }

        // observers

        hasher hash_function() const
        {
            return base.hash_function();
        }

        key_equal key_eq() const
        {
            return base.key_eq();
        }

        // lookup

        const_iterator find(const key_type& k) const
        {
            return const_iterator(base.find(k));
        }

        size_type count(const key_type& k) const
        {
            return base.count(k);
        }

        std::pair<const_iterator, const_iterator>
            equal_range(const key_type& k) const
        {
            return boost::unordered_detail::pair_cast<const_iterator, const_iterator>(
                    base.equal_range(k));
        }

        // bucket interface

        size_type bucket_count() const
        {
            return base.bucket_count();
        }

        size_type max_bucket_count() const
        {
            return base.max_bucket_count();
        }

        size_type bucket_size(size_type n) const
        {
            return base.data_.bucket_size(n);
        }

        size_type bucket(const key_type& k) const
        {
            return base.bucket(k);
        }

        local_iterator begin(size_type n)
        {
            return local_iterator(base.data_.begin(n));
        }

        const_local_iterator begin(size_type n) const
        {
            return const_local_iterator(base.data_.begin(n));
        }

        local_iterator end(size_type n)
        {
            return local_iterator(base.data_.end(n));
        }

        const_local_iterator end(size_type n) const
        {
            return const_local_iterator(base.data_.end(n));
        }

        const_local_iterator cbegin(size_type n) const
        {
            return const_local_iterator(base.data_.begin(n));
        }

        const_local_iterator cend(size_type n) const
        {
            return const_local_iterator(base.data_.end(n));
        }

        // hash policy

        float load_factor() const
        {
            return base.load_factor();
        }

        float max_load_factor() const
        {
            return base.max_load_factor();
        }

        void max_load_factor(float m)
        {
            base.max_load_factor(m);
        }

        void rehash(size_type n)
        {
            base.rehash(n);
        }
    }; // class template unordered_multiset

    template <class T, class H, class P, class A>
    void swap(unordered_multiset<T, H, P, A> &m1,
            unordered_multiset<T, H, P, A> &m2)
    {
        m1.swap(m2);
    }

} // namespace boost

#endif // BOOST_UNORDERED_SET_HPP_INCLUDED
